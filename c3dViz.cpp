#include <stdio.h>
#include <math.h>
#include "camera.h"
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <map>
#include "C3DFile.h"

// Geometry and whatnot
Camera theCamera;

// Our C3D File
C3DFile file("confused.c3d");

// UI Helpers
int lastX = 0, lastY = 0;
int theMenu = 0;
int theButtonState = 0;
int theModifierState = 0;

std::vector<vec3> unlabeledMarkers;
std::map<std::string,vec3> markers;

void onMouseMotionCb(int x, int y)
{
   int deltaX = lastX - x;
   int deltaY = lastY - y;
   bool moveLeftRight = abs(deltaX) > abs(deltaY);
   bool moveUpDown = !moveLeftRight;

   if (theModifierState & GLUT_ACTIVE_ALT) // camera move
   {
      if (theButtonState == GLUT_LEFT_BUTTON)  // Rotate
      {
         if (moveLeftRight && deltaX > 0) theCamera.orbitLeft(deltaX);
         else if (moveLeftRight && deltaX < 0) theCamera.orbitRight(-deltaX);
         else if (moveUpDown && deltaY > 0) theCamera.orbitUp(deltaY);
         else if (moveUpDown && deltaY < 0) theCamera.orbitDown(-deltaY);
      }
      else if (theButtonState == GLUT_MIDDLE_BUTTON) // Pan
      {
         /*if (moveLeftRight && deltaX > 0) theCamera.moveLeft(deltaX);
         else if (moveLeftRight && deltaX < 0) theCamera.moveRight(-deltaX);
         else if (moveUpDown && deltaY > 0) theCamera.moveUp(deltaY);
         else if (moveUpDown && deltaY < 0) theCamera.moveDown(-deltaY);*/
		  if (moveLeftRight && deltaX > 0) theCamera.moveRight(deltaX);
         else if (moveLeftRight && deltaX < 0) theCamera.moveLeft(-deltaX);
         else if (moveUpDown && deltaY > 0) theCamera.moveDown(deltaY);
         else if (moveUpDown && deltaY < 0) theCamera.moveUp(-deltaY);

      }
      else if (theButtonState == GLUT_RIGHT_BUTTON) // Zoom
      {
         //if (moveUpDown && deltaY > 0) theCamera.moveForward(deltaY);
        // else if (moveUpDown && deltaY < 0) theCamera.moveBack(-deltaY);
		 if (moveLeftRight && deltaX > 0) theCamera.moveForward(-deltaX);
         else if (moveLeftRight && deltaX < 0) theCamera.moveBack(deltaX);
      }         
   }
 
   lastX = x;
   lastY = y;
   glutPostRedisplay();
}

void onMouseCb(int button, int state, int x, int y)
{
   theButtonState = button;
   theModifierState = glutGetModifiers();
   lastX = x;
   lastY = y;
   onMouseMotionCb(x, y);
}


void onKeyboardCb(unsigned char key, int x, int y)
{
   unsigned int flag = 0x0;

   if (key == ' ') theCamera.reset();
   else if (key == 'r') file.SetCurrentFrame(0);
   else if (key == 27) exit(0); // ESC Key
   glutPostRedisplay();
}

void onMenuCb(int value)
{
   onKeyboardCb(value, 0, 0);
}

void onKeyboardSpecialCb(int key, int x, int y)
{
}

void onTimerCb(int value)
{
  if (file.Tick())
  {
	int numSubjects = file.GetSubjectCount();
	for (int subject = 0; subject < numSubjects; subject++)
	{
		std::string subjectName = file.GetSubjectName(subject);
		int numMarkers = file.GetMarkerCount(subjectName);
		for (int marker = 0; marker < numMarkers; marker++)
		{
			vec3 position(0,0,0);
			std::string markerName = "test";
			std::string markerParentName;
			file.GetMarkerData(subjectName, marker, markerName, markerParentName, position);
			//std::cout << position << std::endl;
			markers[markerName] = position;
		}
	}

	unlabeledMarkers.clear();
	int numUnlabeledMarkers = file.GetUnlabeledMarkerCount();
	for (int marker = 0; marker < numUnlabeledMarkers; marker++)
	{
     	vec3 position(0,0,0);
		file.GetUnlabeledMarkerData(marker, position);
		unlabeledMarkers.push_back(position);
	}
  }
    glutTimerFunc(1, onTimerCb, 0);
    glutPostRedisplay();
}

void onResizeCb(int width, int height)
{
   // Update viewport
   glViewport(0, 0, width, height);

   // Update camera projection's aspect ratio
   float vfov, aspect, zNear, zFar;
   theCamera.getProjection(&vfov, &aspect, &zNear, &zFar);
   theCamera.setProjection(vfov, ((GLfloat) width)/height, zNear, zFar);
}

void drawScene()
{
	float size = 1000.0;  // axis size
	float ssize = 1; // sphere size
   // Draw axes
   glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(size, 0.0, 0.0);

      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, size, 0.0);

      glColor3f(0.0, 0.0, 1.0);
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, size);
   glEnd();

	std::map<std::string,vec3>::const_iterator it;
	for (it = markers.begin(); it != markers.end(); ++it)
	{
		std::string name = it->first;
		vec3 pos = it->second;
 	    glColor3f(0.0, 1.0, 0.0);
	    glPushMatrix();
	    glTranslatef(pos[0], pos[1], pos[2]);
	    glutSolidSphere(ssize, 10.0, 10.0);
	    glPopMatrix();
	}

   for (int i = 0; i < unlabeledMarkers.size(); i++)
   {
	   vec3 pos = unlabeledMarkers[i];
	   glColor3f(1.0, 1.0, 1.0);
	   glPushMatrix();
	   glTranslatef(pos[0], pos[1], pos[2]);
	   glutSolidSphere(ssize, 10.0, 10.0);
	   glPopMatrix();
   }
}

void onDrawCb()
{
	// Draw Scene and overlay
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	theCamera.draw();
	drawScene();
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);

	glEnable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glCullFace(GL_BACK);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(2.0);
    glLineWidth(2.0);
}


int main(int argc, char **argv)
{    
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(600,600);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("C3D Viz");
   glutDisplayFunc(onDrawCb);
   glutKeyboardFunc(onKeyboardCb);
   glutSpecialFunc(onKeyboardSpecialCb);
   glutMouseFunc(onMouseCb);
   glutMotionFunc(onMouseMotionCb); 
   glutTimerFunc(100, onTimerCb, 0);
   glutReshapeFunc(onResizeCb);

   init();

   glutMainLoop();
   return 0;             
}
