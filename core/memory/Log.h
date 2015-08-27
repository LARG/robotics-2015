#ifndef LOG_H
#define LOG_H

#include <memory/Memory.h>
#include <memory/LogReader.h>
#include "ImageBlock.h"
#include <common/RobotInfo.h>
#include <vector>

class Log {
  public:
    Log(std::string directory, int start = 0, int finish = -1);
    ~Log();
    inline Memory& operator[] (unsigned int idx) {
      return getFrame(idx);
    }
    Memory& getFrame(unsigned int idx, bool manage = true);
    std::vector<ImageParams> getTopParams();
    std::vector<ImageParams> getBottomParams();
    std::vector<unsigned char*> getRawTopImages();
    std::vector<unsigned char*> getRawBottomImages();
    unsigned int size() { return finish_ - start_ + 1; }
    const std::string& directory() const { return directory_; }
    bool& enableCache() { return enableCache_; }

  private:
    std::string directory_;
    LogReader reader_;
    LogMetadata mdata_;
    int start_, finish_;
    std::map<unsigned int, Memory*> cache_;
    bool enableCache_;
    Memory* memory_;
};

#endif
