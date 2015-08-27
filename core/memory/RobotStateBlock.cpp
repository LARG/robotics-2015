#include <memory/RobotStateBlock.h>

RobotStateBlock::RobotStateBlock() : WO_SELF(5), team_(TEAM_BLUE), role_(WO_SELF) {
  header.version = 6;
  header.size = sizeof(RobotStateBlock);
  robot_id_ = 0;
  team_changed_ = true;
  ignore_comms_ = false;
  strncpy(body_id_, "<NO BODY ID SET>", sizeof(body_id_));
  head_version_ = body_version_ = 40;
}
