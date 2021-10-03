extends Label

# Called when the node enters the scene tree for the first time.
func _ready():
	var deathReasonInt:int = Globals.damageSource
	match deathReasonInt:
		0:
			text="You got smushed"
		1:
			text="You impaled youself on some spikes"
		2:
			text="You were bitten by a snake"
		3:
			text="You were bitten by a spider"
		4:
			text="You got hit by a flying object"
		5:
			text="You got blown up"
		6:
			text="You were bitten by a bat"
		7:
			text="You fell to your death"
		8:
			text="You killed youself somehow??"
