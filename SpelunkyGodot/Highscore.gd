extends TextureRect

# Called when the node enters the scene tree for the first time.
func _ready():
	var fastestClear
	var challengeTime
	if (Globals.bestPlayTime==0):
		fastestClear = "fastest clear: --:--"
		challengeTime = ""
	else:
		var minutes:int = floor(Globals.bestPlayTime/60)
		var seconds:int = Globals.bestPlayTime-minutes*60
		var formatter
		if (seconds<10):
			formatter="0"
		else:
			formatter=""
		fastestClear ="fastest clear: "+str(minutes)+":"+formatter+str(seconds)
		challengeTime = "\nchallenge time: 3:00"
	get_node("Small").text="highscore: $"+str(Globals.highScore)+"\nmax depth beaten: "+str(Globals.maxDepthBeaten)+"/4\n"+fastestClear+challengeTime
