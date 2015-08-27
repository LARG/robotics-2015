#pragma once

#include <boost/shared_ptr.hpp>
#include <alcommon/almodule.h>
#include <alaudio/alsoundextractor.h>

class Memory;
class AudioProcessingBlock;

class AudioWrapper : public AL::ALSoundExtractor {
  public:
    AudioWrapper(boost::shared_ptr<AL::ALBroker> broker, const std::string & name);
    virtual ~AudioWrapper();
    void init();
    void process(const int & nbOfChannels,
                 const int & nbrOfSamplesByChannel,
                 const AL_SOUND_FORMAT * buffer,
                 const AL::ALValue & timeStamp);
  private:
    AudioProcessingBlock *audio_block_;
    Memory* memory_;
};
