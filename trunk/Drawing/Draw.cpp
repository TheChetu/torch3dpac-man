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
GLushort w_indices[] = {
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

// Texture Coordinates
	// Top and Bottom
GLfloat tb_texcoords[] = {
	0.0f, 0.0f,
	5.0f, 0.0f,
	5.0f, 3.0f,
	0.0f, 3.0f
};
	// Left and Right
GLfloat lr_texcoords[] = {
	0.0f, 0.0f,
	0.0f, -5.0f,
	3.0f, -5.0f,
	3.0f, 0.0f,
};

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
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTranslatef(0.0f, 1.1f, 0.0f);
		glRotatef(180, 0.0, 1.0, 0.0);

		float timesec = float(elapsed()) / 1000.0f;
		// draw models
		Cloud.DrawModel( bAnimated ? timesec : 0.0f );
		ClWeapon.DrawModel( bAnimated ? timesec : 0.0f );
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, 1.1f, 0.0f );
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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	// draw plane using quads
	glBegin(GL_QUADS);
	// plane
	//glColor3ub(255,255,255); //black
	 glColor3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(50.0f,50.0f);
	glVertex3f(-vPlane1x,0.0f,-vPlane1z);
	glTexCoord2f(50.0f,-50.0f);
	glVertex3f(-vPlane1x,0.0f,-vPlane2z);
	glTexCoord2f(-50.0f,-50.0f);
	glVertex3f(-vPlane2x,0.0f,-vPlane2z);
	glTexCoord2f(-50.0f,50.0f);
	glVertex3f(-vPlane2x,0.0f,-vPlane1z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
//******************************** End Plane **********************************************
}
void Draw::Top(int place)
{
	glPushMatrix();
		glTranslatef(float(lctn[place].x),0.0f,float(lctn[place].t));

	// bind VBOs for vertex array and index array
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBO);         // for vertex coordinates
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, verBI); // for indices

	// do same as vertex array except pointer
	glEnableClientState(GL_VERTEX_ARRAY);			// Enable Vertex Coords Array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	// Enable Texture Coords Array
	
	glVertexPointer(3, GL_FLOAT, 0, 0);				  // last param is offset, not ptr

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBbt);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);		// Set The TexCoord Pointer To The TexCoord Buffer

	//glDrawArrays(GL_QUADS,0,4);
	// draw 1 quads using offset of index array
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLushort*)0+0);
	//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);            // Disable Vertex Coords Array
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// Disable Texture Coords Array

	// bind with 0, so, switch back to normal pointer operation
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
void Draw::Bottom(int place)
{
	glPushMatrix();

	glTranslatef(float(lctn[place].x),0.0f,float(lctn[place].t));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// bind VBOs for vertex array and index array
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBO);         // for vertex coordinates
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, verBI); // for indices

	// do same as vertex array except pointer
	glEnableClientState(GL_VERTEX_ARRAY);             // activate vertex coords array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);				  // last param is offset, not ptr

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBbt);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);		// Set The TexCoord Pointer To The TexCoord Buffer

	// draw 1 quads using offset of index array
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLushort*)0+0);
	//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);            // deactivate vertex array
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// Disable Texture Coords Array

	// bind with 0, so, switch back to normal pointer operation
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
void Draw::Left(int place)
{
	glPushMatrix();
	int t = lctn[place].t;
	if(place > int(worldLayout.size()/2))
		t += 5;
	
	glTranslatef(float(lctn[place].x),0.0f,float(t));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// bind VBOs for vertex array and index array
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBO);         // for vertex coordinates
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, verBI); // for indices

	// do same as vertex array except pointer
	glEnableClientState(GL_VERTEX_ARRAY);				// Activate Vertex Coords Array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Activate TexCoord Array

	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)0+12);	// Size/Number, Type, Distance between Objects, Offset

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBlr);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);				// Set The TexCoord Pointer To The TexCoord Buffer

	// draw 1 quads using offset of index array
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLushort*)0+12);
	
	glDisableClientState(GL_VERTEX_ARRAY);				// Deactivate Vertex Array
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Deactivate TexCoord Array

	// bind with 0, so, switch back to normal pointer operation
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
void Draw::Right(int place)
{
	glPushMatrix();
	int t = lctn[place].t;
	if(place > int(worldLayout.size()/2))
		t += 5;

	glTranslatef(float(lctn[place].x+5),0.0f,float(t));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// bind VBOs for vertex array and index array
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBO);         // for vertex coordinates
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, verBI); // for indices

	// do same as vertex array except pointer
	glEnableClientState(GL_VERTEX_ARRAY);             // activate vertex coords array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)0+12);  // last param is offset, not ptr


	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBlr);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);		// Set The TexCoord Pointer To The TexCoord Buffer

	// draw 1 quads using offset of index array
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLushort*)0+12);
	
	glDisableClientState(GL_VERTEX_ARRAY);            // deactivate vertex array
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// bind with 0, so, switch back to normal pointer operation
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
void Draw::Corner(int place)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTranslatef(float(lctn[place].x),0.0f,float(lctn[place].t));
	if(worldLayout[place] == 'D') {
		if(worldLayout[place+1] == 'Z' || worldLayout[place+1] == 'L') {
			glBegin(GL_QUADS);
				// Down
				glTexCoord2f(0.0,0.0);
				glVertex3f(5.0f,0.0f,0.0f);
				glTexCoord2f(5.0,0.0);
				glVertex3f(5.0f,0.0f,5.0f);
				glTexCoord2f(5.0,3.0);
				glVertex3f(5.0f,3.0f,5.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(5.0f,3.0f,0.0f);
				// Left
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0f,0.0f,0.0f);
				glTexCoord2f(5.0,0.0);
				glVertex3f(5.0f,0.0f,0.0f);
				glTexCoord2f(5.0,3.0);
				glVertex3f(5.0f,3.0f,0.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(0.0f,3.0f,0.0f);
			glEnd();
		}
		else {
			glBegin(GL_QUADS);
				// Down
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0f,0.0f,0.0f);
				glTexCoord2f(5.0,0.0);
				glVertex3f(0.0f,0.0f,5.0f);
				glTexCoord2f(5.0,3.0);
				glVertex3f(0.0f,3.0f,5.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(0.0f,3.0f,0.0f);
				// Right
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0f,0.0f,0.0f);
				glTexCoord2f(5.0,0.0);
				glVertex3f(5.0f,0.0f,0.0f);
				glTexCoord2f(5.0,3.0);
				glVertex3f(5.0f,3.0f,0.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(0.0f,3.0f,0.0f);
			glEnd();
		}
	}
	else {	// 'U'
		if(worldLayout[place+1] == 'Z' || worldLayout[place+1] == 'Y' || worldLayout[place+1] == 'L') {
			glBegin(GL_QUADS);
				// Up
				glTexCoord2f(0.0,0.0);
				glVertex3f(5.0f,0.0f,0.0f);
				glTexCoord2f(-5.0,0.0);
				glVertex3f(5.0f,0.0f,-5.0f);
				glTexCoord2f(-5.0,3.0);
				glVertex3f(5.0f,3.0f,-5.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(5.0f,3.0f,0.0f);
				// Left
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0f,0.0f,0.0f);
				glTexCoord2f(5.0,0.0);
				glVertex3f(5.0f,0.0f,0.0f);
				glTexCoord2f(5.0,3.0);
				glVertex3f(5.0f,3.0f,0.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(0.0f,3.0f,0.0f);
			glEnd();
		}
		else {
			glBegin(GL_QUADS);
				// Up
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0f,0.0f,0.0f);
				glTexCoord2f(-5.0,0.0);
				glVertex3f(0.0f,0.0f,-5.0f);
				glTexCoord2f(-5.0,3.0);
				glVertex3f(0.0f,3.0f,-5.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(0.0f,3.0f,0.0f);
				// Right
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0f,0.0f,0.0f);
				glTexCoord2f(5.0,0.0);
				glVertex3f(5.0f,0.0f,0.0f);
				glTexCoord2f(5.0,3.0);
				glVertex3f(5.0f,3.0f,0.0f);
				glTexCoord2f(0.0,3.0);
				glVertex3f(0.0f,3.0f,0.0f);
			glEnd();		
		}
	}
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void Draw::Dots(int place)
{
	//MessageBox(NULL,"Z","Loading...",MB_OK | MB_ICONINFORMATION);
	dotsRemaining++;
	glPushMatrix();
		glTranslatef(float(lctn[place].x)+2.5f,0.4f,float(lctn[place].t));
		glDisable(GL_LIGHTING);
		glColor3ub(255,255,255);
		gluSphere(quadratic,0.2,10,10);
		glEnable(GL_LIGHTING);
	glPopMatrix();
}
void Draw::GZone(int place)
{
	// For reference only, no walls drawn.
	//MessageBox(NULL,"G","Loading...",MB_OK | MB_ICONINFORMATION);
}
void Draw::Teleport(int place)
{
	// Not Implemented
	//MessageBox(NULL,"Y","Loading...",MB_OK | MB_ICONINFORMATION);
	/*	if(zpos >= 0.0f && zpos <= 6.0f) {
			if((xpos <= -40.0f) || (xpos >= 40.0f)) {
				xpos = -xpos;
			}
		}
		else { } // Do Nothing
*/		
}
void Draw::Start(int place)
{
	//MessageBox(NULL,"S","Loading...",MB_OK | MB_ICONINFORMATION);
	if(!levelStr) {
		levelStr = TRUE;
		strLocx = xpos = float(lctn[place].x / 20);//float(-lctn[place].x);
		strLocz = zpos = float((lctn[place].t / 20) + 12);//float(-lctn[place].t);
	}
}
void Draw::TPWalls(int place)
{
	// Not Implemented
	//MessageBox(NULL,"W","Loading...",MB_OK | MB_ICONINFORMATION);
}
void Draw::LoadWorld(GLvoid)
{
	levelCom = FALSE;
	ifstream worldIn;
	char* level = NULL;
	char worldC = ' ';
	level = (char*)malloc(sizeof("data\\level100.txt")+1);
	sprintf(level,"data\\level%d.txt",currLevel);
	worldIn.open(level);
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
	if(buffEx) {
		glDeleteBuffers(1,&verBO);
		glDeleteBuffers(1,&verBI);
		glDeleteBuffers(1,&verBbt);
		glDeleteBuffers(1,&verBlr);
	}
	else
		buffEx = TRUE;
	glGenBuffersARB(1, &verBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(w_vertices), w_vertices, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &verBI);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, verBI);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(w_indices), w_indices, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &verBbt);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBbt);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(tb_texcoords), tb_texcoords, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &verBlr);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, verBlr);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(lr_texcoords), lr_texcoords, GL_STATIC_DRAW_ARB);
}
void Draw::World(GLvoid)
{
	dotsRemaining = 0;
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
				SpawnLoc.xp = lctn[i].x;
				SpawnLoc.zp = lctn[i].t;
				SpawnLoc.yp = 1.1f;
				break;
			case 'G':
				//GZone(i);
				break;
			case 'Y':
				//Teleport(i);
				break;
			case 'W':
				//TPWalls(i);
				break;
			case 'S':
				Start(i);
				break;
		}
	}
	if(levelStr)
		Ghosts();
	glPopMatrix();
}
void Draw::Ghosts(GLvoid)
{
	while(gLocs.size() < 4) {
		GhP PushIt;
		PushIt.xp = 0.0f;
		PushIt.yp = 0.0f;
		PushIt.zp = 0.0f;
		PushIt.gheading = 0.0f;
		gLocs.push_back(PushIt);
	}

	GLfloat g1_lcolor[]  = { 1.0f, 0.0f, 0.0f, 7.0f};				// Set Red Color
	GLfloat g2_lcolor[]  = { 0.0f, 0.0f, 1.0f, 7.0f};				// Set Green Color
	GLfloat g3_lcolor[]  = { 1.0f, 6.0f, 6.0f, 7.0f};				// Set Pink Color
	GLfloat g4_lcolor[]  = { 0.0f, 1.0f, 0.0f, 7.0f};				// Set Blue Color
	GLfloat g_lightpos[] = { 0.0f, 0.0f, 0.0f };					// Set Positioning of Light
						

	glPushMatrix();
		glRotatef(180, 0.0, 1.0, 0.0);
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		// draw ghosts
		for(int i = 0; i < 4; i++) {
			float timesec = float(elapsed()) / 1000.0f;
			switch(i) {
				case 0:
					if(gDead1) {
						gLocs[i].xp = SpawnLoc.xp;
						gLocs[i].yp = SpawnLoc.yp;
						gLocs[i].zp = SpawnLoc.zp;
						if(!gEdible) {
							g1Wait = FALSE;
							gDead1 = FALSE;
						}
						Bass.SetAnim(DEATH_FALLBACK);
						BaWeapon.SetAnim(DEATH_FALLBACK);
						GAniNum1 = 17;
					}
					// Change Walk Animation After Dead
					if(GAniNum1 == 17) {
						if((elapsed() - gDeadAni1) >= 750) {
							GAniNum1 = 1;
							Bass.SetAnim(GAniNum1);
							BaWeapon.SetAnim(GAniNum1);
							gDeadAni1 = 0;
						}
					}
					if(!g1Wait) {
						glPushMatrix();
							glTranslatef(-gLocs[i].xp,gLocs[i].yp,-gLocs[i].zp);
							glPushMatrix();
								glRotatef((360.0f - gLocs[i].gheading),0,1.0f,0);
							// draw models
							Bass.DrawModel( bAnimated ? timesec : 0.0f );
							BaWeapon.DrawModel( bAnimated ? timesec : 0.0f );
						
							// Set Light Components
							glLightfv(GL_LIGHT2, GL_POSITION, g_lightpos);					// Position Light
							glLightfv(GL_LIGHT2, GL_DIFFUSE, g1_lcolor);					// Diffuse Light Component
							glLightfv(GL_LIGHT2, GL_SPECULAR, g1_lcolor);					// Specular Light Component
							glLightfv(GL_LIGHT2, GL_AMBIENT, g1_lcolor);					// Ambient Light Component
							//Attuned to allow specular lighting component
							glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
							glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
							glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g1_lcolor);		// Specular Lighting Affects Front and Back-Facing Materials

							glPopMatrix();
						glPopMatrix();
					}
					break;
				case 1:
					if(gDead2) {
						gLocs[i].xp = SpawnLoc.xp;
						gLocs[i].yp = SpawnLoc.yp;
						gLocs[i].zp = SpawnLoc.zp;
						if(!gEdible) {
							g2Wait = FALSE;
							gDead2 = FALSE;
						}
						Zero.SetAnim(DEATH_FALLBACK);
						ZeWeapon.SetAnim(DEATH_FALLBACK);
						GAniNum2 = 17;
					}
					// Change Walk Animation After Dead
					if(GAniNum2 == 17) {
						if((elapsed() - gDeadAni2) >= 750) {
							GAniNum2 = 1;
							Zero.SetAnim(GAniNum2);
							ZeWeapon.SetAnim(GAniNum2);
							gDeadAni2 = 0;
						}
					}
					if(!g2Wait) {
						glPushMatrix();
						glTranslatef(-gLocs[i].xp,gLocs[i].yp,-gLocs[i].zp);
							glPushMatrix();
							glRotatef((360.0f - gLocs[i].gheading),0,1.0f,0);
							// draw models
							Zero.DrawModel( bAnimated ? timesec : 0.0f );
							ZeWeapon.DrawModel( bAnimated ? timesec : 0.0f );
						
							// Set Light Components
							glLightfv(GL_LIGHT3, GL_POSITION, g_lightpos);					// Position Light
							glLightfv(GL_LIGHT3, GL_DIFFUSE, g2_lcolor);					// Diffuse Light Component
							glLightfv(GL_LIGHT3, GL_SPECULAR, g2_lcolor);					// Specular Light Component
							glLightfv(GL_LIGHT3, GL_AMBIENT, g2_lcolor);					// Ambient Light Component
							//Attuned to allow specular lighting component
							glLightf (GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
							glLightf (GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
							glLightf (GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g2_lcolor);		// Specular Lighting Affects Front and Back-Facing Materials
	
							glPopMatrix();
						glPopMatrix();
					}
					break;
				case 2:
					if(gDead3) {
						gLocs[i].xp = SpawnLoc.xp;
						gLocs[i].yp = SpawnLoc.yp;
						gLocs[i].zp = SpawnLoc.zp;
						if(!gEdible) {
							g3Wait = FALSE;
							gDead3 = FALSE;
						}
						MegaMan.SetAnim(DEATH_FALLBACK);
						MeWeapon.SetAnim(DEATH_FALLBACK);
						GAniNum3 = 17;
					}
					// Change Walk Animation After Dead
					if(GAniNum3 == 17) {
						if((elapsed() - gDeadAni3) >= 750) {
							GAniNum3 = 1;
							MegaMan.SetAnim(GAniNum3);
							MeWeapon.SetAnim(GAniNum3);
							gDeadAni3 = 0;
						}
					}
					if(!g3Wait) {	
						glPushMatrix();
						glTranslatef(-gLocs[i].xp,gLocs[i].yp,-gLocs[i].zp);
							glPushMatrix();
							glRotatef((360.0f - gLocs[i].gheading),0,1.0f,0);
							// draw models
							MegaMan.DrawModel( bAnimated ? timesec : 0.0f );
							MeWeapon.DrawModel( bAnimated ? timesec : 0.0f );
						
							// Set Light Components
							glLightfv(GL_LIGHT5, GL_POSITION, g_lightpos);					// Position Light
							glLightfv(GL_LIGHT5, GL_DIFFUSE, g3_lcolor);						// Diffuse Light Component
							glLightfv(GL_LIGHT5, GL_SPECULAR, g3_lcolor);					// Specular Light Component
							glLightfv(GL_LIGHT5, GL_AMBIENT, g3_lcolor);						// Ambient Light Component
							//Attuned to allow specular lighting component
							glLightf (GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
							glLightf (GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
							glLightf (GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g3_lcolor);		// Specular Lighting Affects Front and Back-Facing Materials
	
							glPopMatrix();
						glPopMatrix();
					}
					break;
				case 3:
					if(gDead4) {
						gLocs[i].xp = SpawnLoc.xp;
						gLocs[i].yp = SpawnLoc.yp;
						gLocs[i].zp = SpawnLoc.zp;
						if(!gEdible) {
							g4Wait = FALSE;
							gDead4 = FALSE;
						}
						Wily.SetAnim(DEATH_FALLBACK);
						WiWeapon.SetAnim(DEATH_FALLBACK);
						GAniNum4 = 17;
					}
					// Change Walk Animation After Dead
					if(GAniNum4 == 17) {
						if((elapsed() - gDeadAni4) >= 750) {
							GAniNum4 = 1;
							Wily.SetAnim(GAniNum4);
							WiWeapon.SetAnim(GAniNum4);
							gDeadAni4 = 0;
						}
					}
					if(!g4Wait) {	
						glPushMatrix();
						glTranslatef(-gLocs[i].xp,gLocs[i].yp,-gLocs[i].zp);
							glPushMatrix();
							glRotatef((360.0f - gLocs[i].gheading),0,1.0f,0);
							// draw models
							Wily.DrawModel( bAnimated ? timesec : 0.0f );
							WiWeapon.DrawModel( bAnimated ? timesec : 0.0f );
						
							// Set Light Components
							glLightfv(GL_LIGHT6, GL_POSITION, g_lightpos);					// Position Light
							glLightfv(GL_LIGHT6, GL_DIFFUSE, g4_lcolor);						// Diffuse Light Component
							glLightfv(GL_LIGHT6, GL_SPECULAR, g4_lcolor);					// Specular Light Component
							glLightfv(GL_LIGHT6, GL_AMBIENT, g4_lcolor);						// Ambient Light Component
							//Attuned to allow specular lighting component
							glLightf (GL_LIGHT6, GL_CONSTANT_ATTENUATION, 0.1f);			// Constant Light Attenuation
							glLightf (GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.5f);				// Linear Light Attenuation
							glLightf (GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.2f);			// Quadratic Light Attenuation
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g4_lcolor);		// Specular Lighting Affects Front and Back-Facing Materials
	
							glPopMatrix();
						glPopMatrix();
					}
					break;
			}
		}

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	glPopMatrix();

}
