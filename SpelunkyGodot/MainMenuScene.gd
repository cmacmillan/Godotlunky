extends Control

func _ready():
	Globals.isInMainMenu = true
	if (Globals.useClassicControls):
		get_node("Settings").UseClassicLayout()
	else:
		get_node("Settings").UseNouveauLayout()

func _on_MainMenuScene_tree_exited():
	Globals.isInMainMenu = false
