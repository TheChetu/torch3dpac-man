#ifndef DRAW_H
#define DRAW_H

#include <Events/Event.h>		// Global definitions

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
extern bool gEdible;			// Ghosts Edible?
extern bool	levelCom;			// Level Completion
extern bool levelStr;			// Level Started
extern int	currLevel;			// The Level of Gameplay
extern int frameCount;			// Current Number of Frames Rendered
extern double FPS;				// The Current FPS
extern int currScore;			// The Current Score
extern int dotsRemaining;		// Number of Dots Still in Play
extern int	TPtimer;			// TP timer to prevent continuous teleportation
extern vector<char> worldLayout;// World Layout Storage
extern bool buffEx;				// If Buffers Exist
extern GLuint verBO;			// Vertex Buffer Object
extern GLuint verBI;			// Vertex Buffer Indices
extern GLuint verBbt;			// Vertex Buffer Texture Bottom and Top
extern GLuint verBlr;			// Vertex Buffer Texture Left and Right
extern ofstream gloLog;			// Global Log File

extern vector<TLoc> lctn;		// Translation Locations
extern vector<GLint> VBO;		// Vertex Buffer Objects
extern vector<GhP> gLocs;		// Ghost Positions

extern GhP SpawnLoc;			// Ghost Spawn Location

// PI Constants
extern const double sPI;
extern const float PI;
extern const float piover180;

// Controls positioning
extern float heading;		// direction
extern float xpos;			// x-position in 3D space
extern float zpos;			// z-position in 3D space
extern float strLocx;		// starting x-pos
extern float strLocz;		// starting z-pos

extern FILETIME theTime;	// time in milliseconds

// Controls walking
extern GLfloat	yrot;		// Y-Rotation, about the y-axis (turning)
extern GLfloat sGauge;		// Sprinting Gauge


extern GLuint	filter;				// Which Filter To Use
extern GLuint	texture[10];			// Storage For 8 Texture


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


extern CMD2Model		Cloud;
extern CMD2Model		ClWeapon;
extern CMD2Model		Sephiroth;
extern CMD2Model		SeWeapon;
extern CMD2Model		Zero;
extern CMD2Model		ZeWeapon;
extern CMD2Model		MegaMan;
extern CMD2Model		MeWeapon;
extern CMD2Model		Wily;
extern CMD2Model		WiWeapon;

extern int		AniNum;
extern bool		bTextured;
extern bool		bLighGL;
extern bool		bAnimated;
extern float	angle;
extern float	g_angle;

// Ghost Waiting For Respawn
extern bool g1Wait;
extern bool g2Wait;
extern bool g3Wait;
extern bool g4Wait;
extern bool g1Wait;

class Draw {
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
		static void Ghosts(GLvoid);
};

#endif
