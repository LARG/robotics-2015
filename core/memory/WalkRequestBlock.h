#ifndef WALKREQUESTBLOCK_RE8SDRLN
#define WALKREQUESTBLOCK_RE8SDRLN

#include <math/Pose2D.h>
#include "MemoryBlock.h"
#include <common/InterfaceInfo.h>

struct WalkRequestBlock : public MemoryBlock {
public:
  ENUM(Motion,
    WALK,
    STAND,
    FALLING,
    GETUP,
    FALL_PREVENTION,
    NONE,
    STEP_LEFT,
    STEP_RIGHT,
    WAIT,
    STAND_STRAIGHT,
    NUM_OF_MOTIONS
  );

  WalkRequestBlock();

  void set(Motion m, Pose2D speed, bool percentage_speed, bool pedantic);
  void noWalk();
  void stand();
  void standStraight();
  void wait();
  void setStep(bool isLeft, float x, float y, float rotation);
  void setWalk(float x, float y, float rotation);
  void setPedanticWalk(float x, float y, float rotation);
  void setFalling();
  void setKick(float distance, float heading, bool with_left, bool step_into_kick);
  void setOdometryOffsets(float fwd, float side, float turn);
  void setWalkTarget(float relx, float rely, float relang, bool pedantic = false);
  void setKickStepParams(int type, const Pose2D &preStep, const Pose2D &step, float refX);
  void setWalkType(WalkType type);
  bool isStandingStraight() const { return motion_ == STAND_STRAIGHT; }

  bool new_command_;
  bool start_command_;
  Motion motion_; // what type of motion, walk/stand/kick
  Pose2D speed_; // the speed of the walk
  bool percentage_speed_; // true if speed is percentage rather than absolute vel
  bool pedantic_walk_; // true disables the step size stabilization.  "Set it when precision is indispensable"

  bool is_penalised_;

  // target point walk 
  Pose2D target_point_;
  bool walk_to_target_;
  bool target_walk_is_active_;
  bool finished_standing_;
  
  bool rotate_around_target_;
  float rotate_distance_;
  float rotate_heading_;

  // for kicking from walk
  bool perform_kick_;
  float kick_heading_;
  float kick_distance_;
  bool kick_with_left_;
  bool step_into_kick_;

  bool set_kick_step_params_;
  int step_kick_type_;
  Pose2D pre_kick_step_;
  Pose2D kick_step_;
  float kick_step_ref_x_;

  int tilt_fallen_counter_;
  int roll_fallen_counter_;
  bool getup_from_keeper_dive_;
  
  // walk odometry offsets
  float odometry_fwd_offset_;
  float odometry_side_offset_;
  float odometry_turn_offset_;

  // goalie arms
  bool keep_arms_out_;

  bool slow_stand_; // true if we need a slow stand

  bool walk_decides_finished_with_target_;
  float finished_with_target_max_x_error_;
  float finished_with_target_min_y_error_;
  float finished_with_target_max_y_error_;

  bool stand_in_place_;

  WalkType walk_type_;
};

#endif /* end of include guard: WALKREQUESTBLOCK_RE8SDRLN */
