extends Control

func _ready():
	Globals.isInMainMenu = true
	get_node("Settings").UseClassicLayout() #need to read from save

func _on_MainMenuScene_tree_exited():
	Globals.isInMainMenu = false
