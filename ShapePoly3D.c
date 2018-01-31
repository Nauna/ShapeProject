//////////////////////////////////////////////////////////////////////////////////////////
//	Module for 3D vectored polygons using 2D polygons
//	
//
//	Created:	02 Dec 2017
//	Author:		Anna Doulatshahi
//  Based on QUADMESH C class: given in 511CPS
//
//	Issues: -a and b different renders incorrectly
//			-normals
//			-ineffient methor rendering. Uses Immediate mode, can switch to VBO
//			-a,b,r seemingly need to be greater then 1
//			-when malloc is called...currently have no checks for if this worked.
//
//////////////////////////////////////////////////////////////////////////////////////////	
/*
for (i = 0; i < n; i++) {
  printf("%f %f\n",x + r * Math.cos(2 * Math.PI * i / n), y + r * Math.sin(2 * Math.PI * i / n));
}
//(x,y)=center of polygon3d , r = radius
*/
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <string.h>
#include <math.h>
#include <GL/glext.h>
#include "Vector3D.h"
#include "ShapePoly3D.h"
#include <stdlib.h>
#include <malloc.h>

GLfloat pie = 3.14159265;
int radius = 1;

Poly3D New3D_Polygon(float aa, float bb, float radi, GLuint sidesOfPoly, GLuint texture1, GLuint texture2)
{
	radius = radi;
	struct Poly3D pp;        // The new poly3D to be returned
	pp.textureCenter = texture1;
	pp.textureFaces = texture2;
	int numOfP = (sidesOfPoly + 2);
	int numOfV = sidesOfPoly;
	float a = aa, b = bb;
	//Polygon3D
	pp.numOfPolygons = numOfP;
	pp.polygonList = (struct Poly*)malloc(sizeof(Poly)*numOfP);

	//first polygon and last
	pp.polygonList[0].numOfvertices = sidesOfPoly;
	pp.polygonList[0].positionList = (Vector3D*)malloc(sizeof(Vector3D) * sidesOfPoly);
	pp.polygonList[0].normalList = (Vector3D*)malloc(sizeof(Vector3D) * sidesOfPoly);
	pp.polygonList[sidesOfPoly+1].numOfvertices = sidesOfPoly;
	pp.polygonList[sidesOfPoly+1].positionList = (Vector3D*)malloc(sizeof(Vector3D) * sidesOfPoly);
	pp.polygonList[sidesOfPoly + 1].normalList = (Vector3D*)malloc(sizeof(Vector3D) * sidesOfPoly);

	for (int i = 0; i < sidesOfPoly; i++) {
		Set(&pp.polygonList[0].positionList[i], radius*cosf(2 * pie * i / sidesOfPoly) , radius*sinf(2 * pie * i / sidesOfPoly), 0.0f);
	
	}
	
	//middle and last polygons
	int z1, index=0;
	for (int i = 1; i <= sidesOfPoly; i++) {
		pp.polygonList[i].numOfvertices = 4;
		pp.polygonList[i].positionList = (Vector3D*)malloc(sizeof(Vector3D) * 4);
		pp.polygonList[i].normalList = (Vector3D*)malloc(sizeof(Vector3D) * 4);
		//middle value for allocating a and b spacing

		z1 = sidesOfPoly / 2;
		if(i<=z1) {//split to have a and b dimensions
			Set(&pp.polygonList[i].positionList[0], pp.polygonList[0].positionList[i].x, pp.polygonList[0].positionList[i].y, 0.0f);
			Set(&pp.polygonList[i].positionList[1], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, 0.0f);
			Set(&pp.polygonList[i].positionList[2], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, a);
			Set(&pp.polygonList[i].positionList[3], pp.polygonList[0].positionList[i].x, pp.polygonList[0].positionList[i].y, a);


			//for last polygon
			Set(&pp.polygonList[sidesOfPoly+1].positionList[i-1], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, a);
		}
		else {

			if (i == sidesOfPoly) { index = 0; }
			else index = i;

			if (i == (z1 + 1)|| i == sidesOfPoly) {//for the two polygons that will have both a and b
				Set(&pp.polygonList[i].positionList[0], pp.polygonList[0].positionList[index].x, pp.polygonList[0].positionList[index].y, 0.0f);
				Set(&pp.polygonList[i].positionList[1], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, 0.0f);
				Set(&pp.polygonList[i].positionList[2], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, a);
				Set(&pp.polygonList[i].positionList[3], pp.polygonList[0].positionList[index].x, pp.polygonList[0].positionList[index].y, b);
			}
			else {
				Set(&pp.polygonList[i].positionList[0], pp.polygonList[0].positionList[index].x, pp.polygonList[0].positionList[index].y, 0.0f);
				Set(&pp.polygonList[i].positionList[1], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, 0.0f);
				Set(&pp.polygonList[i].positionList[2], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, b);
				Set(&pp.polygonList[i].positionList[3], pp.polygonList[0].positionList[index].x, pp.polygonList[0].positionList[index].y, b);
			}

			//for last polygon
				Set(&pp.polygonList[sidesOfPoly + 1].positionList[i - 1], pp.polygonList[0].positionList[(i - 1)].x, pp.polygonList[0].positionList[(i - 1)].y, b);
			
		}
	}

	// Set up default material used for the mesh
	pp.mat_ambient[0] = 1.0;
	pp.mat_ambient[1] = 1.0;
	pp.mat_ambient[2] = 1.0;
	pp.mat_ambient[3] = 1.0;
	pp.mat_specular[0] = 1.0;
	pp.mat_specular[1] = 1.0;
	pp.mat_specular[2] = 1.0;
	pp.mat_specular[3] = 1.0;
	pp.mat_diffuse[0] = 1.0;
	pp.mat_diffuse[1] = 1.0;
	pp.mat_diffuse[2] = 1.0;
	pp.mat_diffuse[3] = 1.0;
	pp.mat_shininess[0] = 1.0;

	int test1 = 0;
	/*for (int k = 0; k < numOfP; k++) {
		test1 = pp.polygonList[k].numOfvertices;
		for (int i = 0; i < test1; i++) {
			printf("\npolygon%d: vertex%d\m",k,i);
			PrintV(&pp.polygonList[k].positionList[i]);
		}
	}*/
	ComputeNormalsPP(&pp);
	/*
	for (int k = 0; k < numOfP; k++) {
		test1 = pp.polygonList[k].numOfvertices;
		for (int i = 0; i < test1; i++) {
			printf("\npolygon%d: vertex%d\m", k, i);
			PrintV(&pp.polygonList[k].normalList[i]);
		}
	}*/

	return pp;
}

void SetMaterialPP(struct Poly3D* pp, Vector3D ambient, Vector3D diffuse, Vector3D specular, double shininess)
{
	pp->mat_ambient[0] = ambient.x;
	pp->mat_ambient[1] = ambient.y;
	pp->mat_ambient[2] = ambient.z;
	pp->mat_ambient[3] = 1.0;
	pp->mat_specular[0] = specular.x;
	pp->mat_specular[1] = specular.y;
	pp->mat_specular[2] = specular.z;
	pp->mat_specular[3] = 1.0;
	pp->mat_diffuse[0] = diffuse.x;
	pp->mat_diffuse[1] = diffuse.y;
	pp->mat_diffuse[2] = diffuse.z;
	pp->mat_diffuse[3] = 1.0;
	pp->mat_shininess[0] = (float)shininess;
}

// Deallocate dynamic arrays.
void FreeMemoryPP(struct Poly3D* pp)
{
	int countP = pp->numOfPolygons;

	//FOR EACH POLYGON in Polygon3D list
	for (int i = 0; i < countP; i++) {
			if (pp->polygonList[i].positionList != NULL) {
				free(pp->polygonList[i].positionList);
			}
			if (pp->polygonList[i].normalList != NULL) {
				free(pp->polygonList[i].normalList);
			}
			pp->polygonList[i].numOfvertices = 0;
	}
	//FOR OUR POLYGON3D list
	if (pp->polygonList != NULL) {
		free(pp->polygonList);
	}
	pp->numOfPolygons = 0;
}

// Use cross-products to compute the normal vector at each vertex
/*void ComputeNormalsPP(struct Poly3D *pp)
{
	int countP = pp->numOfPolygons;
	int countV = 0;
	int maxCountV = pp->polygonList[0].numOfvertices;
	Vector3D *norm = (Vector3D*)malloc(sizeof(Vector3D) * maxCountV);
	Vector3D *e = (Vector3D*)malloc(sizeof(Vector3D) *maxCountV);
	for (int j = 0; j < countP; j++)
	{
		countV = pp->polygonList[j].numOfvertices;
		
		for ( int i = 0; i < countV; i++ )
		{
			LoadZero(&pp->polygonList[j].normalList[i]) ;
		}
		if ( j == 0 || j == (countP-1) ) {
			for( int m = 1; m < countV; m++ ){
				CrossProduct(&pp->polygonList[j].positionList[m], &pp->polygonList[j].positionList[m-1], &norm[j]);
			}
			CrossProduct( &pp->polygonList[j].positionList[0], &pp->polygonList[j].positionList[(countV - 1)], &norm[(countV - 1)] );
		}
		for (int h = 1; h < countV; h++) {
			Subtract(&pp->polygonList[j].positionList[h], &pp->polygonList[j].positionList[countV-1],&e[h - 1]);
			Normalize(&e[h - 1]);
		}
		Subtract(&pp->polygonList[j].positionList[0], &pp->polygonList[j].positionList[(countV - 1)], &e[countV - 1]);
		Normalize(&e[countV - 1]);

		Vector3D w;    // Working vector;

		for (int g = 1; g <= countV; g++) {
			Negate(&e[countV - g],&w);
			CrossProduct(&e[g - 1], &w, &norm[g - 1]);
			Normalize(&norm[g - 1]);
			Add(&pp->polygonList[j].normalList[g-1], &norm[g - 1], &pp->polygonList[j].normalList[g - 1]);
			Normalize(&pp->polygonList[j].normalList[g - 1]);
		}
	}
	free(norm);
	free(e);
}*/
void ComputeNormalsPP(struct Poly3D *pp)
{
	int countP = pp->numOfPolygons;
	int countV = 0;
	int maxCountV = pp->polygonList[0].numOfvertices;
	Vector3D *norm = (Vector3D*)malloc(sizeof(Vector3D) * maxCountV);
	Vector3D *e = (Vector3D*)malloc(sizeof(Vector3D) *maxCountV);
	
	for (int j = 0; j < countP; j++)
	{
		countV = pp->polygonList[j].numOfvertices;
		
		for (int i = 0; i < countV; i++)
		{
			LoadZero(&pp->polygonList[j].normalList[i]) ;
		}
		for (int h = 1; h < countV; h++) {
			Subtract(&pp->polygonList[j].positionList[h], &pp->polygonList[j].positionList[h-1],&e[h - 1]);
			Normalize(&e[h - 1]);
		}
		Subtract(&pp->polygonList[j].positionList[0], &pp->polygonList[j].positionList[(countV - 1)], &e[countV - 1]);
		Normalize(&e[countV - 1]);

		Vector3D w;    // Working vector;

		for (int g = 1; g <= countV; g++) {
			//Negate(&e[countV - g],&w);
			CrossProduct(&e[g - 1], &w, &norm[g - 1]);
			Normalize(&norm[g - 1]);
			Add(&pp->polygonList[j].normalList[g-1], &norm[g - 1], &pp->polygonList[j].normalList[g - 1]);
			Normalize(&pp->polygonList[j].normalList[g - 1]);
		}
	}
	free(norm);
	free(e);
}
// Draw the mesh by drawing all quads.
void DrawPP(struct Poly3D* pp)
{
	int countP = pp->numOfPolygons;
	int countV = 0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, pp->mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, pp->mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pp->mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, pp->mat_shininess);

	//for polygons that are in the center
	glBindTexture(GL_TEXTURE_2D, pp->textureCenter); 
	for (int i = 1; i < (countP-1); i++) {
		countV = pp->polygonList[i].numOfvertices;
		glBegin(GL_QUADS);
		for (int j = 0; j < countV; j++) {
			glTexCoord2f(radius*cosf(2 * pie * j / countV), radius*sinf(2 * pie * j / countV));
			//glNormal3f(pp->polygonList[i].normalList[j].x, pp->polygonList[i].normalList[j].y, pp->polygonList[i].normalList[j].z);
			glVertex3f(pp->polygonList[i].positionList[j].x, pp->polygonList[i].positionList[j].y, pp->polygonList[i].positionList[j].z);
		}
		glEnd();

	}
	//for face polygons
		//front face
	glBindTexture(GL_TEXTURE_2D, pp->textureFaces);
	countV = pp->polygonList[0].numOfvertices;
	glBegin(GL_POLYGON);
	for (int j = 0; j < countV; j++) {
		glTexCoord2f(radius*cosf(2 * pie * j / countV), radius*sinf(2 * pie * j / countV));
		glNormal3f(pp->polygonList[0].normalList[j].x, pp->polygonList[0].normalList[j].y, pp->polygonList[0].normalList[j].z);
		glVertex3f(pp->polygonList[0].positionList[j].x, pp->polygonList[0].positionList[j].y, pp->polygonList[0].positionList[j].z);
	}
	glEnd();
		//back face
	countV = pp->polygonList[countP-1].numOfvertices;
	glBegin(GL_POLYGON);
	for (int j = 0; j < countV; j++) {
		glTexCoord2f(radius*cosf(2 * pie * j / countV), radius*sinf(2 * pie * j / countV));
		glNormal3f(pp->polygonList[countP - 1].normalList[j].x, pp->polygonList[countP - 1].normalList[j].y, pp->polygonList[countP - 1].normalList[j].z);
		glVertex3f(pp->polygonList[countP - 1].positionList[j].x, pp->polygonList[countP - 1].positionList[j].y, pp->polygonList[countP - 1].positionList[j].z);
	}
	glEnd();
}

