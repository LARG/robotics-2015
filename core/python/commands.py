#!/usr/bin/env python
import core
import UTdebug
import cfgstiff
import math
from memory import kick_params, joint_angles, joint_commands, walk_request, \
    processed_sonar, world_objects, robot_state, behavior_mem, vision_frame_info, speech
import mem_objects

walk_max_vel_y = 90
walk_max_vel_x = 360

rswalk_max_vel_y = 200.0 #was 200.0
rswalk_max_vel_x = 330.0 #was 300.0
rswalk_max_vel_rot = 0.87 #was 0.87

def stand():
  walk_request.stand()

def standStraight():
  walk_request.standStraight()

def setWalkVelocity(velX, velY, velTheta):
  scale = max(math.sqrt(velX * velX + velY  * velY), 1.0)
  velX /= scale; velY /= scale
  walk_request.setWalk(velX, velY, velTheta)

def setStiffness(cfg = cfgstiff.One, time = 0.3):
  if isAtStiffness(cfg): return

  for i in range(core.NUM_JOINTS):
    joint_commands.setJointStiffness(i, cfg[i])
  joint_commands.send_stiffness_ = True
  joint_commands.stiffness_time_ = time * 1000.0

def setHeadPanTilt(pan = 0, tilt = -21, time = 2.0, isChange = False):
  setHeadTilt(tilt)
  setHeadPan(pan, time, isChange)

# tilt=-25  
def setHeadTilt(tilt = -22):
  joint_commands.setHeadTilt(core.DEG_T_RAD * tilt, 200.0, False)

def setHeadPan(target_pos, target_time = 2.0, isChange = None):
  if (isChange == None): isChange = False
  joint_commands.setHeadPan(target_pos, target_time*1000.0, isChange)

def setWalkMode(mode):
  behavior_mem.walk_mode_ = mode

def isAtStiffness(cfg):
  for i in range(core.NUM_JOINTS):
    stiff = core.joint_stiffness[i]
    error = abs(stiff - cfg[i])
    if error > 0.05: return False
  return True
