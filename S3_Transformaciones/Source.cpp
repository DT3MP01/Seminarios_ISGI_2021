/*!
	\file		FormasBasicas.cpp
	\brief		Primitivas en Opengl

	Programa para dibujar primitivas basicas usando *freeglut*,
	OpenGl y listas de dibujo

	\author		Roberto Vivo' <rvivo@upv.es>
	\date		2016-2021
 */


#define PROYECTO "ISGI::S2E01::Formas Basicas"

#include <iostream>			
#include <freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

GLuint petalo, corola, flor;

//! Inicializaciones
void init()
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	// Display List
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(0.15, 0.5, 0.1);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glEndList();

	corola = glGenLists(1);
	glNewList(corola,GL_COMPILE);

	for (auto i = 0; i < 12; i++) {
		glPushMatrix();
		glRotatef(30*i, 0, 0, 1);
		glTranslatef(0, 0.25, 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(petalo);
		glPopMatrix();
	}
	glPushMatrix();
	glColor3f(1, 1, 0);
	glScalef(0.25, 0.25, 0.25);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glEndList();

	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);
	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(0.1, 0.75, 0.1);
	glTranslatef(0,- 0.375, 0);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(7, 0, 0, 1);
	glTranslated(0, 0.25, 0);
	glCallList(corola);
	glPopMatrix();
	

	glScalef(0.5, 0.5, 0.5);
	glEndList();


}

//! Callback de dibujo
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef((rand() % 200 - 100) / 100, (rand() % 200 - 100) / 100, 0);
		glScalef(0.3, 0.3, 0.3);
		glPopMatrix;
		glCallList(flor);
		

	}

	glFlush();
}

//! Callback de redimensionamiento
void reshape(GLint w, GLint h)
{
}

//! Programa principal
int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);

	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}