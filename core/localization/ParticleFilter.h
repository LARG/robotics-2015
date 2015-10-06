#pragma once

#include <math/Pose2D.h>
#include <common/Random.h>
#include <memory/MemoryCache.h>
#include <memory/LocalizationBlock.h>

class ParticleFilter {
  public:
    ParticleFilter(MemoryCache& cache);
    void init(Point2D loc, float orientation);
    void processFrame();
    const Pose2D& pose() const;
    inline const auto& particles() const {
      return cache_.localization_mem->particles;
    }

  private:
    inline auto& particles() {
      return cache_.localization_mem->particles;
    }
    MemoryCache& cache_;
    Random rand_;
    mutable Pose2D mean_;
    mutable bool dirty_;
};
