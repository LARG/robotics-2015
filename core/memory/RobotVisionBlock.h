#ifndef ROBOTVISIONBLOCK_
#define ROBOTVISIONBLOCK_

#include "MemoryBlock.h"
#include <common/RobotInfo.h>
#include <constants/ImageConstants.h>
#include <math/Geometry.h>
#include <vision/structures/HorizonLine.h>
#include <boost/interprocess/offset_ptr.hpp>
#include <vision/enums/Colors.h>
#define MAX_VISION_OPPS 10

struct RobotVisionBlock : public MemoryBlock {

  enum VisionBodyPoints {
    LEFT_SHOULDER,
    RIGHT_SHOULDER,
    LEFT_FOOT_TL,
    LEFT_FOOT_TR,
    LEFT_FOOT_BL,
    LEFT_FOOT_BR,
    RIGHT_FOOT_TL,
    RIGHT_FOOT_TR,
    RIGHT_FOOT_BL,
    RIGHT_FOOT_BR,
    NUM_VISION_BODY_POINTS
  };

private:
  unsigned char *segImgTopLocal, *segImgBottomLocal;
  boost::interprocess::offset_ptr<unsigned char> segImgTop;
  boost::interprocess::offset_ptr<unsigned char> segImgBottom;

public:
  ImageParams top_params_, bottom_params_;
  bool loaded_;
  RobotVisionBlock();
  RobotVisionBlock(const RobotVisionBlock& other);
  ~RobotVisionBlock();
  RobotVisionBlock& operator=(const RobotVisionBlock& other);

  inline const unsigned char* getSegImgTop() const { return segImgTop.get(); }
  inline const unsigned char* getSegImgBottom() const { return segImgBottom.get(); }
  inline unsigned char* getSegImgTop() { return segImgTop.get(); }
  inline unsigned char* getSegImgBottom() { return segImgBottom.get(); }
  inline void setSegImgTop(unsigned char* img) { segImgTop = img; }
  inline void setSegImgBottom(unsigned char* img) { segImgBottom = img; }

  void serialize(StreamBuffer& buffer, std::string);
  bool deserialize(const StreamBuffer& buffer, std::string);

  bool doHighResBallScan;
  bool lookForCross;

  double reported_head_stop_time;
  double reported_head_moving;
  float bottomGreenPct, topGreenPct;
  float bottomUndefPct, topUndefPct;

  Point2D bodyPointsImage[NUM_VISION_BODY_POINTS];
  HorizonLine horizon;

};

#endif
