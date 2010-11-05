#include <ctime>			// Header File For C Time
#include <cstdlib>			// Header File For C Standard Library
#include <windows.h>		// Header File For Windows Library
#include <math.h>			// Header File For Math Library 
#include <string>			// Header File For C++ String Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <vector>			// Header File For Vector Library
#include <fstream>			// Header File For File Stream Library
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <IL\il.h>			// Header File For The Developer's Image Library
#include <IL\ilu.h>			// Header File For The DevIL Utilities
#include <IL\ilut.h>		// Header File For The DevIL Tools
//#include "Config.h"		// Future Development

using namespace std;

extern HDC			hDC;		// Private GDI Device Context
extern HGLRC		hRC;		// Permanent Rendering Context
extern HWND			hWnd;		// Holds Our Window Handle
extern HINSTANCE	hInstance;		// Holds The Instance Of The Application

extern bool	keys[256];			// Array Used For The Keyboard Routine
// Track State
extern bool	active;				// Window Active Flag Set To TRUE By Default
extern bool	fullscreen;			// Fullscreen Flag Set To Fullscreen Mode By Default
extern bool	blend;				// Blending ON/OFF
extern bool	bp;					// B Pressed?
extern bool	fp;					// F Pressed?
extern bool	rp;					// R Pressed?
extern bool	dp;					// Right Arrow or D Pressed?
extern bool	ap;					// Left Arrow or A Pressed?
extern bool	sp;					// Down Arrow or S Pressed?
extern bool	onep;				// 1 Pressed?
extern bool	twop;				// 2 Pressed?
extern bool	threep;				// 3 Pressed?
extern bool	fourp;				// 4 Pressed?
extern bool	flight;				// Flash Light ON/OFF
extern bool	alight;				// Ambient Light ON/OFF
extern bool	glight;				// Ghost Light ON/OFF
extern bool	plight;				// Pacman Light ON/OFF
extern bool	pDead;				// PacMan Dead?
extern bool	gDead1;				// Ghost 1 Dead?
extern bool	gDead2;				// Ghost 2 Dead?
extern bool	gDead3;				// Ghost 3 Dead?
extern bool	gDead4;				// Ghost 4 Dead?
extern bool	levelCom;			// Level Completion
extern int	currLevel;			// The Level of Gameplay
extern vector<char> worldLayout;// World Layout Storage


// PI Constants
extern const double sPI;
extern const float PI;
extern const float piover180;

// Controls positioning
extern float heading;		// direction
extern float xpos;			// x-position in 3D space
extern float zpos;			// z-position in 3D space
extern float moonpos;		// moon-position in 3D space
extern FILETIME theTime;	// time in milliseconds

// Controls walking
extern GLfloat	yrot;		// Y-Rotation, about the y-axis (turning)
extern GLfloat sGauge;		// Sprinting Gauge


extern GLuint	filter;				// Which Filter To Use
extern GLuint	texture[8];			// Storage For 8 Texture


// Texture File Names
//char *GroundBitmap = "Earth.bmp";
extern char *Building1Bitmap;
//char *Building2Bitmap = "stonetile.bmp";
extern char *SkyBox1Bitmap;
extern char *SkyBox2Bitmap;
extern char *SkyBox3Bitmap;
extern char *SkyBox4Bitmap;
extern char *SkyBox5Bitmap;
extern char *SkyBox6Bitmap;

extern GLUquadricObj *quadratic;				// Storage For Quadratic Objects

struct GhostPos
{
	float xp,yp,zp;
	GhostPos()
	{
		xp = yp = zp = 0.0f;
	}

	GhostPos(float xp,float yp, float zp)
	{
		this->xp = xp;
		this->yp = yp;
		this->zp = zp;
	}
};


class Draw {
	private:
		static void RevOrder(int& tar, int first,int last);
	public:
		static void Lighting(GLvoid);
		static void PacMan(GLvoid);
		static void SkyBox(GLvoid);
		static void Plane(GLvoid);
		static void Top(int place);
		static void Bottom(int place);
		static void Left(int place);
		static void Right(int place);
		static void Corner(int place);
		static void Dots(int place);
		static void GZone(int place);
		static void Teleport(int place);
		static void TPWalls(int place);
		static void Start(int place);
		static void LoadWorld(GLvoid);
		static void World(GLvoid);
};