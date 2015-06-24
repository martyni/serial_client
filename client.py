import serial
import os
try:
   ser = serial.Serial('/dev/ttyACM0', 115200)
except:
   ser = serial.Serial('/dev/ttyACM1', 115200)
def read():
   return ser.readline()

def write_read(byte):
   try:
      int(byte)
      ser.write(byte)
   except:
      print "Only accepts integers"
      ser.write('0')
   return read()

if __name__ == "__main__":
   session = True
   while session:
      arg = raw_input("%s>> " % os.getlogin())
      if arg.lower() == "exit":
         session = False
      print write_read(arg)

