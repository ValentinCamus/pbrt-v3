// integrators/adaptive.cpp*
#include "adaptive.h"
#include "camera.h"
#include "filters/box.h"
#include "paramset.h"
#include "progressreporter.h"
#include "stats.h"

namespace pbrt {

STAT_COUNTER("Integrator/Camera rays traced", nCameraRays);
STAT_COUNTER("Integrator/Render time in milliseconds", nElapsedMilliseconds);

Statistics::Statistics(const ParamSet &paramSet, const Film *originalFilm,
                       const Sampler &sampler)
    : mode([](std::string &&modeString) {
              return modeString == "time"  ? Mode::TIME
                   : modeString == "error" ? Mode::ERROR
                   :                         Mode::NORMAL;
          }(paramSet.FindOneString("mode", "normal"))
      ),
      maxSamples(sampler.samplesPerPixel),
      minSamples(paramSet.FindOneInt("minsamples", 64)),
      errorThreshold(paramSet.FindOneFloat("errorthreshold", 0.01)),
      errorHeuristic(paramSet.FindOneString("errorheuristic", "relative")),
      batchSize(paramSet.FindOneInt("batchsize", 16)),
      targetSeconds(paramSet.FindOneInt("targetseconds", 60)),
      originalFilm(originalFilm),
      pixelBounds(originalFilm->croppedPixelBounds),
      pixels(new Pixel[pixelBounds.Area()]) {}

void Statistics::RenderBegin() {
    startTime = Clock::now();
}

void Statistics::RenderEnd() const {
    nElapsedMilliseconds = ElapsedMilliseconds();
}

void Statistics::WriteImages() const {
    CHECK(originalFilm);
    Film samplesFilm(*originalFilm, StatImagesFilter(), "_samples");
    Film varianceFilm(*originalFilm, StatImagesFilter(), "_variance");
    Film errorFilm(*originalFilm, StatImagesFilter(), "_error");

    for (Point2i pixel : pixelBounds) {
        Point2f floatPixel(static_cast<Float>(pixel.x),
                           static_cast<Float>(pixel.y));
        const Pixel &statsPixel = GetPixel(pixel);
        samplesFilm.AddSplat(floatPixel, Sampling(statsPixel));
        varianceFilm.AddSplat(floatPixel, Variance(statsPixel));
        errorFilm.AddSplat(floatPixel, Error(statsPixel));
    }

    samplesFilm.WriteImage();
    varianceFilm.WriteImage();
    errorFilm.WriteImage();
}

bool Statistics::StartNextBatch(int number) {
    switch (mode) {
    default:
        if (batchOnce) {
            batchOnce = false;
            return true;
        } else
            return false;
    }
}

long Statistics::BatchSize() const {
    return batchSize;
}

void Statistics::SamplingLoop(Point2i pixel, const SamplingFunctor &sampleOnce){
    auto loop = [&]() {
        UpdateStats(pixel, sampleOnce());
    };

    switch (mode) {
    default:
        for (long i = 0; i < maxSamples; ++i)
            loop();
        break;
    }
}

void Statistics::UpdateStats(Point2i pixel, Spectrum &&L) {
    if (!InsideExclusive(pixel, pixelBounds))
        return;

    Pixel &statsPixel = GetPixel(pixel);
    long &samples = statsPixel.samples;
    Spectrum &mean = statsPixel.mean;
    Spectrum &moment2 = statsPixel.moment2;

    // Welford's online algorithm
    ++samples;
    Spectrum delta1 = L - mean;
    mean += delta1 / samples;
    Spectrum delta2 = L - mean;
    moment2 += delta1 * delta2;
}

Float Statistics::Sampling(const Pixel &statsPixel) const {
    return static_cast<Float>(statsPixel.samples);
}

Spectrum Statistics::Variance(const Pixel &statsPixel) const {

    if (statsPixel.samples < 2)
        return 0;

    return statsPixel.moment2 / statsPixel.samples - 1;
}

Spectrum Statistics::Error(const Pixel &statsPixel) const {
    return 0;
}

bool Statistics::StopCriterion(Point2i pixel) const {
    if (!InsideExclusive(pixel, pixelBounds))
        return true;
    const Pixel &statsPixel = GetPixel(pixel);

    // Control approximation error

    return true;
}

long Statistics::ElapsedMilliseconds() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(Clock::now() - startTime).count();
}

std::string Statistics::WorkTitle() const {
    std::string type = mode == Mode::TIME  ? "time"
                     : mode == Mode::ERROR ? "error"
                     :                       "sampling";
    return "Rendering (equal " + type + ')';
}

long Statistics::UpdateWork() {
    return mode == Mode::TIME ? 0 : 1;
}

Statistics::Pixel &Statistics::GetPixel(Point2i pixel) {
    const auto *that = this;
    return const_cast<Pixel &>(that->GetPixel(pixel));
}

const Statistics::Pixel &Statistics::GetPixel(Point2i pixel) const {
    CHECK(InsideExclusive(pixel, pixelBounds));
    int width = pixelBounds.pMax.x - pixelBounds.pMin.x;
    int offset = (pixel.x - pixelBounds.pMin.x)
               + (pixel.y - pixelBounds.pMin.y) * width;
    return pixels[offset];
}

std::unique_ptr<Filter> Statistics::StatImagesFilter() {
    return std::unique_ptr<Filter>(new BoxFilter({0, 0}));
}

VolPathAdaptive::VolPathAdaptive(Statistics stats, int maxDepth,
                                 std::shared_ptr<const Camera> camera,
                                 std::shared_ptr<Sampler> sampler,
                                 const Bounds2i &pixelBounds, Float rrThreshold,
                                 const std::string &lightSampleStrategy)
    : VolPathIntegrator(maxDepth, std::move(camera), sampler, pixelBounds,
                        rrThreshold, lightSampleStrategy),
      stats(std::move(stats)),
      sampler(std::move(sampler)),
      pixelBounds(pixelBounds) {}

void VolPathAdaptive::Render(const Scene &scene) {
    Preprocess(scene, *sampler);
    // Render image tiles in parallel

    // Compute number of tiles, _nTiles_, to use for parallel rendering
    Bounds2i sampleBounds = camera->film->GetSampleBounds();
    Vector2i sampleExtent = sampleBounds.Diagonal();
    const int tileSize = 16;
    Point2i nTiles((sampleExtent.x + tileSize - 1) / tileSize,
                   (sampleExtent.y + tileSize - 1) / tileSize);

    stats.RenderBegin();
    ProgressReporter reporter(nTiles.x * nTiles.y, stats.WorkTitle());
    for (int batch = 0; stats.StartNextBatch(batch); ++batch) {
        ParallelFor2D([&](Point2i tile) {
            // Render section of image corresponding to _tile_

            // Allocate _MemoryArena_ for tile
            MemoryArena arena;

            // Get sampler instance for tile
            int seed = nTiles.x * nTiles.y * batch + nTiles.x * tile.y + tile.x;
            std::unique_ptr<Sampler> tileSampler = sampler->Clone(seed);

            // Compute sample bounds for tile
            int x0 = sampleBounds.pMin.x + tile.x * tileSize;
            int x1 = std::min(x0 + tileSize, sampleBounds.pMax.x);
            int y0 = sampleBounds.pMin.y + tile.y * tileSize;
            int y1 = std::min(y0 + tileSize, sampleBounds.pMax.y);
            Bounds2i tileBounds(Point2i(x0, y0), Point2i(x1, y1));
            LOG(INFO) << "Starting image tile " << tileBounds;

            // Get _FilmTile_ for tile
            auto filmTile = camera->film->GetFilmTile(tileBounds);

            // Loop over pixels in tile to render them
            for (Point2i pixel : tileBounds) {
                {
                    ProfilePhase pp(Prof::StartPixel);
                    tileSampler->StartPixel(pixel);
                    tileSampler->SetSampleNumber(batch * stats.BatchSize());
                }

                // Do this check after the StartPixel() call; this keeps
                // the usage of RNG values from (most) Samplers that use
                // RNGs consistent, which improves reproducibility /
                // debugging.
                if (!InsideExclusive(pixel, pixelBounds))
                    continue;

                stats.SamplingLoop(pixel, [&]() {
                    // Initialize _CameraSample_ for current sample
                    CameraSample cameraSample =
                        tileSampler->GetCameraSample(pixel);

                    // Generate camera ray for current sample
                    RayDifferential ray;
                    Float rayWeight =
                        camera->GenerateRayDifferential(cameraSample, &ray);
                    ray.ScaleDifferentials(
                        1 / std::sqrt((Float)tileSampler->samplesPerPixel));
                    ++nCameraRays;

                    // Evaluate radiance along camera ray
                    Spectrum L(0.f);
                    if (rayWeight > 0)
                        L = Li(ray, scene, *tileSampler, arena, 0);

                    // Add camera ray's contribution to image
                    filmTile->AddSample(cameraSample.pFilm, L, rayWeight);

                    // Free _MemoryArena_ memory from computing image sample
                    // value
                    arena.Reset();

                    tileSampler->StartNextSample();
                    return L;
                });
            }
            LOG(INFO) << "Finished image tile " << tileBounds;

            // Merge image tile into _Film_
            camera->film->MergeFilmTile(std::move(filmTile));
            reporter.Update(stats.UpdateWork());
        }, nTiles);
    }
    reporter.Done();
    LOG(INFO) << "Rendering finished";
    stats.RenderEnd();

    // Save final image after rendering
    camera->film->WriteImage();
    stats.WriteImages();
}

VolPathAdaptive *CreateVolPathAdaptive(const ParamSet &params,
                                       std::shared_ptr<Sampler> sampler,
                                       std::shared_ptr<const Camera> camera) {
    int maxDepth = params.FindOneInt("maxdepth", 5);
    int np;
    const int *pb = params.FindInt("pixelbounds", &np);
    Bounds2i pixelBounds = camera->film->GetSampleBounds();
    if (pb) {
        if (np != 4)
            Error("Expected four values for \"pixelbounds\" parameter. Got %d.",
                  np);
        else {
            pixelBounds = Intersect(pixelBounds,
                                    Bounds2i{{pb[0], pb[2]}, {pb[1], pb[3]}});
            if (pixelBounds.Area() == 0)
                Error("Degenerate \"pixelbounds\" specified.");
        }
    }
    Float rrThreshold = params.FindOneFloat("rrthreshold", 1.);
    std::string lightStrategy =
        params.FindOneString("lightsamplestrategy", "spatial");

    return new VolPathAdaptive({params, camera->film, *sampler}, maxDepth,
                               camera, sampler, pixelBounds, rrThreshold,
                               lightStrategy);
}

}  // namespace pbrt
