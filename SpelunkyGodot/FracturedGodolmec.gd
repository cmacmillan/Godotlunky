extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var pieces = []
var velocities = []
var rots = []

export var yVelMin=0
export var yVelMax=0
export var xVelMin=0
export var xVelMax=0
export var angularMin=0
export var angularMax=0
export var gravity=0
var rng = RandomNumberGenerator.new()
# Called when the node enters the scene tree for the first time.
func _ready():
	rng.randomize()
	for i in  range(1,36):
		var sprite:Sprite = get_node("GodolmecDeathFragment"+String(i))
		pieces.push_back(sprite)
		var pos = sprite.global_position
		var dir = sign(pos.x-global_position.x)
		velocities.push_back(Vector2(dir*rng.randf_range(xVelMin,xVelMax),rng.randf_range(yVelMin,yVelMax)))
		rots.push_back(rng.randf_range(angularMin,angularMax)*dir)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if (!visible):
		return
	for i in range(pieces.size()):
		var curr = pieces[i]
		curr.global_position +=delta*velocities[i]
		curr.global_rotation +=delta*rots[i]
		velocities[i].y-=delta*gravity
