/*!
	\file		Camara.cpp
	\brief		Primitivas en Opengl

	Dibujar dos objetos vistos desde diferentes puntos de vista y con diferentes cámaras

	\author		Roberto Vivo' <rvivo@upv.es>
	\date		2016-2021
 */


#define PROYECTO "ISGI::S4E01::Cámaras"

#include <iostream>			
#include <freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

GLuint estrellaDavid, esfera;

//! Inicializaciones
void init()
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

//! Callback de dibujo
void display()

{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(/*posicion de la camara */3, 2, 7,
		/*punto de interes*/0, 0, 0,
		/*vertical de la camara*/0, 1, 0);


	glColor3f(1, 1, 1);
	glutSolidTeapot(0.5);
	glColor3f(0, 1, 0);
	glutWireTeapot(0.51);

	glPushMatrix();
	glTranslatef(-1, 0, -1);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidTeapot(0.5);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.51);
	glPopMatrix();

	glFlush();

}

//! Callback de redimensionamiento
void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	float ar = (float) w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Camara ortografica
	if (ar > 1) {
		glOrtho(-2.0 * ar, 2.0 * ar, -2, 2, -20, 20);

	}
	else {
		glOrtho(-2, 2, -2 / ar, 2 / ar, -20, 20);
	}

	//camara perspectiva
	//gluPerspective(25, ar, 1, 100);

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