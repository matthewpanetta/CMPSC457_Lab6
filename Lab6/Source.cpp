/* Program: lab6.cpp
*
* I reused code from the snowman example found here: http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/.
* The movment code was taken from that tutorial, as well as the gluLookAt() function. I added the ability to change the eyeHeight and the Y angle.
*
*/

/* Controls: 
LEFT ARROW:		move heading left
RIGHT ARROW:	move heading right
UP ARROW:		move forward at angle
DOWN ARROW:		move backward at angle

PAGE UP:		ascend vertically
PAGE DOWN:		descend vertically
HOME:			look up
END:			look down
*/

#ifdef __APPLE__
#include <unistd.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

float overhead_intensity = 0.5f;

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, ly = 0.5f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, y = 1.9f, z = 5.0f;

/* These control the overhead lighting */
GLfloat fLtAmbient[4] = { 0, 0, 0, 1 };
GLfloat fLtDiffuse[4] = { 0.85, 0.85, 0.85, 1 };
GLfloat fLtSpecular[4] = { 1, 1, 1, 1 };
GLfloat lightPosition[4] = { 5, 5, 5, 1 };

/* These control the lamp lighting */
GLfloat redDiffuse[4] = { 1, 0, 0, 1 };
GLfloat greenDiffuse[4] = { 0, 1, 0, 1 };
GLfloat blueDiffuse[4] = { 0, 0, 1, 1 };

/* These control the lamp and light position */
GLfloat redPosition[4] = { 0.2, 1.2, 0.3, 1 };
GLfloat greenPosition[4] = { 0.8, 1.2, 0.3, 1 };
GLfloat bluePosition[4] = { 1.4, 1.2, 0.3, 1 };

/* These control various materials in the environment */
GLfloat noMat[4] = { 0, 0, 0, 1 };
GLfloat groundMat[4] = { 0.9, 0.0, 0.15, 1 };
GLfloat grayMat[4] = { 0.7, 0.7, 0.7, 1 };
GLfloat redMat[4] = { 1, 0, 0, 1 };
GLfloat greenMat[4] = { 0, 1, 0, 1 };
GLfloat blueMat[4] = { 0, 0, 1, 1 };

/* Counter variable used for the lamp colors */
int i = 0;

void draw_light_sphere() {
	// This draws the overhead wire sphere, which symbolizes the overhead light
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glutWireSphere(0.1, 15, 15);
	glPopMatrix();
}

void draw_lamp(GLfloat material[]) {
	// This draws one lamp. The lamp includes a solid cube base, a solid cube support, and a wire sphere top.

	glPushMatrix();
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, grayMat);	// Set the cube and support's material to gray
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grayMat);
	glScalef(0.35, 0.05, 0.35);
	glutSolidCube(1);			// Draw the base
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	glPushMatrix();
	glScalef(0.05, 0.35, 0.05);
	glutSolidCube(1);			// Draw the support stick
	glPopMatrix();
	glTranslatef(0, 0.2, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, material);	// Set wire sphere's color to the argument passed in.
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material);

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glutWireSphere(0.1, 15, 15);	// Draw wire sphere
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, grayMat);	// Reset material color
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grayMat);
}

void draw_floor() {
	// This draws the floor. It is a flat polygon.
	glPushMatrix();
	glBegin(GL_POLYGON);
	glVertex3f(-7, 0, -7);
	glVertex3f(7, 0, -7);
	glVertex3f(7, 0, 7);
	glVertex3f(-7, 0, 7);
	glEnd();
	glPopMatrix();
}

void draw_table_leg() {
	// This draws one table leg. It is a stretched cube.
	glPushMatrix();
	glScalef(0.07, 1.25, 0.07);
	glutSolidCube(1);
	glPopMatrix();
}

void draw_table_top() {
	// This draws the table top. It is a flat polygon.
	glPushMatrix();
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(1.9, 0, 0);
	glVertex3f(1.9, 0, 1);
	glVertex3f(0, 0, 1);
	glEnd();
	glPopMatrix();
}

void draw_table() {
	// This function draws a whole table using the above methods.
	// Draws four table legs and one table top.
	glPushMatrix();
	glTranslatef(0, 0.23, 0);
	draw_table_leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.7, 0.23, 0);
	draw_table_leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.23, 0.7);
	draw_table_leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.7, 0.23, 0.7);
	draw_table_leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.12, 0.856, -0.2);
	draw_table_top();
	glPopMatrix();
}

void draw_objects() {
	// This function draws the entire environment.
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	draw_light_sphere();
	glPopMatrix();

	draw_floor();

	glPushMatrix();
	draw_table();				// Draw first table.
	glPopMatrix();

	glTranslatef(0, 0, -5);
	draw_table();				// Draw second table. This is the table with lamps on it.

	glPushMatrix();
	glTranslatef(0.2, 0.87, 0.3);
	draw_lamp(redMat);			// Draw each lamp and pass in its color.
	glTranslatef(0.6, 0, 0);
	draw_lamp(greenMat);
	glTranslatef(0.6, 0, 0);
	draw_lamp(blueMat);
	glPopMatrix();


}

void renderScene(void) {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z,				// x, y, and z determine the eye position. lx, ly, and lz determine where the eye is looking.
		x + lx, y - ly, z + lz,
		0.0f, 1.0f, 0.0f);

	/* Apply the overhead lighting */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fLtDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fLtSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	/* Enable all the lights */
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	/* Draw the environment */
	draw_objects();

	/* Add in the lamp's lighting */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, redDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, redPosition);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, greenDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, greenPosition);

	glLightfv(GL_LIGHT3, GL_DIFFUSE, blueDiffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, bluePosition);

	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y) {
	/* You can toggle the overhead light's intensity with digits 0-9 on the keyboard.*/
	switch (key) {
	case 96: overhead_intensity = 0.0f;	break;
	case 48: overhead_intensity = 0.0f;	break;
	case 97: overhead_intensity = 0.1f;	break;
	case 49: overhead_intensity = 0.1f;	break;
	case 98: overhead_intensity = 0.2f;	break;
	case 50: overhead_intensity = 0.2f;	break;
	case 99: overhead_intensity = 0.3f;	break;
	case 51: overhead_intensity = 0.3f;	break;
	case 100: overhead_intensity = 0.4f;	break;
	case 52:  overhead_intensity = 0.4f;	break;
	case 101: overhead_intensity = 0.5f;	break;
	case 53:  overhead_intensity = 0.5f;	break;
	case 102: overhead_intensity = 0.6f;	break;
	case 54:  overhead_intensity = 0.6f;	break;
	case 103: overhead_intensity = 0.7f;	break;
	case 55:  overhead_intensity = 0.7f;	break;
	case 104: overhead_intensity = 0.8f;	break;
	case 56:  overhead_intensity = 0.8f;	break;
	case 105: overhead_intensity = 0.9f;	break;
	case 57:  overhead_intensity = 0.9f;	break;
	default: overhead_intensity = 0.5f;	break;
	}

	GLfloat fLtAmbient[4] = { overhead_intensity, overhead_intensity, overhead_intensity };

	glLightfv(GL_LIGHT0, GL_AMBIENT, fLtAmbient);	// Apply changes to the scene
}

void processSpecialKeys(int key, int xx, int yy) {
	/* Controls: 
		LEFT ARROW:		move heading left
		RIGHT ARROW:	move heading right
		UP ARROW:		move forward at angle
		DOWN ARROW:		move backward at angle

		PAGE UP:		ascend vertically
		PAGE DOWN:		descend vertically
		HOME:			look up
		END:			look down	
	*/

	// Part of this code was taken from the snowman example at http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/.
	// I adjusted the speed at which the user moves and I added the pageup, pagedown, home, and end functionalities.


	float fraction = 0.3f;	// General movement speed

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.1f;		// Adjust the turning speed
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.1f;		// Adjust turning speed
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;		// Move forward at angle
		z += lz * fraction;	
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;		// Move backward at angle
		z -= lz * fraction;
		break;
	case GLUT_KEY_PAGE_UP:
		y += ly * fraction;		// Ascend vertically
		break;
	case GLUT_KEY_PAGE_DOWN:
		y -= ly * fraction;		// Descend vertically
		break;
	case GLUT_KEY_HOME:
		ly -= fraction * 0.25;	// Look Up
		break;
	case GLUT_KEY_END:
		ly += fraction * 0.25;	// Look Down
		break;
	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(60, ratio, 1, 1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Lab 6");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(processSpecialKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}