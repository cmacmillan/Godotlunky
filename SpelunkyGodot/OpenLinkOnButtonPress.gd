extends TextureButton


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
export var url=""
export var hoverStream:AudioStream
export var clickStream:AudioStream

func OnButtonPressed():
	var audio = get_node("AudioStreamPlayer")
	audio.stream = clickStream
	audio.play()
	OS.shell_open(url)

func _on_mouse_entered():
	var audio = get_node("AudioStreamPlayer")
	audio.stream = hoverStream
	audio.play()
	pass # Replace with function body.
