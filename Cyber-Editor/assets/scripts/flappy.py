import Cyber
import glm

speed=10
jumped=False
playing=False
def Start():
	pass

def UpdatePlay(ts):
	global speed,jumped,this
	if(Cyber.Input.IsKeyPressed(" ") and not jumped):
		speed=-7
		jumped=True
	elif(not Cyber.Input.IsKeyPressed(" ")):
		jumped=False
	speed+=20 * ts
	if(speed>10):
		speed=10
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
