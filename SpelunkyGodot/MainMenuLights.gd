extends ColorRect


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var torchPos = get_node("../CenterContainer/Torch1").global_position
	var torchPos2 = get_node("../CenterContainer/Torch2").global_position
	var light1 = Vector2(torchPos.x,get_viewport().get_visible_rect().size.y-torchPos.y+50)
	var light2 = Vector2(torchPos2.x,get_viewport().get_visible_rect().size.y-torchPos2.y+50)
	var sm = material as ShaderMaterial
	sm.set_shader_param("LightPos1",light1)
	sm.set_shader_param("LightPos2",light2)
	pass


func _ob_mouse_entered():
	pass # Replace with function body.
