import Cyber
import glm
import random

def Colides(R1):
	global this
	transform=this.GetTransform()
	R2=[
		transform.Translation.x-transform.Scale.x/2,
		transform.Translation.y-transform.Scale.y/2,
		transform.Translation.x+transform.Scale.x/2,
		transform.Translation.y+transform.Scale.y/2
	]
	if("Up" in this.Class):
		tmp=R2[1]
		R2[1]=R2[3]
		R2[3]=tmp
	if (R1[0] >= R2[2]) or (R1[2] <= R2[0]) or (R1[3] <= R2[1]) or (R1[1] >= R2[3]):
		return False
	else:
		return True

Public={
	"Colides":Colides,
	"Passed":False,
}

other=None
gap=10
deviation=4
def Start():
	global other
	if("Up" in this.Class):
		other=Cyber.Scene.FindByID(this.Id.replace("Up","Down"))
		rnd=random.random()*deviation*2-deviation
		this.GetTransform().Translation.y=rnd + gap
		other.GetTransform().Translation.y=rnd - gap

def Update(ts):
	if(not Cyber.Scene.FindByID("Bird").GetScript().Public["playing"]):
		return
	this.GetTransform().Translation.x-=10*ts
	if(this.GetTransform().Translation.x<-19):
		this.GetTransform().Translation.x=Cyber.Scene.FindByID("Pipe"+str((int(this.Id[4])+3)%4)+" Up").GetTransform().Translation.x + 10.5
		
		if("Up" in this.Class):
			Public["Passed"]=False
			rnd=random.random()*deviation*2-deviation
			this.GetTransform().Translation.y=rnd + gap
			other.GetTransform().Translation.y=rnd - gap
def Destroy():
	pass
