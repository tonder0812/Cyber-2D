import Cyber
import glm

speed=6
jumped=False
def Start():
	pass

def Update(ts,transform):
	global speed,jumped
	if(Cyber.Input.IsKeyPressed(" ") and not jumped):
		speed=-6
		jumped=True
	elif(not Cyber.Input.IsKeyPressed(" ")):
		jumped=False
	speed+=0.1
	if(speed>6):
		speed=6
	transform.Translation.y-=speed*ts
	
def Event(e):
	pass

def Destroy():
	Cyber.Log.Info("Destroy")
