#!/usr/bin/env python

import subprocess, sys, os

from setup_robotv4 import runRemoteCommand, ask, makeTempDir, removeTempDir, copyFile
 

usage = 'fix_autoload ip'
if (len(sys.argv) != 2) or ('-h' in sys.argv) or ('--help' in sys.argv):
  print usage
  sys.exit(1)
ip = sys.argv[1]
print 'USING IP (should be full ip): %s' % ip
if not ask('continue'):
  sys.exit(1)

script = '#!/bin/bash\n\n'
script += 'rm /etc/naoqi/autoload.ini\n'
script += 'rm /home/nao/naoqi/preferences/autoload.ini\n'
script += 'ln -s /home/nao/data/autoload.ini /etc/naoqi/autoload.ini\n'


d = makeTempDir()
try:
  scriptName = 'fixAutoload.sh'
  scriptPath = os.path.join(d,scriptName)
  with open(scriptPath,'w') as f:
    f.write(script)
  copyFile(ip,scriptPath)
  print 'You need to put in the password "root" after it prompts for the password'
  subprocess.check_call('ssh -t nao@%s "su -c \'bash %s\'"' % (ip,scriptName),shell=True)
  runRemoteCommand(ip,'rm %s' % scriptName)
  os.chdir('../build2/')
  cmd = './copy_robot %s scripts autoload --atom' % ip
  subprocess.check_call(cmd,shell=True)
finally: 
  removeTempDir(d)
