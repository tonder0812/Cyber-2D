import Cyber
import glm
yspeed=-30
def Start():
	print("Start")

def Update(ts,translation):
	global yspeed
	translation.y+=yspeed*ts
	if(translation.y<-100):
		translation.y=100
	#print(translation.y)

def Destroy():
	print("Destroy")
