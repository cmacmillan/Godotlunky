extends TextureButton

export var hoverStream:AudioStream
export var clickStream:AudioStream
export var sceneName:String

func _process(delta):
	if (Input.is_action_pressed("whip") && sceneName=="res://MainScene.tscn"):
		get_tree().change_scene("res://MainScene.tscn")

func _on_mouse_entered():
	var audio = get_node("AudioStreamPlayer")
	audio.stream = hoverStream
	audio.play()

func OnButtonPressed():
	var audio = get_node("AudioStreamPlayer")
	audio.stream = clickStream
	audio.play()
	get_tree().change_scene(sceneName)
