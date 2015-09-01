#include <memory/StreamBuffer.h>

#define VERIFY_RESIZE \
  if(n == 0 || n > 10e8) {\
    fprintf(stderr, "INVALID BUFFER REQUEST: %i!! The log file is most likely broken\n", n);\
    return;\
  }\

StreamBuffer::StreamBuffer() : size(0), capacity(0), buffer(NULL) {
}

StreamBuffer::StreamBuffer(unsigned char* b, unsigned int n) : size(n), capacity(n), buffer(b) {
}

StreamBuffer::StreamBuffer(char* b, unsigned int n) : size(n), capacity(n), buffer((unsigned char*)b) {
}

void StreamBuffer::clear() {
  if(buffer != NULL) delete [] buffer;
  capacity = 0;
  size = 0;
  buffer = NULL;
}

void StreamBuffer::rclear() {
  clear();
  for(auto child : children)
    child.rclear();
  children.clear();
}

void StreamBuffer::resize(unsigned int n) {
  if(capacity < n) {
    if(buffer != NULL)
      delete [] buffer;
    buffer = new unsigned char[n];
    capacity = n;
  }
}

void StreamBuffer::reset() {
  size = 0;
}

void StreamBuffer::add(const char* data, unsigned int n) {
  StreamBuffer sb;
  sb.read(data, n);
  children.push_back(sb);
}

void StreamBuffer::add(const unsigned char* data, unsigned int n) {
  StreamBuffer sb;
  sb.read(data, n);
  children.push_back(sb);
}

void StreamBuffer::add(std::istream& is, unsigned int n) {
  StreamBuffer sb;
  sb.read(is, n);
  children.push_back(sb);
}

void StreamBuffer::add(std::istream& is) {
  StreamBuffer sb;
  sb.read(is);
  children.push_back(sb);
}

void StreamBuffer::read(const unsigned char* data, unsigned int n) {
  VERIFY_RESIZE;
  resize(n);
  memcpy(buffer, data, n);
  size = n;
}

void StreamBuffer::read(const char* data, unsigned int n) {
  VERIFY_RESIZE;
  resize(n);
  memcpy((char*)buffer, data, n);
  size = n;
}

void StreamBuffer::read(std::istream& is, unsigned int n) {
  VERIFY_RESIZE;
  resize(n);
  is.read((char*)buffer, n);
  size = n;
}

void StreamBuffer::read(std::istream& is) {
  unsigned int n;
  is.read((char*)&n, sizeof(unsigned int));
  read(is, n);
}
  
void StreamBuffer::write(std::ostream& os) {
  write(os, this->size);
}

void StreamBuffer::write(std::ostream& os, unsigned int n) {
  os.write((char*)&n, sizeof(unsigned int));
  os.write((char*)this->buffer, n);
}

StreamBuffer StreamBuffer::combine(const std::vector<StreamBuffer>& buffers) {
  StreamBuffer sb;
  combine(buffers, sb);
  return sb;
}

void StreamBuffer::combine(const std::vector<StreamBuffer>& buffers, StreamBuffer& combined) {
  unsigned int totalSize = 0;
  for(auto buffer : buffers)
    totalSize += buffer.size;

  combined.resize(totalSize + buffers.size() * sizeof(unsigned int) + sizeof(unsigned int));
  unsigned int offset = 0;
  assert(buffers.size() < 0xFFFFFF);
  unsigned int size = buffers.size();
  memcpy((char*)combined.buffer, &size, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  for(auto& buffer : buffers) {
    unsigned int *pieceSize = (unsigned int*)(combined.buffer + offset);
    *pieceSize = buffer.size;
    offset += sizeof(unsigned int);
    memcpy(combined.buffer + offset, buffer.buffer, buffer.size);
    offset += buffer.size;
  }
  combined.size = offset;
}

void StreamBuffer::combine(const std::list<StreamBuffer>& buffers, StreamBuffer& combined) {
  unsigned int totalSize = 0;
  for(auto buffer : buffers)
    totalSize += buffer.size;

  combined.resize(totalSize + buffers.size() * sizeof(unsigned int) + sizeof(unsigned int));
  unsigned int offset = 0;
  assert(buffers.size() < 0xFFFFFFFF);
  unsigned int size = buffers.size();
  memcpy((char*)combined.buffer, &size, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  for(auto& buffer : buffers) {
    unsigned int *pieceSize = (unsigned int*)(combined.buffer + offset);
    *pieceSize = buffer.size;
    offset += sizeof(unsigned int);
    memcpy(combined.buffer + offset, buffer.buffer, buffer.size);
    offset += buffer.size;
  }
  combined.size = offset;
}

std::vector<StreamBuffer> StreamBuffer::separate() const {
  std::vector<StreamBuffer> separated;
  separate(*this, separated);
  return separated;
}

void StreamBuffer::contract() {
  combine(children, *this);
  clear(children);
  children.clear();
}

void StreamBuffer::expand() {
  separate(*this, children);
  clear();
}

void StreamBuffer::separate(const StreamBuffer& buffer, std::vector<StreamBuffer>& separated) {
  unsigned int offset = 0;
  unsigned int bufCount;
  memcpy((char*)&bufCount, buffer.buffer, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  while(separated.size() < bufCount) {
    StreamBuffer piece;
    unsigned int *pieceSize = (unsigned int*)(buffer.buffer + offset);
    offset += sizeof(unsigned int);
    piece.read(buffer.buffer + offset, *pieceSize);
    offset += piece.size;
    separated.push_back(piece);
  }
}

void StreamBuffer::separate(const StreamBuffer& buffer, std::list<StreamBuffer>& separated) {
  unsigned int offset = 0;
  unsigned int bufCount;
  memcpy((char*)&bufCount, buffer.buffer, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  while(separated.size() < bufCount) {
    StreamBuffer piece;
    unsigned int *pieceSize = (unsigned int*)(buffer.buffer + offset);
    offset += sizeof(unsigned int);
    piece.read(buffer.buffer + offset, *pieceSize);
    offset += piece.size;
    separated.push_back(piece);
  }
}

void StreamBuffer::clear(std::vector<StreamBuffer>& buffers) {
  for(auto sb : buffers) sb.clear();
}

void StreamBuffer::clear(std::list<StreamBuffer>& buffers) {
  for(auto sb : buffers) sb.clear();
}

