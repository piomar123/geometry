#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <random>
#include <string>
#include <vector>
#include "geometry.h"

#define MBTN_LEFT   1
#define MBTN_RIGHT  2
#define MBTN_MIDDLE 4

//double time = 0, rotation = 0, displace = 0;

Vector2d halfScreen = Vector2d(400., 300.);
int mouseBtns = 0;

std::vector<Vector2d> points;
std::vector<std::pair<Vector2d, Vector2d>> lines;

bool isLeftSide(Vector2d from, Vector2d to, Vector2d point) {
	double d = (point.x - from.x) * (to.y - from.y) - (point.y - from.y) * (to.x - from.x);
	return d < 0;
}

void SlowConvexHull(const std::vector<Vector2d>& points, std::vector<std::pair<Vector2d, Vector2d>>& lines) {
	lines.clear();
	for (size_t i = 0; i < points.size(); i++) {
		for (size_t j = 0; j < points.size(); j++) {
			bool valid = true;
			if (i == j) {
				continue;
			}
			for (size_t r = 0; r < points.size(); r++) {
				if (r == i || r == j) {
					continue;
				}
				if (isLeftSide(points[i], points[j], points[r])) {
					valid = false;
				}
			}
			if (valid) {
				lines.push_back(std::make_pair(points[i], points[j]));
			}
		}
	}
}

/**
Cross product of vectors p->q and p->r
If result > 0 then point r is on the left side of the vector p->q
If result < 0 then point r is on the right side of the vector p->q
If result == 0 then point r is in line with vector p->q
*/
double crossProductZValue(const Vector2d & p, const Vector2d & q, const Vector2d & r) {
	return (p.x * q.y) + (q.x * r.y) + (r.x * p.y) - (q.y * r.x) - (r.y * p.x) - (p.y * q.x);
}

void QuickConvexHull(const std::vector<Vector2d>& inputPoints, std::vector<std::pair<Vector2d, Vector2d>>& lines) {
	if (inputPoints.empty()) {
		return;
	}
	lines.clear();
	std::vector<Vector2d> points = inputPoints;
	std::sort(points.begin(), points.end());
	std::deque<Vector2d> upper;
	std::deque<Vector2d> lower;

	for (auto it = points.begin(); it != points.end(); it++) {
		upper.push_back(*it);
		while (true) {
			if (upper.size() < 3) {
				break;
			}
			const auto& lastPoint = upper[upper.size() - 1];
			const auto& middlePoint = upper[upper.size() - 2];
			const auto& thirdPoint = upper[upper.size() - 3];
			bool lastThreePointsMakeRightTurn = crossProductZValue(thirdPoint, middlePoint, lastPoint) < 0;
			if (lastThreePointsMakeRightTurn) {
				break;
			}
			upper.erase(std::prev(upper.end(), 2));
		}
	}

	for (auto it = points.rbegin(); it != points.rend(); it++) {
		lower.push_back(*it);
		while (true) {
			if (lower.size() < 3) {
				break;
			}
			const auto& lastPoint = lower[lower.size() - 1];
			const auto& middlePoint = lower[lower.size() - 2];
			const auto& thirdPoint = lower[lower.size() - 3];
			bool lastThreePointsMakeRightTurn = crossProductZValue(thirdPoint, middlePoint, lastPoint) < 0;
			if (lastThreePointsMakeRightTurn) {
				break;
			}
			lower.erase(std::prev(lower.end(), 2));
		}
	}
	for (size_t i = 0; i < upper.size() - 1; i++) {
		lines.push_back(std::make_pair(upper[i], upper[i + 1]));
	}
	for (size_t i = 0; i < lower.size() - 1; i++) {
		lines.push_back(std::make_pair(lower[i], lower[i + 1]));
	}
}

void Init() {
	glClearColor(0, 0, 0, 0);
}


void DrawTriangle(float size, double angle, double translation, const GLfloat* color) {
	glPushMatrix();
	glTranslated(translation, translation, 0);
	glTranslated(size / 3, size / 3, 0);
	glRotated(angle, 0, 0, 1);
	glTranslated(-size / 3, -size / 3, 0);
	glBegin(GL_TRIANGLES); {
		glColor4fv(color);
		glVertex2f(0, 0);
		glColor4fv(color + 4);
		glVertex2f(0, size);
		glColor4fv(color + 8);
		glVertex2f(size, 0); }
	glEnd();
	glPopMatrix();
}

void DisplayScene() {
	const GLfloat colors[4] = { 1.0f, 0.0f, 0.0f, 0.9f };
	const GLfloat lineOriginColor[4] = { 0.8f, 0.8f, 0.8f, 0.3f };
	const GLfloat lineEndColor[4] = { 0.9f, 0.9f, 0.2f, 1.0f };

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	glBegin(GL_LINES);
	for (size_t i = 0; i < lines.size(); i++) {
		const auto &p1 = lines[i].first;
		const auto &p2 = lines[i].second;
		glColor4fv(lineOriginColor);
		glVertex2d(p1.x, p1.y);
		glColor4fv(lineEndColor);
		glVertex2d(p2.x, p2.y);
	}
	glEnd();

	glPointSize(8.);
	glBegin(GL_POINTS);
	for (auto &p : points) {
		glColor4fv(colors);
		glVertex2d(p.x, p.y);
	}
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void Reshape(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	halfScreen.x = width / 2.f;
	halfScreen.y = height / 2.f;
	gluOrtho2D(-halfScreen.x, halfScreen.x, -halfScreen.y, halfScreen.y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void KeyboardHandler(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 'f':
		glutFullScreenToggle();
		break;

	case 'z':
		if (points.empty()) {
			break;
		}
		points.pop_back();
		glutPostRedisplay();
		break;
	}
	QuickConvexHull(points, lines);
}

void MouseButtonHandler(int button, int state, int x, int y) {
	//currMouse.x = x;
	//currMouse.y = y;

	int btnFlag = 0;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		btnFlag = MBTN_LEFT;
		break;

	case GLUT_RIGHT_BUTTON:
		btnFlag = MBTN_RIGHT;
		break;
	}
	if (!btnFlag) return;

	if (state == GLUT_DOWN) {
		mouseBtns |= btnFlag;
		if (mouseBtns == MBTN_LEFT) {
			auto point = Vector2d(x - halfScreen.x, halfScreen.y - y);
			points.push_back(point);
		}
	}
	else {
		mouseBtns &= ~btnFlag;
		if (mouseBtns == 0 && btnFlag == MBTN_LEFT) {

		}
	}
	QuickConvexHull(points, lines);
	std::string title = std::string("Lines: ") + std::to_string(lines.size());
	glutSetWindowTitle(title.c_str());
	glutPostRedisplay();
}

void GenerateRandomPoints(int count, std::vector<Vector2d>& points) {
	std::mt19937 generator((unsigned int) std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> distribution(-1., 1.);
	for (int i = 0; i < count; i++) {
		points.push_back(Vector2d(distribution(generator) * halfScreen.x / 2, distribution(generator) * halfScreen.y / 2));
	}
}


// TODO Use CMake

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	glutInit(&__argc, __argv);
#else
int main(int argc, char** argv) {
	glutInit(&argc, argv);
#endif

	GenerateRandomPoints(50, points);
	QuickConvexHull(points, lines);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(int(2 * halfScreen.x), int(2 * halfScreen.y));
	glutCreateWindow("Convex hull");
	Init();
	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardHandler);
	glutMouseFunc(MouseButtonHandler);
	//glutTimerFunc(20, RotationTimer, 1);
	glutMainLoop();
	return 0;
}