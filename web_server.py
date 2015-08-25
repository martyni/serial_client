import bottle
import json
import os
from client import Robot, get_commands, write_read

class Controller(object):
   def __init__(self, robot, path=os.getcwd()):
      self.robot = robot
      self.bottle = bottle
      self.commands = self.robot.commands

   @self.bottle.route('/command_tree')
   def command_tree(self):
      return json.dumps(self.commands)

   @self.bottle.route('/run/<command>/')
   def run(self, command):
      return json.dumps({"response" 
                         : write_read(self.commands[command])})

   @self.bottle.route('/style.css')
   def style(self):
      return bottle.static_file('/style.css',root=path)
      
   @self.bottle.route('/')
   def controller(self):
      return bottle.template('controller', 
                             do = self.robot.do, 
                             question = self.robot.question,
                             info = self.robot.info,
                             stop = self.robot.do)


if __name__ == "__main__":
   my_bot = Robot(get_commands())
   my_controller = Controller(my_bot)
   bottle.run(host="localhost", port="8000")
