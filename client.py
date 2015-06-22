import serial

ser = serial.Serial('/dev/ttyACM0', 9600)

def read():
   return ser.readline()

def write_read(byte):
   if type(byte) == str and len(byte) == 1:
      ser.write(byte)
   else:
      print "Only accepts single characters"
      ser.write('0')
   return read()

if __name__ == "__main__":
   session = True
   while session:
      arg = raw_input(">")
      if arg.lower() == "exit":
         session = False
      print write_read(arg)

