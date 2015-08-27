#include "LogReader.h"
#include <iostream>
#include <memory/ImageBlock.h>
#include <memory/RobotVisionBlock.h>
#include <memory/MemoryBlockOperations.h>

#define MAX_EXPECTED_MODULES_PER_MEMORY 40

LogReader::LogReader(const char *directory):
  using_buffers_(false) {
  directory_ = directory;
  filename_ = directory_ + "/frames.log";
  log_file_.open(filename_.c_str(),std::ios::binary);
  if (!good() || !mdata_.loadFromFile(directory_ + "/metadata.yaml")) {
    std::cout << "problem opening log" << std::endl << std::flush;
  }
}

LogReader::LogReader(const std::string& directory) : LogReader(directory.c_str()) { }

LogReader::LogReader(const StreamBuffer& buffer) :
  using_buffers_(true), main_buffer_(buffer) {
}

LogReader::~LogReader() {
  close();
}

bool LogReader::readMemoryHeader(const StreamBuffer& buffer, MemoryHeader& header) {
  std::vector<StreamBuffer> blockNames;
  StreamBuffer::separate(buffer, blockNames);
  if (blockNames.size() > MAX_EXPECTED_MODULES_PER_MEMORY) {
    std::cout << "LogReader::readMemoryHeader: BAD NUMBER OF BLOCKS: " << blockNames.size() << std::endl;
    return false;
  }
  for(unsigned int i = 0; i < blockNames.size(); i++) {
    std::string name = (const char*)blockNames[i].buffer;
    header.block_names.push_back(name);
  }
  StreamBuffer::clear(blockNames);
  return true;
}

Memory* LogReader::readFrame(int frame) {
  Memory* memory = new Memory(false,MemoryOwner::TOOL_MEM, 0, 1);
  MemoryHeader header;
  unsigned int position = mdata_.offsets[frame];
  log_file_.seekg(position);
  main_buffer_.read(log_file_);
  if(!readMemory(*memory)) {
    printf("Error reading frame %i\n", frame);
  }
  return memory;
}

bool LogReader::readMemory(Memory &memory, bool /*suppress_errors*/) {
  // get the header and see how many blocks we need to read
  auto buffers = main_buffer_.separate();

  MemoryHeader header;
  bool res;
  res = readMemoryHeader(buffers[0], header);
  if (!res) {
    StreamBuffer::clear(buffers);
    return false;
  }
  if (header.block_names.size()<1) {
    printf("Error: header has %i blocks\n", header.block_names.size());
    return false;
  }
  for (unsigned int i = 1; i < buffers.size(); i++) {
    std::string &id = header.block_names[i - 1];
    MemoryBlock *block = memory.getBlockPtrByName(id);
    if (block == NULL) {
      bool res = memory.addBlockByName(id);
      if (!res) {
        std::cout << "Adding block " << id << " failed, just skipping" << std::endl << std::flush;
        continue;
      }
      else {
        block = memory.getBlockPtrByName(id);
        assert(block != NULL); // really shouldn't happen
      }
    }
    block->buffer_logging_ = using_buffers_;
    block->log_block = true; // if we're reading a log we should be able to re-log it
    bool valid = true;
    if(id == "raw_image")
      valid = ((ImageBlock*)block)->deserialize(buffers[i], directory_);
    else if (id == "robot_vision")
      valid = ((RobotVisionBlock*)block)->deserialize(buffers[i], directory_);
    else
      valid = MEMORY_BLOCK_TEMPLATE_FUNCTION_CALL(id,block->deserialize,false,buffers[i]);
    if(!valid)
      fprintf(stderr, "Error deserializing %s\n", id.c_str());
  }
  StreamBuffer::clear(buffers);
  return true;
}

void LogReader::close() {
  if (!using_buffers_)
    log_file_.close();
}

bool LogReader::good() {
  if (using_buffers_) {
    return true;
  } else
    return log_file_.good();
}
