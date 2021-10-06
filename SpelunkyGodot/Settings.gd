extends Control

var runTextClassic:Label
var runTextNouveau:Label
var classicRoot:Control
var nouveauRoot:Control
var controlModeName:Label
var audioStreamPlayer:AudioStreamPlayer
var musicCheckbox:TextureButton
var autorunCheckbox:TextureButton
var semicolonTexture:TextureRect
var shiftTexture:TextureRect
export var hoverSFX:AudioStream
export var clickSFX:AudioStream
export var hoverCheckedTexture:Texture
export var checkedTexture:Texture
export var hoverUncheckedTexture:Texture
export var uncheckedTexture:Texture
export var attentionColor:Color
var attentionAmount:float = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	runTextClassic = get_node("ClassicLayout/Shift/TextureRect/Small")
	runTextNouveau = get_node("NouveauLayout/;/TextureRect/Small")
	classicRoot = get_node("ClassicLayout")
	nouveauRoot = get_node("NouveauLayout")
	controlModeName = get_node("ControlModeSelector/Large")
	audioStreamPlayer = get_node("AudioStreamPlayer")
	musicCheckbox = get_node("Music/TextureButton")
	autorunCheckbox = get_node("AutoRun/TextureButton")
	semicolonTexture = get_node("NouveauLayout/;/TextureRect")
	shiftTexture = get_node("ClassicLayout/Shift/TextureRect")
	attentionAmount = 0

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	attentionAmount = clamp(attentionAmount-2.5*delta,0,1)
	shiftTexture.modulate = lerp(Color.white,attentionColor,attentionAmount)
	semicolonTexture.modulate = lerp(Color.white,attentionColor,attentionAmount)
	if (Globals.useAutoRun):
		runTextClassic.text = "walk"
		runTextNouveau.text = "walk"
		autorunCheckbox.texture_hover = hoverCheckedTexture
		autorunCheckbox.texture_normal = checkedTexture
	else:
		runTextClassic.text = "run"
		runTextNouveau.text = "run"
		autorunCheckbox.texture_hover = hoverUncheckedTexture
		autorunCheckbox.texture_normal = uncheckedTexture
	if (Globals.useMusic):
		musicCheckbox.texture_hover = hoverCheckedTexture
		musicCheckbox.texture_normal = checkedTexture
	else:
		musicCheckbox.texture_hover = hoverUncheckedTexture
		musicCheckbox.texture_normal = uncheckedTexture
	if (Globals.useClassicControls):
		controlModeName.text = "Classic"
		classicRoot.visible = true
		nouveauRoot.visible = false
	else:
		controlModeName.text = "Nouveau"
		classicRoot.visible = false
		nouveauRoot.visible = true
		
func MusicPressed():
	Globals.useMusic = !Globals.useMusic
	audioStreamPlayer.stream=clickSFX
	audioStreamPlayer.play()

func AutoRunPressed():
	attentionAmount = 1
	Globals.useAutoRun = !Globals.useAutoRun
	audioStreamPlayer.stream=clickSFX
	audioStreamPlayer.play()

func QuitPressed():
	audioStreamPlayer.stream=clickSFX
	audioStreamPlayer.play()
	
func ResumeAndBackPressed():
	audioStreamPlayer.stream=clickSFX
	audioStreamPlayer.play()
	
func ButtonHovered():
	audioStreamPlayer.stream=hoverSFX
	audioStreamPlayer.play()
	
func ChangeModeButtonPressed():
	Globals.useClassicControls = !Globals.useClassicControls
	audioStreamPlayer.stream = clickSFX
	audioStreamPlayer.play()
