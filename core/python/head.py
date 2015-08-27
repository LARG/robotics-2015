#!/usr/bin/env python
from task import Task
import commands

class MoveHead(Task):
  def __init__(self, tilt = None, pan = 0.0, time = 2.0):
    Task.__init__(self)
    self.tilt = tilt
    self.pan = pan
    self.time = time

  def run(self):
    commands.setHeadPan(self.pan, self.time)
    if self.tilt is not None:
      commands.setHeadTilt(self.tilt)

    if self.getTime() > self.time: self.finish()
