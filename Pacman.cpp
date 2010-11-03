/*
	Pac-man 3D
	Uses modified Lesson 10 code from nehe.gamedev.net for window creation, OpenGL initialization, and key handling.
	Usage: 
		WASD & Arrow keys to move:
		W/Up Arrow: Move Forward.
		A/Left Arrow: Turn Left.
		S/Down Arrow: Move Backward.
		D/Right Arrow: Turn Right.
		Shift + W/Up: "Sprint", moves double speed.
		R: Reset.
		B: Enable Blending, changes Blending of Primitives.
		F: Enable Filtering between Textures and Primitives.
		PageUp: Look Up.
		PageDown: Look Down.
		Control: Crouch.
		Escape: Exit.		
 */
#include <ctime>			// Header File For C Time
#include <cstdlib>			// Header File For C Standard Library
#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Math Library 
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
bool	rp;					// R Pressed?
bool	dp;					// Right Arrow or D Pressed?
bool	ap;					// Left Arrow or A Pressed?
bool	sp;					// Down Arrow or S Pressed?
bool	onep;				// 1 Pressed?
bool	twop;				// 2 Pressed?
bool	threep;				// 3 Pressed?
bool	fourp;				// 4 Pressed?
bool	flight = TRUE;		// Flash Light ON/OFF
bool	alight = TRUE;		// Ambient Light ON/OFF
bool	slight = TRUE;		// Moon & Sun (Sky) Light ON/OFF
bool	llight = TRUE;		// Fixed Lamp Light ON/OFF
bool	pDead = FALSE;		// PacMan Dead?
bool	gDead1 = FALSE;		// Ghost 1 Dead?
bool	gDead2 = FALSE;		// Ghost 2 Dead?
bool	gDead3 = FALSE;		// Ghost 3 Dead?
bool	gDead4 = FALSE;		// Ghost 4 Dead?
bool	levelCom = TRUE;	// Level Completion
int		currLevel =  1;		// The Level of Gameplay
vector<char> worldLayout;	// World Layout Storage


// PI Constants
const double sPI = 3.1415926535897;
const float PI = 3.14159265f;
const float piover180 = 0.0174532925f;

// Controls positioning
float heading;		// direction
float xpos;			// x-position in 3D space
float zpos;			// z-position in 3D space
float moonpos = 0;	// moon-position in 3D space
FILETIME theTime;	// time in milliseconds

// Controls walking
GLfloat	yrot;				// Y-Rotation, about the y-axis (turning)
GLfloat walkbias = 0;		// Current position of "head bob"
GLfloat walkbiasangle = 0;	// Allows for "head bobbing" realistic walking.
GLfloat lookupdown = 0.0f;	// Viewing height on the y-axis
GLfloat	z=0.0f;				// Depth Into The Screen
GLfloat sGauge = 20.0f;		// Sprinting Gauge


GLuint	filter;				// Which Filter To Use
GLuint	texture[8];			// Storage For 8 Texture


// Texture File Names
//char *GroundBitmap = "Earth.bmp";
char *Building1Bitmap = "textures\\brick.bmp";
//char *Building2Bitmap = "stonetile.bmp";
char *SkyBox1Bitmap = "textures\\neg_x.bmp";
char *SkyBox2Bitmap = "textures\\neg_y.bmp";
char *SkyBox3Bitmap = "textures\\neg_z.bmp";
char *SkyBox4Bitmap = "textures\\pos_x.bmp";
char *SkyBox5Bitmap = "textures\\pos_y.bmp";
char *SkyBox6Bitmap = "textures\\pos_z.bmp";

GLUquadricObj *quadratic;				// Storage For Quadratic Objects

//#define GL_CLAMP_TO_EDGE 0x812F	// OpenGL 1.2 definition - Unneeded


/* Vertex Struct
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
};
*/

// Prototypes
int glLoadTexture();
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc


/* Vertex Normalization
Vertex* normalize(Vertex* in) 
{
    float l = sqrtf(in->x * in->x + in->y * in->y + in->z * in->z);
    in->x = in->x / l;
    in->y = in->y / l;
    in->z = in->z / l;
    return in;
}
*/

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glRotatef(18.0f,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,-3.0f,-5.0f);						// Increase Height of Camera Position

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	if(!glLoadTexture()) {
		MessageBox(NULL,"Failed to create texture.","TEXTURE LOADING ERROR",MB_OK | MB_ICONSTOP);
		exit(1);
	}
	
	glEnable(GL_LIGHTING);								// Enable Lighting
	glEnable(GL_LIGHT0);								// Flash Light
	glEnable(GL_LIGHT1);								// Ambient Lightning
	glEnable(GL_LIGHT2);								// Moon Lightning
	glEnable(GL_LIGHT3);								// Sun Lighting
	glEnable(GL_LIGHT4);								// Fixed Light
	glEnable(GL_TEXTURE_2D);							// Allow Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	
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

	
	// Ambient Light
	GLfloat light_ambient[] = {1.0f, 50.0f, -50.0f, 1.0f};			// Position of Ambient Light
	GLfloat ambientColor[4] = {0.6f, 0.7f, 0.9f, 0.5f};				// Color of Ambient Light
	GLfloat ambientCoeff[] = {0.8f, 0.8f, 0.8f, 0.7f};				// Strength of Effect
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeff);				// Set Effect Strength
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientColor);					// Set Ambient Color
	glLightfv(GL_LIGHT1, GL_POSITION, light_ambient);				// Set Position of Ambient Light

	// Used for simple drawing of Sphere and Cylinder
	quadratic=gluNewQuadric();					// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords

	return TRUE;								// Initialization Went OK
}

int glLoadTexture() 
{
	int Status=FALSE;

	texture[0] = ilutGLLoadImage(SkyBox2Bitmap);	// Load Image into OpenGL Texture
	ILenum error = ilGetError();
	if (error == IL_NO_ERROR) {							// If the texture was successfully created
		Status=TRUE;                            // Set The Status To TRUE
		
		// Setup Texture 0 (Ground)
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	texture[1] = ilutGLLoadImage(Building1Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 1 (Building1)
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	Status = FALSE;

/*	Unused, Set Aside for Future Development
	texture[2] = ilutGLLoadImage(Building2Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 2 (Building2)
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	Status = FALSE;*/

	texture[3] = ilutGLLoadImage(SkyBox1Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 3 (SkyBox Negative-X)
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}


	Status = FALSE;

	texture[4] = ilutGLLoadImage(SkyBox4Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 4 (SkyBox Positive-X)
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	Status = FALSE;

	texture[5] = ilutGLLoadImage(SkyBox5Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 5 (SkyBox Positive-Y)
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}


	Status = FALSE;

	texture[6] = ilutGLLoadImage(SkyBox3Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 6 (SkyBox Negative-Z)
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	Status = FALSE;

	texture[7] = ilutGLLoadImage(SkyBox6Bitmap); // Load Image into OpenGL Texture
	error = ilGetError();
	if (error == IL_NO_ERROR) {
		Status=TRUE;

		// Setup Texture 7 (SkyBox Positive-Z)
		glBindTexture(GL_TEXTURE_2D, texture[7]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	return Status;
}


void Lighting(GLvoid)
{
	// Flash Light
	if(flight)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	// Ambient Light
	if(alight)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
	// Sky Light
	if(slight) {
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
	}
	else {
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
	}
	// Fixed Lamp Light
	if(llight) 
		glEnable(GL_LIGHT4);
	else 
		glDisable(GL_LIGHT4);
}
void DrawPacMan(GLvoid)
{
	glPushMatrix();
		glScalef(0.5f,0.5f,0.5f);
		glTranslatef(0.0f,0.9f,-10.0f);
		glDisable(GL_LIGHTING);
		glColor3ub(255,255,0);
		gluSphere(quadratic,1.0,100,100);
		glEnable(GL_LIGHTING);
	glPopMatrix();
}
void DrawSkyBox(GLvoid)
{
//**************************************** Begin SkyBox **********************************************
	
	//************ left "wall"
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[6]);
	}
	glBegin(GL_QUADS);
		glColor3ub(150,150,0);
	if(filter == 0)
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(-60.0f,-1.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-60.0f,-1.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-60.0f,10.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(-60.0f,10.0f,-60.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//************ right "wall"
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
	}
	glBegin(GL_QUADS);
		glColor3ub(250,150,250);
	if(filter == 0)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(60.0f,-1.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(60.0f,-1.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(60.0f,10.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(60.0f,10.0f,60.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//************ back "wall"
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[7]);
	}
	glBegin(GL_QUADS);
		glColor3ub(255,255,255);
	if(filter == 0)
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(60.0f,-1.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-60.0f,-1.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-60.0f,10.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(60.0f,10.0f,60.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//************ top "sky"
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[5]);
	}
	glBegin(GL_QUADS);
		glColor3ub(120,175,255);
	if(filter == 0)
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(-60.0f,10.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(60.0f,10.0f,60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(60.0f,10.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-60.0f,10.0f,-60.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	//************ front "wall"
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[4]);
	}
	glBegin(GL_QUADS);
		glColor3ub(140,19,50);
	if(filter == 0)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-60.0f,-1.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(60.0f,-1.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(60.0f,10.0f,-60.0f);
	if(filter == 0)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-60.0f,10.0f,-60.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

//**************************************** End SkyBox **********************************************
}
void MoonAndSun(GLvoid)
{
//******************************** Begin Moon and Sun **********************************************

	//************ Create Moon
	glDisable(GL_LIGHTING); // Moon drawn unaffected by Lighting

	glPushMatrix();
		GLfloat tempMoonPos = 360.0f - moonpos;
		// Set Moon Location
		glRotatef(moonpos,1.0f,0.0f,0.0f);
		glTranslatef(0.0f,0.0f,-30.0f);
		if(moonpos >= 359.0f)
		moonpos = 0.0f;
		// Move Moon
		FILETIME temp = theTime;
		GetSystemTimeAsFileTime(&theTime);
		if((theTime.dwLowDateTime) > (temp.dwLowDateTime)) {
			moonpos = moonpos + 0.25f;
		}
		// Draw Moon
		glBegin(GL_POLYGON);
		glColor3ub(200,225,250);
		for(int j = 0; j < 100; j++) {
			float angle = 2.0f * PI * j / 100.0f;
			glVertex3f(0.35f*cos(angle), 0.35f*sin(angle), 0.0f);
		}
		glEnd();
	
		glEnable(GL_LIGHTING);	// Re-enable Lighting
	
		// Cut light when moon out of scene
		if(moonpos > 185.0f)
			glDisable(GL_LIGHT2);
		GLfloat m_diffuse[] = { 0.2f, 0.3f, 0.5f, 0.7f };
		GLfloat m_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat m_light_direction[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_DIFFUSE, m_diffuse);
		/* Not Implemented, future development, emulated light fading
		if(moonpos > 165.0f) {
			glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, 10.0f);			// Constant Light Attenuation
			glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
			glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
		}
		*/
		glLightfv(GL_LIGHT2, GL_POSITION, m_light_position);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, m_light_direction);
	glPopMatrix();
		
	//************ Create Sun
	glDisable(GL_LIGHTING); // Sun drawn unaffected by lighting
	
	glPushMatrix();
		// Set Sun Location
		glRotatef(moonpos,1.0f,0.0f,0.0f);
		glTranslatef(0.0f,0.0f,30.0f);
	
		// Draw Sun
		glBegin(GL_POLYGON);
		glColor3ub(225,250,235);
		// Draw circle with 500 vertices
		for(int j = 0; j < 500; j++)	{
			float angle = 2.0f * PI * j / 500.0f;
			glVertex3f(0.9f*cos(angle), 0.9f*sin(angle), 0.0f);
		}
		glEnd();
	
		glEnable(GL_LIGHTING);		// Re-enable Lighting
	
		// Cut light when sun out of scene
		if(moonpos < 175.0f)
			glDisable(GL_LIGHT3);
	
		glClearColor(0.0, 0.0, 0.0, 0.0);									// Clear Color to Black
		// Set Light Components
		GLfloat s_diffuse[] = { 1.0f, 0.8f, 0.2f, 1.0f };
		GLfloat s_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat s_light_direction[] = { 0.0f, 0.0f, -1.0f, 1.0f };
		glLightfv(GL_LIGHT3, GL_DIFFUSE, s_diffuse);						// Diffuse Light Component
		/* Not Implemented, future development, emulated light fading
		if(moonpos > 340.0f) {
			glLightf (GL_LIGHT3, GL_CONSTANT_ATTENUATION, 10.0f);			// Constant Light Attenuation
			glLightf (GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
			glLightf (GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
		}
		*/
		glLightfv(GL_LIGHT3, GL_POSITION, s_light_position);				// Set Light Position
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, s_light_direction);			// Set Light Direction
	glPopMatrix();
//******************************** End Moon and Sun **********************************************
}
void DrawLamp(GLvoid)
{
//**************************************** Begin Fixed Light *****************************************
	glPushMatrix();
		glTranslatef(0.0f, 1.0f, -10.0f);	// Move for creating "Street Lamp" Head
		if(llight)
			glDisable(GL_LIGHTING);
		else { } // Do Nothing
		glColor3ub(255,245,230);			// Off-White Color
		glScalef(0.3f,0.3f,0.3f);			// Scale Down Size
		// Make Sphere where
			// rad = 1.0, 
			// # of z slices and z stacks = 50 each
		gluSphere(quadratic,1.0,50,50);
	/*	Replaced by gluSphere(4)
	// Draw Sphere, equivalent of glutSolidSphere(3)
	double r = 1.0;
	int lats = 10;
	int longs = 10;
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = sPI * (-0.5 + (double) (i - 1) / lats);
		double z0  = sin(lat0);
		double zr0 =  cos(lat0);

		double lat1 = sPI * (-0.5 + (double) i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);
		
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * sPI * (double) (j - 1) / longs;
			double x = cos(lng) / 3;
			double y = sin(lng) / 3;

			glColor3ub(255,255,255);
			glNormal3f(float(x * zr0), float(y * zr0), float(z0) / 3.0f);
			glVertex3f(float(x * zr0), float(y * zr0), float(z0) / 3.0f);
			glNormal3f(float(x * zr1), float(y * zr1), float(z1) / 3.0f);
			glVertex3f(float(x * zr1), float(y * zr1), float(z1) / 3.0f);
		}
		glEnd();
	}*/
		if(llight)
			glEnable(GL_LIGHTING);
		else {}	// Do Nothing
	glPopMatrix();

	// Fixed Light
	glPushMatrix();
		glTranslatef(0.0f,1.0f,-10.0f);									// Move Light to same Position as "Street Lamp"
		glClearColor (0.0, 0.0, 0.0, 0.0);								// Clear Color to Black
		// Set Light Components
		GLfloat f_lightpos[] = { 0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat f_lightdir[] = { 0.0f, -1.0f, 0.0f, 1.0f};
		GLfloat f_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat f_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
		glLightfv(GL_LIGHT4, GL_POSITION, f_lightpos);					// Positioning of Light
		glLightfv(GL_LIGHT4, GL_DIFFUSE, f_diffuse);					// Diffuse Light Component
		glLightfv(GL_LIGHT4, GL_SPECULAR, f_specular);					// Specular Light Component
		// Attuned to allow specular lighting component
		glLightf (GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
		glLightf (GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
		glLightf (GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f_specular);		// Specular Lighting Affects Front and Back-Facing Materials
	glPopMatrix();
	
	// Create Cylinder Lamp Base
	glPushMatrix();
		glTranslatef(0.0f,0.0f,-10.0f);				// Move to same Position as "Street Lamp"
		glRotatef(270.0f,1.0f,0.0f,0.0f);			// Rotate to Vertical Facing
		glScalef(0.3f,0.3f,0.3f);					// Reduce Size
		glColor3ub(25,25,25);						// Dark Grey Coloring
		// Make Cylinder where 
				// b-rad = 0.5, 
				// t-rad = 0.5, 
				// h = 0.3, 
				// # of z slices and z stacks = 32 each
		gluCylinder(quadratic,0.5,0.5,3.0,32,32);
	glPopMatrix();

//**************************************** End Fixed Light *******************************************
}
void DrawPlane(GLvoid)
{
//**************************************** Begin Plane ***********************************************
	
	GLfloat vPlane1x, vPlane2x, vPlane1z, vPlane2z;	// Plane Coordinates
	// Find vertices for a plane that is always existing with a relative position
	/*if(xpos == 0.0f) {
		vPlane1x = -100.0f * 2.0f;
		vPlane2x = 100.0f * 2.0f;
	}
	else {
		vPlane1x = (xpos - 100.0f) * 2.0f;
		vPlane2x = (xpos + 100.0f) * 2.0f;
	}
	if(zpos == 0.0f) {
		vPlane1z = -100.0f * 2.0f;
		vPlane2z = 100.0f * 2.0f;
	}
	else { 
		vPlane1z = (zpos - 100.0f) * 2.0f;
		vPlane2z = (zpos + 100.0f) * 2.0f;
	}*/

	// Static Plane with an 800x800 size
	vPlane1x = -400.0f;
	vPlane2x = 400.0f;
	vPlane1z = -400.0f;
	vPlane2z = 400.0f;

	

	glPushMatrix();

	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	
	// draw plane and buildings using quads
	glBegin(GL_QUADS);
	// plane
	if(filter == 0) {
		glColor3ub(175,175,175);
		glTexCoord2f(50.0f,0.0f);
		glVertex3f(-vPlane1x,0.0f,-vPlane1z);
		glTexCoord2f(50.0f,50.0f);
		glVertex3f(-vPlane1x,0.0f,-vPlane2z);
		glTexCoord2f(0.0f,50.0f);
		glVertex3f(-vPlane2x,0.0f,-vPlane2z);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-vPlane2x,0.0f,-vPlane1z);
	}
	else {
		glColor3ub(15,50,15);
		glVertex3f(vPlane1x,0.0f,vPlane1z);
		glVertex3f(vPlane1x,0.0f,vPlane2z);
		glVertex3f(vPlane2x,0.0f,vPlane2z);
		glVertex3f(vPlane2x,0.0f,vPlane1z);
	}
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
//******************************** End Plane **********************************************
}
void DrawBuildings(GLvoid)
{
//************************************* Begin Buildings *************************************
	

	// building1
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}
	glBegin(GL_QUADS);
		// draw left bottom half of front face
		glColor3ub(119,0,0);
		if(filter == 0)
			glTexCoord2f(10.0f,0.0f);
			glVertex3f(10.0f, 0.0f, -10.0f);
		if(filter == 0)
			glTexCoord2f(7.0f,0.0f);
			glVertex3f(7.0f, 0.0f, -10.0f);
		if(filter == 0)
			glTexCoord2f(7.0f,5.0f);
			glVertex3f(7.0f, 2.5f, -10.0f);
		if(filter == 0)
			glTexCoord2f(10.0f,5.0f);
			glVertex3f(10.0f, 2.5f, -10.0f);
		// draw right bottom half of front face
		// draw top half of front face
		glColor3ub(119,0,0);
	if(filter == 0)
		glTexCoord2f(10.0f,0.0f);
		glVertex3f(10.0f, 2.5f, -10.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,0.0f);
		glVertex3f(7.0f, 2.5f, -10.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,5.0f);
		glVertex3f(7.0f, 5.0f, -10.0f);
	if(filter == 0)
		glTexCoord2f(10.0f,5.0f);
		glVertex3f(10.0f, 5.0f, -10.0f);
		// draw bottom half of left face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(-10.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -10.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,5.0f);
		glVertex3f(7.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-10.0f,5.0f);
		glVertex3f(7.0f, 2.5f, -10.0f);
		// draw top half of left face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(-10.0f,0.0f);
		glVertex3f(7.0f, 2.5f, -10.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,0.0f);
		glVertex3f(7.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,5.0f);
		glVertex3f(7.0f, 5.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-10.0f,5.0f);
		glVertex3f(7.0f, 5.0f, -10.0f);
		// draw bottom half of right face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(-10.0f,0.0f);
		glVertex3f(10.0f, 0.0f, -10.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,0.0f);
		glVertex3f(10.0f, 0.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,5.0f);
		glVertex3f(10.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-10.0f,5.0f);
		glVertex3f(10.0f, 2.5f, -10.0f);
		// draw top half of right face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(-10.0f,0.0f);
		glVertex3f(10.0f, 2.5f, -10.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,0.0f);
		glVertex3f(10.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-12.0f,5.0f);
		glVertex3f(10.0f, 5.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(-10.0f,5.0f);
		glVertex3f(10.0f, 5.0f, -10.0f);
		// draw bottom half of back face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(10.0f,0.0f);
		glVertex3f(10.0f, 0.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,5.0f);
		glVertex3f(7.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(10.0f,5.0f);
		glVertex3f(10.0f, 2.5f, -12.0f);
		// draw top half of back face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(10.0f,0.0f);
		glVertex3f(10.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,0.0f);
		glVertex3f(7.0f, 2.5f, -12.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,5.0f);
		glVertex3f(7.0f, 5.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(10.0f,5.0f);
		glVertex3f(10.0f, 5.0f, -12.0f);
		// draw top face
		glColor3ub(119, 0, 0);
	if(filter == 0)
		glTexCoord2f(10.0f,-12.0f);
		glVertex3f(10.0f, 5.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,-12.0f);
		glVertex3f(7.0f, 5.0f, -12.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,-10.0f);
		glVertex3f(7.0f, 5.0f, -10.0f);
	if(filter == 0)
		glTexCoord2f(10.0f,-10.0f);
		glVertex3f(10.0f, 5.0f, -10.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	
	// building2
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}
	glBegin(GL_QUADS);
		// draw front face
		glColor3ub(255, 0, 0);
	if(filter == 0)
		glTexCoord2f(5.0f,0.0f);
		glVertex3f(5.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,6.0f);
		glVertex3f(7.0f, 6.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(5.0f,6.0f);
		glVertex3f(5.0f, 6.0f, -5.0f);
		// draw left face
		glColor3ub(255,0,0);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-6.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(-6.0f,6.0f);
		glVertex3f(7.0f, 6.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,6.0f);
		glVertex3f(7.0f, 6.0f, -5.0f);
		// draw right face
		glColor3ub(255, 0, 0);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(5.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-6.0f,0.0f);
		glVertex3f(5.0f, 0.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(-6.0f,6.0f);
		glVertex3f(5.0f, 6.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,6.0f);
		glVertex3f(5.0f, 6.0f, -5.0f);
		// draw back face
		glColor3ub(255, 0, 0);
	if(filter == 0)
		glTexCoord2f(5.0f,0.0f);
		glVertex3f(5.0f, 0.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,0.0f);
		glVertex3f(7.0f, 0.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,6.0f);
		glVertex3f(7.0f, 6.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(5.0f,6.0f);
		glVertex3f(5.0f, 6.0f, -6.0f);
		// draw top face
		glColor3ub(255, 0, 0);
	if(filter == 0)
		glTexCoord2f(5.0f,-5.0f);
		glVertex3f(5.0f, 6.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,-5.0f);
		glVertex3f(7.0f, 6.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(7.0f,-6.0f);
		glVertex3f(7.0f, 6.0f, -6.0f);
	if(filter == 0)
		glTexCoord2f(5.0f,-6.0f);
		glVertex3f(5.0f, 6.0f, -6.0f);
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();	
	

	// building3
	glPushMatrix();
	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}
	glBegin(GL_QUADS);
		// draw bottom half of front face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-7.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-5.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-5.0f, 1.5f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-7.0f, 1.5f, -5.0f);
		// draw top half of front face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-7.0f, 1.5f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-5.0f, 1.5f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-5.0f, 3.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-7.0f, 3.0f, -5.0f);
		// draw bottom half of left face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-7.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-7.0f, 0.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-7.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-7.0f, 1.5f, -5.0f);
		// draw top half of left face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-7.0f, 1.5f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-7.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-7.0f, 3.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-7.0f, 3.0f, -5.0f);
		// draw bottom half of right face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-5.0f, 0.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-5.0f, 0.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-5.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-5.0f, 1.5f, -5.0f);
		// draw top half of right face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-5.0f, 1.5f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-5.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-5.0f, 3.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-5.0f, 3.0f, -5.0f);
		// draw bottom half of back face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-5.0f, 0.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-7.0f, 0.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-7.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-5.0f, 1.5f, -7.0f);
		// draw top half of back face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,0.0f);
		glVertex3f(-5.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,0.0f);
		glVertex3f(-7.0f, 1.5f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,3.0f);
		glVertex3f(-7.0f, 3.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,3.0f);
		glVertex3f(-5.0f, 3.0f, -7.0f);
		// draw top face
		glColor3ub(0, 0, 255);
	if(filter == 0)
		glTexCoord2f(-5.0f,-5.0f);
		glVertex3f(-5.0f, 3.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,-5.0f);
		glVertex3f(-7.0f, 3.0f, -5.0f);
	if(filter == 0)
		glTexCoord2f(-7.0f,-7.0f);
		glVertex3f(-7.0f, 3.0f, -7.0f);
	if(filter == 0)
		glTexCoord2f(-5.0f,-7.0f);
		glVertex3f(-5.0f, 3.0f, -7.0f);	
	glEnd();
	if(filter == 0)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
//************************************* End Buildings *************************************
}
void DrawTop(int place)
{
	//MessageBox(NULL,"T","Loading...",MB_OK | MB_ICONINFORMATION);
	glPushMatrix();
	if(place < 10) {
		glTranslatef(-float(place)*5.0f,0.0f,-30.0f);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
	}
	else if((place < 20) && (place >= 10)) {
		glTranslatef(float(place-10)*5.0f,0.0f,-30.0f);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
	}
	else {
		float t;
		t = float((place/20)%13);
		if (t < 7)
			t *= -3.0f;
		else {
			t = int(t) % 7;
			t *= 3.0f;
		}
		int mx = place%19;
		if(mx < 10)
			mx *= -5;
		else {
			mx = mx % 10;
			mx *= 5;
		}
		glTranslatef(float(mx),0.0f,t);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
	}
		glScalef(5.0f,5.0f,1.0f);

		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,-3.0f);
			glVertex3f(0.0f,-1.0f,-3.0f);
			glVertex3f(0.0f,-1.0f,0.0f);
		glEnd();
	glPopMatrix();
}
void DrawBottom(int place)
{
	//MessageBox(NULL,"B","Loading...",MB_OK | MB_ICONINFORMATION);
	int fpos;
	fpos = int(worldLayout.size()) - 1 - int(place);
	glPushMatrix();
	if(fpos < 10) {
		glTranslatef(-float(fpos)*5.0f,0.0f,30.0f);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
	}
	else if((fpos >= 10) && (fpos < 20)) {
		glTranslatef(float(fpos-11)*5.0f,0.0f,30.0f);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
	}
	else {
		float b = float((place/20)%13);
		if (b < 7)
			b *= -3.0f;
		else {
			b = int(b) % 7;
			b *= 3.0f;
		}
	
		int mx = (place%19) / 13;
		if(mx < 7)
			mx *= -5;
		else {
			mx = mx % 7;
			mx *= 5;
		}
		glTranslatef(float(mx),0.0f,b);
		glRotatef(90,1,0,0);
		glRotatef(90,0,0,1);
	}
	glScalef(5.0f,5.0f,1.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,-3.0f);
			glVertex3f(0.0f,-1.0f,-3.0f);
			glVertex3f(0.0f,-1.0f,0.0f);
		glEnd();
	glPopMatrix();
}
void DrawLeft(int place)
{
	//MessageBox(NULL,"L","Loading...",MB_OK | MB_ICONINFORMATION);
	
}
void DrawRight(int place)
{
	//MessageBox(NULL,"R","Loading...",MB_OK | MB_ICONINFORMATION);
}
void DrawCorner(int place)
{
	//MessageBox(NULL,"C","Loading...",MB_OK | MB_ICONINFORMATION);
}
void DrawDots(int place)
{
	//MessageBox(NULL,"Z","Loading...",MB_OK | MB_ICONINFORMATION);
}
void DrawGZone(int place)
{
	//MessageBox(NULL,"G","Loading...",MB_OK | MB_ICONINFORMATION);
}
void DrawTeleport(int place)
{
	//MessageBox(NULL,"Y","Loading...",MB_OK | MB_ICONINFORMATION);
}
void PlaceStart(int place)
{
	//MessageBox(NULL,"S","Loading...",MB_OK | MB_ICONINFORMATION);
}
void DrawTPWalls(int place)
{
	//MessageBox(NULL,"W","Loading...",MB_OK | MB_ICONINFORMATION);
}
void DrawWorld(GLvoid)
{
	if(levelCom) {
		levelCom = FALSE;
		ifstream world;
		char* level = NULL;
		char worldC = ' ';
		world.open("data\\level1.txt");
		for(int count = 0;count < 294; count++) {
			world.get(worldC);
			string worldN; worldN = worldC;
			if(strcmp(worldN.c_str(),"\n") != 0)
				worldLayout.push_back(worldC);
		}
		world.close();
	}
	for(unsigned int i = 0; i < worldLayout.size(); i++) {
		switch(worldLayout[i]) {
			case 'T':
				DrawTop(i);
				break;
			case 'B':
				DrawBottom(i);
				break;
			case 'L':
				DrawLeft(i);
				break;
			case 'R':
				DrawRight(i);
				break;
			case 'U':
			case 'D':
				DrawCorner(i);
				break;
			case 'Z':
				DrawDots(i);
				break;
			case 'X':
				break;
			case 'G':
				DrawGZone(i);
				break;
			case 'Y':
				DrawTeleport(i);
				break;
			case 'W':
				DrawTPWalls(i);
				break;
			case 'S':
				PlaceStart(i);
				break;
		}
	}
}
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View

	// Enable and Disable Lighting
	Lighting();

	// PacMan
	DrawPacMan();


	// Calculate Translation and Rotations
	GLfloat xtrans = -xpos;							// Movement in the X-direction
	GLfloat ztrans = -zpos;							// Movement in the Z-direction
	GLfloat ytrans = 0;								// No Vertical Movement
	GLfloat sceneroty = 360.0f - yrot;				// Rotation around the Y-axis

	glRotatef(sceneroty,0,1.0f,0);					// Rotation around the Y-axis (looking around)

	// Skybox
	//DrawSkyBox();

	// Draw Moon and Sun
	//MoonAndSun();

	glTranslatef(xtrans, 0.0f, ztrans);		// Move in the X and Z directions the correct respective amounts

	// Draw Fixed Lamp
	//DrawLamp();

	// Draw Plane
	DrawPlane();

	// Draw Buildings
	//DrawBuildings();
	// Draw World
	DrawWorld();

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
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
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
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","JHusein GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
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
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
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

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Jacob Husein CS536 Assignment 2",800,600,16,fullscreen))
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
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
				if (keys['B'] && !bp)
				{
					bp=TRUE;
					if (blend)
					{
						blend = FALSE;
						glDisable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						blend = TRUE;
						glEnable(GL_BLEND);
						glDisable(GL_DEPTH_TEST);
					}
				}
				if (!keys['B'])
				{
					bp=FALSE;
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

				if (keys[VK_PRIOR])
				{
					z-=0.02f;
				}

				if (keys[VK_NEXT])
				{
					z+=0.02f;
				}

				if (keys[VK_UP] || keys['W'])
				{	

					// Sprint, double movement
					if ((keys[VK_SHIFT]) && (sGauge > 0.0f))
					{
						sGauge -= 0.1f;
						xpos -= (float)sin(heading*piover180) * 0.10f;
						zpos -= (float)cos(heading*piover180) * 0.10f;
						if (walkbiasangle >= 359.0f)
						{
							walkbiasangle = 0.0f;
						}
						else
						{
							walkbiasangle+= 20;
						}
						walkbias = (float)sin(walkbiasangle * piover180)/40.0f;
					}
					// Walk, normal movement
					else
					{
						xpos -= (float)sin(heading*piover180) * 0.05f;
						zpos -= (float)cos(heading*piover180) * 0.05f;
						if (walkbiasangle >= 359.0f)
						{
							walkbiasangle = 0.0f;
						}
						else
						{
							walkbiasangle+= 10;
						}
						walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
					}
				}

				if (keys['S'] && !sp)
				{
					// Crouch, half movement
					/*if (keys[VK_CONTROL])
					{
						xpos += (float)sin(heading*piover180) * 0.025f;
						zpos += (float)cos(heading*piover180) * 0.025f;
						// Taken care of by VK_CONTROL condition
						/*if (walkbiasangle <= 1.0f)
						{
							walkbiasangle = 359.0f;
						}
						else
						{
							walkbiasangle-= 5;
						}
						walkbias = (float)sin(walkbiasangle * piover180)/10.0f;*/
					/*}
					else 
					{
						xpos += (float)sin(heading*piover180) * 0.05f;
						zpos += (float)cos(heading*piover180) * 0.05f;
						if (walkbiasangle <= 1.0f)
						{
							walkbiasangle = 359.0f;
						}
						else
						{
							walkbiasangle-= 10;
						}
						walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
					}*/
					sp = TRUE;
					heading -= 180.0f;
					yrot = heading;
				}

				if (!keys['S'])
				{
					sp = FALSE;
				}

				if(keys[VK_DOWN])
				{						
					xpos += (float)sin(heading*piover180) * 0.05f;
					zpos += (float)cos(heading*piover180) * 0.05f;
					if (walkbiasangle <= 1.0f)
					{
						walkbiasangle = 359.0f;
					}
					else
					{
						walkbiasangle-= 10;
					}
					walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
				}


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

				if(keys[VK_RIGHT])
				{
					heading -= 1.0f;
					yrot = heading;
				}

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

				if(keys[VK_LEFT])
				{
					heading += 1.0f;
					yrot = heading;
				}

				if (keys[VK_CONTROL])
				{
					walkbiasangle = 270.0f;
					walkbias = (float)sin(walkbiasangle * piover180) / 5.0f;
					GLfloat ytrans = -walkbias-0.25f;
					glTranslatef(-xpos,ytrans,-zpos);
				}

				if (keys[VK_PRIOR])
				{
					if(lookupdown >= -85.0f)
						lookupdown-= 1.0f;
				}

				if (keys[VK_NEXT])
				{
					if(lookupdown <= 85.0f)
						lookupdown+= 1.0f;
				}

				if (keys['R'] && !rp)
				{
					rp = TRUE;
					// reset all positioning
					yrot = 0.0f;
					xpos = 0.0f;
					zpos = 0.0f;
					heading = 0.0f;
					walkbias = 0.0f;
					walkbiasangle = 0.0f;
					lookupdown = 0.0f;
					z = 0.0f;
					filter = 0;
					if(blend) {
						blend = FALSE;
						glDisable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);
					}
					
					DrawGLScene();
				}

				if (!keys['R'])
				{
					rp = FALSE;
				}

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

				if (keys['3'] && !threep)
				{
					threep = TRUE;
					// set sky light state
					if(slight)
						slight = FALSE;
					else
						slight = TRUE;
				}

				if (!keys['3'])
				{
					threep = FALSE;
				}

				if (keys['4'] && !fourp)
				{
					fourp = TRUE;
					// set fixed light state
					if(llight)
						llight = FALSE;
					else
						llight = TRUE;
				}

				if (!keys['4'])
				{
					fourp = FALSE;
				}

				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("Jacob Husein CS536 Assignment 2",800,600,16,fullscreen))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}
