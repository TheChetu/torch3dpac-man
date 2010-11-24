#ifndef GLOBALREF_H
#define GLOBALREG_H

// Ignore Non-Forced Depreciation
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>			// Header File For C Time
#include <cstdlib>			// Header File For C Standard Library
#include <windows.h>		// Header File For Windows Library
#include <math.h>			// Header File For Math Library 
#include <string>			// Header File For C++ String Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <vector>			// Header File For Vector Library
#include <fstream>			// Header File For File Stream Library
#include <iomanip>			// Header File For I/O Manipulation Library
#include <glee\GLee.h>		// Header File For The GL Easy Extension Library
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <IL\il.h>			// Header File For The Developer's Image Library
#include <IL\ilu.h>			// Header File For The DevIL Utilities
#include <IL\ilut.h>		// Header File For The DevIL Tools
#include <AL\al.h>			// Header File For The OpenAL Library
#include <AL\alc.h>			// Header File For The OpenAL Components
#include <AL\alut.h>		// Header File For The OpenAL Utilities
#include <Modelling\texture.h>
#include <Modelling\md2.h>
//#include <global\timer.h>	// Header File For The Program Timer
//#include <options\Config.h>	// Header File For External Options Resource

using namespace std;

extern HDC			hDC;				// Private GDI Device Context
extern HGLRC		hRC;				// Permanent Rendering Context
extern HWND			hWnd;				// Holds Our Window Handle
extern HINSTANCE	hInstance;			// Holds The Instance Of The Application
// Allocate Global Setting for Height and Width
extern GLsizei globwidth;
extern GLsizei globheight;

static GLuint	base;				// Base Display List For The Font Set
static GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
static GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring
static bool buffEx = FALSE;			// Vertex Buffer Object State
static GLuint	verBO = 0;			// Vertex Buffer Object
static GLuint	verBI = 0;			// Vertex Buffer Indices
static GLuint	verBbt;				// Vertex Buffer Texture Bottom and Top
static GLuint	verBlr;				// Vertex Buffer Texture Left and Right

static ofstream gloLog("data\\log.txt");	// Global Log

typedef struct TransLoc TLoc;
typedef struct GhostPos GhP;
typedef struct Vertex Ver;

// Current Ghost Position Structure
struct GhostPos {
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

// Translate Location Structure
struct TransLoc {
	GLint t;
	GLint x;
};


// Vertex Structure
struct Vertex 
{
    float x, y, z;
    Vertex() 
    {
        x = y = z = 0.0f;
    }

    Vertex(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vertex(const Vertex& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;        
    }

    Vertex& operator=(const Vertex& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;        

        return *this;
    }
};

/* Vertex Normalization */
static Vertex* normalize(Vertex* in) 
{
    float l = sqrtf(in->x * in->x + in->y * in->y + in->z * in->z);
    in->x = in->x / l;
    in->y = in->y / l;
    in->z = in->z / l;
    return in;
}


// For Moving Initialization from Pacman.cpp
/* MOVE INITIALIZATION IN FUTURE
bool SetLight(GLvoid)
{
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
	GLfloat light_ambient[] = {1.0f, 50.0f, -50.0f, 1.0f};			// Position of Ambient Light
	GLfloat ambientColor[4] = {0.6f, 0.7f, 0.9f, 0.7f};				// Color of Ambient Light
	GLfloat ambientCoeff[] = {0.2f, 0.2f, 0.2f, 0.7f};				// Strength of Effect
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeff);				// Set Effect Strength
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientColor);					// Set Ambient Color
	glLightfv(GL_LIGHT1, GL_POSITION, light_ambient);				// Set Position of Ambient Light

	return TRUE;
}

bool InitLight(GLvoid)
{
	glEnable(GL_LIGHTING);								// Enable Lighting
	glEnable(GL_LIGHT0);								// Flash Light
	glEnable(GL_LIGHT1);								// Ambient Lightning
	glEnable(GL_LIGHT2);								// Moon Lightning
	glEnable(GL_LIGHT3);								// Sun Lighting
	glEnable(GL_LIGHT4);								// Fixed Light

	return TRUE;
}

bool InitImage(GLvoid)
{
	//*************************** IL Initialization *******************************************
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	if(!glLoadTexture()) {
		PrintToLog("Failed to create texture !!");
		exit(3);
	}
	//**************************** End IL Initialization ****************************************

	return TRUE;
}

bool InitAudio(GLvoid) 
{
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
		PrintToLog("Error creating buffers !!");
		exit(1);
	}
	else {
		PrintToLog("Created buffers");
	}

	// Start Sound Load
	alutLoadWAVFile("sounds//Start.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[0],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	// Looping Music Load
	alutLoadWAVFile("sounds//Loop.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[1],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alGetError(); // clear error
	alGenSources(NUM_SOURCES, source);

	if(alGetError() != AL_NO_ERROR) {
		PrintToLog("Error creating sources !!");
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
	//*************************** End AL Initialization ***************************************

	return TRUE;
}

static int InitGL(GLvoid)
{
	// Initialize Timer
	initTime();

	// Initialize Log
	if(gloLog.is_open()); // do nothing
	else // open log
		gloLog.open("data\\log.txt",ios_base::out);

	InitAudio();

	InitImage();

	//**************************** Lighting and GL Initialization *******************************
	InitLight();

	glEnable(GL_TEXTURE_2D);							// Allow Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Initialize Font
	BuildFont();										// Build The Font

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	// Create Lights
	SetLight();

	// Used for simple drawing of Sphere and Cylinder
	quadratic=gluNewQuadric();					// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords

	//********************************** End Lighting and GL Initialization *************************

	return TRUE;								// Initialization Went OK

}
*/

// Simple Set Orthogonal Projection
static void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, globwidth, globheight, 0);
	
	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

// Simple Reset Projection
static void resetPerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


// From NeHe Lesson 13
//	Font Building, Printing, and Destruction
static GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-24,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

static GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

static GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

#endif