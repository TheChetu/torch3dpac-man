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
//#include "Config.h"		// Future Development

extern HDC			hDC;				// Private GDI Device Context
extern HGLRC		hRC;				// Permanent Rendering Context
extern HWND			hWnd;				// Holds Our Window Handle
extern HINSTANCE	hInstance;			// Holds The Instance Of The Application
extern GLsizei globwidth, globheight;	// Allocate Global Setting for Height and Width

static GLuint	base;				// Base Display List For The Font Set
static GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
static GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring
static bool buffEx = FALSE;			// Vertex Buffer Object State
static GLuint	verBO = 0;			// Vertex Buffer Object
static ofstream gloLog;				// Global Log

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