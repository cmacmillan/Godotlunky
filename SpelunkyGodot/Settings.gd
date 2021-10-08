extends Control

var runTextClassic:Label
var runTextNouveau:Label
var classicRoot:Control
var nouveauRoot:Control
var controlModeName:Label
var backText:Label
var audioStreamPlayer:AudioStreamPlayer
var musicCheckbox:TextureButton
var autorunCheckbox:TextureButton
var semicolonTexture:TextureRect
var shiftTexture:TextureRect
var quitButton:Control
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
	backText = get_node("ResumeAndBack/Large")
	quitButton = get_node("Quit")
	attentionAmount = 0

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if (Globals.isInMainMenu):
		backText.text = "Back"
		quitButton.visible = false
	else:
		backText.text = "Resume"
		quitButton.visible = true
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
		
func UseClassicLayout():
	EraseActions()
	Bind("jump",KEY_Z)
	Bind("left",KEY_LEFT)
	Bind("right",KEY_RIGHT)
	Bind("run",KEY_SHIFT)
	Bind("lookup",KEY_UP)
	Bind("crouch",KEY_DOWN)
	Bind("whip",KEY_X)
	Bind("bomb",KEY_C)
	Bind("rope",KEY_D)

func ClearAction(action):
	InputMap.erase_action(action)
	InputMap.add_action(action)
	
func EraseActions():
	ClearAction("jump")
	ClearAction("left")
	ClearAction("right")
	ClearAction("run")
	ClearAction("lookup")
	ClearAction("crouch")
	ClearAction("whip")
	ClearAction("bomb")
	ClearAction("rope")

func Bind(action,key):
	var new_input = InputEventKey.new()
	new_input.scancode = key
	InputMap.action_add_event(action,new_input)

func UseNouveauLayout():
	EraseActions()
	Bind("jump",KEY_J)
	Bind("left",KEY_A)
	Bind("right",KEY_D)
	Bind("run",KEY_SEMICOLON)
	Bind("lookup",KEY_W)
	Bind("crouch",KEY_S)
	Bind("whip",KEY_K)
	Bind("bomb",KEY_L)
	Bind("rope",KEY_O)	

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
	get_node("/root/GameScene/Level").WriteHighScoreToGlobals()
	get_tree().paused=false
	get_tree().change_scene("res://MainMenuScene.tscn")
		
func ResumeAndBackPressed():
	audioStreamPlayer.stream=clickSFX
	audioStreamPlayer.play()
	if (Globals.isInMainMenu):
		visible=false
		get_node("../StartButton").visible=true
		get_node("../SettingsButton").visible=true
		get_node("../Logo").visible=true
		get_node("../Highscore").visible=true
	else:
		Globals.UnPause();
	
func ButtonHovered():
	audioStreamPlayer.stream=hoverSFX
	audioStreamPlayer.play()
	
func ChangeModeButtonPressed():
	Globals.useClassicControls = !Globals.useClassicControls
	if (Globals.useClassicControls):
		UseClassicLayout()
	else:
		UseNouveauLayout()
	audioStreamPlayer.stream = clickSFX
	audioStreamPlayer.play()

func _on_Settings_visibility_changed():
	if (!visible):
		Globals.Save()
