#ifndef PBRT_INTEGRATORS_ADAPTIVE_H
#define PBRT_INTEGRATORS_ADAPTIVE_H

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

// integrators/adaptive.h*
#include "pbrt.h"
#include "film.h"
#include "volpath.h"
#include <chrono>
#include <functional>

namespace pbrt {

// Statistics is a reusable class that can be added to an Integrator to compute
// convergence statistics during the render loop, and control it adaptively.
class Statistics {
    enum class Mode {NORMAL, TIME, ERROR};
    using Clock = std::chrono::steady_clock;
    using SamplingFunctor = std::function<Spectrum()>;

  public:
    // Holds the three base statistics at each pixel
    struct Pixel {
        long samples = 0;
        Spectrum mean = 0;
        Spectrum moment2 = 0;
    };

    Statistics(const ParamSet &paramSet, const Film *originalFilm,
               const Sampler &sampler);

    void RenderBegin();
    void RenderEnd() const;
    void WriteImages() const;

    bool StartNextBatch(int number);
    long BatchSize() const;

    void SamplingLoop(Point2i pixel, const SamplingFunctor &sampleOnce);
    void UpdateStats(Point2i pixel, Spectrum &&L);
    Float Sampling(const Pixel &statsPixel) const;
    Spectrum Variance(const Pixel &statsPixel) const;
    Spectrum Error(const Pixel &statsPixel) const;
    bool StopCriterion(Point2i pixel) const;

    long ElapsedMilliseconds() const;
    std::string WorkTitle() const;
    long UpdateWork();

  private:
    // Parameters from the scene file
    const Mode mode;
    // normal mode
    const long maxSamples;
    // error mode
    const long minSamples;
    const float errorThreshold;
    const std::string errorHeuristic;
    // time mode
    const long batchSize;
    const long targetSeconds;

    // Pixel data updated with each sample and exported after the render
    const Film *originalFilm;
    const Bounds2i pixelBounds;
    std::unique_ptr<Pixel[]> pixels;

    // Variables to control the rendering loop
    Clock::time_point startTime;
    bool batchOnce = true;

    // Access to the stored pixels
    Pixel &GetPixel(Point2i pixel);
    const Pixel &GetPixel(Point2i pixel) const;

    // Just a dummy filter to construct the statistics films
    static std::unique_ptr<Filter> StatImagesFilter();
};

// VolPathAdaptive is similar to the original VolPathIntegrator, except for the
// Render function that is configured to use Statistics.
class VolPathAdaptive : public VolPathIntegrator {
  public:
    VolPathAdaptive(Statistics stats, int maxDepth,
                    std::shared_ptr<const Camera> camera,
                    std::shared_ptr<Sampler> sampler,
                    const Bounds2i &pixelBounds, Float rrThreshold = 1,
                    const std::string &lightSampleStrategy = "spatial");
    void Render(const Scene &scene) override;

  private:
    Statistics stats;
    std::shared_ptr<Sampler> sampler;
    const Bounds2i pixelBounds;
};

VolPathAdaptive *CreateVolPathAdaptive(const ParamSet &params,
                                       std::shared_ptr<Sampler> sampler,
                                       std::shared_ptr<const Camera> camera);

}  // namespace pbrt

#endif  // PBRT_INTEGRATORS_ADAPTIVE_H
