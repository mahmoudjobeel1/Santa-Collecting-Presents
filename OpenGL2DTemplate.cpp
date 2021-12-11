#include<iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <MMSystem.h>
#include <mciapi.h>
#include <stdlib.h>
#include <glut.h>
#include<algorithm>
#include <math.h>
#include <random>
#include <Mmsystem.h>
#include <mciapi.h>
#include <playsoundapi.h>
#include <chrono>
#include <thread>

#pragma comment(lib,"winmm.lib")
#define min(a,b) (((a) > (b)) ? (b) : (a))
#define max(a,b) (((a) > (b)) ? (a) : (b))
float fmodf(float x, float y);


#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

void drawStar();
void setupLights();
void setupCamera();
void drawFence();
void drawGate();
void drawTree();
void drawTrees();
void drawStar();
void drawPresent();
void drawPresents();
void drawIceMan();
void drawIce();
void drawIceFall();
void drawSantaClause();
void drawBase(double thickness);
void Display();
void drawFences();
bool checkIntersect(float santaPostions [], float present []);

float santaPostions[3] = { 0,0,0 };
float fenceColor[3] = { 0.1,0.1,0.1 };
float presentsPositions[3][2] = { {0.1,0.8},{0.5,0.6},{0.8,0.85} };
bool presentCollected[3];
float IcePos[9][3] = {
	{0.1,0.7,0.8},
	{0.2,0.8,0.7},
	{0.3,0.6,0.8},
	{0.4,0.8,0.7},
	{0.5,0.4,0.8},
	{0.6,0.1,0.7},
	{0.7,0.2,0.8},
	{0.8,0.6,0.7},
	{0.9,0.7,0.8}
};

float santaAngleRotation;
float iceManAngleRotation;
int c = 0;
bool collected;
bool timeOver;

float snataScale = 1.0, fenceScale = 1.0, treeScale = 1.0,translateGate=0.0,rotateIce=0.0,presentScale=1.0;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case '5':
		santaAngleRotation = 180;
		santaPostions[2] = max(0.0, santaPostions[2] - 0.01);
		break;
	case '2':
		santaAngleRotation = 0;
		santaPostions[2] = min(1.0, santaPostions[2] + 0.01);
		break;
	case '1':
		santaAngleRotation = -90;
		santaPostions[0] = max(0.0, santaPostions[0] - 0.01);
		break;
	case '3':
		santaAngleRotation = 90;
		santaPostions[0] = min(1.0, santaPostions[0] + 0.01);
		break;
	case '7':
		//topView
		camera.eye = Vector3f(0, 2, 0);
		camera.center = Vector3f(0, 0,0);
		camera.up = Vector3f(0, 0, -1);
		break;
	case '8':
		//frontView
		camera.eye = Vector3f(0.4, 1.5, 2);
		camera.center = Vector3f(0, 0, 0);
		camera.up = Vector3f(0, 1, 0);
		break;
	case '9':
		//sideView
		camera.eye = Vector3f(2, 2, 2);
		camera.center = Vector3f(0, 0, 0);
		camera.up = Vector3f(0, 1, 0);
		break;
	case '[':
		snataScale += 0.01;
		break;
	case ']':
		snataScale -= 0.01;
		break;
	case '\'':
		fenceScale -= 0.01;
		break;
	case ';':
		fenceScale += 0.01;
		break;
	case '.':
		treeScale += 0.01;
		break;
	case '/':
		treeScale -= 0.01;
		break;
	case 'o':
		translateGate -= 0.01;
		break;
	case 'p':
		translateGate += 0.01;
		break;
	case 'k':
		rotateIce -= 1;
		break;
	case 'l':
		rotateIce += 1;
		break;
	case 'm':
		presentScale -= 0.01;
		break;
	case ',':
		presentScale += 0.01;
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}


	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}


void Timer(int value) {
	c++;
	if (c % 3 == 0) {
		int r = RandomFloat(0, 3);
		fenceColor[r] = fmodf((fenceColor[r] + 0.1) , 1.0);
		
	}
	else iceManAngleRotation = -15;


	if (c == 90 && !timeOver && !collected) {
		presentCollected[0] = presentCollected[1] = presentCollected[2] = true;
		timeOver = true;
		sndPlaySound(TEXT("gameOver.wav"), SND_ASYNC);
	}

	bool f = true;
	for (int i = 0;i < 3 && !timeOver;i++) {
		if (presentCollected[i]==false && checkIntersect(santaPostions, presentsPositions[i])) {
			presentCollected[i] = true;
		}
		if (!collected) {
			f = f && presentCollected[i];
		}
	}
	if (!collected && f && !timeOver) {
		collected = true;
		sndPlaySound(TEXT("win.wav"), SND_ASYNC);
	}

	for (int i = 0;i < 9;i++) {
		IcePos[i][1] = IcePos[i][1] - 0.05;
		if (IcePos[i][1] < 0) IcePos[i][1] = 0.7;
	}

	glutTimerFunc(500, Timer, 0);
	glutPostRedisplay();
}


void main(int argc, char** argv) {

	
	glutInit(&argc, argv);
	
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Santa Collecting Presents");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutTimerFunc(0, Timer, 0);
	glShadeModel(GL_SMOOTH);
	sndPlaySound(TEXT("playSound.wav"), SND_ASYNC| SND_LOOP| SND_FILENAME);

	
	glutMainLoop();
}
void Display() {
	setupCamera();
	setupLights();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glPushMatrix();
	glRotated(0, 0, 0, 1.0);
	drawBase(0.02);
	glPopMatrix();

	drawPresents();

	glPushMatrix();
	glTranslated(santaPostions[0], santaPostions[1], santaPostions[2]);
	glRotated(santaAngleRotation, 0, 1, 0);
	drawSantaClause();
	glPopMatrix();

	glPushMatrix();
	drawIceMan();
	glPopMatrix();


	drawFences();

	drawTrees();

	drawGate();

	if(collected)
		drawIceFall();

	glFlush();
}
bool checkIntersect(float santaPostions[], float present[]) {
	float x1 = santaPostions[0]-0.1, x2 = santaPostions[0] + 0.1;
	float z1 = santaPostions[2]-0.1, z2 = santaPostions[2] + 0.1;
	return (present[0] >= x1 && present[0] <= x2 && present[1] >= z1 && present[1] <= z2);
}
void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	camera.look();
}

void drawFence() {
	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.04);
	glScaled(0.01, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.14);
	glScaled(0.01, 0.15, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.24);
	glScaled(0.01, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.34);
	glScaled(0.01, 0.15, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.44);
	glScaled(0.01, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.54);
	glScaled(0.01, 0.15, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.64);
	glScaled(0.01, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.74);
	glScaled(0.01, 0.15, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.84);
	glScaled(0.01, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 0.94);
	glScaled(0.01, 0.15, 0.11);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.5 / 2, 1);
	glScaled(0.01, 0.5, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();

}

void drawFences() {
	
	glPushMatrix();
	glScaled(1.0, fenceScale, 1.0);
	glPushMatrix();
	glColor3f(fenceColor[0], fenceColor[1], fenceColor[2]);
	glPushMatrix();
	drawFence();
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	drawFence();
	glPopMatrix();
	glPushMatrix();
	glTranslated(1.0, 0, 0);
	drawFence();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void drawGate() {


	glPushMatrix();
	glTranslated(translateGate, 0, 0);

	glPushMatrix();
	glScaled(0.8, 0.9, 1);

	glPushMatrix();
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslated(.4, 0.08 / 2, .1);
	glutSolidCube(.08);
	glPopMatrix();



	glPushMatrix();
	glTranslated(.6, 0.08 / 2, .1);
	glutSolidCube(.08);
	glPopMatrix();


	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(.4, 0.23, .1);
	glScaled(0.06, .7 / 2, 0.06);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(.6, 0.23, .1);
	glScaled(0.06, .7 / 2, 0.06);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(.5, 0.43, .1);
	glScaled(0.4, .06, 0.06);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(.4, 0.475, .1);
	glScaled(0.06, 0.03, 0.06);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(.6, 0.475, .1);
	glScaled(0.06, 0.03, 0.06);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(.5, 0.52, .1);
	glScaled(0.6, .06, 0.06);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(.5, 0.58, .1);
	glScaled(0.8, .06, 0.06);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void drawTree() {
	glColor3f(0 , 0.6 , 0.1);
	glPushMatrix();
	glTranslated(0, 0, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.15, .5, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.2, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.12, .5, 50, 50);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, 0.4, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.08, .3, 50, 50);
	glPopMatrix();

	
	glColor3f(0.1 , 0.3 , 1);
	glPushMatrix();
	glTranslated(0.08, 0.4, 0);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

	glColor3f(0.5 , 0.2 , 0);
	glPushMatrix();
	glTranslated(0.06, 0.3, 0.08);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

	glColor3f(0.6 , 0 , 0);
	glPushMatrix();
	glTranslated(-0.03, 0.5, 0.05);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslated(0.1, 0.42, 0);
	glRotated(20, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	glScaled(0.7, 0.7, 0.7);
	drawStar();
	glPopMatrix();
}

void drawTrees() {
	glPushMatrix();
	
	//tree 1
	glPushMatrix();
	glScaled(treeScale, treeScale, treeScale);
	glPushMatrix();
	glTranslated(0.15, 0, 0.15);
	drawTree();
	glPopMatrix();
	glPopMatrix();

	//tree 2
	glPushMatrix();
	
	glTranslated(0.85, 0, 0.15);
	glPushMatrix();
	
	glScaled(treeScale, treeScale, treeScale);
	drawTree();
	glPopMatrix();
	
	glPopMatrix();
	
	glPopMatrix();
}

void drawStar() {
	glPushMatrix();
	glTranslated(0, 0.4, 0);
	glRotated(0, 1, 0, 0);
	glutSolidCone(0.02, .1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.4, 0);
	glRotated(72, 1, 0, 0);
	glutSolidCone(0.02, .1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.4, 0);
	glRotated(144, 1, 0, 0);
	glutSolidCone(0.02, .1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.4, 0);
	glRotated(216, 1, 0, 0);
	glutSolidCone(0.02, .1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.4, 0);
	glRotated(288, 1, 0, 0);
	glutSolidCone(0.02, .1, 50, 50);
	glPopMatrix();
}

void drawPresent() {


	


	glTranslated(0, 0.02, 0);

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.1 / 2, 0);
	glScaled(1, 1, 0.4);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0, 0.1 / 2, 0.03);
	glScaled(1, 1, 0.2);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0, 0.1 / 2, 0.03);
	glRotated(90, 0, 1, 0);
	glScaled(1, 1, 0.2);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.1 / 2, 0.06);
	glScaled(1, 1, 0.4);
	glutSolidCube(0.1);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.4 , 0.5 , 1);
	glTranslated(0, 0.1, 0.03);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

}



void drawPresents() {


	//present[0];
	if (!presentCollected[0]) {
		glPushMatrix();
		glTranslated(presentsPositions[0][0], 0, presentsPositions[0][1]);
		glScaled(presentScale, presentScale, presentScale);
		glPushMatrix();
		drawPresent();
		glPopMatrix();
		glPopMatrix();
	}

	//present[1];
	if (!presentCollected[1]) {
		glPushMatrix();
		glTranslated(presentsPositions[1][0], 0, presentsPositions[1][1]);
		glPushMatrix();
		glScaled(presentScale, presentScale, presentScale);
		drawPresent();
		glPopMatrix();
		glPopMatrix();
	}

	//present[2];
	if (!presentCollected[2]) {
		glPushMatrix();
		glTranslated(presentsPositions[2][0], 0, presentsPositions[2][1]);
		glPushMatrix();
		glScaled(presentScale, presentScale, presentScale);
		drawPresent();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawIceMan() {
	glPushMatrix();
	glTranslated(0.8, 0, 0.4);
	glPushMatrix();
	glRotated(rotateIce, 0, 0, 1);
	glPushMatrix();

	
	glPushMatrix();
	glColor3f(1,1, 1);
	glTranslated(0, 0.1 / 2, 0);
	glutSolidSphere(0.1, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0.15, 0);
	glutSolidSphere(0.08, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0.25, 0);
	glutSolidSphere(0.06, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.3, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.035, 0.1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.6, 0);
	glTranslated(0, 0.4, 0);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.6, 0);
	glTranslated(0, 0.26, 0);
	glutSolidCone(0.03, 0.1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.035, 0.27, 0.04);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(-0.035, 0.27, 0.04);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}





void drawIce() {

	
	glPushMatrix();
	

	glPushMatrix();
	glColor3f(0.4 , 1 , 1);
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glPushMatrix();
	glPushMatrix();
	glTranslated(0, 0.005, 0);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.01f, 0.01f, 0.2f, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.09, 0.15, 0);
	glRotated(30, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.01f, 0.01f, 0.2f, 20, 20);
	glPopMatrix();

	
	glPushMatrix();
	glTranslated(0.09, 0.05, 0);
	glRotated(-30, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.01f, 0.01f, 0.2f, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	
}

void drawIceFall() {
	for (int i = 0;i < 9;i++) {
		glPushMatrix();
		glTranslated(IcePos[i][0], IcePos[i][1], IcePos[i][2]);
		drawIce();
		glPopMatrix();
	}
}
void drawSantaClause() {

	glPushMatrix();

	glScaled(snataScale, snataScale, snataScale);

	glPushMatrix();
	glTranslated(0, 0.15, 0);
	glScaled(0.8, 0.8, 0.8);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.18, .4, 50, 50);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, 0.4 / 2, 0);
	glutSolidSphere(0.1, 15, 15);
	glPopMatrix();

	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslated(0, 0.35, 0);
	glutSolidSphere(0.07, 15, 15);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, .38, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.06, .15, 50, 50);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(0, 0.53, 0);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0.05, 0.25, 0);
	glRotated(-30, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.025f, 0.025f, 0.15f, 20, 20);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(-0.18, 0.18, 0);
	glRotated(30, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.025f, 0.025f, 0.15f, 20, 20);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(0.03, 0.37, 0.06);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(-0.03, 0.37, 0.06);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

	glPopMatrix();


	glPushMatrix();

	glTranslated(0.05, 0, 0);

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, 0.05 / 2, 0);
	glScaled(1.5, 1, 1);
	glutSolidCube(0.05);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.025f, 0.025f, 0.15f, 20, 20);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();

	glTranslated(-0.05, 0, 0);

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, 0.05 / 2, 0);
	glScaled(1.5, 1, 1);
	glutSolidCube(0.05);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.025f, 0.025f, 0.15f, 20, 20);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}
void drawBase(double thickness) {
	glColor3f(0.2, 0.5, 0.6);
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
