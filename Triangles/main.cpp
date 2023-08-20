#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <cmath>

double time = 0, rotation = 0, displace = 0;

void Init(){
	glClearColor(0, 0, 0, 0);
}

void DrawTriangle(double size, double angle, double translation, const GLfloat* color){
	glPushMatrix();
	glTranslated(translation, translation, 0);
	glTranslated(size / 3, size / 3, 0);
	glRotated(angle, 0, 0, 1);
	glTranslated(-size / 3, -size / 3, 0);
	glBegin(GL_TRIANGLES); {
		glColor4fv(color);
		glVertex2f(0, 0);
		glColor4fv(color+4);
		glVertex2f(0, size);
		glColor4fv(color+8);
		glVertex2f(size, 0); }
	glEnd();
	glPopMatrix();
}

void DisplayScene(){
	const GLfloat colors[][4] = { 
		{ 1.0f, 0.0f, 0.0f, 0.9f },	// red
		{ 1.0f, 0.5f, 0.0f, 0.9f },
		{ 1.0f, 0.0f, 0.0f, 0.9f },
		{ 0.0f, 1.0f, 0.0f, 0.9f },	// green
		{ 0.7f, 1.0f, 0.0f, 0.9f },
		{ 0.0f, 1.0f, 0.7f, 0.9f },
		{ 0.0f, 0.0f, 1.0f, 0.9f },	// blue
		{ 0.0f, 0.3f, 1.0f, 0.9f },
		{ 0.3f, 0.0f, 1.0f, 0.9f },
		{ 1.0f, 1.0f, 0.0f, 0.9f }, // orange
		{ 1.0f, 1.0f, 0.5f, 0.9f },
		{ 0.5f, 1.0f, 0.5f, 0.9f },
		{ 1.0f, 0.5f, 0.0f, 0.9f }, // yellow
		{ 1.0f, 0.5f, 0.0f, 0.9f },
		{ 1.0f, 0.5f, 0.0f, 0.9f },
		{ 1.0f, 0.0f, 1.0f, 0.9f }, // violet
		{ 1.0f, 0.0f, 0.6f, 0.9f },
		{ 0.6f, 0.0f, 0.8f, 0.9f },
	};
	const GLfloat *red   =  (const GLfloat*) colors + 12*0;
	const GLfloat *green  = (const GLfloat*) colors + 12*1;
	const GLfloat *blue	  = (const GLfloat*) colors + 12*2;
	const GLfloat *yellow = (const GLfloat*) colors + 12*3;
	const GLfloat *orange = (const GLfloat*) colors + 12*4;
	const GLfloat *violet = (const GLfloat*) colors + 12*5;
	const double triangleSize = 50.;
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotated(rotation / 2., 0, 0, 1);
	double nonlinearRot = rotation - (180. * M_1_PI * sin(rotation * M_PI / 180.));
	for (int i = 0; i < 4; i++){
		glPushMatrix(); {
			DrawTriangle(triangleSize, nonlinearRot, displace, red);
			glTranslated(triangleSize, 0, 0);
			DrawTriangle(triangleSize, nonlinearRot, displace*2, orange);
			glTranslated(triangleSize, 0, 0);
			DrawTriangle(triangleSize, nonlinearRot, displace*3, yellow); 
		} glPopMatrix();
		glPushMatrix(); {
			glTranslated(0, triangleSize, 0);
			DrawTriangle(triangleSize, nonlinearRot, displace*2, violet);
			glTranslated(0, triangleSize, 0);
			DrawTriangle(triangleSize, nonlinearRot, displace*3, blue);
			glTranslated(triangleSize, -triangleSize, 0);
			DrawTriangle(triangleSize, nonlinearRot, displace*4, green);
		} glPopMatrix();
		glRotated(90, 0, 0, 1);
	}
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void Reshape(int width, int height){
	if (height == 0) height = 1;
	GLdouble aspect = (GLdouble) width / height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const GLdouble halfX = 500 * aspect / 2.;
	const GLdouble halfY = 500. / 2.;
	gluOrtho2D(-halfX, halfX, -halfY, halfY);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void RotationTimer(int){
	time += 1.2;
	rotation = -time;
	displace = (-cos(time * M_PI / 180.) + 1.)*10; //fabs(fmod(time/180+1, 2)-1)*20;
	glutTimerFunc(20, RotationTimer, 1);
	glutPostRedisplay();
}

void KeyboardHandler(unsigned char key, int mouseX, int mouseY){
	switch (key){
	case 'f':
		glutFullScreenToggle();
		break;
	}
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	glutInit(&__argc, __argv);
#else
int main(int argc, char** argv){
	glutInit(&argc, argv);
#endif

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Triangles");
	Init();
	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardHandler);
	glutTimerFunc(20, RotationTimer, 1);
	glutMainLoop();
	return 0;
}