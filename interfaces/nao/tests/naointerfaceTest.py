"""
  This is a simple python script to test the autogenerated module

"""
print '        START TESTING naointerface MODULE'

from naoqi import ALProxy

IP = "127.0.0.1"
PORT = 9559

#________________________________
# Generic Proxy creation 
#________________________________

try:
  naointerface_Proxy = ALProxy("naointerface",IP,PORT)
except Exception,e:
  print "Error when creating naointerface_Proxy proxy:"
  print str(e)
  exit(1)

#________________________________
# Remote procedure call
#________________________________
inParam = ["test", 1, 45, [123, 456, "789"]]

try:
  ret = naointerface_Proxy.dummyFunction( "hello, world", inParam )
except Exception,e:
  print "dummyFunction test Failed"
  exit(1)

print 'Returned Value : '
print(ret)
if (ret == inParam):
  print 'Success, the return value is correct'
exit(0)
