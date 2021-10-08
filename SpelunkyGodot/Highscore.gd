extends TextureRect

# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("Small").text="highscore: $"+str(Globals.highScore)+"\nmax depth beaten: "+str(Globals.maxDepthBeaten)+"/4"
