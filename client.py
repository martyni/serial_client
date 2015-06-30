import serial
import os
from pprint import pprint
serial_connection = 9600

try:
   ser = serial.Serial('/dev/ttyACM0', serial_connection, timeout=2)
except:
   ser = serial.Serial('/dev/ttyACM1', serial_connection, timeout=2)

def read():
   return ser.readline()
   

def write_read(byte):
   try:
      int(byte)
      ser.write(byte)
   except:
      print "Only accepts integers not %s : %s" % (byte, type(byte))
      ser.write('0')
   return read()

def get_commands():
   print "getting commands"
   command_tree = {}
   command = 1 
   new_commands = True
   while new_commands:
      outcome = write_read(str(command))[:-2:]
      if outcome == "Unknown":
         break
      if "?" in outcome:
         write_read('0')
      command_tree[command] = outcome 
      command += 1
      print ".",
   pprint(command_tree)
   return command_tree

class Robot(object):
   def __init__(self, commands, verbose=False):
      self.commands = commands
      self.do = {cmd : commands[cmd] for cmd in commands if '!' in commands[cmd]}
      self.question = {cmd : commands[cmd] for cmd in commands if '?' in commands[cmd]}
      self.info = {cmd : commands[cmd] for cmd in commands if '!' not in commands[cmd] and '?' not in commands[cmd]}
      self.session = True
      self.verbose = verbose
      self.user = os.getlogin()
      self.last_arg = "dummy"
      self.arg = None
   
   def verbose_print(self, *say):
      if self.verbose:
         pprint(say) 
  
   def pointer(self, arg):
      try:
         return ".." if int(arg) in self.question else ">>"   
      except:
         return ">>"

   def start_session(self):
      while self.session:
         self.verbose_print(self.do, self.arg, self.info)
         self.arg = raw_input("%s%s " % (self.user, self.pointer(self.last_arg)))
         print write_read(self.arg)
         if self.arg == "exit":
            self.session = False
            write_read(self.do.get('Stop!','0'))
         self.last_arg = self.arg
         

if __name__ == "__main__":
   session = True
   [pprint(read()) for _ in range(2)] #empty serial bus 
   my_bot = Robot(get_commands())
   my_bot.start_session()
   
