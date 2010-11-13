#include <Drawing\Draw.h>

GLfloat w_vertices[] = {
	// Top and Bottom 0 - 11
	0.0f, 0.0f, 0.0f,
	5.0f, 0.0f, 0.0f,
	5.0f, 3.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	// Left and Right 12 - 23
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -5.0f,
	0.0f, 3.0f, -5.0f,
	0.0f, 3.0f, 0.0f,
	// Corners, Down -> Left 24 - 35
	5.0f, 0.0f, 0.0f,
	5.0f, 0.0f, 5.0f,
	5.0f, 3.0f, 5.0f,
	5.0f, 3.0f, 0.0f,
	// Left 36 - 47
	5.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	5.0f, 3.0f, 0.0f,
	// Down -> Right 48 - 59
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 5.0f,
	0.0f, 3.0f, 5.0f,
	0.0f, 3.0f, 0.0f,
	// Right 60 - 71
	0.0f, 0.0f, 0.0f,
	5.0f, 0.0f, 0.0f,
	5.0f, 3.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	// Up -> Left 72 - 83
	5.0f, 0.0f, 0.0f,
	5.0f, 0.0f, -5.0f,
	5.0f, 3.0f, -5.0f,
	5.0f, 3.0f, 0.0f,
	// Left 84 - 95
	5.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	5.0f, 3.0f, 0.0f,
	// Up -> Right 96 - 107
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -5.0f,
	0.0f, 3.0f, -5.0f,
	0.0f, 3.0f, 0.0f,
	// Right 108 - 119
	0.0f, 0.0f, 0.0f,
	5.0f, 0.0f, 0.0f,
	5.0f, 3.0f, 0.0f,
	0.0f, 3.0f, 0.0f
};
/*
GLuint w_indices[] = {
	// Top and Bottom 0 - 11
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	// Left and Right 12 - 23
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23,
	// Corners, Down -> Left 24 - 35
	24, 25, 26, 27,
	28, 29, 30, 31,
	32, 33, 34, 35,
	// Left 36 - 47
	36, 37, 38, 39,
	40, 41, 42, 43,
	44, 45, 46, 47,
	// Down -> Right 48 - 59
	48, 49, 50, 51,
	52, 53, 54, 55,
	56, 57, 58, 59,
	// Right 60 - 71
	60, 61, 62, 63,
	64, 65, 66, 67,
	68, 69, 70, 71,
	// Up -> Left 72 - 83
	72, 73, 74, 75,
	76, 77, 78, 79,
	80, 81, 82, 83,
	// Left 84 - 95
	84, 85, 86, 87,
	88, 89, 90, 91,
	92, 93, 94, 95,
	// Up -> Right 96 - 107
	96, 97, 98, 99,
	100, 101, 102, 103,
	104, 105, 106, 107,
	// Right 108 - 119
	108, 109, 110, 111,
	112, 113, 114, 115,
	116, 117, 118, 119
};
*/
void Draw::Lighting(GLvoid)
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
	// Ghost Lights
	if(glight) {
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
	}
	else {
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
	}
	// Fixed Pacman Light
	if(plight) 
		glEnable(GL_LIGHT4);
	else 
		glDisable(GL_LIGHT4);
}
void Draw::PacMan(GLvoid)
{
	glPushMatrix();
		//glScalef(0.5f,0.5f,0.5f);
		glTranslatef(0.0f,1.0f,0.0f);
		glDisable(GL_LIGHTING);
		glColor3ub(255,255,0);
		gluSphere(quadratic,1.0,100,100);
		glEnable(GL_LIGHTING);
		// Fixed Light
			glClearColor (0.0, 0.0, 0.0, 0.0);								// Clear Color to Black
			// Set Light Components
			GLfloat f_lcolor[]  = { 1.0f, 1.0f, 0.0f, 7.0f};				// Set Yellow Color
			GLfloat f_lightpos[] = { 0.0f, 0.0f, 0.0f };					// Set Positioning of Light
			glLightfv(GL_LIGHT4, GL_POSITION, f_lightpos);					// Position Light
			glLightfv(GL_LIGHT4, GL_DIFFUSE, f_lcolor);						// Diffuse Light Component
			glLightfv(GL_LIGHT4, GL_SPECULAR, f_lcolor);					// Specular Light Component
			glLightfv(GL_LIGHT4, GL_AMBIENT, f_lcolor);						// Ambient Light Component
			//Attuned to allow specular lighting component
			glLightf (GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
			glLightf (GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
			glLightf (GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f_lcolor);			// Specular Lighting Affects Front and Back-Facing Materials
	glPopMatrix();
}
void Draw::SkyBox(GLvoid)
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
void Draw::Plane(GLvoid)
{
//**************************************** Begin Plane ***********************************************
	
	GLfloat vPlane1x, vPlane2x, vPlane1z, vPlane2z;	// Plane Coordinates

	vPlane1x = -50.0f;
	vPlane2x = 50.0f;
	vPlane1z = -50.0f;
	vPlane2z = 50.0f;

	

	glPushMatrix();

	if(filter == 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	
	// draw plane using quads
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
void Draw::Top(int place)
{
	//MessageBox(NULL,"T","Loading...",MB_OK | MB_ICONINFORMATION);
	glPushMatrix();
/*		int t = place/20;
		t *= 5;
		int mx = place%20;
		mx *= 5;*/
		//glTranslatef(-50.0f,0.0f,-50.0f);
		glTranslatef(float(lctn[place].x),0.0f,float(lctn[place].t));
		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(5.0f,0.0f,0.0f);
			glVertex3f(5.0f,3.0f,0.0f);
			glVertex3f(0.0f,3.0f,0.0f);
		glEnd();
	glPopMatrix();
}
void Draw::Bottom(int place)
{
	glPushMatrix();
/*	int b = place/20;
	b *= 5;
	int mx = place%20;
	mx *= 5;*/
		//glTranslatef(-50.0f,0.0f,-50.0f);
		glTranslatef(float(lctn[place].x),0.0f,float(lctn[place].t));
		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(5.0f,0.0f,0.0f);
			glVertex3f(5.0f,3.0f,0.0f);
			glVertex3f(0.0f,3.0f,0.0f);
		glEnd();
	glPopMatrix();
}
void Draw::Left(int place)
{
/*	int t = place/20;
	if(place > int(worldLayout.size()/2))
		t++;
	t *= 5;
	int mx = place%20;
	mx *= 5;*/
	glPushMatrix();
	int t = lctn[place].t;
	if(place > int(worldLayout.size()/2))
		t += 5;
		//glTranslatef(-50.0f,0.0f,-50.0f);
		glTranslatef(float(lctn[place].x),0.0f,float(t));
		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,-5.0f);
			glVertex3f(0.0f,3.0f,-5.0f);
			glVertex3f(0.0f,3.0f,0.0f);
		glEnd();
	glPopMatrix();
}
void Draw::Right(int place)
{
/*	int t = place/20;
	if(place > int(worldLayout.size()/2))
		t++;
	t *= 5;
	int mx = (place%20) + 1;
	mx *= 5;*/
	glPushMatrix();
	int t = lctn[place].t;
	if(place > int(worldLayout.size()/2))
		t += 5;
		//glTranslatef(-50.0f,0.0f,-50.0f);
		glTranslatef(float(lctn[place].x+5),0.0f,float(t));
		glBegin(GL_QUADS);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,-5.0f);
			glVertex3f(0.0f,3.0f,-5.0f);
			glVertex3f(0.0f,3.0f,0.0f);
		glEnd();
	glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(4, GL_FLOAT, 0, w_vertices);
		//glDrawArrays(GL_QUADS, 12, 1);
		//glDrawElements(GL_QUADS, 12, GL_UNSIGNED_SHORT, (GLushort*)0+12);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

}
void Draw::Corner(int place)
{

/*	int t = place/20;
	t *= 5;
	int mx = place%20;
	mx *= 5;*/
	glPushMatrix();
	//glTranslatef(-50.0f,0.0f,-50.0f);
	glTranslatef(float(lctn[place].x),0.0f,float(lctn[place].t));
	if(worldLayout[place] == 'D') {
		if(worldLayout[place+1] == 'Z' || worldLayout[place+1] == 'L') {
			glBegin(GL_QUADS);
				// Down
				glVertex3f(5.0f,0.0f,0.0f);
				glVertex3f(5.0f,0.0f,5.0f);
				glVertex3f(5.0f,3.0f,5.0f);
				glVertex3f(5.0f,3.0f,0.0f);
				// Left
				glVertex3f(5.0f,0.0f,0.0f);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,3.0f,0.0f);
				glVertex3f(5.0f,3.0f,0.0f);
			glEnd();
		}
		else {
			glBegin(GL_QUADS);
				// Down
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,0.0f,5.0f);
				glVertex3f(0.0f,3.0f,5.0f);
				glVertex3f(0.0f,3.0f,0.0f);
				// Right
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(5.0f,0.0f,0.0f);
				glVertex3f(5.0f,3.0f,0.0f);
				glVertex3f(0.0f,3.0f,0.0f);
			glEnd();
		}
	}
	else {	// 'U'
		if(worldLayout[place+1] == 'Z' || worldLayout[place+1] == 'Y' || worldLayout[place+1] == 'L') {
			glBegin(GL_QUADS);
				// Up
				glVertex3f(5.0f,0.0f,0.0f);
				glVertex3f(5.0f,0.0f,-5.0f);
				glVertex3f(5.0f,3.0f,-5.0f);
				glVertex3f(5.0f,3.0f,0.0f);
				// Left
				glVertex3f(5.0f,0.0f,0.0f);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,3.0f,0.0f);
				glVertex3f(5.0f,3.0f,0.0f);
			glEnd();
		}
		else {
			glBegin(GL_QUADS);
				// Up
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,0.0f,-5.0f);
				glVertex3f(0.0f,3.0f,-5.0f);
				glVertex3f(0.0f,3.0f,0.0f);
				// Right
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(5.0f,0.0f,0.0f);
				glVertex3f(5.0f,3.0f,0.0f);
				glVertex3f(0.0f,3.0f,0.0f);
			glEnd();		
		}
	}

	glPopMatrix();
}
void Draw::Dots(int place)
{
	//MessageBox(NULL,"Z","Loading...",MB_OK | MB_ICONINFORMATION);
/*	int t = place/20;
	if(place > int(worldLayout.size()/2))
		t++;
	t *= 5;
	int mx = place%20;
	mx *= 5;*/
	//if(place > int(worldLayout.size()/2))
		//lctn[place].t += 5;
	glPushMatrix();
		//glTranslatef(-50.0f,0.0f,-50.0f);
		glTranslatef(float(lctn[place].x)+2.5f,0.4f,float(lctn[place].t));
		glDisable(GL_LIGHTING);
		glColor3ub(255,255,255);
		gluSphere(quadratic,0.2,10,10);
		/*glTranslatef(2.0f,0.0f,0.0f);
		gluSphere(quadratic,0.2,10,10);*/
		glEnable(GL_LIGHTING);
	glPopMatrix();
}
void Draw::GZone(int place)
{
	//MessageBox(NULL,"G","Loading...",MB_OK | MB_ICONINFORMATION);
}
void Draw::Teleport(int place)
{
	//MessageBox(NULL,"Y","Loading...",MB_OK | MB_ICONINFORMATION);
		if(zpos >= 0.0f && zpos <= 6.0f) {
			if((xpos <= -40.0f) || (xpos >= 40.0f)) {
				xpos = -xpos;
			}
		}
		else { } // Do Nothing
		
}
void Draw::Start(int place)
{
	//MessageBox(NULL,"S","Loading...",MB_OK | MB_ICONINFORMATION);
	if(!levelStr) {
		levelStr = TRUE;
		xpos = float(lctn[place].x / 20);
		zpos = float((lctn[place].t / 20) + 12);
	}
}
void Draw::TPWalls(int place)
{
	//MessageBox(NULL,"W","Loading...",MB_OK | MB_ICONINFORMATION);
}
void Draw::LoadWorld(GLvoid)
{
	levelCom = FALSE;
	ifstream worldIn;
	char* level = NULL;
	char worldC = ' ';
	//sprintf(level,"data\\level%d.txt",currLevel);
	worldIn.open("data\\level1.txt");
	if(!worldIn.is_open()) {
		MessageBox(NULL,"ERROR: Unable to find world data.","Loading...",MB_OK | MB_ICONERROR);
		exit(1);
	}
	for(int count = 0;!worldIn.eof(); count++) {
		worldIn.get(worldC);
		string worldN; worldN = worldC;
		if(strcmp(worldN.c_str(),"\n") != 0)
			worldLayout.push_back(worldC);
	}
	worldIn.close();
	TLoc location;
	// Calculate and Store all Transformation data.
	for(int calc = 0; calc < int(worldLayout.size()); calc++) {
		location.t = calc/20;
		location.t *= 5;
		location.x = calc%20;
		location.x *= 5;
		lctn.push_back(location);
	}

	// Create Vertex Buffer Objects
	if(buffEx)
		glDeleteBuffers(int(worldLayout.size()),&verBO);
	else
		buffEx = TRUE;
	glGenBuffersARB(1, &verBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, verBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(w_vertices), 0, GL_STREAM_DRAW_ARB);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(w_vertices), w_vertices);
//	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(w_vertices), sizeof(teapotNormals), teapotNormals);
}
void Draw::World(GLvoid)
{
	if(levelCom)
		LoadWorld();
	glPushMatrix();
	glTranslatef(-50.0f,0.0f,-50.0f);
	for(unsigned int i = 0; i < worldLayout.size(); i++) {
		switch(worldLayout[i]) {
			case 'T':
				Top(i);
				break;
			case 'B':
				Bottom(i);
				break;
			case 'L':
				Left(i);
				break;
			case 'R':
				Right(i);
				break;
			case 'U':
			case 'D':
				Corner(i);
				break;
			case 'Z':
				Dots(i);
				break;
			case 'X':
				break;
			case 'G':
				GZone(i);
				break;
			case 'Y':
				Teleport(i);
				break;
			case 'W':
				TPWalls(i);
				break;
			case 'S':
				Start(i);
				break;
		}
	}
	glPopMatrix();
}
void Draw::RevOrder(int &tar, int first, int last)
{
	bool done = false;
	for(int i = first;( (i != last) && (!done) ); i++,last--) {
		if(tar == i) {
			tar = last;
			done = true;
		}
	}
}
void Draw::FPS(double FrPerSec)
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	// Set Location
/*	glLoadIdentity();
	setOrthographicProjection();
	char* fps_p = NULL;
	int sprintf_s(	fps_p,
					size_t(5), "%4.2f",FrPerSec);*/
	// Print FPS
		// Print "FPS: "
	// Set Color
	glColor3f(1.0f,1.0f,1.0f);
	//glTranslatef(0.0f,0.0f,-1.0f);
	glRasterPos2f(0.0f,0.0f);
 	glPrint("FPS: %4.2f", FrPerSec);	// Print GL Text To The Screen
	glPopMatrix();
	// Reset Location
	//resetPerspectiveProjection();
}
