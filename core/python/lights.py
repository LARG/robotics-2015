#!/usr/bin/env python
import core
from memory import *
import mem_objects
import math

ledsC = core.ledsC

def processFrame():
  doFootLights()
  doStateLights()
  doEyeLights()
  doEarLights()
  doHead()

def doEarLights():
  if (game_state.state() == core.INITIAL or game_state.state() == core.FINISHED):
    doInitialEarLights()
  else:
    doPlayingEarLights()

def doInitialEarLights(): pass

def doPlayingEarLights():
  # chaser
  if (robot_state.role_ == core.CHASER):
    ledsC.frontRightEar(1)
  else:
    ledsC.frontRightEar(0)

  # I'd like not to send this every frame, rather
  # only send when seen changes
  if (world_objects.getObjPtr(core.WO_BALL).seen):
    ledsC.frontLeftEar(1)
  else:
    ledsC.frontLeftEar(0)

  # default to off
  ledsC.backLeftEar(0)
  ledsC.backRightEar(0)

  # left arm bumper
  if (processed_sonar.bump_left_):
    ledsC.backLeftEar(1)
  if (processed_sonar.bump_right_):
    ledsC.backRightEar(1)


def doStateLights():
  # do chest led based on state
  state = game_state.state()
  if (state == core.INITIAL): 
    ledsC.chest(0,0,0)
  elif (state == core.READY):
    ledsC.chest(0,0,1)
  elif (state == core.SET):
    ledsC.chest(1,1,0)
  elif (state == core.PLAYING):
    ledsC.chest(0,1,0)
  elif (state == core.PENALISED):
    ledsC.chest(1,0,0)
  elif (state == core.TESTING):
    ledsC.chest(1,0,1)
  elif (state == core.FINISHED):
    ledsC.chest(1,1,1)

def doFootLights():
  # do feet leds
  if (robot_state.team_== 0): #blue
    ledsC.rightFoot(0,0,1)
  elif (robot_state.team_==1): #red
    ledsC.rightFoot(1,0,0)
  
  if (game_state.ourKickOff):
    ledsC.leftFoot(1,1,1)
  else:
    ledsC.leftFoot(0,0,0)

def doEyeLights():
  if robot_state.WO_SELF == core.WO_TEAM_COACH:
    doCoachEyes()
  elif game_state.state() == core.INITIAL:
    doInitialEyeLights()
  elif (game_state.state() == core.FINISHED):
    doInitialEyeLights()
  elif game_state.state() == core.TESTING:
    doTestingEyeLights()
  elif game_state.state() in [core.SET,core.READY]:
    ledsC.allRightEye(0,0,0)
    ledsC.allLeftEye(0,0,0)
  else:
    ledsC.allRightEye(0,0,0)
    doPlayingEyeLights()
  if robot_state.ignore_comms_:
    vframe = vision_frame_info.frame_id
    ledsC.allLeftEye(0,0, (vframe % 8) * .125)

def doInitialEyeLights():
  doEyeBalls()
  doEyePower()
  doEyeHeat()
  #doLeftEyeGoal()

def doFinishedEyeLights():
  doEyeBalls()
#  doEyePower()
#  doEyeHeat()
  doLeftEyeGoal()

def doPlayingEyeLights():
  #doLeftEyeObjects()
  #doRightEyeSonar()
  #doLeftEyeKicks()
  #doRightEyeKicks()
  doLeftEyeGoal()
#  pass

def doTestingEyeLights():
  doLeftEyeGoal()
  doEyeBalls()

#  if processed_sonar.on_left_ or processed_sonar.on_center_:
#    ledsC.allLeftEye(0,0,1)
#  else:
#    ledsC.allLeftEye(1,0,0)
#  if processed_sonar.on_right_ or processed_sonar.on_center_:
#    ledsC.allRightEye(0,0,1)
#  else:
#    ledsC.allRightEye(1,0,0)


def doLeftEyeObjects():
  goal = world_objects.getObjPtr(core.WO_UNKNOWN_GOAL)
  circle = world_objects.getObjPtr(core.WO_CENTER_CIRCLE)
  if goal.seen:
    ledsC.allLeftEye(0,0,1)
  elif circle.seen:
    ledsC.allLeftEye(1,0,0)
  else:
    ledsC.allLeftEye(0,0,0)

def doLeftEyeGoal():
  goal = world_objects.getObjPtr(core.WO_UNKNOWN_GOAL)
  soloPost = world_objects.getObjPtr(core.WO_UNKNOWN_GOALPOST)
  if soloPost.seen:
    ledsC.allTopLeftEye(0,0,1)
  else:
    ledsC.allTopLeftEye(0,0,0)
  if goal.seen:
    ledsC.allBottomLeftEye(0,0,1)
  else:
    ledsC.allBottomLeftEye(0,0,0)

def doRightEyeSonar():
  if not(processed_sonar.sonar_module_enabled_):
    ledsC.allRightEye(0,0,0)
  else:
    if processed_sonar.on_left_:
      ledsC.allRightEye(1,0,0)
    elif processed_sonar.on_center_:
      ledsC.allRightEye(0,1,0)
    elif processed_sonar.on_right_:
      ledsC.allRightEye(0,0,1)
    else:
      ledsC.allRightEye(1,1,1)

def doEyePower(eye=ledsC.allBottomLeftEye):
  battery = sensors.getValue(core.battery)
  if (battery > 0.95):
    eye(1,1,1)
    return False
  elif (battery > 0.8):
    eye(1,1,0)
    return False
  elif (battery > 0.65):
    eye(1,.5,1)
    return False
  else:
    eye(1,0,0)
    return True

def doEyeHeat():
  max_temp = getMaxTemp()
  if (max_temp > 74):
    ledsC.allTopLeftEye(1,0,0)
    return True
  elif (max_temp > 64):
    ledsC.allTopLeftEye(1,.5,0)
    return True
  elif (max_temp > 54):
    ledsC.allTopLeftEye(1,1,0)
    return True
  else:
    ledsC.allTopLeftEye(1,1,1)
    return False

def doCoachEyes():
  robot = world_objects.getObjPtr(robot_state.WO_SELF)
  ball = world_objects.getObjPtr(core.WO_BALL)
  minAngle, maxAngle = math.pi / 4, 3 * math.pi / 4
  if robot.height > 100 and robot.orientation > minAngle and robot.orientation < maxAngle and robot.loc.y < -core.HALF_FIELD_Y:
    ledsC.allLeftEye(1, .25, 0)
    if ball.seen:
      ledsC.allTopRightEye(0,0,1)
    else:
      ledsC.allTopRightEye(0,1,0)
    doEyePower(ledsC.allBottomRightEye)
  elif robot.height > 100 and robot.orientation < -minAngle and robot.orientation > -maxAngle and robot.loc.y > core.HALF_FIELD_Y:
    ledsC.allRightEye(1, .25, 0)
    if ball.seen:
      ledsC.allTopLeftEye(0,0,1)
    else:
      ledsC.allTopLeftEye(0,1,0)
    doEyePower(ledsC.allBottomLeftEye)
  else: # Invalid position
    ledsC.allLeftEye(1,0,0)
    ledsC.allRightEye(1,0,0)

def doEyeBalls():
  ball = mem_objects.world_objects[core.WO_BALL]
  if ball.seen:
    if ball.fromTopCamera:
      ledsC.allTopRightEye(0,0,1)
      ledsC.allBottomRightEye(0,1,0)
    else:
      ledsC.allTopRightEye(0,1,0)
      ledsC.allBottomRightEye(0,0,1)
  else:
    ledsC.allTopRightEye(0,1,0)
    ledsC.allBottomRightEye(0,1,0)
  if robot_vision.topGreenPct < .25 or robot_vision.topUndefPct > 0.25:
    ledsC.allTopRightEye(1,0,0)
  if robot_vision.bottomGreenPct < .5 or robot_vision.bottomUndefPct > 0.5:
    ledsC.allBottomRightEye(1,0,0)

def getMaxTemp():
  max_temp = 0
  for i in range(core.NUM_JOINTS):
    temp = sensors.getJointTemperature(i)
    if temp > max_temp:
      max_temp = temp
  return max_temp

def doHead():
  if odometry.walkDisabled:
    vframe = vision_frame_info.frame_id
    ledsC.headCircle()
    ledsC.allLeftEar(1.0 if vframe % 4 >= 2 else 0.0)
    ledsC.allRightEar(1.0 if vframe % 4 >= 2 else 0.0)
  else:
    ledsC.headOff()
    ledsC.allLeftEar(0.0)
    ledsC.allRightEar(0.0)
