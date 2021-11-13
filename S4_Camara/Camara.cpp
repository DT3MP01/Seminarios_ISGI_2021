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

GLuint estrellaDavid;

//! Inicializaciones
void init()
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// Display List
	estrellaDavid = glGenLists(1);

	glNewList(estrellaDavid, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		double angle = ((i * 2) % 6) * M_PI / 3 + M_PI / 2;
		glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
		glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		double angle = (((i * 2) + 1) % 6) * M_PI / 3 + M_PI / 2;
		glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
		glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
	}
	glEnd();



	glEndList();





}

//! Callback de dibujo
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(/*posicion de la camara */3, 4, 5,
		/*punto de interes*/0, 0, 0,
		/*vertical de la camara*/0, 1, 0);

	for (int i = 0; i < 6; i++) {
		glColor3f(0, 0 + 0.25 * i, 1 - 0.25 * i);
		glPushMatrix();
		glRotatef(30 * i, 0, 1, 0);
		glCallList(estrellaDavid);
		glPopMatrix();
	}
	glColor3f(0, 0, 1);
	glutWireSphere(1, 30, 30);

	glFlush();
}

//! Callback de redimensionamiento
void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	float ar = (float)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float distancia = sqrt(pow(3, 2) + pow(4, 2) + pow(5, 2));
	float fovy = (asin(2.0 / distancia)) * 180 / M_PI;
	gluPerspective(fovy, ar, 1, 100);

}

//! Programa principal
int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);

	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}