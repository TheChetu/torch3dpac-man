/*
	TorchMan
	Uses modified Lesson 10 code from nehe.gamedev.net for window creation, OpenGL initialization, and key handling.
	Usage: 
		WASD & Arrow keys to move:
		W/Up Arrow: Move Forward.
		A: Look Left.
		Left Arrow: Turn Left.
		S: Look Behind.
		Down Arrow: Move Backward.
		D: Look Right.
		Right Arrow: Turn Right.
		Shift + W/Up: "Sprint", moves double speed while sprint gauge is not empty.
		F: Toggle FPS.
		X: Debug Logging Toggle.
		Escape: Exit.		
 */

#include <Drawing\Draw.h>
#include <textfile\textfile.h>
#include "resource\resource.h"


#define WS_NO_BORDER 0xFF7FFFFF		// Remove Window Borders
#define P_ICON MAKEINTRESOURCE(IDI_ICON1)	// Create Icon Integer

/***************** Shader Declarations ***************/
GLint	loc;
GLuint	// Texture Shaders
		t_v,
		t_f,
		tShaders,
		l_v,
		l_f,
		lShaders;

/**************** Animation Declarations *************/
CMD2Model		Cloud;
CMD2Model		ClWeapon;

CMD2Model		Bass;
CMD2Model		BaWeapon;

CMD2Model		Zero;
CMD2Model		ZeWeapon;

CMD2Model		MegaMan;
CMD2Model		MeWeapon;

CMD2Model		Wily;
CMD2Model		WiWeapon;

int				AniNum		=	0;
long			AniElapsed	=	0;
bool			bTextured	= true;
bool			bLighGL		= false;
bool			bAnimated	= true;
float			angle		= 0.0;
extern float	g_angle;

long gDeadAni1;				// Time Elapsed For Ghost Animation
long gDeadAni2;				// Time Elapsed For Ghost Animation
long gDeadAni3;				// Time Elapsed For Ghost Animation
long gDeadAni4;				// Time Elapsed For Ghost Animation
int GAniNum1;				// Ghost 1 Animation
int GAniNum2;				// Ghost 2 Animation
int GAniNum3;				// Ghost 3 Animation
int GAniNum4;				// Ghost 4 Animation
/**************** End Animation Declarations *************/

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine

// Track State
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	blend = FALSE;		// Blending ON/OFF
bool	bp;					// B Pressed?
bool	fp;					// F Pressed?
bool	dp;					// D Pressed?
bool	ap;					// A Pressed?
bool	sp;					// S Pressed?
bool	vp;					// V Pressed?
bool	wp;					// W Pressed?
bool	onep;				// 1 Pressed?
bool	twop;				// 2 Pressed?
bool	threep;				// 3 Pressed?
bool	fourp;				// 4 Pressed?
bool	flight = TRUE;		// Flash Light ON/OFF
bool	alight = TRUE;		// Ambient Light ON/OFF
bool	glight = TRUE;		// Ghost Light ON/OFF
bool	plight = TRUE;		// Pacman Light ON/OFF
bool	pDead = FALSE;		// PacMan Dead?
bool	gDead1 = TRUE;		// Ghost 1 Dead?
bool	gDead2 = TRUE;		// Ghost 2 Dead?
bool	gDead3 = TRUE;		// Ghost 3 Dead?
bool	gDead4 = TRUE;		// Ghost 4 Dead?
bool	gEdible = FALSE;	// Ghosts Edible?
bool	levelCom = TRUE;	// Level Completion
bool	levelStr = FALSE;	// Level Started
int		currLevel =  1;		// The Level of Gameplay
int		frameCount = 0;		// Counter for the number of frames drawn
double	FPS	= 0;			// Current FPS
int		currScore = 0;		// Current Score
int		TPtimer = 0;		// TP timer to prevent continuous teleportation
int		alcheck = 0;		// Check Current State of Audio Source
int		currLives = 4;		// Current Lives
int		dotsRemaining = 0;	// Number of dots still in play
int		vsync = 1;			// Vertical Sync State 1 = True, 0 = False
bool	gameOver = FALSE;	// Set the state of the game ending
extern ofstream gloLog;		// Global Log
GLsizei globwidth, globheight; // Allocate Global Setting for Height and Width
vector<char> worldLayout;	// World Layout Storage
long reElapsed = 0;			// Reward Elapsed Time
bool dbug = FALSE;			// Debug Logging

vector<TLoc> lctn;				// Translation Locations
vector<zLoc> dotpos;			// Positioning of Dots
vector<GhP> gLocs;				// Ghost Positions
vector<gMap> gMapSet;			// Ghost Map
qwall q1, q2, q3, q4;			//walls separated into 4 quadrants

GhP SpawnLoc;					// Ghost Spawn Location

// PI Constants
const double sPI = 3.1415926535897;
const float PI = 3.14159265f;
const float piover180 = 0.0174532925f;

// Controls positioning
float heading;		// direction
float xpos;			// x-position in 3D space
float zpos;			// z-position in 3D space
float strLocx;		// starting x-pos
float strLocz;		// starting z-pos

FILETIME theTime;	// time in milliseconds

// Controls walking
GLfloat	yrot;				// Y-Rotation, about the y-axis (turning)
GLfloat sGauge = 20.0f;		// Sprinting Gauge


GLuint	filter;				// Which Filter To Use
GLuint	texture[10];		// Storage For 10 Textures

// Sound 
#define NUM_BUFFERS 5
#define NUM_SOURCES 5

ALfloat listenerPos[]={0.0,0.0,0.0};	
ALfloat listenerVel[]={0.0,0.0,0.0};	
ALfloat	listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};

ALfloat sourcePos[]={ -2.0, 0.0, 0.0};
ALfloat sourceVel[]={ 0.0, 0.0, 0.0};

ALuint buffers[NUM_BUFFERS];
ALuint source[NUM_SOURCES];

int GLwin;

ALsizei size,freq;
ALenum format;
ALvoid *data;
int ch;

// Texture File Names
char *Wall1Bitmap	= "textures\\Wall1.bmp";
char *GroundBitmap	= "textures\\neg_y.bmp";
char *PLives		= "textures\\life.bmp";
char *White			= "textures\\white.bmp";
char *Red			= "textures\\red.bmp";
char *Green			= "textures\\green.bmp";
char *Orange		= "textures\\orange.bmp";

GLUquadricObj *quadratic;				// Storage For Quadratic Objects


// Prototypes
int glLoadTexture();
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
GLvoid KillGLWindow(GLvoid);							// Declaration For KillGLWindow
extern void PrintToLog(const char* prnErr);				// Print To Log Definition

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	globheight = height;								// Set Global Height
	globwidth = width;									// Set Global Width

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(35.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
	glRotatef(10.0f,1.0f,0.0f,0.0f);					// Angle Camera Downward
	glTranslatef(0.0f,-3.0f,-15.0f);					// Increase Height of Camera Position
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	
}

void setShaders() 
{

	char *ts = NULL, *fts = NULL; // texture

	t_v = glCreateShader(GL_VERTEX_SHADER);   // texture
	t_f = glCreateShader(GL_FRAGMENT_SHADER); // texture

	ts = textFileRead("shaders\\textureSimple.vert"); // texture
	fts = textFileRead("shaders\\textureSimple.frag");// texture

	if (sizeof(ts) == 0 || sizeof(fts) == 0)
	{
		PrintToLog("Texture Shaders do not exist");
	}

	const char * tt = ts;			// texture
	const char * tff = fts;			// texture

	glShaderSource(t_v, 1, &tt,NULL);// texture
	glShaderSource(t_f, 1, &tff,NULL);// texture

	free(ts);free(fts); // texture
	
	glCompileShader(t_v); // texture
	glCompileShader(t_f); // texture

	GLint result = 0;

	// Check Compilation Status
	glGetShaderiv(t_v, GL_COMPILE_STATUS, &result);
	if(!result) {
		string prn = "Could not compile shader "; prn += t_v;
		PrintToLog(prn.c_str());
	}
	glGetShaderiv(t_f, GL_COMPILE_STATUS, &result);
	if(!result) {
		string prn = "Could not compile shader "; prn += t_f;
		PrintToLog(prn.c_str());
	}
	
	PrintToLog("Shaders Set");
	
	tShaders = glCreateProgram(); // texture
	glAttachShader(tShaders,t_v); // texture
    glAttachShader(tShaders,t_f); // texture

	glLinkProgram(tShaders);	  // texture
	printProgramInfoLog(tShaders);// texture

	glUseProgram(tShaders);

}


int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	// Check if Log is Open
	if(gloLog.is_open()); // do nothing
	else // open log
		gloLog.open("data\\log.txt",ios_base::out);

	// Initialize Timer
	initTime();

	//****************************** GLEW Initialization *********************************
	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		PrintToLog("Ready for OpenGL 2.0");
	else {
		PrintToLog("OpenGL 2.0 not supported");
		exit(1);
	}
	//****************************** End GLEW Initialization *****************************
	
	//******************************** AL Initialization **********************************
	char al_bool;
	ALfloat		listenerPos[]={0.0,0.0,0.0};				// At the origin
	ALfloat		listenerVel[]={0.0,0.0,0.0};				// The velocity (no doppler here)
	ALfloat		listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};	// LookAt then Up

	// Init openAL
	alutInit(0, NULL);	
	// Clear Error Code
	alGetError(); 

	// Generate buffers, or no sound will be produced
	alGenBuffers(NUM_BUFFERS, buffers);

	if(alGetError() != AL_NO_ERROR) {
		PrintToLog("Error creating sound buffers !!");
		exit(1);
	}
	else {
		PrintToLog("Created sound buffers");
	}

	// Start Sound Load
	alutLoadWAVFile("sounds//Start.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[0],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	// Looping Music Load
	alutLoadWAVFile("sounds//Loop.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[1],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	// Slash Sound Load
	alutLoadWAVFile("sounds//stroke.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[2],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if(alGetError() != AL_NO_ERROR) {
		PrintToLog("Error creating sound sources !!");
		exit(2);
	}
	
	// Start Sound
	alSourcef(source[0],AL_PITCH,1.0f);
	alSourcef(source[0],AL_GAIN,1.0f);
	alSourcefv(source[0],AL_POSITION,sourcePos);
	alSourcefv(source[0],AL_VELOCITY,sourceVel);
	alSourcei(source[0],AL_BUFFER,buffers[0]);
	alSourcei(source[0],AL_LOOPING,AL_FALSE);
	alSourcePlay(source[0]);

	// Looping Music
	alSourcef(source[1],AL_PITCH,1.0f);
	alSourcef(source[1],AL_GAIN,1.0f);
	alSourcefv(source[1],AL_POSITION,sourcePos);
	alSourcefv(source[1],AL_VELOCITY,sourceVel);
	alSourcei(source[1],AL_BUFFER,buffers[1]);
	alSourcei(source[1],AL_LOOPING,AL_TRUE);

	// Slash Sound
	alSourcef(source[2],AL_PITCH,1.0f);
	alSourcef(source[2],AL_GAIN,2.0f);
	alSourcefv(source[2],AL_POSITION,sourcePos);
	alSourcefv(source[2],AL_VELOCITY,sourceVel);
	alSourcei(source[2],AL_BUFFER,buffers[2]);
	alSourcei(source[2],AL_LOOPING,AL_FALSE);
	//*************************** End AL Initialization ***************************************

	//*************************** IL Initialization *******************************************
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	if(!glLoadTexture()) {
		PrintToLog("Failed to create texture !!");
		exit(3);
	}
	//**************************** End IL Initialization ****************************************
	
	//**************************** Lighting and GL Initialization *******************************
	glEnable(GL_LIGHTING);								// Enable Lighting
	glEnable(GL_LIGHT0);								// Flash Light
	glEnable(GL_LIGHT1);								// Ambient Lightning
	glEnable(GL_LIGHT2);								// Ghost 1 Lighting
	glEnable(GL_LIGHT3);								// Ghost 2 Lighting
	glEnable(GL_LIGHT5);								// Ghost 3 Lighting
	glEnable(GL_LIGHT6);								// Ghost 4 Lighting
	glEnable(GL_LIGHT4);								// Fixed Light
	glEnable(GL_TEXTURE_2D);							// Allow Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	BuildFont();										// Build The Font

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	// Flash Light
	GLfloat light_position_0[] = { 0.0f, 0.0f, 0.0f, 1.0f };		// Positioned at the Camera Origin
	GLfloat light_color_0[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// White Light Color
	GLfloat light_direction_0[] = { 0.0f, 0.0f, -1.0f, 1.0f };		// Light in the -Z direction
	// Set Light Attenuation
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
	// Specify Light Aspects
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color_0);				// Specular Coloring
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 15.0f);					// With 30 Degree Angle
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_0);			// Light At Viewpoint
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,light_direction_0);		// Spot Light
	glMaterialfv(GL_FRONT, GL_SPECULAR, light_color_0);				// Set Material Light Effect	
	glMaterialfv(GL_BACK, GL_SPECULAR, light_color_0);				// Set Material Light Effect
	glMaterialfv(GL_LEFT, GL_SPECULAR, light_color_0);				// Set Material Light Effect
	glMaterialfv(GL_RIGHT, GL_SPECULAR, light_color_0);				// Set Material Light Effect

	
	// Ambient Light
	GLfloat light_ambient[] = {-50.0f, 50.0f, -50.0f, 1.0f};			// Position of Ambient Light
	GLfloat ambientColor[4] = {0.6f, 0.7f, 0.9f, 0.7f};				// Color of Ambient Light
	GLfloat ambientCoeff[] = {0.2f, 0.2f, 0.2f, 0.7f};				// Strength of Effect
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeff);				// Set Effect Strength
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientColor);					// Set Ambient Color
	glLightfv(GL_LIGHT1, GL_POSITION, light_ambient);				// Set Position of Ambient Light

	// Used for simple drawing of Sphere and Cylinder
	quadratic=gluNewQuadric();					// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords

	//********************************** End Lighting and GL Initialization *************************
	

	//********************************** Model and Animation Initialization *************************

	CTextureManager::GetInstance()->Initialize();
	// load and initialize the Cloud model
	Cloud.LoadModel( "Models/Cloud/tris.md2" );
	Cloud.LoadSkin( "Models/Cloud/cloud.pcx" );
	Cloud.SetAnim(AniNum);
	Cloud.ScaleModel( 0.05f );

	// load and initialize Cloud weapon model
	ClWeapon.LoadModel( "Models/Cloud/Weapon.md2" );
	ClWeapon.LoadSkin( "Models/Cloud/Weapon.pcx" );
	ClWeapon.SetAnim( STAND );
	ClWeapon.ScaleModel( 0.05f );

	// load and initialize the Sephiroth model
	Bass.LoadModel( "Models/Bass/tris.md2" );
	Bass.LoadSkin( "Models/Bass/bass.pcx" );
	Bass.SetAnim(AniNum);
	Bass.ScaleModel( 0.07f );
	
	// load and initialize Sephiroth weapon model
	BaWeapon.LoadModel( "Models/Bass/Weapon.md2" );
	BaWeapon.LoadSkin( "Models/Bass/Weapon.pcx" );
	BaWeapon.SetAnim( STAND );
	BaWeapon.ScaleModel( 0.07f );

	// load and initialize the Zero model
	Zero.LoadModel( "Models/Zero/TRIS.MD2" );
	Zero.LoadSkin( "Models/Zero/ZERO.PCX" );
	Zero.SetAnim(AniNum);
	Zero.ScaleModel( 0.07f );
	
	// load and initialize Zero weapon model
	ZeWeapon.LoadModel( "Models/Zero/WEAPON.md2" );
	ZeWeapon.LoadSkin( "Models/Zero/WEAPON.pcx" );
	ZeWeapon.SetAnim( STAND );
	ZeWeapon.ScaleModel( 0.07f );

	// load and initialize the MegaMan model
	MegaMan.LoadModel( "Models/Megaman/TRIS.MD2" );
	MegaMan.LoadSkin( "Models/Megaman/YMEGAMAN.PCX" );
	MegaMan.SetAnim(AniNum);
	MegaMan.ScaleModel( 0.07f );
	
	// load and initialize Megaman weapon model
	MeWeapon.LoadModel( "Models/Megaman/WEAPON.md2" );
	MeWeapon.LoadSkin( "Models/Megaman/WEAPON.pcx" );
	MeWeapon.SetAnim( STAND );
	MeWeapon.ScaleModel( 0.07f );

	// load and initialize the Wily model
	Wily.LoadModel( "Models/Wily/TRIS.MD2" );
	Wily.LoadSkin( "Models/Wily/Wily.PCX" );
	Wily.SetAnim(AniNum);
	Wily.ScaleModel( 0.07f );
	
	// load and initialize Wily weapon model
	WiWeapon.LoadModel( "Models/Wily/WEAPON.md2" );
	WiWeapon.LoadSkin( "Models/Wily/WEAPON.pcx" );
	WiWeapon.SetAnim( STAND );
	WiWeapon.ScaleModel( 0.07f );

	//**************************** End Model and Animation Initialization *************************

	srand(time(0));		// Seed Random

	//Set the shaders
	setShaders();

	return TRUE;								// Initialization Went OK
}

int glLoadTexture() 
{
	int Status=FALSE;

	texture[0] = ilutGLLoadImage(GroundBitmap);	// Load Image into OpenGL Texture
	ILenum error = ilGetError();
	if (error == IL_NO_ERROR) {							// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 0 (Ground)
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	if(Status == FALSE) {
		string Prn = GroundBitmap;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}

	Status=FALSE;

	texture[1] = ilutGLLoadImage(PLives);	// Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {					// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 1 (Lives)
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	
	if(Status == FALSE) {
		string Prn = PLives;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}

	Status=FALSE;

	texture[2] = ilutGLLoadImage(Wall1Bitmap);	// Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {					// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 2 (Wall)
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	
	if(Status == FALSE) {
		string Prn = Wall1Bitmap;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}

	Status=FALSE;

	texture[3] = ilutGLLoadImage(White);	// Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {					// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 3 (White)
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	
	if(Status == FALSE) {
		string Prn = White;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}

	texture[4] = ilutGLLoadImage(Red);	// Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {					// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 4 (Red)
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	
	if(Status == FALSE) {
		string Prn = Red;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}

	texture[5] = ilutGLLoadImage(Green);	// Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {					// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 5 (Green)
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	
	if(Status == FALSE) {
		string Prn = Green;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}

	texture[6] = ilutGLLoadImage(Orange);	// Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {					// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 6 (Orange)
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	
	if(Status == FALSE) {
		string Prn = Orange;
		Prn = "Error on texture load at " + Prn;
		PrintToLog(Prn.c_str());
	}


	return Status;
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	// Draw Interface
	frameCount++;
	// Calculate FPS
	FILETIME temp = theTime;
	GetSystemTimeAsFileTime(&theTime);
	double fps_p = 0.0;
	if(((theTime.dwLowDateTime ) - (temp.dwLowDateTime)) > 1000) {
		FPS = (double)frameCount;
		frameCount = 0;
		fps_p = (FPS*1000.0)/(theTime.dwLowDateTime-temp.dwLowDateTime)*10000.0;
		FPS = fps_p;
	}
	else {
		theTime = temp;
	}
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(0.0f,0.0f,-12.0f);
	glMatrixMode(GL_MODELVIEW);
	// Draw FPS
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	if(filter >= 1) {
		if(FPS <= 20) {
			glBindTexture(GL_TEXTURE_2D,texture[4]);
			glColor3f(1.0f,0.0f,0.0f);
		}
		else if(FPS <= 45) {
			glBindTexture(GL_TEXTURE_2D,texture[6]);
			glColor3f(1.0f,1.0f,0.0f);
		}
		else {
			glBindTexture(GL_TEXTURE_2D,texture[5]);
			glColor3f(0.0f,1.0f,0.0f);
		}
		glColor3f(0.0f,0.0f,0.0f);
		glRasterPos2f(-6.0f,4.7f);  // Set Position of FPS
 		glPrint("FPS: %4.2f", FPS);	// Print GL Text To The Screen
		glBindTexture(GL_TEXTURE_2D,texture[3]);
	}
	// Draw Score
	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2f(-1.0f,4.5f);	// Set Position of Score
	glPrint("Score: %d",currScore);	// Score
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	// Draw Lives
	glDisable(GL_LIGHTING);
	glRasterPos2f(3.5f,4.7f);
	glPrint("Lives");
	glTranslatef(4.0f,4.5f,0.0f);
	glRotatef(180.0f,0,1,0);
	glRotatef(180.0f,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	if(currLives > 5)
		glTranslatef(1.0f,0.0f,0.0f);
	for(int i = currLives; i > 0; i--) {
		glTranslatef(-0.4f,0.0f,0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(0.4f,0.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(0.4f,0.4f,0.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(0.0f,0.4f,0.0f);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	// Draw Dots Remaining
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRasterPos2f(-1.0f,3.5f);
	glPrint("Dots Left: %d", dotsRemaining);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	// Draw Current Level
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRasterPos2f(-6.0f,4.0f);
	glPrint("Level %d", currLevel);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	// Draw Sprint Gauge
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRasterPos2f(-3.0f,4.0f);
	glPrint("Sprint Meter: ");
	glTranslatef(0.3f,4.3f,0.0f);
	glRotatef(180.0f,0,1,0);
	glRotatef(180.0f,1,0,0);
	glBindTexture(GL_TEXTURE_2D,texture[5]);
	for(float i = sGauge; i > 0; i = i - 0.1f) {
		glTranslatef(-0.01f,0.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.01f,0.0f,0.0f);
			glVertex3f(0.01f,0.4f,0.0f);
			glVertex3f(0.0f,0.4f,0.0f);
		glEnd();
	}
	glColor3f(0.0f,0.0f,0.0f);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();									// Reset The View
	// Enable and Disable Lighting
	Draw::Lighting();

	if(pDead) {
		xpos = strLocx;
		zpos = strLocz;
		AniNum = 18;
		Cloud.SetAnim(AniNum);
		ClWeapon.SetAnim(AniNum);
		AniElapsed = elapsed();
		pDead = FALSE;

		gDead1 = TRUE;
		gDead2 = TRUE;
		gDead3 = TRUE;
		gDead4 = TRUE;
	}

	// Display Eat the Ghosts Message
	if(gEdible) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glTranslatef(0.0f,0.0f,-1.0f);
		glMatrixMode(GL_MODELVIEW);
		// Draw Message
		glPushMatrix();
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glRasterPos2f(-0.5f,3.0f);
			if(((elapsed() - reElapsed) / 1000)  > 15) {
				glBindTexture(GL_TEXTURE_2D,texture[4]);
				glColor3f(0.0f,1.0f,0.0f);
			}
			else if(((elapsed() - reElapsed) / 1000) < 5) {
				glBindTexture(GL_TEXTURE_2D,texture[5]);
				glColor3f(1.0f,0.0f,0.0f);
			}
			else {
				glBindTexture(GL_TEXTURE_2D,texture[6]);
				glColor3f(1.0f,1.0f,0.0f);
			}
			glPrint("EAT THE GHOSTS!!!");
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	// PacMan
	glPushMatrix();
		//glTranslatef(-50.0f,-50.0f,0.0f);
		Draw::PacMan();
	glPopMatrix();
	
	// Reset Camera Prior to Movement
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f,(GLfloat)globwidth/(GLfloat)globheight,0.1f,200.0f);
	glRotatef(10.0f,1.0f,0.0f,0.0f);					// Angle Camera Downward
	// if near wall then translate closer than -15.0f
		//glTranslatef(0.0f,-3.0f,-3.0f);					// Increase Height of Camera Position
	// else translate -15.0f
		//glTranslatef(0.0f,-3.0f,-15.0f);					// Increase Height of Camera Position
		glTranslatef(0.0f,-3.0f,-3.0f);					// Increase Height of Camera Position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Calculate Translation and Rotations
	GLfloat xtrans = -xpos;							// Movement in the X-direction
	GLfloat ztrans = -zpos;							// Movement in the Z-direction
	GLfloat ytrans = 0;								// No Vertical Movement
	GLfloat sceneroty = 360.0f - yrot;				// Rotation around the Y-axis

	glRotatef(sceneroty,0,1.0f,0);					// Rotation around the Y-axis (looking around)

	// Skybox
	//Draw::SkyBox();
	
	glTranslatef(xtrans, 0.0f, ztrans);		// Move in the X and Z directions the correct respective amounts

	// Draw Plane
	Draw::Plane();
	
	// Draw World
	Draw::World();

	// Detect Collision
	if(Event::CheckCollideDot()) {
		if(sGauge < 30.0f)
			sGauge += 0.5f;
	}
	Event::CheckCollideWall(Event::currentQ());

	int colGho = 0;
	colGho = Event::CheckCollideGhosts();
	currScore += colGho;
	if(colGho < 0)
		currLives--;

	// Check Rewards
	Event::Reward();

	// Move Ghosts
	if(alcheck != AL_STOPPED);
	else
		Event::MoveGhosts();

	// Do Nothing Until Start Music Finished
	while(alcheck != AL_STOPPED) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		// Keep World and PacMan Drawn at Initial State
		Draw::PacMan();
		Draw::Plane();
		Draw::World();

		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glTranslatef(0.0f,0.0f,-10.0f);
			glMatrixMode(GL_MODELVIEW);
			// Draw Message
			glPushMatrix();
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture[3]);
				glLoadIdentity();
				glDisable(GL_LIGHTING);
				glRasterPos2f(-2.0f,3.5f);
				glColor3f(1.0f,0.0f,0.0f);
				glPrint("Welcome to TorchMan");
				glEnable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		SwapBuffers(hDC);
		alGetSourcei(source[0],AL_SOURCE_STATE,&alcheck);
		// Start Looping Music
		if(alcheck == AL_STOPPED) {
			alSourcePlay(source[1]);
		}
	}
	
	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}

	PrintToLog("Exiting Program");

	//Garbage Cleanup
	gloLog.close();			// Close the Log
	KillFont();				// Destroy The Font
	lctn.clear();			// Translation Locations
	dotpos.clear();			// Positioning of Dots
	gLocs.clear();			// Ghost Positions
	gMapSet.clear();		// Ghost Map
	worldLayout.clear();	// World Map

}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASSEX	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.cbSize			= sizeof(WNDCLASSEX);					// Set Size of Block
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(hInstance, P_ICON);				// Load The Default Icon
	wc.hIconSm			= LoadIcon(hInstance, P_ICON);				// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Failed to register the window class !!");
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
/*		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				PrintToLog("Cannot display fullscreen !!");
				return FALSE;									// Return FALSE
			}
		}*/
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_TOPMOST;				// Window Extended Style
		dwStyle=WS_POPUP | WS_MAXIMIZE;			// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Window creation error !!");
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Cannot create a GL Device Context !!");
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Cannot find a suitable PixelFormat !!");
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Cannot set the PixelFormat !!");
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Cannot create a GL Rendering Context !!");
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Cannot activate the GL Rendering Context !!");
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		PrintToLog("Initialization failed !!");
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	DWORD dwWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	DWORD dwHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
		dwWidth = 800;
		dwHeight = 600;
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("TorchMan",dwWidth,dwHeight,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				alSourceStop(source[0]);
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)

				if(levelStr) {
					if(dotsRemaining == 0) {
						levelCom = TRUE;
						levelStr = FALSE;
						currLevel++;
						ifstream checkFile;
						char* openCheck = NULL;
						openCheck = (char*)malloc(sizeof("data\\level100.txt"));
						sprintf(openCheck,"data\\level%d.txt",currLevel);
						checkFile.open(openCheck);
						for(int i = 0; i < 10; i++) {
							Rewards[i] = FALSE;
						}
						EveInit = FALSE;		// Allow for ReInitialization
						lctn.clear();			// Translation Locations
						dotpos.clear();			// Positioning of Dots
						gLocs.clear();			// Ghost Positions
						gMapSet.clear();		// Ghost Map
						worldLayout.clear();	// World Map

						// Clear Wall Locations for Collision
						q1.xwalls.clear();
						q1.zwalls.clear();
						q2.xwalls.clear();
						q2.zwalls.clear();
						q3.xwalls.clear();
						q3.zwalls.clear();
						q4.xwalls.clear();
						q4.zwalls.clear();

						if(!checkFile.is_open())
							gameOver = TRUE;
					}
				}

			// Game Over
			if((currLives == 0) || gameOver) {
				long exitTime = elapsed();
				bool exitIt = FALSE;
				while(!exitIt) {
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
					glMatrixMode(GL_PROJECTION);
					glPushMatrix();
					glTranslatef(0.0f,0.0f,-1.0f);
					glMatrixMode(GL_MODELVIEW);
					// Draw Message
					glPushMatrix();
						glLoadIdentity();
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D,texture[4]);
						glDisable(GL_LIGHTING);
						glRasterPos2f(-0.5f,3.0f);
						glColor3f(1.0,0.0,0.0);
						if(currLives == 0) {
							glPrint("GAME OVER");
							glRasterPos2f(-0.5f,2.5f);
							glPrint("Exiting in %d",(10-((elapsed()-exitTime)/1000)));
						}
						else {
							glBindTexture(GL_TEXTURE_2D,texture[5]);
							glPrint("CONGRATULATIONS YOU WON!");
							glRasterPos2f(-0.5f,2.5f);
							glPrint("Exiting in %d",(10-((elapsed()-exitTime)/1000)));
						}
						glDisable(GL_TEXTURE_2D);
						glEnable(GL_LIGHTING);
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
					glPopMatrix();
					glMatrixMode(GL_MODELVIEW);
					SwapBuffers(hDC);
					if((elapsed() - exitTime) >= 10000) {
						exitIt = TRUE;
						alSourceStop(source[0]);
						done=TRUE;
					}
				}		
			}

				if(blend) {
					glEnable(GL_BLEND);
					if((elapsed() - reElapsed) >= 20000) { // 20 seconds
						glDisable(GL_BLEND);
						blend = FALSE;
					}
				}

				if(gEdible) {
					// Show Edible
					if((elapsed() - reElapsed) >= 20000) { // 20 seconds
						gEdible = FALSE;
					}
				}

				if (keys['F'] && !fp)
				{
					fp=TRUE;
					filter+=1;
					if (filter>1)
					{
						filter=0;
					}
				}
				if (!keys['F'])
				{
					fp=FALSE;
				}

				// Change Walk Animation After Swing
				if(AniNum == 2 || AniNum == 18) {
					if((elapsed() - AniElapsed) >= 750) {
						AniNum = 1;
						Cloud.SetAnim(AniNum);
						ClWeapon.SetAnim(AniNum);
						AniElapsed = 0;
					}
				}

				// Walk Forward
				if (keys[VK_UP] || keys['W'])
				{	
					//****play sound *** forward//
					if(wp == FALSE) {
						AniNum = 1;
						Cloud.SetAnim(AniNum);
						ClWeapon.SetAnim(AniNum);
						wp = TRUE;
					}
									
					// Sprint, double movement
					if ((keys[VK_SHIFT]) && (sGauge > 0.0f))
					{
						sGauge -= 6.0f / FPS;
						xpos -= (float)sin(heading*piover180) * (6.0f / FPS);
						zpos -= (float)cos(heading*piover180) * (6.0f / FPS);
					}
					// Walk, normal movement
					else
					{
						xpos -= (float)sin(heading*piover180) * (3.5f / FPS);
						zpos -= (float)cos(heading*piover180) * (3.5f / FPS);
					}
				}

				if(!keys['W'] && !keys[VK_UP])
				{
					if((wp) && (AniNum != 0)) {
						AniNum = 0;
						Cloud.SetAnim(AniNum);
						ClWeapon.SetAnim(AniNum);
						wp = FALSE;
					}
				}

				// Turn Around
				if (keys['S'] && !sp)
				{
					sp = TRUE;
					heading -= 180.0f;
					yrot = heading;
				}

				if (!keys['S'])
				{
					sp = FALSE;
				}

				// Walk Backward
/*				if(keys[VK_DOWN])
				{			
					if(xpos < 50.0f && xpos > -50.0f) {
						xpos += (float)sin(heading*piover180) * (3.0f / FPS);
					}
					if(zpos < 50.0f && zpos > -50.0f) {
						zpos += (float)cos(heading*piover180) * (3.0f / FPS);
					}
				}
*/
				// Turn Around
				if (keys['D'] && !dp)
				{
					dp = TRUE;
					heading -= 90.0f;
					yrot = heading;
				}

				if(!keys['D'])
				{
					dp = FALSE;
				}

				// Turn Right
				if(keys[VK_RIGHT])
				{
					heading -= (60.0f / FPS);
					yrot = heading;
				}

				// Turn to the Left
				if (keys['A'] && !ap)
				{
					ap = TRUE;
					heading += 90.0f;	
					yrot = heading;
				}

				if(!keys['A'])
				{
					ap = FALSE;
				}

				// Turn Left
				if(keys[VK_LEFT])
				{
					heading += (60.0f / FPS);
					yrot = heading;
				}

				// Debug Logging
				if (keys['B'] && !bp)
				{
					bp = TRUE;
					dbug = !dbug;
				}

				if (!keys['B'])
				{
					bp = FALSE;
				}

				// Vertical Sync
				if (keys['V'] && !vp)
				{
					vp = TRUE;
					if(vsync == 1) {
						vsync = 0;
						wglSwapIntervalEXT(0);
					}
					else {
						vsync = 1;
						wglSwapIntervalEXT(1);
					}
				}

				if (!keys['V'])
				{
					vp = FALSE;
				}

				// Flash Light
				if (keys['1'] && !onep)
				{
					onep = TRUE;
					// set flash light state
					if(flight)
						flight = FALSE;
					else
						flight = TRUE;
				}

				if (!keys['1'])
				{
					onep = FALSE;
				}

				// Ambient Light
				if (keys['2'] && !twop)
				{
					twop = TRUE;
					// set ambient light state
					if(alight)
						alight = FALSE;
					else
						alight = TRUE;
				}

				if (!keys['2'])
				{
					twop = FALSE;
				}

				// Ghost 1 Light
				if (keys['3'] && !threep)
				{
					threep = TRUE;
					// set ghost light state
					if(glight)
						glight = FALSE;
					else
						glight = TRUE;
				}

				if (!keys['3'])
				{
					threep = FALSE;
				}

				// Pacman Light
				if (keys['4'] && !fourp)
				{
					fourp = TRUE;
					// set fixed light state
					if(plight)
						plight = FALSE;
					else
						plight = TRUE;
				}

				if (!keys['4'])
				{
					fourp = FALSE;
				}

			/* Disable Fullscreen Switching
				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
					DWORD dwWidth = GetSystemMetrics(SM_CXFULLSCREEN);
					DWORD dwHeight = GetSystemMetrics(SM_CYFULLSCREEN);
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("TorchMan",dwWidth,dwHeight,16,fullscreen))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}*/
			}
		}
	}

	// Shutdown
	KillGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}
