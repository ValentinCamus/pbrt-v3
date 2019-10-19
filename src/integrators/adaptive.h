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
    // Three modes are available to render the image:
    // - normal: render using an uniform number of samples throughout the image
    // - error: render with an adaptive number of samples according to the error
    // - time: render with a fixed target computation time
    enum class Mode {NORMAL, ERROR, TIME};

    // A steady clock gives the most robust estimation of computation time
    using Clock = std::chrono::steady_clock;

    // The _SamplingFunctor_ is given by the integrator to the _Statistics_
    // instance. It allows the _Statistics_ instance to draw an arbitrary number
    // of samples at a given pixel. The functor does not take any parameter and
    // returns a Spectrum value, thus its signature is Spectrum(void).
    using SamplingFunctor = std::function<Spectrum()>;

public:
    // The object keeps a 2D grid _Pixel_s up to date during the render. A
    // _Pixel_ holds the three base statistics needed to evaluate statistics:
    // - number of samples that were drawn at the pixel
    // - mean of the sample distribution
    // - second moment of the sample distribution
    struct Pixel {
        long samples = 0;
        Spectrum mean = 0;
        Spectrum moment2 = 0;
    };

    Statistics(const ParamSet &paramSet, const Film *originalFilm,
               const Sampler &sampler);

    // Utility functions that are called only once during the render
    void RenderBegin();
    void RenderEnd() const;
    void WriteImages() const;

    // Determines if a new batch should be rendered. By default, only one batch
    // will be rendered regardless of the total rendering time and batch index.
    // @param number The index of the batch that is about to be rendered
    // @return If the next batch should be rendered
    bool StartNextBatch(int number);

    // @return the number of samples a batch contains
    long BatchSize() const;

    // This function controls the rendering process using the information that
    // is given by the integrator and the statistics. By default, it just draw
    // the same number of samples throughout the image while updating the
    // statistics.
    // @param pixel The pixel being rendered, rounded to the nearest position
    // @param sampleOnce The functor given by the integrator. Calling it draw
    //                   generates exactly one random sample. Usually a lambda.
    void SamplingLoop(Point2i pixel, const SamplingFunctor &sampleOnce);

    // Update the statistics grid to account for a newly sampled value.
    // @param pixel The pixel where the samples was drawn
    // @param L The newly obtained _Spectrum_ value
    void UpdateStats(Point2i pixel, Spectrum &&L);

    // @param statsPixel The three base statistics
    // @return Either the total number of drawn samples, or the ratio of
    //         additional samples that were used
    Float Sampling(const Pixel &statsPixel) const;

    // @param statsPixel The three base statistics
    // @return The estimated variance
    Spectrum Variance(const Pixel &statsPixel) const;

    // @param statsPixel The three base statistics
    // @return The estimation error according to the chosen heuristic
    Spectrum Error(const Pixel &statsPixel) const;

    // A criterion to determine if a given pixel has converged or not.
    // @return If the pixel should stop being rendered, e.g. if the maximum
    //         number of samples was reached || the error is below the threshold
    bool StopCriterion(Point2i pixel) const;

    // @return The elapsed time from the beginning of the render, is ms
    long ElapsedMilliseconds() const;

    // Functions used to setup and update the progress bar
    std::string WorkTitle() const;
    long UpdateWork();

private:
    // The rendering mode indicated in the .pbrt scene file
    const Mode mode;

    // For all modes, the maximum number of samples that can be drawn
    const long maxSamples;

    // For error mode:
    // - the minimum number of samples used to estimate the statistics
    const long minSamples;
    // - the error threshold that should be reached by each pixel
    const float errorThreshold;
    // - the chosen error heuristic (relative, standard, confidence...)
    const std::string errorHeuristic;

    // For time mode:
    // - the number of samples a batch contains
    const long batchSize;
    // - the target time
    const long targetSeconds;

    // Original film used to construct the statistics images
    const Film *originalFilm;
    // Film size
    const Bounds2i pixelBounds;
    // Pixel data updated with each sample, and exported after the render
    std::unique_ptr<Pixel[]> pixels;

    // Starting time of the render
    Clock::time_point startTime;
    // A boolean value used to render exactly one batch in normal / error modes
    bool batchOnce = true;

    // Access functions to the stored pixels grid;
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

    // The only methods required by the _Integrator_ interface
    void Render(const Scene &scene) override;

private:
    // An instance of _Statistics_ that will compute convergence statistics and
    // control the rendering loop
    Statistics stats;
    std::shared_ptr<Sampler> sampler;
    const Bounds2i pixelBounds;
};

VolPathAdaptive *CreateVolPathAdaptive(const ParamSet &params,
                                       std::shared_ptr<Sampler> sampler,
                                       std::shared_ptr<const Camera> camera);

}  // namespace pbrt

#endif  // PBRT_INTEGRATORS_ADAPTIVE_H