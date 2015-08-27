#!/usr/bin/env python

import os, shutil, subprocess

def runRemoteCommand(ip,command):
  subprocess.check_call('ssh nao@%s "%s"' % (ip,command),shell=True)

def copyFile(ip,filename,dest=''):
  subprocess.check_call('scp %s nao@%s:%s' % (filename,ip,dest),shell=True)

def copyFileAndMove(ip,filename,dest):
  tmpDest = os.path.basename(filename) + '.tmp'
  copyFile(ip,filename,tmpDest)
  return 'mv %s %s\n' % (tmpDest,dest)

def setHostname(hostname):
  return 'echo %s > /etc/hostname\n' % hostname

def removeTempDir(d):
  shutil.rmtree(d)

def makeTempDir():
  d = os.path.join(os.path.expandvars('$NAO_HOME/install/tmp'))
  os.mkdir(d)
  return d

def filloutID(name,robotID,d):
  with open('../data/scripts/%s' % name,'r') as f:
    contents = f.read()
  contents = contents.replace('${ID}',robotID)
  with open(os.path.join(d,name),'w') as f:
    f.write(contents)

def setupNetworking(ip,d):

  script = '#!/bin/bash\n\n'
  script += copyFileAndMove(ip,'../data/scripts/wpa_supplicant.conf','/etc/wpa_supplicant/wpa_supplicant.conf')
  script += '/etc/init.d/utwireless stop\n'
  script += '/etc/init.d/utwireless start\n'

  scriptName = 'setupScript.sh'
  scriptPath = os.path.join(d,scriptName)
  

  with open(scriptPath,'w') as f:
    f.write(script)
  copyFile(ip,scriptPath)
  print 'You need to put in the password "root" after it prompts for the password'
  subprocess.check_call('ssh -t nao@%s "su -c \'bash %s\'"' % (ip,scriptName),shell=True)
  runRemoteCommand(ip,'rm %s' % scriptName)

def main(ip):
  try:
    d = makeTempDir()
    print 'setting up networking'
    setupNetworking(ip,d)
  finally:
    removeTempDir(d)

if __name__ == '__main__':
  import sys
  usage = 'setup_robotv4 currentIP'
  helpStrings = ['-h','--help']
  args = sys.argv[1:]
  for s in helpStrings:
    if s in args:
      print usage
      sys.exit(0)
  atom = None
  if len(args) != 1:
    print usage
    sys.exit(1)
  currentIP = args[0]
  main(currentIP)
