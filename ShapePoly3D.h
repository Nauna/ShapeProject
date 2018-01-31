//////////////////////////////////////////////////////////////////////////////////////////
//	Module for 3D vectored polygons using 2D polygons
//	
//
//	Created:	02 Dec 2017
//	Author:		Anna Doulatshahi
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SHAPEPOLY3D_H
#define SHAPEPOLY3D_H
#ifdef __cplusplus
extern "C" {
#endif

#include "Vector3D.h"
#include <gl/glut.h>



	typedef struct Poly{
		Vector3D *positionList;
		Vector3D *normalList;
		int numOfvertices;
	} Poly;

	typedef struct Poly3D {
		Poly *polygonList;
		int numOfPolygons;
		GLfloat mat_ambient[4];
		GLfloat mat_specular[4];
		GLfloat mat_diffuse[4];
		GLfloat mat_shininess[1];
		GLuint textureCenter, textureFaces;

	} Poly3D;

	void ComputeNormalsPP(Poly3D *pp);
	Poly3D New3D_Polygon(float a, float b, float r, GLuint c, GLuint textureCenter, GLuint textureFaces);
	void SetMaterialPP(Poly3D* pp, Vector3D ambient, Vector3D diffuse, Vector3D specular, double shininess);
	void FreeMemoryPP(Poly3D* pp);
	void DrawPP(Poly3D* pp);

#ifdef __cplusplus
}
#endif
#endif	//SHAPEPOLY3D_H
