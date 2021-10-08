extends TextureButton

export var hoverStream:AudioStream
export var clickStream:AudioStream
export var fadeInAudio:AudioStream
export var fadeOutAudio:AudioStream

var wipe:ColorRect
var shaderMat:ShaderMaterial

var Revealing = false
var LoadingScene = false
var wipeLerp = 0

func _ready():
	LoadingScene = false
	wipe = get_node("../FullscreenWipe") as ColorRect
	shaderMat = wipe.material as ShaderMaterial
	wipeLerp=-.5
	shaderMat.set_shader_param("C",0)
	if (Globals.useMusic):
		Music.currentVolume=1.0
	else:
		Music.currentVolume=0.0
	Music.nextAudio = Music.mainMenuTheme
	var fadeAudioPlayer = get_node("FadeAudioPlayer")
	fadeAudioPlayer.stream = fadeInAudio
	fadeAudioPlayer.play()
	if (Globals.isFirstLoad):
		var audio = (Music.get_node("audioSource") as AudioStreamPlayer)
		audio.stream = Music.mainMenuTheme
		if (Globals.useMusic):
			audio.play()
	Revealing = true
	
func _process(delta):
	var target=wipeLerp
	if (Revealing):
		target = 1
	else:
		target = 0
		if (LoadingScene && wipeLerp==0):
			get_tree().change_scene("res://MainScene.tscn")
	wipeLerp = move_toward(wipeLerp,target,delta)
	shaderMat.set_shader_param("C",clamp(wipeLerp,0,1))

func OnButtonPressed():
	var audio = get_node("AudioStreamPlayer")
	var fadeAudioPlayer = get_node("FadeAudioPlayer")
	fadeAudioPlayer.stream = fadeOutAudio
	fadeAudioPlayer.play()
	audio.stream = clickStream
	audio.play()
	Music.nextAudio = null
	Revealing=false
	LoadingScene = true

func _on_mouse_entered():
	var audio = get_node("AudioStreamPlayer")
	audio.stream = hoverStream
	audio.play()
