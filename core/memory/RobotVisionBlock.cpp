#include <memory/RobotVisionBlock.h>
RobotVisionBlock::RobotVisionBlock() : top_params_(Camera::TOP), bottom_params_(Camera::BOTTOM) {
  header.version = 5;
  header.size = sizeof(RobotVisionBlock);
  reported_head_stop_time = 0.0;
  reported_head_moving = false;
  segImgBottomLocal = new unsigned char[bottom_params_.size];
  segImgTopLocal = new unsigned char[top_params_.size];
  segImgTop = NULL;
  segImgBottom = NULL;
  loaded_ = false;
  bottomGreenPct = topGreenPct = 0;
  bottomUndefPct = topUndefPct = 0;
}

RobotVisionBlock::RobotVisionBlock(const RobotVisionBlock& other) : RobotVisionBlock() {
  log_block = other.log_block;
  owner = other.owner;
  buffer_logging_ = other.buffer_logging_;
  *this = other;
}

RobotVisionBlock::~RobotVisionBlock() {
  delete [] segImgTopLocal;
  delete [] segImgBottomLocal;
}

RobotVisionBlock& RobotVisionBlock::operator=(const RobotVisionBlock& other) {
  header = other.header;
  if(top_params_.size != other.top_params_.size) {
    delete [] segImgTopLocal;
    top_params_ = other.top_params_;
    segImgTopLocal = new unsigned char[top_params_.size];
  }
  if(bottom_params_.size != other.bottom_params_.size) {
    delete [] segImgBottomLocal;
    bottom_params_ = other.bottom_params_;
    segImgBottomLocal = new unsigned char[bottom_params_.size];
  }

  if(other.segImgTop != NULL)
    memcpy(segImgTopLocal, other.getSegImgTop(), top_params_.size);
  segImgTop = segImgTopLocal;
  if(other.segImgBottom != NULL)
    memcpy(segImgBottomLocal, other.getSegImgBottom(), bottom_params_.size);
  segImgBottom = segImgBottomLocal;
  
  horizon = other.horizon;
  loaded_ = true;
  return *this;
}

void RobotVisionBlock::serialize(StreamBuffer& buffer, std::string) {
  std::vector<StreamBuffer> all;

  StreamBuffer main;
  main.read((unsigned char*)&header, sizeof(MemoryBlockHeader));
  all.push_back(main);

  StreamBuffer tparams, bparams;
  tparams.read((unsigned char*)&top_params_, sizeof(ImageParams));
  all.push_back(tparams);
  bparams.read((unsigned char*)&bottom_params_, sizeof(ImageParams));
  all.push_back(bparams);

  StreamBuffer top;
  top.read(getSegImgTop(), top_params_.size);
  // Fill in values for the segmented image based on last classified pixel
  for (int i = 0; i < top_params_.size; i++){
    if (top.buffer[i] == c_UNDEFINED){
      int x = (i % top_params_.width);
      x -= (x % (1 << top_params_.defaultHorizontalStepScale));
      int y = (i / top_params_.width);
      y -= (y % (1 << top_params_.defaultVerticalStepScale));
      top.buffer[i] = top.buffer[x + (y * top_params_.width)];
    }
  }

  all.push_back(top);

  StreamBuffer bottom;
  bottom.read(getSegImgBottom(), bottom_params_.size);
  all.push_back(bottom);

  StreamBuffer hbuff;
  hbuff.read((unsigned char*)&horizon, sizeof(HorizonLine));
  all.push_back(hbuff);

  StreamBuffer::combine(all, buffer);
  StreamBuffer::clear(all);
}

bool RobotVisionBlock::deserialize(const StreamBuffer& buffer, std::string) {
  auto parts = buffer.separate();
  if(!validateHeader(parts[0])) {
    StreamBuffer::clear(parts);
    return false;
  }
  if(top_params_.rawSize < parts[3].size) {
    delete [] segImgTopLocal;
    segImgTopLocal = new unsigned char[parts[3].size];
  }
  if(bottom_params_.rawSize < parts[4].size) {
    delete [] segImgBottomLocal;
    segImgBottomLocal = new unsigned char[parts[4].size];
  }

  parts[0].write(header);
  parts[1].write(top_params_);
  parts[2].write(bottom_params_);
  parts[3].write(segImgTopLocal);
  parts[4].write(segImgBottomLocal);
  parts[5].write(horizon);
  loaded_ = true;

  segImgTop = segImgTopLocal;
  segImgBottom = segImgBottomLocal;
  StreamBuffer::clear(parts);
  return true;
}
