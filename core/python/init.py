#!/usr/bin/env python
import core
core.init() # this has to be run before anything else can be imported
import memory, mem_objects, commands
import sys, os, traceback
import logging; logging.disable(logging.ERROR)
import lights
import primary_bvr as behavior
import cfgwalk, cfgmap
import UTdebug
import cfglocalization

def init():
  global firstFrame
  firstFrame = True
  initMemory()
  initNonMemory()
  print "Python initialized"

def runBehavior(bvr):
  print "Running " + bvr + " behavior"
  behavior.load(bvr)

def initMemory():
  memory.init()
  mem_objects.init()
  cfgwalk.initWalk()

def initNonMemory(initLoc=True):
  core.CONFIG_ID = cfgmap.getConfigId()
  cfgwalk.initWalk()
  if initLoc:
    core.localizationC.reInit()
  core.localizationC.loadParams(cfglocalization.params)
  core.opponentsC.reInit()
  commands.setWalkMode(core.WalkMode.SLOW)

def processFrame():
  try:
    global firstFrame
    core.instance.preVision()
    if firstFrame:
      memory.world_objects.init(memory.robot_state.team_)
      core.visionC.initSpecificModule()
      initNonMemory(initLoc=False)
      memory.speech.say("Vision")
      firstFrame = False
    
    core.visionC.processFrame()
    core.instance.postVision()
    core.localizationC.processFrame()
    core.opponentsC.processFrame()
    processBehaviorFrame()
    lights.processFrame()
    core.instance.publishData()
  except:
    handle()

behaviorLoaded = False
def processBehaviorFrame():
  global behaviorLoaded
  if not behaviorLoaded: 
    runBehavior("keeper")
    behaviorLoaded = True
  try:
    if memory.robot_state.WO_SELF != core.WO_TEAM_COACH:
      mem_objects.update()
    if core.TOOL:
      initMemory()
    core.pythonC.updatePercepts()
    behavior.processFrame()
  except: 
    handle()

def handle():
  lines = traceback.format_exception(*(sys.exc_info()))
  message = ''.join('!! ' + line for line in lines)
  if core.instance.type_ == core.CORE_TOOLSIM:
    print message
  else:
    UTdebug.log(0,'PYTHON ERROR:')
    UTdebug.log(0, message)
  memory.speech.say("python")
  core.pythonC.is_ok_ = False
  with open(os.path.expanduser('~/error.txt'), 'w') as f:
    f.write(message)

