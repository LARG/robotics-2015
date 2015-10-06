#include <localization/ParticleFilter.h>
#include <memory/FrameInfoBlock.h>

#define NUM_PARTICLES 100

ParticleFilter::ParticleFilter(MemoryCache& cache) : cache_(cache), dirty_(true) {
}

void ParticleFilter::init(Point2D loc, float orientation) {
  mean_.translation = loc;
  mean_.rotation = orientation;
}

void ParticleFilter::processFrame() {
  particles().resize(NUM_PARTICLES);
  auto frame = cache_.frame_info->frame_id;
  dirty_ = true;
  for(auto& p : particles()) {
    p.x = rand_.sampleN(frame, 250);
    p.y = rand_.sampleN(0, 250);
    p.t = rand_.sampleN(0, M_PI / 4);
    p.w = rand_.sampleU();
  }
}

const Pose2D& ParticleFilter::pose() const {
  if(dirty_) {
    for(const auto& p : particles())
      mean_ += Pose2D(p.t, p.x, p.y) * p.w;
    if(particles().size() > 0)
      mean_ /= particles().size();
    dirty_ = false;
  }
  return mean_;
}
