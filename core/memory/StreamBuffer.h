#ifndef STREAM_BUFFER_H
#define STREAM_BUFFER_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <vector>
#include <list>

struct StreamBuffer {
  unsigned int size;
  unsigned int capacity;
  unsigned char* buffer;
  std::list<StreamBuffer> children;

  StreamBuffer();
  StreamBuffer(unsigned char* b, unsigned int n);
  StreamBuffer(char* b, unsigned int n);
  void clear();
  void rclear();
  void resize(unsigned int n);
  void reset();
  void add(const unsigned char* data, unsigned int n);
  void add(const char* data, unsigned int n);
  void add(std::istream& is, unsigned int n);
  void add(std::istream& is);
  void read(const unsigned char* data, unsigned int n);
  void read(const char* data, unsigned int n);
  void read(std::istream& is, unsigned int n);
  void read(std::istream& is);
  template<typename T>
  void read(const std::vector<T>& v) {
    read((unsigned char*)v.data(), sizeof(T) * v.size());
  }
  template<typename T>
  void write(T& dest) {
    write(&dest);
  }
  template<typename T>
  void write(T* dest) {
    if(children.size()) {
      auto sb = children.front();
      children.pop_front();
      sb.write(dest);
    } else {
      memcpy((unsigned char*)dest, this->buffer, this->size);
    }
  }
  template<typename T>
  void write(std::vector<T>& dest) {
    int n = this->size / sizeof(T);
    dest.resize(n);
    memcpy((unsigned char*)dest.data(), this->buffer, this->size);
  }
  void write(std::ostream& os);
  void write(std::ostream& os, unsigned int n);
  void contract();
  void expand();
  std::vector<StreamBuffer> separate() const;
  static StreamBuffer combine(const std::vector<StreamBuffer>& buffers);
  static void combine(const std::vector<StreamBuffer>& buffers, StreamBuffer& combined);
  static void separate(const StreamBuffer& buffer, std::vector<StreamBuffer>& separated);
  static void clear(std::vector<StreamBuffer>& buffers);
  static void combine(const std::list<StreamBuffer>& buffers, StreamBuffer& combined);
  static void separate(const StreamBuffer& buffer, std::list<StreamBuffer>& separated);
  static void clear(std::list<StreamBuffer>& buffers);
};

#endif
