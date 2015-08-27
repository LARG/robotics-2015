#ifndef LOGREADER_ZN55JIC8
#define LOGREADER_ZN55JIC8

#include <fstream>
#include <cstring>
#include <vector>
#include <memory/LogMetadata.h>
#include <memory/StreamBuffer.h>
#include <memory/Memory.h>
#include <memory/MemoryBlock.h>

class LogReader {
  public:
    LogReader (const char *directory);
    LogReader (const std::string& directory);
    LogReader (const StreamBuffer& buffer);
    ~LogReader ();
    
    Memory* readFrame(int frame);
    bool readMemory(Memory &memory, bool suppress_errors = false);
    const LogMetadata& mdata() const { return mdata_; }

    const std::string& directory() { return directory_; }

  private:

    bool readMemoryHeader(const StreamBuffer& buffer, MemoryHeader &header);
    bool readBlock(const MemoryBlockHeader &header,MemoryBlock &module);
    void readAndIgnoreBlock(const MemoryBlockHeader &header);
    void close();

    std::ifstream log_file_;
    bool using_buffers_;
    std::string filename_;
    std::string directory_;
    LogMetadata mdata_;

    void read();
    bool good();

    StreamBuffer main_buffer_;
};

#endif /* end of include guard: LOGREADER_ZN55JIC8 */
