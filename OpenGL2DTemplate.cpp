#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

void drawStar();
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

void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
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




void drawTest() {
	
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5, 0.9, 0);

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


void drawGate() {
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslated(.4, 0.08/2, .1);
	glutSolidCube(.08);
	glPopMatrix();

	

	glPushMatrix();
	glTranslated(.6, 0.08 / 2, .1);
	glutSolidCube(.08);
	glPopMatrix();


	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(.4, 0.23, .1);
	glScaled(0.06, .7/2, 0.06);
	glutSolidCube(1);
	glPopMatrix();

	
	glPushMatrix();
	glTranslated(.6, 0.23, .1);
	glScaled(0.06,.7/2,0.06);
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
}

void drawTree() {
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(0, 0, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.15,.5,50,50);
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

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0.13, 0.3, 0);
	glRotated(20, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	drawStar();
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
	
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.1/2, 0);
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
	glColor3f(1, 0, 0);
	glTranslated(0, 0.1 / 2, 0.06);
	glScaled(1, 1, 0.4);
	glutSolidCube(0.1);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, 0.1, 0.03);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();
}

void drawIceMan() {
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.1/2, 0);
	glutSolidSphere(0.1, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslated(0, 0.15, 0);
	glutSolidSphere(0.08, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, 0.25, 0);
	glutSolidSphere(0.06, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0, 0.3, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.035, 0.1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.4, 0);
	glutSolidSphere(0.02, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0, 0.26, 0);
	glutSolidCone(0.03, 0.1, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.035, 0.27,0.04);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(-0.035, 0.27, 0.04);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();
}

void drawIce() {
	glColor3f(0,0,0);
	glPushMatrix();
	glTranslated(0, 0.005, 0);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.01f, 0.01f, 0.2f, 20, 20);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(0.09, 0.15, 0);
	glRotated(30, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.01f, 0.01f, 0.2f, 20, 20);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(0.09, 0.05, 0);
	glRotated(-30, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.01f, 0.01f, 0.2f, 20, 20);
	glPopMatrix();
}
void drawSantaClause() {

	glPushMatrix();
	glTranslated(0,0.15,0);

	glColor3f(0, 0, 0);
	glPushMatrix();
	glRotated(-90,1, 0, 0);
	glutSolidCone(0.18, .4, 50, 50);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0, 0.4 / 2, 0);
	glutSolidSphere(0.1,15,15);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(0, 0.35, 0);
	glutSolidSphere(0.07, 15, 15);
	glPopMatrix();

	glColor3f(0, 0, 0);
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
	glRotated(-30,0,0,1);
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

	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslated(0.03, 0.37, 0.06);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

	glColor3f(0, 0, 1);
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
}
void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotated(0, 0, 0, 1.0);
	drawWall(0.02);
	glPopMatrix();


	//drawSantaClause();

	//drawIce();

	//drawIceMan();

	//drawPresent();
	//drawTree();
	//
	//glPushMatrix();
	//glColor3f(1, 0, 0);
	//glTranslated(0.5, 0.3, 0.5);
	//glRotated(-90, 0, 1, 0);
	////glRotated(20, 0, 0, 1);
	//drawStar();
	//drawTree();
	//glPopMatrix();


	
	/*drawGate();

	drawTest();

	glPushMatrix();
	glRotated(90, 0, 1, 0);
	drawTest();

	glPopMatrix();
	glTranslated(1, 0, 0);
	drawTest();
	glPushMatrix();

	glPopMatrix();*/

	glFlush();
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

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
