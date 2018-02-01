
# ShapeProject

This project started with a school assignment for graphics.
I wrote ShapePoly3d to play with shape generation.
The .c and .h files here will generate a 3D polygon with vector mapped images onto the surfaces. 
Can use object in main file with a few simple lines conviently lines.
Create object: Poly3D New3D_Polygon(float aa, float bb, float radi, GLuint sidesOfPoly, GLuint texture1, GLuint texture2);
where aa and bb are depth dimensions. 

There are still some issues with ShapePoly:
//	Issues: 
//      -aa and bb different renders incorrectly
//			-normals
//			-ineffient methor rendering. Uses Immediate mode, can switch to VBO
//			-a,b,r seemingly need to be greater then 1
//			-when malloc is called...currently have no checks for if this worked.

Also main.cpp will not run without the other classes that where associated with my school project. 
Since some of the code was the proffessors, I'm leaving this till I can rewrite the files. 
Missing files:
QuadMesh.c //similar idea as in ShapePoly. But instead of 3D polygons with images its a floor by prof
Matrix3D.c //Basic matrix calculations by prof
Vector3D.c //Basic vector calculations by prof
RGBpixmap.cpp //Basic image reader by prof

QUadMesh.h
Matrix3D.h
Vector3D.h
RGBpixmap.h
Vector3D.h
