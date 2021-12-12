/***************************************************
ISGI:: Videojuego de conduccion
Autor: Luis Alberto Alvarez Zavaleta
***************************************************/
#define TITULO "Videojuego de conduccion"
#define _USE_MATH_DEFINES
#include <iostream>
#include <sstream>
#include <gl/freeglut.h>
#include <Utilidades.h>
#include <math.h>
#include <ctime>
#include <cmath>
#include <chrono>


// Coordenadas de la cámara
float pos_cam[3] = { 0, 1, 0 };
float ant_pos = pos_cam[0];
int max_vision = 90;

// Velocidad y dirección del vehiculo
float velocidad = 0;
float angulo = 0;
float giro = 0;
float direccion[3] = { 1, 1, 0 };

// Propiedades del trazado del circuito
float vector_t[2] = { 1,0 };
float vector_n[2] = { 0,1 };
int numero_quads = 100;
int longitud = 1;
float amplitud = 10;
float periodo = 150;
float anchura = 10;
// Farolas
static float distancia_farolas = 20;
static int inicio_farolas[4] = { distancia_farolas, distancia_farolas * 2 , distancia_farolas * 3, distancia_farolas * 4 };
// Captura
float cap_w = 960;
float cap_h = 540;
// Giro dinamico
static GLfloat sueloMdifuso[3] = { 0.8,0.8,0.8 };
static GLfloat sueloMspecular[3] = { 0.3,0.3,0.3 };

void control_luces();


void init()
{
	glClearColor(0, 0, 0, 1);



	control_luces();





	// Caracteristicas del render
	glEnable(GL_DEPTH_TEST);

}

float funcion_trazado(float x, float amplitud, float periodo) {
	return amplitud * sin((x * 2 * (float)M_PI )/ periodo);
}

float derivada_trazado(float x, float amplitud, float periodo) {
	return 2 * (float)M_PI * amplitud /(periodo *  cos(x * 2 * (float)M_PI / periodo) / periodo);
}

void control_luces() {

	float ambiental0[] = { 0.05,0.05,0.05,1.0 };
	float difusa0[] = { 0.05,0.05,0.05,1.0 };
	float especular0[] = { 0.0,0.0,0.0,1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiental0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular0);
	float posicion0[4] = { 0, 10, 0, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicion0);

	glEnable(GL_LIGHT0);

	float ambiental1[] = { 1,1,1 };
	float difusa1[] = { 1,1,1 };
	float especular1[] = { 0.3,0.3,0.3 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambiental1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difusa1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, especular1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20);
	GLfloat posicion1[] = { 0,0.7,0,1 };
	GLfloat direccion1[] = { 0.2,-0.7,-1 };
	glLightfv(GL_LIGHT1, GL_POSITION, posicion1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direccion1);

	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);


}

void gen_circuito() {

	//GLfloat v0[3] = { 0,0,5 }, v1[3] = { 20,0,5 }, v2[3] = { 20,0,-5 }, v3[3] = { 0,0,-5 };
	//glPolygonMode(GL_FRONT, GL_LINE);
	//quad(v0, v1, v2, v3, 10, 5);
	float f_x, aux;

	GLfloat v0[3] = { 0,0,0}, v1[3] = { 0,0,0 }, v2[3] = { 0,0,0 }, v3[3] = { 0,0,0 };
	for (int i = pos_cam[1] + ant_pos - 1; i < pos_cam[0] + longitud*numero_quads; i+= longitud) {
		glPushMatrix();
		v3[0] = i;
		v0[0] = i;
		v3[2] = v2[2];
		v0[2] = v1[2];
		f_x = -funcion_trazado(i, amplitud, periodo);
		vector_t[1] = derivada_trazado(i, amplitud, periodo);
		aux = sqrt(1 + pow(vector_n[1], 2));
		vector_n[0] = -vector_t[1] / aux;
		vector_n[1] = 1 / aux;
		v2[0] =  ((float)i + longitud );
		v2[2] =  f_x + vector_n[1] * anchura/2 ;
		v1[0] = ( (float)i + longitud )  ;
		v1[2] =  f_x - vector_n[1] * anchura/2 ;
		glMaterialfv(GL_BACK, GL_DIFFUSE, sueloMdifuso);
		glMaterialfv(GL_BACK, GL_SPECULAR, sueloMspecular);
		glMaterialf(GL_BACK, GL_SHININESS, 3);
		quad(v0, v1, v2, v3, 30, 30);


		glPopMatrix();
	}

}


void control_camara() {
	gluLookAt(pos_cam[0], pos_cam[1], pos_cam[2],
		static_cast<double>(direccion[0]) + pos_cam[0], 1, static_cast<double>(direccion[2]) + pos_cam[2],
		0, 1, 0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Colocar la camara
	control_camara();

	// Control de avance
	if (pos_cam[0] >= ant_pos + longitud) {
		ant_pos = pos_cam[0];
	}
	glPushMatrix();
	glTranslatef(2, 1, 2);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();
	// Generar circuito
	gen_circuito();

	glutSwapBuffers();
}
void onSpecialKey(int specialKey, int x, int y)
{
	switch (specialKey) {
	case GLUT_KEY_UP:
		velocidad += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		if (velocidad > 0) {
			velocidad -= 0.1f;
		}
		break;
	case GLUT_KEY_RIGHT:
		
		 direccion[2] += rad(0.25);
		
		break;
	case GLUT_KEY_LEFT:
			direccion[2] -= rad(0.25);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void reshape(GLint w, GLint h)
{
	cap_w = w;
	cap_h = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	gluPerspective(45, razon, 1, max_vision);
}

void onTimer(int valor)
{
	// Soluciona error de precisión de variables de coma flotante
	if (velocidad <= 0) {
		velocidad = 0;
	}

	// Actualizar posición camara
	static int antes = 0;
	int ahora, tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes; //Tiempo transcurrido desde antes msg

	pos_cam[0] += static_cast<double>(velocidad) * direccion[0] * (tiempo_transcurrido / 1000.0);
	pos_cam[2] += static_cast<double>(velocidad) * direccion[2] * (tiempo_transcurrido / 1000.0);

	stringstream titulo;
	titulo << "Velocidad: " << velocidad << "m/s";
	glutSetWindowTitle(titulo.str().c_str());

	antes = ahora;

	glutTimerFunc(1000 / 60, onTimer, 60);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);													// Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);				// Alta de buffers a usar
	glutInitWindowSize(cap_w, cap_h);										// Tamanyo inicial de la ventana
	glutCreateWindow(TITULO);													// Creacion de la ventana con su titulo
	cout << "Practica 6 Luis Alberto" << endl;			// Mensaje por consola
	glutDisplayFunc(display);												// Alta de la funcion de atencion a display
	glutReshapeFunc(reshape);												// Alta de la funcion de atencion a reshape
	glutSpecialFunc(onSpecialKey);											// Alta de la funcion de atencion al teclado
	glutTimerFunc(1000 / 60, onTimer, 60);									// Se encola un nuevo timer
	init();																	// Inicializacion propia

	glutMainLoop();															// Puesta en marcha del programa
	return 1;
}