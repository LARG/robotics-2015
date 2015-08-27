#include <common/RobotInfo.h>

ImageParams::ImageParams(Camera::Type camera) {
  width = 320;
  height = 240;
  defaultHorizontalStepScale = 0;
  defaultVerticalStepScale = 0;

  size = width * height;
  rawSize = size * 2;
  factor = width / 160;
  origFactor = width / 640.0f;
}
