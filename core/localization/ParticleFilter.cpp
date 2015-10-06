#include <localization/ParticleFilter.h>
#include <memory/FrameInfoBlock.h>
#include <memory/OdometryBlock.h>

#define NUM_PARTICLES 100

ParticleFilter::ParticleFilter(MemoryCache& cache, TextLogger*& tlogger) 
  : cache_(cache), tlogger_(tlogger), dirty_(true) {
}

void ParticleFilter::init(Point2D loc, float orientation) {
  mean_.translation = loc;
  mean_.rotation = orientation;
}

void ParticleFilter::processFrame() {
  particles().resize(NUM_PARTICLES);
  auto frame = cache_.frame_info->frame_id;
  dirty_ = true;
  const auto& disp = cache_.odometry->displacement;
  log(41, "Updating particles from odometry: %2.f,%2.f @ %2.2f", disp.translation.x, disp.translation.y, disp.rotation * RAD_T_DEG);
  for(auto& p : particles()) {
    p.x = rand_.sampleN(frame * 5, 250);
    p.y = rand_.sampleN(0, 250);
    p.t = rand_.sampleN(0, M_PI / 4);
    p.w = rand_.sampleU();
  }
}

const Pose2D& ParticleFilter::pose() const {
  if(dirty_) {
    mean_ = Pose2D();
    using T = decltype(mean_.translation);
    for(const auto& p : particles()) {
      mean_.translation += T(p.x,p.y);
      mean_.rotation += p.t;
    }
    if(particles().size() > 0)
      mean_ /= particles().size();
    dirty_ = false;
  }
  return mean_;
}
