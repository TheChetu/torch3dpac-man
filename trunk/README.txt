Torch 3D Pacman
	Programming Notes:
		Uses modified Lesson 10 code from nehe.gamedev.net for window creation, OpenGL initialization, and input handling.
		Uses modified Lesson 13 code from nehe.gamedev.net for text printing.
	Models:
		Ghost models courtesy of tomaz from http://users.sentvid.org/~tomaz/digizet/models/MD2/megaman/
	Textures:
		Life texture from http://fc08.deviantart.net/fs45/f/2009/120/e/b/Buster_Sword_by_Xkado.jpg
		Ground texture from assignment 2 demo program
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
		F: Enable FPS Display
		2: Enable/Disable Ambient Light (Default Enabled)
		4: Enable/Disable PacMan Light (Default Enabled)
		Escape: Exit.		

For Compiling: 
	Note	The program is ready to compile when the files are in the proper location
	Requirements
		
		AL (Directory)
			Contains al.h, alc.h, alut.h, efx.h, efx-creative.h, EFS-Util.h, xram.h

		Drawing (Directory)
			Contains Draw.cpp, Draw.h

		Events (Directory)
			Contains Event.cpp, Event.h

		global (Directory)
			Contains globref.h

		GL (Directory)
			Contains glew.h, glut.h, glxew.h, wglew.h

		glx (Directory)
			Contains glext.h, glxext.h

		IL (Directory)
			Contains config.h, devil_internal_exports.h, il.h, il_wrap.h, ilu.h, ilu_region.h, ilut.h, ilut_config.h

		Modelling (Directory)
			Contains anorms.h, anormtab.h, bmp.cpp, bmp.h, md2.cpp, md2.h, pcx.cpp, pcx.h, texture.cpp, texture.h, tga.cpp, tga.h
		
		lib (Directory)
			Contains alut.lib, DevIL.lib, EFX-Util.lib, glew32.lib, glew32s.lib, glut.lib, glut32.lib, ILU.lib, ILUT.lib, OpenAL32.lib

		Root (Directory)
			Contains Pacman.cpp

		textfile (Directory)
			Contains textfile.cpp, textfile.h

For Running:
	Requirements
		MUST HAVE VC++ 2010 x86 Redistributable Package INSTALLED. Included in Root Directory as "VC++ 2010 x86.exe".
		MUST HAVE OpenAL Redistributable Package INSTALLED. Included as "oalinst.exe".

		data (Directory)
			Contains level1.txt, log.txt created after running.

		Models (Directory)
			Contains Bass, Cloud, Megaman, Sephiroth, Wily, Zero Directories

		shaders (Directory)
			textureSimple.frag, textureSimple.vert

		sounds (Directory)
			Contains Loop.wav, Start.wav, stroke.wav

		textures (Directory)
			Contains green.bmp, life.bmp, neg_y.bmp, orange.bmp, red.bmp, Wall1.bmp, white.bmp

		DLLs
		 Root (Directory)
			Contains alut.dll DevIL.dll, freeglut.dll, glew32.dll, glut.dll, glut32.dll, ILU.dll, ILUT.dll, OpenAL32.dll