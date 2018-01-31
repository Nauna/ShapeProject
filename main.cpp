/*****************************************

/*****************************************
ASSIGNMENT 3: CPS 511
BATTLE BOT SIMULATION
DUE DEC 3, 2017
BY: ANNAHITA DOULATSHAHI
STUDENT # 500608597
******************************************/
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <GL/glext.h>
#include "Vector3D.h"
#include "CubeMesh.h"
#include "QuadMesh.h"
#include "Matrix3D.h"
#include "ShapePoly3D.h"
#include "RGBpixmap.h"


//**************************************VARIABLE******************************************
const int vWidth   = 900;     // Viewport width in pixels
const int vHeight  = 700;    // Viewport height in pixels

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[]   = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[]  = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[]   = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat submarine_mat_ambient[]   = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat submarine_mat_specular[]  = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat submarine_mat_diffuse[]   = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat submarine_mat_shininess[] = { 0.0F };

//quad mesh GROUND
static QuadMesh groundMesh;
double meshLength = 16.0, meshWidth = 16.0;
const int meshSize = 60;    // Default Mesh Size

//TEXTURE
#define numOfImgs 7
RGBpixmap pix[numOfImgs]; // make empty pixmaps
GLuint textureNames[numOfImgs]; //make names for each pixmap
//shapes = poly3D
#define numOfShapes 14
struct Poly3D shape[numOfShapes];

//Anna code variables
//camera vairables
static GLdouble xView = 0.1;
static GLdouble yView = 8.0;
static GLdouble zView = 20.0;
static GLdouble alpha  = 13.6;
static GLdouble beta   = 1;
static GLdouble radius = 20;
static GLdouble increment        = 0.1;
static GLdouble radius_increment = 1;
static GLdouble prevY = 0;
static GLdouble prevX = 0;
//FPV
static GLdouble x_look=0, y_look=0, z_look=0, p_cam_x = 0, p_cam_y = 0, p_cam_z = 0;
static boolean fpv = false;
double xX = 0;
double zZ = 0;
boolean backwards = false;

//START POSITION
	//one
#define ONE_X -3.0
#define ONE_Y 0.1
#define ONE_Z 1.0
	//two
#define TWO_X 3.0
#define TWO_Y 0.1
#define TWO_Z 1.0	

//BOT MOVEMENT
	//ONE
static GLfloat botOne_x = ONE_X;
static GLfloat botOne_y = ONE_Y;
static GLfloat botOne_z = ONE_Z;
static GLfloat spin     = 0.0;
static GLfloat fwdSpeed = 0.0;
static int countOne        = 0;
static GLdouble botOne_increase = 0.001;
static GLdouble maxSpeed = 0.02;
	//TWO
static GLdouble botTwo_x = TWO_X;
static GLdouble botTwo_y = TWO_Y;
static GLdouble botTwo_z = TWO_Z;
static GLfloat spin2     = 0.0;
static GLdouble fwdSpeed2 = 0.0;
static int count2        = 0;

	//movements
static GLdouble botTwo_increase = 0.01;
#define BOARDER 7
static GLdouble leftBoundary = -BOARDER;
static GLdouble rightBoundary = BOARDER;
static GLdouble topBoundary = BOARDER;
static GLdouble bottomBoundary = -BOARDER;
	//y calculator
static GLdouble heightTolerance = 0.3;
	//monster
static bool colide = false;
static int monsterCount = 0;

//tracking tip
GLdouble tr_x = ONE_X;
GLdouble tr_y = ONE_Y;
GLdouble tr_z = ONE_Z;

GLdouble tr_x2 = TWO_X;
GLdouble tr_y2 = TWO_Y;
GLdouble tr_z2 = TWO_Z;
GLdouble prev_x = 5;
GLdouble prev_z = 5;

//wheel rotation
GLdouble wheel = 0;

//damage bar
GLdouble dam_x1 = 0, dam_y1 = 0, dam_z1 = 0,dam_x2=0, dam_y2=0, dam_z2=0;
GLdouble dmg1 = 0, dmg2 = 0;
int colideCount = 1;
boolean winner = false;

//Bot one weapon: shooter
#define numOfShots 3
GLfloat s_x[numOfShots] = { 0,0,0 }, s_y[numOfShots] = { 0,0,0 }, s_z[numOfShots] = { 0,0,0 };
GLfloat ss[numOfShots] = { 0,0,0 }, x_inc[numOfShots] = { 0,0,0 }, z_inc[numOfShots] = { 0,0,0 };
int shooter_count = numOfShots;

GLfloat pi = 3.14159265;
static Matrix3D m;


//**************************************PROTOTYPES******************************************
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void calculateCameraRotation(char direction[10]);
void botOneMovement(void);
void botTwoMovement(void);
void terrainBuilder(void);
void hole(GLfloat x, GLfloat y);
void heightTracer(GLfloat x, GLfloat z,int botNum);
void collision(void);
void botDetection(int botNum);
void looped_func(void);
GLfloat differenceCalc(char val);
void boundaryBotCheck(void);
void setTexture(RGBpixmap *p, GLuint textureID);
void setUpPictures(void);
void setUpShapes(void);
double absolute(double val);
void damage(int botNum);
void shooter_movement(void);
void shooterStart(void);
void shooterDirection(void);
void fvp_camera(void);
void shooter_boundary(void);
void calculateAngle(void);
void monster_Move(void);



//**************************************MAIN******************************************
int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(150, 10); //defines the bottom left corner of window
	glutCreateWindow("Assignment 3: Bot Wars");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);
	glutIdleFunc(looped_func);

	glutMainLoop();

	return 0;
}


//**********************************************WINDOW_FUNCTIONS**********************************************
void reshape(int w, int h) {
	//called at initialization and whenever user resizes the window.
	// Set up viewport, projection, then change to modelview matrix mode - HERE
	// display function will then set up camera and do modeling transforms. - NEXT
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initOpenGL(int w, int h) {
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);   // This light is currently off

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.3F, 0.3F,0.3F, 0.3F); // Color and depth for glClear BACKGROUND COLOUR
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective
													 
	//texture
		//part of
	glEnable(GL_TEXTURE_2D); 
		//names
	glGenTextures(6, textureNames);
		//Pictures
	setUpPictures();
	
	//MESH
		// Set up ground
	Vector3D origin = NewVector3D(-8.0f, 0.0f, 8.0f);
	Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
	Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
		//lighting ---also used in Shapes
	Vector3D ambient = NewVector3D(1.0f, 1.0f, 1.0f);
	Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
	Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
		//Build Mesh/ground
	groundMesh = NewQuadMesh(meshSize, textureNames[1]);	
	InitMeshQM(&groundMesh, meshSize, origin, meshLength, meshWidth, dir1v, dir2v);
	SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);

	//Build Shapes
	setUpShapes();

	// Set up texture mapping assuming no lighting/shading 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_REPLACE GL_MODULATE

	//makes holes
	terrainBuilder();
}
/*
void onExit() {
	FreeMemoryQM(&groundMesh);
	for (int i = 0; i < numOfShapes; i++) {
		FreeMemoryPP(&shape[i]);
	}
}*/

//***************************************UTILITY_DRAWING_FUNCTIONS**********************************
void setUpPictures(void) {
	pix[0].readBMPFile("red.bmp");
	setTexture(&pix[0], textureNames[0]);
	pix[1].readBMPFile("clover01.bmp"); 
	setTexture(&pix[1], textureNames[1]);
	pix[2].readBMPFile("glass.bmp");
	setTexture(&pix[2], textureNames[2]);
	pix[3].readBMPFile("scale.bmp");
	setTexture(&pix[3], textureNames[3]);
	pix[4].readBMPFile("purpleCall.bmp");
	setTexture(&pix[4], textureNames[4]);
	pix[5].readBMPFile("metal2.bmp"); 
	setTexture(&pix[5], textureNames[5]);
	pix[6].readBMPFile("plank01.bmp");  
	setTexture(&pix[6], textureNames[6]);

}
void setTexture(RGBpixmap *p, GLuint textureID)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // store pixels by byte	
	glBindTexture(GL_TEXTURE_2D, textureID); // select current texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(      // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0,             // resolution level 0
		GL_RGB,        // internal format
		p->nCols,    // image width
		p->nRows,    // image height
		0,             // no border
		GL_RGB,        // my format
		GL_UNSIGNED_BYTE, // my type
		p->pixel);   // the pixels
}
void setUpShapes() {
	int hair = 4;
			//base 0ne
	shape[0] = New3D_Polygon(1.0f, 1.0f, 1.0f, 4, textureNames[5], textureNames[5]);
	//SetMaterialPP(&shape[0], ambient, diffuse, specular, 0.2);
		//base two
	shape[1] = New3D_Polygon(1.0f, 1.0f, 1.0f, 4, textureNames[3], textureNames[5]);
	//SetMaterialPP(&shape[1], ambient, diffuse, specular, 0.2);
			//head one
	shape[2] = New3D_Polygon(1.0f, 1.0f, 1.0f, 6, textureNames[5], textureNames[6]);
	//SetMaterialPP(&shape[2], ambient, diffuse, specular, 0.2);
			//healthbar one
	shape[3] = New3D_Polygon(1.0f, 1.0f, 1.0f, 4, textureNames[5], textureNames[6]);
	//SetMaterialPP(&shape[3], ambient, diffuse, specular, 0.2);
	shape[4] = New3D_Polygon(1.0f, 1.0f, 1.0f, 6, textureNames[2], textureNames[2]);
	//SetMaterialPP(&shape[4], ambient, diffuse, specular, 0.2);
			//mouth one
	shape[5] = New3D_Polygon(1.0f, 1.0f, 1.0f, 4, textureNames[0], textureNames[0]);
	//SetMaterialPP(&shape[5], ambient, diffuse, specular, 0.2);
			//head two
	shape[6] = New3D_Polygon(1.0f, 1.0f, 1.0f, 10, textureNames[5], textureNames[3]);
	//SetMaterialPP(&shape[6], ambient, diffuse, specular, 0.2);
		//wheels
	shape[7] = New3D_Polygon(1.0f, 1.0f, 1.0f, 8, textureNames[6], textureNames[6]);
	//SetMaterialPP(&shape[7], ambient, diffuse, specular, 0.2);
	shape[8] = New3D_Polygon(1.0f, 1.0f, 1.0f, 8, textureNames[3], textureNames[2]);
	//SetMaterialPP(&shape[8], ambient, diffuse, specular, 0.2);
		//mouth two
	shape[9] = New3D_Polygon(1.0f, 1.0f,1.0f, 6, textureNames[0], textureNames[0]);
	//SetMaterialPP(&shape[9], ambient, diffuse, specular, 0.2);
		//hair two
	shape[10] = New3D_Polygon(1.0f, 1.0f,1.0f, 9, textureNames[hair], textureNames[hair]);
	//SetMaterialPP(&shape[10], ambient, diffuse, specular, 0.2);
		//health two
	shape[11] = New3D_Polygon(1.0f, 1.0f, 1.0f, 4, textureNames[5], textureNames[3]);
	//SetMaterialPP(&shape[11], ambient, diffuse, specular, 0.2);
		//dmg bar
	shape[12] = New3D_Polygon(1.0f, 1.0f, 1.0f, 4, textureNames[5], textureNames[0]);
	//SetMaterialPP(&shape[12], ambient, diffuse, specular, 0.2);
	//projectilew
	shape[13] = New3D_Polygon(1.0f, 1.0f, 1.0f, 7, textureNames[5], textureNames[5]);
	//SetMaterialPP(&shape[13], ambient, diffuse, specular, 0.2);

}

//**********************************************DRAWING**********************************************
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(xView, yView, zView, x_look, y_look, z_look, 0.0, 1.0, 0.0);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, submarine_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, submarine_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, submarine_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, submarine_mat_shininess);
	
	float srf = 2, srf2 = 0.6,/*wheels base_face z-position*/w_dis = 2,/*wheel dis x-apart*/y_floor =-1.5;
	float off = 0.7; /*offset of bots to floor*/
	// BOT ONE Begin()
	float fdis = 0.4, epart = 4.0, hie = 1.6, dis = 0.7;

	//BOT ONE shooter
	for (int b = 0; b < (numOfShots - shooter_count); b++) {
		glPushMatrix();
		glTranslatef(s_x[b], s_y[b], s_z[b]);//(0,(hie - dis),fdis)+(botOne_x, (botOne_y + off), botOne_z)
		glScalef((0.3f*ss[b]), (0.1f*ss[b]), (0.1f*ss[b]));
		glRotatef(-45, 0.0, 0.0, 1.0);
		DrawPP(&shape[13]);
		glPopMatrix();
	}
	//HEALTH BARS
		//BOT ONE health
	glPushMatrix();
	glTranslatef(-3.0f, 5.0f, 0.0f);
	glScalef(2, 0.3, 0.2);
	glRotatef(-45, 0.0, 0.0, 1.0);
	DrawPP(&shape[3]);
	glPopMatrix();
	//damageBar
	glPushMatrix();
	glTranslatef(-3.0f, 5.0f, -0.05f);
	glScalef(dam_x1, dam_y1, dam_z1); 
	glRotatef(-45, 0.0, 0.0, 1.0);
	DrawPP(&shape[12]);
	glPopMatrix();
		//BOT TWO health
	glPushMatrix();
	glTranslatef(3.0f, 5.0f, 0.0f);
	glScalef(2, 0.3, 0.2);
	glRotatef(-45, 0.0, 0.0, 1.0);
	DrawPP(&shape[11]);
	glPopMatrix();
	//damageBar
	glPushMatrix();
	glTranslatef(3.0f, 5.0f, -0.05f);
	glScalef(dam_x2, dam_y2, dam_z2);
	glRotatef(-45, 0.0, 0.0, 1.0);
	DrawPP(&shape[12]);
	glPopMatrix();
	
	// BOT ONE Begin()
	//float fdis = 0.4, epart = 4.0, hie = 1.6 , dis =0.7;
	glPushMatrix();
	glTranslatef(botOne_x, (botOne_y + off), botOne_z);
	glRotatef(spin, 0.0, 1.0, 0.0);
	glScalef(0.6f, 0.6f, 0.6f);
	//BASE
	glPushMatrix();
	glScalef(1, 0.5, 0.5);
	glRotatef(-45,0.0,0.0,1.0);
	DrawPP(&shape[0]);
	glPopMatrix();
	//wheels
	//one
	glPushMatrix(); 
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(w_dis, y_floor, srf);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
	//two
	glPushMatrix();	
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(-w_dis, y_floor, srf);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
	//three
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(w_dis, y_floor, -srf2);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
	//four
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(-w_dis, y_floor, -srf2);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
	//Monster
	glPushMatrix();
	glRotatef(-90, 0.0, 1.0, 0.0);
	glTranslatef(0.2f, 0.0f, -0.2f);
	//HEAD
	glPushMatrix();
	glScalef(1, 1, 0.4);
	glTranslatef(0.0f, 1.2f, 0.0f);
	DrawPP(&shape[2]);
	glPopMatrix();
	//EYES
	glPushMatrix();
	glTranslatef(0.0f, hie, fdis);
	glScalef(0.1, 0.1, 0.1);	
		//one
	glPushMatrix();
	glTranslatef(-epart, 0.0f, 0.0f);
	DrawPP(&shape[4]);
	glPopMatrix();
		//two
	glPushMatrix();
	glTranslatef(epart, 0.0f, 0.0f);
	DrawPP(&shape[4]);
	glPopMatrix();
	glPopMatrix();
	//MOUTH
	glPushMatrix();
	glTranslatef(0.0f, (hie-dis), fdis);
	glScalef(0.2f, 0.1f, 0.1f);
	DrawPP(&shape[5]);
	glPopMatrix();
	glPopMatrix();//monster
	glPopMatrix();//bot
	// BOT TWO Begin()
	float fhie =hie-0.3/*y=0 to eyes*/,mdis=dis/*fhie-mdis to mouth*/,hdis=0.65/*fhie+hdis to hair*/,edis2=epart-2/*eye distance*/,fwd=fdis/*fwd axis on face*/, hpart=0.45/*hairpart*/;
	glPushMatrix();
	glTranslatef(botTwo_x, (botTwo_y+off), botTwo_z);
	glRotatef(spin2, 0.0, 1.0, 0.0);
	glScalef(0.6f, 0.6f, 0.6f);
	//BASE
	glPushMatrix();
	glScalef(1, 0.5, 0.5);
	glRotatef(-45, 0.0, 0.0, 1.0);
	DrawPP(&shape[1]);
	glPopMatrix();
	//wheels
		//one
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(w_dis, y_floor, srf);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
		//two
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(-w_dis, y_floor, srf);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
		//three
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(w_dis, y_floor, -srf2);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
		//four
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(-w_dis, y_floor, -srf2);
	glRotatef(wheel, 0.0, 0.0, 1.0);
	DrawPP(&shape[7]);
	glPopMatrix();
	//Monster 
	glPushMatrix();
	glRotatef(-90, 0.0, 1.0, 0.0);
	glTranslatef(0.2f, 0.0f, -0.2f);
	//HEAD
	glPushMatrix();
	glScalef(0.9, 0.9, 0.4);
	glTranslatef(0.0f, 1.2f, 0.0f);
	DrawPP(&shape[6]);
	glPopMatrix();
	//EYES
	glPushMatrix();
	glTranslatef(0.0f, fhie, fwd);
	glScalef(0.2, 0.1, 0.1);
		//one
	glPushMatrix();
	//glRotatef(-30, 1.0, 0.0, 0.0);
	glTranslatef(-edis2, 0.0f, 0.0f);
	DrawPP(&shape[7]);
	glPopMatrix();
		//two
	glPushMatrix();
	//glRotatef(30, 1.0, 0.0, 0.0);
	glTranslatef(edis2, 0.0f, 0.0f);
	DrawPP(&shape[7]);
	glPopMatrix();
	glPopMatrix();//eye
	//MOUTH
	glPushMatrix();
	glTranslatef(0.0f, (fhie-mdis), fwd);
	glScalef(0.1f, 0.1f, 0.1f);
	DrawPP(&shape[9]);
	glPopMatrix();
	//HAIR
	glPushMatrix();
	glTranslatef(0.0f, (fhie+hdis), 0.1);
		//bottom middle
	glPushMatrix();
	glScalef(0.2f, 0.2f, 0.2f);
	DrawPP(&shape[10]);
	glPopMatrix();
		//bottom right
	glPushMatrix();
	glTranslatef(-hpart, 0.0f, 0.0f);
	glScalef(0.25f, 0.25f, 0.25f);
	DrawPP(&shape[10]);
	glPopMatrix();
		//bottom left
	glPushMatrix();
	glTranslatef(hpart, 0.0f, 0.0f);
	glScalef(0.25f, 0.25f, 0.25f);
	DrawPP(&shape[10]);
	glPopMatrix();
		//top
	glPushMatrix();
	glTranslatef(0.0, 0.4f, 0.0f);
	glScalef(0.3f, 0.3f, 0.3f);
	DrawPP(&shape[10]);
	glPopMatrix();

	glPopMatrix();//hair
	glPopMatrix();//monster
	glPopMatrix();//bot 2
	//glPopMatrix();//all-scaled

	// Draw ground
	DrawMeshQM(&groundMesh, meshSize);
	glFlush();
	glutSwapBuffers();  
}


//**************************************SHOOTER*****************************************
void shooterStart() {
	if(shooter_count>0){
		ss[numOfShots - shooter_count] = 1; //scale factor - when 0 object disappears
		//starting position of shooter = mouth of bot one
		s_x[numOfShots - shooter_count] = botOne_x;
		s_y[numOfShots - shooter_count] = 0.9 + 0.6 + botOne_y;//dis_mouth + dis_ground + current_Position
		s_z[numOfShots - shooter_count] = botOne_z;
		//to find direction of shot
		shooterDirection();
	}
	shooter_count--;
}
void shooter_movement() {
	for (int i = 0; i < (numOfShots - shooter_count); i++) {
		s_x[i] += x_inc[i];
		s_z[i] += z_inc[i];
		heightTracer(s_x[i], s_z[i], (10 - i));//10 is an offset convention used to avoid messing with bot_one&two tracers
		shooter_boundary();
	}
}
void shooter_boundary() {
	//boundary check
	int i = 0;
	bool check = true;
	for (i = 0; i < (numOfShots - shooter_count); i++) {
		if (s_x[i] > rightBoundary) { ss[i] = 0; }
		if (s_x[i] < leftBoundary) { ss[i] = 0; }
		if (s_z[i] > topBoundary) { ss[i] = 0; }
		if (s_z[i] < bottomBoundary) { ss[i] = 0; }
	}
	for (i = 0; i < numOfShots; i++) {//checking all shots. If any still active do not reset
		if (ss[i] != 0) { check = false; }
	}
	if (check) { shooter_count = numOfShots; }//no shot active reset
}
void shooterDirection() {
	double forwardSpeed=0.01;
		x_inc[numOfShots - shooter_count] =xX*forwardSpeed + 0.001;
		z_inc[numOfShots - shooter_count] = zZ*forwardSpeed + 0.001;
	
}


//**************************************CAMERA******************************************
void rangeCheckZoom() {
	if (radius < 1) {
		radius = 1;
		printf("Sorry cannot zoom any further");
	}
	if (radius > 30) {
		radius = 30;
		printf("Sorry cannot zoom any further");
	}
}
void calculateCameraRotation(char direction[10]) {
	switch (direction[0]) {
	case 'r': //right
		beta -= increment;
		break;
	case 'l': //left
		beta += increment;
		break;
	case 'u': //up
		alpha -= increment;
		if (alpha < 12.7) { alpha = 12.7; }
		break;
	case 'd': //down
		alpha += increment;
		if (alpha > 14.1) { alpha = 14.1; }
		break;
	default: //update
		break;
	}
	xView = radius*sin(beta)*sin(alpha);
	zView = radius*cos(beta)*sin(alpha);
	yView = radius*cos(alpha);
}

//**********************************************BOT ONE TRACKER**********************************************
void calculate_tracker_ONE(void) {
	Vector3D v = NewVector3D(0.0, 0.0, 0.0);
	m = NewIdentity();
	MatrixRightMultiplyV(&m, NewTranslate(botOne_x, botOne_y, botOne_z));
	MatrixRightMultiplyV(&m, NewRotateY(spin));

	VectorLeftMultiply(&v, &m);

	tr_x = v.x;
	tr_y = v.y;
	tr_z = v.z;
}
void calculate_tracker_TWO(void) {

	Vector3D v = NewVector3D(0.0, 0.0, 0.0);
	m = NewIdentity();
	MatrixRightMultiplyV(&m, NewTranslate(botTwo_x, botTwo_y, botTwo_z));
	MatrixRightMultiplyV(&m, NewRotateY(spin2));

	VectorLeftMultiply(&v, &m);

	tr_x2 = v.x;
	tr_y2 = v.y;
	tr_z2 = v.z;
}


//**********************************************MESH_HOLE**********************************************

void hole(GLfloat x, GLfloat z) {
	int siz = groundMesh.numVertices;
	Vector3D *positio;
	double wholeZ = 5;
	double pos = 0.0;
	double pos2 = 0.0;
	double pos3 = 0.0;
	double difx = 0;
	double difz = 0;
	double temp = 0;
	for (int i = 0; i < siz; i++) {
		positio = &groundMesh.vertices[i].position; 
		pos = positio->x;
		pos2 = positio->y;
		pos3 = positio->z;
		difx = pos - x;
		difz = pos3 - z;
		if (difx > -wholeZ && difx < wholeZ && difz > -wholeZ && difz < wholeZ) {
			temp = absolute(difx) + absolute(difz);
			pos2 = pos2 - (0.2*(3 - temp));
		}
		Set(&groundMesh.vertices[i].position, pos, pos2, pos3);
	}
	ComputeNormalsQM(&groundMesh);
	glutPostRedisplay();
}
void terrainBuilder() {
	hole(-4, 4);
	hole(4, 4);
	hole(4,-4);
}


//**********************************************KEY_FUNCTIONS**********************************************
void functionKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		spin -= 10.0;
		if (spin < (-180.0)) { spin = 180.0; }
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		spin += 10.0;
		if (spin > 180) { spin = -180.0; }
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		fwdSpeed += botOne_increase;
		if (fwdSpeed > maxSpeed) { fwdSpeed = maxSpeed; }
		break;
	case GLUT_KEY_DOWN:
		fwdSpeed -= botOne_increase;
		if (fwdSpeed < -maxSpeed) { fwdSpeed = -maxSpeed; }
		backwards = true;
		break;
	case GLUT_KEY_F1: // Help key
		printf("\n*********WELCOME TO BOT WARS***************\n");
		printf("\n  **************GAME PLAY***************\n");
		printf("\nUser Bot(Plank_Head) weapon = shooter\n");
		printf("\nShooter: 3 shot gun - reloads after 3 shots\n");
		printf("\nMonster Bot(Alien_Head) weapon = poison touch\n");
		printf("\nPoison Touch: Touching Alien_Head causes immediate damage\n");
		printf("\nRules: First to completely damage other bot wins\n\n");
		printf("\n\tDamage displayed in health bars floating above\n\n");
		printf("\n*********CAMERA ROTATION***************\n");
		printf("'i' = zoom in\n");
		printf("'o' = zoom out\n");
		printf("\n{'w','s','a','d'} = {up,down,left,right} = same as dragging mouse = camera rotation\n");
		printf("\n******************FVP*****************\n");
		printf("'q' = switch to FPV\n");
		printf("'e' = switch out of FPV\n");
		printf("\n*********MOVING USER BOT(Plank Head)****************\n");
		printf("Keyboard arrow keys up = FORWARD\n");
		printf("Keyboard arrow keys down = BACKWARD\n");
		printf("Keyboard arrow keys left = TURN LEFT\n");
		printf("Keyboard arrow keys right = TURN RIGHT\n");
		printf("SPACE =  shoot\n");
		
		break;
	}
	glutPostRedisplay(); 
}
void fvp_camera(void) {
	xView = botOne_x; yView = botOne_y+1.5; zView = botOne_z;
}
void mouseMotionHandler(int xMouse, int yMouse)
{
	if ((prevX - xMouse)>0)
		calculateCameraRotation("right");
	if ((prevX - xMouse)<0)
		calculateCameraRotation("left");
	if ((prevY - yMouse)>0)
		calculateCameraRotation("up");
	if ((prevY - yMouse)<0)
		calculateCameraRotation("down");
	prevY = yMouse; prevX = xMouse;
	glutPostRedisplay();   // Trigger a window redisplay
}

void keyboard(unsigned char key, int x, int y)
{
	//printf("key pressed");
	switch (key)
	{
		//CAMERA KEYS
	case 'i':
		radius -= radius_increment;
		alpha += increment;
		rangeCheckZoom();
		calculateCameraRotation("update");
		break;
	case 'o': 
		radius += radius_increment;
		alpha += increment;
		rangeCheckZoom();
		calculateCameraRotation("update");
		break;
		//KEYS
	case 'w':
		if (!fpv)
			calculateCameraRotation("up");
		break;
	case 'a':
		if (!fpv)
			calculateCameraRotation("right");
		break;
	case 's':
		if (!fpv)
			calculateCameraRotation("down");
		break;
	case 'd':
		if (!fpv)
			calculateCameraRotation("left");
		break;
	case 'q':
		if (!fpv) { p_cam_x = xView; p_cam_y = yView; p_cam_z = zView;}
		fpv = true;
		x_look = 5;  y_look = 2.5; z_look = 5;
		fvp_camera();
		break;
	case 'e':
		if(fpv){
			xView = p_cam_x; yView = p_cam_y; zView = p_cam_z;
			x_look = 0; y_look = 0; z_look = 0;
			fpv = false;
		}
		break;
	case ' ':
		if(shooter_count>0){
			shooterStart();
		}
		break;
	}
	glutPostRedisplay();   // Trigger a window redisplay
}
//**********************************************boundary**********************************************
void boundaryMonsterCheck() {
	if (spin2 > 180) { spin2 = -180.0; }
	if (spin2 < (-180.0)) { spin2 = 0; }
}
void boundaryBotCheck() {
	if (spin > 180) { spin = -180.0; }
	if (spin < (-180.0)) { spin = 0; }
	if (fwdSpeed < -maxSpeed) { fwdSpeed = -maxSpeed; }
	if (fwdSpeed > maxSpeed) { fwdSpeed = maxSpeed; }
}
void botOneMove() {
	boundaryBotCheck();
	botDetection(1);
	botOneMovement();
	calculate_tracker_ONE();
	heightTracer(tr_x, tr_z, 1);
}
//**********************************************LOOP-IDLE CALLBACK**********************************************
void looped_func() { //set in idle function ... called idlly often

	//animated wheel
	wheel += 0.1;
	if (wheel > 360) { wheel = 0; }
	//shooter 
	shooter_movement();
	//bot one move
	botOneMove();
	//fpv
	int e = (backwards) ? 1 : -1;
	if (fpv) { 
		fvp_camera(); 
		x_look = (xX * 5 * e) + botOne_x;
		z_look = (zZ * 5 * e) + botOne_z;
		y_look = botOne_y + 1.5;
	}
	//alien movement
	monster_Move();

	glutPostRedisplay();  
}


//**********************************************ADVERSARY**********************************************
void monster_Move(void) {
	//Variables for collide
	char key = 'u';
	GLfloat difx = 0;
	GLfloat difz = 0;

	//scenerio collide
	if (colide) {
		if (countOne < 5) { key = 'h'; countOne++; }
		else { key = 'k'; /*spin2 += 90; */count2++; }
		if (count2 > 5) { countOne = 0; count2 = 0; }
	}
	//scenerio corner, or stuck
	if (monsterCount > 0 && dmg2 < 2) {
		monsterCount--;
		key = 'k';
		spin2 += 90;
	}
	if (botTwo_increase == 0) { key = ' '; }

	//move with choosen move value
	switch (key) {
	case 'u':
		fwdSpeed2 = botTwo_increase;
		botDetection(2);
		botTwoMovement();
		calculate_tracker_TWO();
		heightTracer(tr_x2, tr_z2, 2);
		break;
	case 'h':
		spin2 += 10.0;
		boundaryMonsterCheck();
		botDetection(2);
		botTwoMovement();
		calculate_tracker_TWO();
		heightTracer(tr_x2, tr_z2, 2);
		break;
	case 'j':
		fwdSpeed2 = -botTwo_increase;
		botDetection(2);
		botTwoMovement();
		calculate_tracker_TWO();
		heightTracer(tr_x2, tr_z2, 2);
		break;
	case 'k':
		spin2 -= 10.0;
		boundaryMonsterCheck();
		botDetection(2);
		botTwoMovement();
		calculate_tracker_TWO();
		heightTracer(tr_x2, tr_z2, 2);
		break;
	}
	if (prev_x == botTwo_x && prev_z == botTwo_z) { monsterCount = 1; }
	prev_x = botTwo_x;
	prev_z = botTwo_z;
}

//**********************************************DIFFERENCE CALCULATOR************************************
GLfloat differenceCalc(char val) {
	GLfloat dif = 0;
	switch (val) {
	case 'x':
		dif =absolute(botOne_x - botTwo_x);
		break;
	case 'z':
		dif = absolute(botOne_z - botTwo_z);
		break;
	case 'a':
		dif = absolute(s_x[0] - botTwo_x);
		break;
	case 's':
		dif = absolute(s_z[0] - botTwo_z);
		break;
	case 'd':
		dif = absolute(s_x[1] - botTwo_x);
		break;
	case 'f':
		dif = absolute(s_z[1] - botTwo_z);
		break;
	case 'g':
		dif = absolute(s_x[2] - botTwo_x);
		break;
	case 'h':
		dif = absolute(s_z[2] - botTwo_z);
		break;
	}
	return dif;
}

//**********************************************BOT COLLIDE**********************************************
void botDetection(int botNum) {
	GLfloat difx = differenceCalc('x');
	GLfloat difz = differenceCalc('z');
	int range = 1, range_s=1;//range of collision btw bot one and two, and btw bot two and shooter. respectfully.
	char key = ' ';
	bool cc = true;
	if ((difx > 0 && difx < range) && (difz > 0 && difz < range) ){
		collision();
		damage(2); //if bot
	}
	else {
		for (int i = 0; i < (numOfShots - shooter_count); i++) {//check the range of each shooter to bot two
			
			if (ss[i] != 0) {
				if (i == 0) { key = 'a'; }
				if (i == 1) { key = 'd'; }
				if (i == 2) { key = 'g'; }
				difx = differenceCalc(key);//calc shooter to monster bot two
				if (i == 0) { key = 's'; }
				if (i == 1) { key = 'f'; }
				if (i == 2) { key = 'h'; }
				difz = differenceCalc(key);
				if ((difx > 0 && difx < range_s) && (difz > 0 && difz < range_s)) {
					collision();
					damage(1);//only bot one has shooter
					cc = false;
					ss[i] = 0;
				}
			}
		}
		if(cc){
		colide = false;
		colideCount = 1;
		}
	}
}
void collision() {
	printf("\n	- - - - -	\n");
	printf("	- _ - _ -	\n");
	printf("   -    -    -	\n");
	printf("  -   KABOOM   -	\n");
	printf("   - _  -  _ -	\n");
	printf("	- - - - -	\n");

	colide = true;
}
void damage(int botNum) { //int input is the bot doing damage
	if(colideCount == 1 && !winner){
		if (botNum == 2) {
			maxSpeed -= 0.004;
			dmg1 += 0.4;
			dam_x1 = dmg1;
			dam_y1 = 0.3;
			dam_z1 = 0.3;
			if (dmg1 > 2) { 
				dmg1 = 2; 
				botOne_increase = 0; 
				maxSpeed = 0; 
				printf("\n\n ********WINNER MONSTER MACHINE*******\n\n"); 
				winner = true;
			}
		}
		if (botNum == 1) {
			botTwo_increase -= 0.002;
			dmg2 += 0.4;
			dam_x2 = dmg2;
			dam_y2 = 0.3;
			dam_z2 = 0.3;
			if (dmg2 > 2) {
				dmg2 = 2; 
				botTwo_increase = 0; 
				printf("\n\n ********WINNER USER BOT*******\n\n");
				winner = true;
			}
		}
	}
	colideCount--;
}
//**********************************************BOt Move**********************************************
void botTwoMovement() {
	double angle = 0;
	double xX = 0;
	double zZ = 0;
	double forwardSpeed = (double)fwdSpeed2;
	float val = pi / 180.0;

	//forward direction rotation movement calculations
	if (spin2 > 0 && spin2 <= 90) {
		xX -= cosf(spin2*val)*forwardSpeed;
		zZ += sinf(spin2*val)*forwardSpeed;
	}
	if (spin2 > 90 && spin2 <= 180) {
		angle = 180 - (double)spin2;
		xX += cosf(angle*val)*forwardSpeed;
		zZ += sinf(angle*val)*forwardSpeed;
	}
	if (spin2 <= 0 && spin2 > -90) {
		angle = absolute((double)spin2);
		xX -= cosf(angle*val)*forwardSpeed;
		zZ -= sinf(angle*val)*forwardSpeed;
	}
	if (spin2 <= -90 && spin2 >= -180) {
		angle = 180 + (double)spin2;
		xX += cosf(angle*val)*forwardSpeed;
		zZ -= sinf(angle*val)*forwardSpeed;
	}
	botTwo_x += xX;
	botTwo_z += zZ;

	//boundary check
	if (botTwo_x > rightBoundary) { botTwo_x = rightBoundary; spin2 += 120; }
	if (botTwo_x < leftBoundary) { botTwo_x = leftBoundary; spin2 += 120;}
	if (botTwo_z > topBoundary) { botTwo_z = topBoundary; spin2 += 120;}
	if (botTwo_z < bottomBoundary) { botTwo_z = bottomBoundary; spin2 += 120;}
}
void botOneMovement(void) {
	xX = 0;
	zZ = 0;
	calculateAngle();

	botOne_x += (xX*fwdSpeed);
	botOne_z += (zZ*fwdSpeed);

	//boundary check
	if (botOne_x > rightBoundary) { botOne_x = rightBoundary; }
	if (botOne_x < leftBoundary) { botOne_x = leftBoundary; }
	if (botOne_z > topBoundary) { botOne_z = topBoundary; }
	if (botOne_z < bottomBoundary) { botOne_z = bottomBoundary; }

}


//********************************************CALCULATE *********************************************
void calculateAngle() {
	float val = pi / 180.0;
	double angle = 0;

	if (spin > 0 && spin <= 90) {
		xX -= cosf(spin*val);
		zZ += sinf(spin*val);
	}
	if (spin > 90 && spin <= 180) {
		angle = 180 - (double)spin;
		xX += cosf(angle*val);
		zZ += sinf(angle*val);
	}
	if (spin <= 0 && spin > -90) {
		angle = absolute((double)spin);
		xX -= cosf(angle*val);
		zZ -= sinf(angle*val);
	}
	if (spin <= -90 && spin >= -180) {
		angle = 180 + (double)spin;
		xX += cosf(angle*val);
		zZ -= sinf(angle*val);
	}
}

double absolute(double val) {
	if (val < 0) { val *= (-1); }
	return val;
}
//*********************************************MOVING: CALCULATE Y_**********************************************
void heightTracer(GLfloat x, GLfloat z, int botNum) {
	int siz = groundMesh.numVertices;
	Vector3D *positio;
	double aveY = 0, cy = 0;;
	double pos = 0.0;
	double pos2 = 0.0;
	double pos3 = 0.0;
	double difx = 0;
	double difz = 0;
	double temp = 0;
	for (int i = 0; i < siz; i++) {//LOOP every vertex
		positio = &groundMesh.vertices[i].position;
		pos = positio->x;
		pos2 = positio->y;
		pos3 = positio->z;
		difx = pos - x;
		difz = pos3 - z;
		if (difx > -heightTolerance && difx < heightTolerance && difz > -heightTolerance && difz < heightTolerance) {
			//printf("\nbot is on ground at y = %2f\n", pos2);
			cy++;
			aveY += pos2;
		}
		if (botNum == 1) { botOne_y = aveY / cy; }
		if (botNum == 2) { botTwo_y = aveY / cy; }
		if (botNum > 2) { if (s_y[10 - botNum] < (aveY / cy)) { ss[10 - botNum] = 0; }; }//makes shooter disappear
	}
}

