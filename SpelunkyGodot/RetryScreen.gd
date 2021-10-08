extends Control

func _ready():
	var node = get_node("Retry/Label")
	if (Globals.useClassicControls):
		node.text = "Retry (X)"
	else:
		node.text = "Retry (K)"
