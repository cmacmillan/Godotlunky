extends AudioStreamPlayer

func _ready():
	var file = File.new()
	if (file.file_exists("Save.kush")):
		file.open("Save.kush",File.READ)
		if (file.get_8()==1):
			Globals.useClassicControls = true
		else:
			Globals.useClassicControls = false
		if (file.get_8()==1):
			Globals.useAutoRun = true
		else:
			Globals.useAutoRun = false
		if (file.get_8()==1):
			Globals.useMusic = true
		else:
			Globals.useMusic = false
		Globals.maxDepthBeaten = file.get_8()
		Globals.highScore = file.get_32()
		Globals.bestPlayTime = file.get_float()
		file.close()
