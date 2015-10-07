#pragma once

#include <math/Pose2D.h>
#include <common/Random.h>
#include <memory/MemoryCache.h>
#include <memory/LocalizationBlock.h>
#include <localization/Logging.h>

class ParticleFilter {
  public:
    ParticleFilter(MemoryCache& cache, TextLogger*& tlogger);
    void init(Point2D loc, float orientation);
    void processFrame();
    const Pose2D& pose() const;
    inline const std::vector<Particle>& particles() const {
      return cache_.localization_mem->particles;
    }

  private:
    inline std::vector<Particle>& particles() {
      return cache_.localization_mem->particles;
    }
    MemoryCache& cache_;
    Random rand_;
    mutable Pose2D mean_;
    mutable bool dirty_;
    TextLogger*& tlogger_;
};
