import Cyber
import glm

speed=10
jumped=False
playing=False
score=0
ended=False
digits=[
	Cyber.Texture("assets/textures/Flappy bird/"+str(i)+".png") for i in range(10)
]
def Start():
	pass

def DisplayScore():
	n=0
	tmpScore=score
	while(n<3 and tmpScore>0):
		digit=tmpScore % 10
		Cyber.Scene.FindByID("Digit"+str(n)).GetSpriteRenderer().Texture=digits[digit]
		tmpScore//=10
		n+=1

def UpdatePlay(ts):
	global speed,jumped,this,timer,score,playing,ended
	if(Cyber.Input.IsKeyPressed(" ") and not jumped):
		speed=-7
		jumped=True
	elif(not Cyber.Input.IsKeyPressed(" ")):
		jumped=False
	speed+=20 * ts
	if(speed>10):
		speed=10

	transform=this.GetTransform()
	transform.Translation.y-=speed*ts
	R=[
		transform.Translation.x-transform.Scale.x/2,
		transform.Translation.y-transform.Scale.y/2,
		transform.Translation.x+transform.Scale.x/2,
		transform.Translation.y+transform.Scale.y/2
	]
	for entity in Cyber.Scene.FindByClass("Pipe"):
		if(entity.GetScript().Public["Colides"](R)):
			Cyber.Scene.FindByID("GameOver").GetTransform().Translation.z=1
			Public["playing"]=False
			playing=False
			ended=True
		elif("Up" in entity.Class and entity.GetTransform().Translation.x<-10 and not entity.GetScript().Public["Passed"]):
			entity.GetScript().Public["Passed"]=True
			score+=1
			Cyber.Log.Info(score)
			DisplayScore()

Public={
	"playing":False
}

def Update(ts):
	global playing
	if(playing):
		UpdatePlay(ts)
	elif(Cyber.Input.IsKeyPressed(" ") and not ended):
		Cyber.Scene.FindByID("Start").Destroy()
		Public["playing"]=True
		playing=True
	

def Destroy():
	pass
