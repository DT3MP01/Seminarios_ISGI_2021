/***************************************************
ISGI:: Reloj 3D
Autor: Mario Campos Mocholí
***************************************************/

constexpr auto TITULO = "Reloj 3D";
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <freeglut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <math.h>
#include <ctime>
#include <cmath>
#include <chrono>

// Angulos y variables de animación
static float anguloSec;
static float anguloMin;
static float anguloHora;
static float rotacion = 0;
static float seno = 0.0f;

// Indices de las listas de dibujo
static GLint estrella;
static GLint triangulo;
static GLint manecilla;
static GLint circulo;

// Coordenadas de la cámara
static int posCam[] = { 2, 3, 5 };

void draw_estrella() {
	estrella = glGenLists(1);

	glNewList(estrella, GL_COMPILE);
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

void draw_manecilla() {
	manecilla = glGenLists(1);
	glNewList(manecilla, GL_COMPILE);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glEndList();
}

void draw_triangulo() {
	triangulo = glGenLists(1);
	glNewList(triangulo, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		double angle = ((i * 2) % 6) * M_PI / 3 + M_PI / 2;
		glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
		glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
	}
	glEnd();
	glEndList();
}
void draw_elipse(float r, int n) {
	circulo = glGenLists(1);
	glNewList(circulo, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		float aux = 2.0f * M_PI * ((float)i) / ((float)n);
		float x = r * cosf(aux);
		float y = r * sinf(aux);
		glVertex2f(x, y);
	}
	glEnd();
	glEndList();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2);	// Grosor de las líneas
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Estrellas de David (práctica 4)
	draw_estrella();

	// Varilla de reloj
	draw_manecilla();

	// Triangulo marcador de horas y minutos
	draw_triangulo();

	draw_elipse(1,100);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Borra la pantalla
	glMatrixMode(GL_MODELVIEW);								// Selecciona la modelview
	glLoadIdentity();										// Carga la matriz identidad

	// Colocamos la cámara
	gluLookAt(posCam[0], posCam[1], posCam[2], 0, 0, 0, 0, 1, 0);

	// Dibujamos la estrella de David de la práctica anterior
	glPushMatrix();
	float aux =(abs(sinf(seno)) / 10)+ 0.2;
	//float angle = (90.0f + (float)anguloSec) * M_PI / 180;
	//glTranslatef(cosf(angle), sinf(angle), 0.0);
	for (int i = 1; i <= 6; i++) {
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(0, 0 + 0.25 * i, 1 - 0.25 * i);
		glPushMatrix();
		glScalef(aux, aux, aux);
		glRotatef(30.0f * i + rotacion, 1, 1, 1);
		glCallList(estrella);
		glPopMatrix();
		glPopAttrib();
	}
	glPopMatrix();

	// Dibujamos las manecillas que marcan las horas, minutos y segundos
	// Manecilla que marca los segundos

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glScalef(1, 1, 1);
	float angle = (90.0f + (float)anguloSec) * M_PI / 180;
	glTranslatef(0.4*cosf(angle),0.4 *sinf(angle), 0.0);
	glRotatef(anguloSec, 0, 0, 1);
	glRotatef(2*anguloSec, 0, 1, 0);
	glRotatef(-90, 1,0,0);
	glutWireCone(0.1, 0.4, 5, 5);
	glPopAttrib();
	glPopMatrix();

	// Manecilla que marca los minutos
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glScalef(1, 1, 1);
	angle = (90.0f + (float)anguloMin) * M_PI / 180;
	glTranslatef(0.4 * cosf(angle), 0.4 * sinf(angle), 0.0);
	glRotatef(anguloMin, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glutWireCone(0.1, 0.4, 5, 5);
	glPopAttrib();
	glPopMatrix();

	// Manecilla que marca las horas
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glScalef(1, 1, 1);
	angle = (90.0f + (float)anguloHora) * M_PI / 180;
	glTranslatef(0.4 * cosf(angle), 0.4 * sinf(angle), 0.0);
	glRotatef(anguloHora, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glutWireCone(0.1, 0.4, 5, 5);

	glPopAttrib();
	glPopMatrix();

	// Dibujamos los triangulos que marcan las horas y los minutos
	for (int i = 0; i < 12; i++) {
		glPushMatrix();
		float angle = i * 30 * M_PI / 180.f;
		glTranslatef(cosf(angle), sinf(angle), 0.0f);
		glRotatef(90.0f + i * 30, 0.0f, 0.0f, 1.0f);
		glPushAttrib(GL_CURRENT_BIT);
		if (i % 3 == 0) {
			glScalef(1.5, 1.2, 1.5);
			glColor3f(1, 0, 0);
		}
		else {
			glScalef(0.8, 1, 1);
			glColor3f(0, 1, 0);
		}
		glScalef(0.1, 0.1, 0.1);
		glCallList(triangulo);
		glPopMatrix();
		glPopAttrib();
	}

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	glRotatef(90, 1,0 , 0);
	glRotatef(-20, 0, 1, 0);
	glCallList(circulo);
	glPopAttrib();
	glPopMatrix();



	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	double distancia = sqrt(pow(posCam[0], 2) + pow(posCam[1], 2) + pow(posCam[2], 2));
	double fovy = (asin(2.0 / distancia)) * 180 / M_PI;
	gluPerspective(fovy, razon, 1, 10);
}

void onTimer(int valor)
{
	// Variables temporales
	static int antes = 0;
	int ahora, tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME);		// Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes;	// Tiempo transcurrido desde antes msg
	antes = ahora;

	// Calculos para las animaciones
	seno += 0.5f * tiempo_transcurrido / 1000.0f;
	rotacion += 1 % 360;

	// Angulos de las manecillas
	const std::time_t now = std::time(nullptr);
	const std::tm calendar_time = *std::localtime(std::addressof(now));
	anguloSec = 360.0f - (calendar_time.tm_sec * 6);
	anguloMin = 360.0f - (calendar_time.tm_min * 6);
	anguloHora = 360.0f - (calendar_time.tm_hour * 30);

	glutTimerFunc(1000 / 60, onTimer, 60);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);													// Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);				// Alta de buffers a usar
	glutInitWindowSize(400, 400);											// Tamanyo inicial de la ventana
	glutCreateWindow(TITULO);												// Creacion de la ventana con su titulo
	init();																	// Inicializacion propia
	std::cout << TITULO << " por Mario Campos Mocholi" << std::endl;		// Mensaje por consola
	glutDisplayFunc(display);												// Alta de la funcion de atencion a display
	glutReshapeFunc(reshape);												// Alta de la funcion de atencion a reshape
	glutTimerFunc(1000 / 60, onTimer, 60);									// Se encola un nuevo timer
	glutMainLoop();															// Puesta en marcha del programa
	return 1;
}