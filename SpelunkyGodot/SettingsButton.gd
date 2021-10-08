extends TextureButton

var StartButton:Control
var Logo:Control
var Settings:Control
var Highscore:Control
var audioStreamPlayer:AudioStreamPlayer
export var hoverSFX:AudioStream
export var clickSFX:AudioStream

# Called when the node enters the scene tree for the first time.
func _ready():
	audioStreamPlayer = get_node("AudioStreamPlayer")
	StartButton = get_node("../StartButton")
	Settings = get_node("../Settings")
	Highscore = get_node("../Highscore")
	Logo = get_node("../Logo")

func _on_SettingsButton_pressed():
	audioStreamPlayer.stream=clickSFX
	audioStreamPlayer.play()
	Settings.visible = true
	Logo.visible = false
	StartButton.visible = false
	Highscore.visible = false
	visible = false
	

func _on_SettingsButton_mouse_entered():
	audioStreamPlayer.stream=hoverSFX
	audioStreamPlayer.play()
