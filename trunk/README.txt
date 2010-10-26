// Assignment2 Folder Contains Code
Jacob Husein Assignment 2
	Uses modified Lesson 10 code from nehe.gamedev.net for window creation, OpenGL initialization, and input handling.
	Usage: 
		WASD & Arrow keys to move:
		W/Up Arrow: Move Forward.
		A/Left Arrow: Turn Left.
		S/Down Arrow: Move Backward.
		D/Right Arrow: Turn Right.
		Shift + W/Up: "Sprint", moves double speed.
		R: Reset.
		B: Enable Blending, changes Blending of Primitives to allow transparency.
		F: Enable Filtering between Textures and Primitives (disables/re-enables ground texture).
		PageUp: Look Up.
		PageDown: Look Down.
		Control: Crouch.
		1: Enable/Disable Flash Light (Default Enabled)
		2: Enable/Disable Ambient Light (Default Enabled)
		3: Enable/Disable Sun & Moon, Sky, Light (Default Enabled)
		4: Enable/Disable Fixed, Lamp, Light (Default Enabled)
		Escape: Exit.		
		Textures From: Assignment 2 Demo Program - Located in textures folder

Requires 	
		// The following is included in the OpenGL95 & DevIL directories
		// THe program is ready to compile when the files are in the proper location
		// VC Include(include) Directory, entire Directory contents must be moved
		// IL Folder (Directory)
			IL Folder (Subdirectory)
				Contains config.h, config.h.win, devil_internal_exports.h, il.h, il_wrap.h, ilu.h, ilu_region.h, ilut.h, ilut_config.h
		
		// VC Library(lib) Directory
		lib Folder
			Contains DevIL.lib, ILU.lib, ILUT.lib

		// DLLs
		 Root Program Folder
			Contains DevIL.dll, ILU.dll, ILUT.dll