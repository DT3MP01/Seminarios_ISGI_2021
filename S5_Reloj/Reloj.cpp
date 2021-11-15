/***************************************************
ISGI:: Reloj 3D
Autor: Luis Alberto Alvarez Zavaleta
***************************************************/

constexpr auto TITULO = "Reloj 3D";
#define _USE_MATH_DEFINES
#include <iostream>
#include <gl/freeglut.h>
#include <math.h>
#include <ctime>
#include <cmath>


// Angulos y variables de animación
static float anguloSec;
static float anguloMin;
static float anguloHora;
static float anguloSiempre;
static float rotacion = 0;
static float seno = 0.0f;

// Indices de las listas de dibujo
static GLint estrella;
static GLint triangulo;
static GLint manecilla;
static GLint circulo;
static const int TASAFPS = 60;
// Coordenadas de la cámara
static int posCam[] = { 2, 3, 5 };

void init_estrellaDavid() {
	estrella = glGenLists(1);
	glNewList(estrella, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		float angle = ((i * 2) % 6) * M_PI / 3 + M_PI / 2;
		glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
		glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		float angle = (((i * 2) + 1) % 6) * M_PI / 3 + M_PI / 2;
		glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
		glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
	}
	glEnd();
	glEndList();
}

void init_manecilla() {
	manecilla = glGenLists(1);
	glNewList(manecilla, GL_COMPILE);
	glBegin(GL_LINES);
	glVertex3f(0, 0.7, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glEndList();
}

void init_triangulo() {
	triangulo = glGenLists(1);
	glNewList(triangulo, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		float angle = ((i * 2) % 6) * M_PI / 3 + M_PI / 2;
		glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
		glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
	}
	glEnd();
	glEndList();
}
void init_circulo(float r, int n) {
	circulo = glGenLists(1);
	glNewList(circulo, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		float aux = 2.0f * M_PI * ((float)i) / ((float)n);
		float x = r * cos(aux);
		float y = r * sin(aux);
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
	init_estrellaDavid();


	// Triangulo marcador de horas y minutos
	init_triangulo();
	init_manecilla();

	init_circulo(1,100);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Borra la pantalla
	glMatrixMode(GL_MODELVIEW);								// Selecciona la modelview
	glLoadIdentity();										// Carga la matriz identidad
	float angle;
	// Cámara
	gluLookAt(posCam[0], posCam[1], posCam[2], 0, 0, 0, 0, 1, 0);

	// Estrella de David 
	glPushMatrix();
	float rebote =(abs(sin(seno)) / 10)+ 0.2;
	for (int i = 1; i <= 6; i++) {
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(0, 0 + 0.25 * i, 1 - 0.25 * i);
		glPushMatrix();
		glScalef(rebote, rebote, rebote);
		glRotatef(30.0f * i + rotacion, 1, 1, 1);
		glCallList(circulo);
		glCallList(estrella);
		glPopMatrix();
		glPopAttrib();
	}
	glPopMatrix();

	// Manecilla para los segundos
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	angle = (90.0 + anguloSec) * M_PI / 180;
	glTranslatef(0.4 * cos(angle), 0.4 * sin(angle), 0.0);
	glRotatef(anguloSec, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.02, 0.6, 5, 10);
	glPopAttrib();
	glPopMatrix();

	// Manecilla para los minutos , rota con los segundos
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 1);
	glScalef(1, 1, 1);
	angle = (90.0 + anguloMin) * M_PI / 180;
	glTranslatef(0.3 * cos(angle), 0.3 * sin(angle), 0.0);
	glRotatef(anguloMin, 0, 0, 1);
	glRotatef(2 * anguloSec, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glutWireCone(0.1, 0.6, 5, 5);
	glPopAttrib();
	glPopMatrix();

	// Manecilla que marca las horas
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 1, 0);
	glScalef(1, 1, 1);
	angle = (90.0 + anguloHora) * M_PI / 180;
	glTranslatef(0.4 * cos(angle), 0.4 * sin(angle), 0.0);
	glRotatef(anguloHora, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.4, 25, 25);
	glPopAttrib();
	glPopMatrix();

	// Triangulos y lineas que marcan horas y minutos
	for (int i = 0; i < 60; i++) {
		glPushMatrix();
		float angle = i * 6.0 * M_PI / 180.0;
		glTranslatef(cos(angle), sin(angle), 0.0f);
		glRotatef(90.0f + i * 6, 0.0f, 0.0f, 1.0f);
		glPushAttrib(GL_CURRENT_BIT);
		if (i % 5 == 0) {
			if (i % 15 == 0) {
				glScalef(1.5, 1.2, 1.5);
				glColor3f(1, 0, 0);
			}
			else {
				glColor3f(0, 0, 1);
			}
			glScalef(0.08, 0.1, 0.1);
			glCallList(triangulo);
			glPopMatrix();
			glPopAttrib();
		}
		else {
			glScalef(0.1, 0.1, 0.1);
			glColor3f(0, 0, 0);
			glCallList(manecilla);
			glPopMatrix();
			glPopAttrib();
		}

	}

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(0, 0, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	glCallList(circulo);
	//glColor3f(1, 1, 0);
	//glutWireSphere(1, 20, 20);
	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	float distancia = sqrt(pow(posCam[0], 2) + pow(posCam[1], 2) + pow(posCam[2], 2));
	float fovy = (asin(2.0 / distancia)) * 180 / M_PI;
	gluPerspective(fovy, razon, 1, 10);
}
void onIdle()
{
	// Variables temporales
	static const float vueltasXsegundo = 1/60.0f;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float ttrans = (ahora - antes) / 1000.0f;
	anguloSiempre += vueltasXsegundo * 360 * ttrans;
	antes = ahora;

	// Calculos para las animaciones de rebote
	seno += 0.5f * ttrans;
	rotacion += 1 % 360;
	// Angulos de las manecillas
	struct tm timeinfo;
	time_t actual = time(0);
	localtime_s(&timeinfo, &actual);
	//sacar tiempos
	anguloSec = 360.0f - (timeinfo.tm_sec * 6);
	anguloMin = 360.0f - (timeinfo.tm_min * 6);
	anguloHora = 360.0f - (timeinfo.tm_hour * 30);

	glutPostRedisplay();
}

void onTimer(int tiempo)
{
	glutTimerFunc(tiempo, onTimer, tiempo);
	onIdle();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);													// Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);				// Alta de buffers a usar
	glutInitWindowSize(400, 400);											// Tamanyo inicial de la ventana
	glutCreateWindow(TITULO);												// Creacion de la ventana con su titulo
	init();		

	// Inicializacion propia
	std::cout << TITULO << " por Luis Alberto Alvarez Zavaleta" << std::endl;		// Mensaje por consola
	glutDisplayFunc(display);												// Alta de la funcion de atencion a display
	glutReshapeFunc(reshape);												// Alta de la funcion de atencion a reshape
	glutTimerFunc(1000 / TASAFPS, onTimer, TASAFPS);									// Se encola un nuevo timer
	glutMainLoop();															// Puesta en marcha del programa
}