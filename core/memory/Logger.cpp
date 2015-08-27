#include "Logger.h"
#include <iostream>
#include <common/File.h>
#include <memory/ImageBlock.h>
#include <memory/RobotVisionBlock.h>

Logger::Logger(bool useBuffers, const char* directory, bool appendUniqueId, bool useAllBlocks):
  using_buffers_(useBuffers), use_all_blocks_(useAllBlocks)
{
  if (directory) {
    open(directory, appendUniqueId);
  }
}

Logger::~Logger() {
  if(using_buffers_) {
    if(main_buffer_.buffer)
      delete [] main_buffer_.buffer;
  }
  close();
}

bool Logger::isOpen() {
  return log_file_.is_open();
}

void Logger::clearBuffer() {
  main_buffer_.reset();
}

void Logger::writeMemoryHeader(const MemoryHeader &header, StreamBuffer& buffer) {
  unsigned int num_blocks = header.block_names.size();
  std::vector<StreamBuffer> buffers;
  for (unsigned int i = 0; i < num_blocks; i++) {
    StreamBuffer bbuffer;
    bbuffer.read(header.block_names[i].c_str(), header.block_names[i].size() + 1);
    buffers.push_back(bbuffer);
  }
  StreamBuffer::combine(buffers, buffer);
}

void Logger::writeMemory(Memory &memory) {
  if (using_buffers_ || log_file_.is_open()) {
    mdata_.frames++;
    mdata_.offsets.push_back(log_file_.tellp());
    MemoryHeader header;
    MemoryBlock *block;
    // first get a list of the blocks we're loading
    memory.getBlockNames(header.block_names, !use_all_blocks_);
    // write the header
    StreamBuffer hbuffer;
    writeMemoryHeader(header, hbuffer);
    // write each block
    std::vector<StreamBuffer> buffers;
    buffers.push_back(hbuffer);
    for (unsigned int i = 0; i < header.block_names.size(); i++) {
      std::string& id = header.block_names[i];
      block = memory.getBlockPtrByName(id);
      block->buffer_logging_ = using_buffers_;
      block->header.frameid = frame_id_;
      StreamBuffer sb;
      if(id == "raw_image")
        ((ImageBlock*)block)->serialize(sb, directory_);
      else if(id == "robot_vision")
        ((RobotVisionBlock*)block)->serialize(sb, directory_);
      else
        block->serialize(sb);
      buffers.push_back(sb);
    }
    StreamBuffer::combine(buffers, main_buffer_);
    if(!using_buffers_) {
      write();
      // Write out metadata every 100 frames just in case of a crash
      if(mdata_.frames % 100 == 0) mdata_.saveToFile(directory_ + "/metadata.yaml");
    }
    StreamBuffer::clear(buffers);
    frame_id_++;
  }
}

void Logger::open(const char *directory, bool appendUniqueId) {
  close(); // any previously opened logs
  mdata_.frames = 0;
  mdata_.offsets.clear();
  if (appendUniqueId) {
    directory_ = generateDirectoryName(directory);
  } else {
    directory_ = directory;
  }
  mkdir_recursive(directory_.c_str());
  filename_ = directory_ + "/frames.log";
  printf("Logging to file: %s\n", filename_.c_str());
  log_file_.open(filename_.c_str(), std::ios::binary);
}

void Logger::mkdir_recursive(const char *dir) {
  char tmp[256];
  char *p = NULL;
  size_t len;

  snprintf(tmp, sizeof(tmp),"%s",dir);
  len = strlen(tmp);
  if(tmp[len - 1] == '/')
    tmp[len - 1] = 0;
  for(p = tmp + 1; *p; p++)
    if(*p == '/') {
      *p = 0;
      mkdir(tmp, S_IRWXU);
      *p = '/';
    }
  mkdir(tmp, S_IRWXU);
}

void Logger::close() {
  if (log_file_.is_open()) {
    std::cout << "Closing log file" << std::endl;
    log_file_.close();
    mdata_.saveToFile(directory_ + "/metadata.yaml");
  }
  frame_id_ = 0;
}

void Logger::setType(int t){
  type_ = t;
}

std::string Logger::generateDirectoryName(const char* basename) {
  std::string robotbase = "/home/nao/logs/";
  std::string base = robotbase;
  if (type_ == CORE_SIM) {
    std::string naohome = getenv("NAO_HOME");
    std::string simbase = naohome + "/logs/";
    base = simbase;
  } else if (type_ == CORE_TOOL) {
    std::string naohome = getenv("NAO_HOME");
    std::string toolbase = naohome + "/logs/";
    base = toolbase;
  }

  std::string directory = base + std::string(basename) + generateTimestamp();
  return directory;
}

void Logger::write() {
  log_file_.write((const char*)&(main_buffer_.size), sizeof(int));
  log_file_.write((const char*)main_buffer_.buffer, main_buffer_.size);
}

int Logger::frame_id_ = 0;
