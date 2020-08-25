import sst
from sst.pyproto import *

"""
General pyproto notes learned from experience:
* _clockHandle must be included
* In _clockHandle, return True ends the simulation. Returnsing nothing False, or anything other than true keeps the simulation going. 

About this example:
A conversation occurs between two "Person" objects. 

To run:
Type the following command
sst helloWorld.py
"""

class MyEvent(PyEvent):
	def __init__(self, message, count, sender):
		PyEvent.__init__(self)
		self.message = message
		self.count = count
		self.sender = sender

	def __str__(self):
		return ("Message from {}: {}\n\n".format(self.sender, self.message))

class Person(PyProto):
	def __init__(self, name, startConversation):
		PyProto.__init__(self, name)
		self.name = name
		self.countdown = 10

		"""
		self.startConversation True means that the person starts the 
		conversation

		There is no error or error handling for if no one or multiple
		people start the conversation. 
		"""
		self.startConversation = startConversation 


		self.addClock(self._clockHandle, "1MHz")

		self.TIMEOUT = 10000

	def setLink(self, link):
		self.myLink = self.addLink(link, "1us", self._linkHandle)

	def _linkHandle(self, event):
		
		event.count += 1
		if (event.count <=10):
			print(event) 
			ev = MyEvent("Hello", event.count, self.name)
			self.myLink.send(ev)

	def _clockHandle(self, cycle): # This one is 100% necessary
		#print(self.name, "Clock ", cycle)
		if (cycle == 1) and self.startConversation:
			#print("{}: Hello World! ({})".format(self.name, cycle))
			print("{} here. Starting conversation".format(self.name))
			ev = MyEvent("Hello", 1, self.name)
			self.myLink.send(ev)
			return False # Keeps the simulation going, can be nothing or anything but True
		elif (cycle >= self.TIMEOUT): # Time out so the simulation will end 
			return True # Ends the simulation

	"""
	def construct(self):
		print("{}: Construct()".format(self.name))

	def init(self, phase):
		print("{}: init({})".format(self.name, phase))

	def setup(self):
		print("{}: setup()".format(self.name))

	def finish(self):
		print("{}: finish()".format(self.name))
	"""

link0 = sst.Link("MyLink0")

shelby = Person("Shelby", True)
stephen = Person("Stephen", False)

shelby.setLink(link0)
stephen.setLink(link0)
