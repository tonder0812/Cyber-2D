import Cyber
import glm

speed=6
jumped=False
playing=False
def Start():
	pass

def UpdatePlay(ts):
	global speed,jumped,this
	if(Cyber.Input.IsKeyPressed(" ") and not jumped):
		speed=-6
		jumped=True
	elif(not Cyber.Input.IsKeyPressed(" ")):
		jumped=False
	speed+=0.1
	if(speed>6):
		speed=6
	this.GetTransform().Translation.y-=speed*ts

def Update(ts):
	global speed,jumped,playing,this
	if(playing):
		UpdatePlay(ts)
	elif(Cyber.Input.IsKeyPressed(" ")):
		playing=True
def Event(e):
	pass

def Destroy():
	pass
