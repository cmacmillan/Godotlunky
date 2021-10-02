extends TextureButton


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
export var url=""

func OnButtonPressed():
	OS.shell_open(url)
