#include <memory/ImageBlock.h>
  
ImageBlock::ImageBlock() : top_params_(Camera::TOP), bottom_params_(Camera::BOTTOM) {
  header.version = 3;
  header.size = sizeof(ImageBlock);
  img_top_local_ = new unsigned char[top_params_.rawSize];
  img_bottom_local_ = new unsigned char[bottom_params_.rawSize];
  img_top_ = NULL;
  img_bottom_ = NULL;
  loaded_ = false;
}

ImageBlock::ImageBlock(const ImageBlock& other) : ImageBlock() {
  log_block = other.log_block;
  owner = other.owner;
  buffer_logging_ = other.buffer_logging_;
  *this = other;
}

ImageBlock::~ImageBlock() {
  delete [] img_top_local_;
  delete [] img_bottom_local_;
  img_top_local_ = img_bottom_local_ = NULL;
  img_top_ = img_bottom_ = NULL;
}

ImageBlock& ImageBlock::operator=(const ImageBlock& other) {
  header = other.header;
  if(top_params_.rawSize != other.top_params_.rawSize) {
    delete [] img_top_local_;
    top_params_ = other.top_params_;
    img_top_local_ = new unsigned char[top_params_.rawSize];
  }
  if(bottom_params_.rawSize != other.bottom_params_.rawSize) {
    delete [] img_bottom_local_;
    bottom_params_ = other.bottom_params_;
    img_bottom_local_ = new unsigned char[bottom_params_.rawSize];
  }

  memcpy(img_top_local_, other.img_top_local_, top_params_.rawSize);
  memcpy(img_bottom_local_, other.img_bottom_local_, bottom_params_.rawSize);
  img_top_ = img_top_local_;
  img_bottom_ = img_bottom_local_;
  loaded_ = other.loaded_;
  return *this;
}
  
void ImageBlock::serialize(StreamBuffer& buffer, std::string data_dir) {
  std::vector<StreamBuffer> all;

  StreamBuffer main;
  main.read((unsigned char*)&header, sizeof(MemoryBlockHeader));
  all.push_back(main);

  StreamBuffer tparams, bparams;
  tparams.read((unsigned char*)&top_params_, sizeof(ImageParams));
  all.push_back(tparams);
  bparams.read((unsigned char*)&bottom_params_, sizeof(ImageParams));
  all.push_back(bparams);

  StreamBuffer imageLoaded;
  imageLoaded.read((unsigned char*)&loaded_, sizeof(bool));
  all.push_back(imageLoaded);

  if(buffer_logging_) {
    StreamBuffer top;
    top.read(getImgTop(), top_params_.rawSize);
    all.push_back(top);

    StreamBuffer bottom;
    bottom.read(getImgBottom(), bottom_params_.rawSize);
    all.push_back(bottom);
  }
  else {
    std::stringstream ss;
    char buf[10];
    sprintf(buf, "%04d", header.frameid);
    ss << data_dir << "/" << buf << "top.yuv";
    writeImageBinary(getImgTop(), ss.str(), top_params_);
    //writeImage(getImgTop(), ss.str(), top_params_);
    ss.str("");
    ss << data_dir << "/" << buf << "bottom.yuv";
    writeImageBinary(getImgBottom(), ss.str(), bottom_params_);
    //writeImage(getImgBottom(), ss.str(), bottom_params_);
  }

  StreamBuffer::combine(all, buffer);
  StreamBuffer::clear(all);
}


void ImageBlock::writeImageBinary(const unsigned char *imgraw, std::string path, const ImageParams &iparams){
  ofstream out;
  out.open(path, ios::out | ios::binary);
  out.write((const char*)imgraw, iparams.rawSize);
  out.close();
}

void ImageBlock::readImageBinary(unsigned char* imgraw, std::string path, const ImageParams& iparams){
  ifstream in(path, ios::in | ios::binary);
  in.read((char*)imgraw, iparams.rawSize);
  in.close();
}


bool ImageBlock::deserialize(const StreamBuffer& buffer, std::string data_dir) {
  auto parts = buffer.separate();
  if(!validateHeader(parts[0])) {
    StreamBuffer::clear(parts);
    return false;
  }
  int tsize = top_params_.rawSize, bsize = bottom_params_.rawSize;
  parts[0].write(header);
  parts[1].write(top_params_);
  parts[2].write(bottom_params_);
  parts[3].write(loaded_);
  if(tsize < top_params_.rawSize) {
    delete [] img_top_local_;
    img_top_local_ = new unsigned char[top_params_.rawSize];
  }
  if(bsize < bottom_params_.rawSize) {
    delete [] img_bottom_local_;
    img_bottom_local_ = new unsigned char[bottom_params_.rawSize];
  }
  if(buffer_logging_) {
    parts[4].write(img_top_local_);
    parts[5].write(img_bottom_local_);
  }
  else {
    std::stringstream ss;
    char buf[10];
    sprintf(buf, "%04d", header.frameid);
    ss << data_dir << "/" << buf << "top.yuv";
    ifstream fExists(ss.str());
    if (fExists){
      fExists.close();
      readImageBinary(img_top_local_, ss.str(), top_params_);
      ss.str("");
      ss << data_dir << "/" << buf << "bottom.yuv";
      readImageBinary(img_bottom_local_, ss.str(), bottom_params_);
    }
    else {
      ss.str("");
      ss << data_dir << "/" << buf << "top.bmp";
      readImage(img_top_local_, ss.str(), top_params_);
      ss.str("");
      ss << data_dir << "/" << buf << "bottom.bmp";
      readImage(img_bottom_local_, ss.str(), bottom_params_);
    }
  }
  img_top_ = boost::interprocess::offset_ptr<unsigned char>(img_top_local_);
  img_bottom_ = boost::interprocess::offset_ptr<unsigned char>(img_bottom_local_);
  StreamBuffer::clear(parts);
  return true;
}
