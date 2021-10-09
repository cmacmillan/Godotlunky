extends Node2D

var vBox:VBoxContainer
export var height=500

# Called when the node enters the scene tree for the first time.
func _ready():
	vBox = get_node("CanvasLayer/VBoxContainer")
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	vBox.margin_top-=delta*45
	if (vBox.margin_top<-(get_viewport().get_visible_rect().size.y+height)):
		get_tree().change_scene("res://MainMenuScene.tscn")
	pass
 
