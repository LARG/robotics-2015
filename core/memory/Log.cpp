#include <memory/Log.h>

Log::Log(std::string directory, int start, int finish) : directory_(directory), reader_(directory), memory_(NULL) {
  start_ = start;
  mdata_ = reader_.mdata();
  enableCache_ = false;
  if(finish > 0)
    finish_ = std::min((int)mdata_.frames - 1, finish);
  else
    finish_ = mdata_.frames - 1;
}

Log::~Log() {
  for(auto& kvp : cache_)
    delete kvp.second;
}

std::vector<ImageParams> Log::getTopParams() {
  std::vector<ImageParams> params;
  int count = size();
  for(int i = 0; i < count; i++){
    Memory* m = &getFrame(i, false);
    ImageBlock* block = 0;
    m->getBlockByName(block, "raw_image", false);
    if (block != NULL)
      params.push_back(block->top_params_);
    delete m;
  }
  return params;
}

std::vector<ImageParams> Log::getBottomParams() {
  std::vector<ImageParams> params;
  int count = size();
  for(int i = 0; i < count; i++){
    Memory* m = &getFrame(i, false);
    ImageBlock* block = 0;
    m->getBlockByName(block, "raw_image", false);
    if (block != NULL)
      params.push_back(block->bottom_params_);
    delete m;
  }
  return params;
}

std::vector<unsigned char*> Log::getRawTopImages() {
  std::vector<unsigned char*> images;
  int count = size();
  for(int i = 0; i < count; i++){
    Memory* m = &getFrame(i, false);
    ImageBlock* block = 0;
    m->getBlockByName(block, "raw_image", false);
    if (block != NULL) {
      unsigned char* cpy = new unsigned char[block->top_params_.rawSize];
      memcpy(cpy, block->img_top_.get(), block->top_params_.rawSize);
      images.push_back(cpy);
    }
    delete m;
  }
  return images;
}

std::vector<unsigned char*> Log::getRawBottomImages() {
  std::vector<unsigned char*> images;
  int count = size();
  for(int i = 0; i < count; i++){
    Memory* m = &getFrame(i, false);
    ImageBlock* block = 0;
    m->getBlockByName(block, "raw_image", false);
    if (block != NULL) {
      unsigned char* cpy = new unsigned char[block->bottom_params_.rawSize];
      memcpy(cpy, block->img_bottom_.get(), block->bottom_params_.rawSize);
      images.push_back(cpy);
    }
    delete m;
  }
  return images;
}

Memory& Log::getFrame(unsigned int idx, bool manage) {
  int frame = idx + start_;
  if(frame >= mdata_.frames || frame < 0)
    fprintf(stderr, "INVALID LOG FRAME REQUESTED: %i --> %i\n", idx, frame);
  if(!manage)
    return *reader_.readFrame(frame);
  if(enableCache_) {
    Memory* memory;
    if(cache_.find(frame) == cache_.end()) {
      memory = reader_.readFrame(frame);
      cache_[frame] = memory;
    }
    else {
      memory = cache_[frame];
    }
    return *memory;
  } else {
    if(memory_) delete memory_;
    memory_ = reader_.readFrame(frame);
    return *memory_;
  }
}
