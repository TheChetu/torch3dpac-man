Torch 3D Pacman
	Programming Notes:
		Uses modified Lesson 10 code from nehe.gamedev.net for window creation, OpenGL initialization, and input handling.
		Uses modified Lesson 13 code from nehe.gamedev.net for text printing.
	Usage: 
		WASD & Arrow keys to move:
		W/Up Arrow: Move Forward.
		A: Face Left
		Left Arrow: Turns Left.
		S: Turn Around
		Down Arrow: Move Backward.
		D: Face Right
		Right Arrow: Turns Right.
		Shift + W/Up: "Sprint", moves faster while sprint gauge is not empty.
		R: Reset.
		B: Enable Blending, changes Blending of Primitives to allow transparency.
		F: Enable FPS Display
		2: Enable/Disable Ambient Light (Default Enabled)
		4: Enable/Disable PacMan Light (Default Enabled)
		Escape: Exit.		
		Textures From: Assignment 2 Demo Program - Located in textures folder

For Compiling: 
	Note	The program is ready to compile when the files are in the proper location
	Requirements
		
		AL (Directory)
			Contains al.h, alc.h, alut.h, efx.h, efx-creative.h, EFS-Util.h, xram.h

		Drawing (Directory)
			Contains Draw.cpp, Draw.h

		glee (Directory)
			Contains GLee.c, GLee.h

		global (Directory)
			Contains globref.h, timer.h

		IL (Directory)
			Contains config.h, devil_internal_exports.h, il.h, il_wrap.h, ilu.h, ilu_region.h, ilut.h, ilut_config.h
		
		lib (Directory)
			Contains alut.lib, DevIL.lib, EFX-Util.lib, ILU.lib, ILUT.lib, OpenAL32.lib

		Root (Directory)
			Contains Pacman.cpp

For Running:
	Requirements
		MUST HAVE VC++ 2010 x86 Redistributable Package INSTALLED. Included in Root Directory as "VC++ 2010 x86.exe".

		data (Directory)
			Contains level1.txt, log.txt created after running.

		sounds (Directory)
			Contains Loop.wav, M1.WAV, M2.WAV, M3.WAV, pacman_dies_y.wav, pacman_intro.wav, pacman_x.wav, pacman2_x.wav, Start.wav

		textures (Directory)
			Contains brick.bmp, neg_x.bmp, neg_y.bmp, neg_z.bmp, pos_x.bmp, pos_y.bmp, pos_z.bmp, stonetile.bmp

		DLLs
		 Root (Directory)
			Contains alut.dll DevIL.dll, ILU.dll, ILUT.dll, OpenAL32.dll