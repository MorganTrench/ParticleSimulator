/* gas.cpp - Morgan Trench
Basic bunch of particles in a box simulation

Usage ./programName

Compile Instructions
- OSX: g++ -o gas Particle.cpp gas.cpp -lglew -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo 0

Depedencies: GLFW3, GLEW

TODO
*/

#define particleNum 5000

/* Imports */
#include <iostream>
#include "Particle.h"
/* Graphics */
//#define GLEW_STATIC
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

/* Global Variables */
// Number of Particles
// TODO Use this, implement with dynamic memory allocation
int n;
// Size of timestep
float timeStep;
// Window Dimensions
int windowWidth;
int windowHeight;

/* Parses commandline arguments into variables */
void parseArguments(int argc, char *argv[]){
	// TODO Actually parse arguments
	n = 1000;
	timeStep = 0.001;
	windowWidth = 600;
	windowHeight = 400;
}

/* GLFW Error Callback */
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* GLFW Keyboard Input callback function */
// Currently allows 'esc' key to terminate the program
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/* Dedicated Function for drawing particles */
// Allows easier edits / readability, currently unused
void drawParticle(Particle p, float rgb[3]){
	float *pos;
	pos = p.getPosition();
	glPointSize(p.getRadius());
	glBegin(GL_POINTS);
	glColor3f(rgb[0], rgb[1], rgb[2]);
	glVertex3f(pos[0], pos[1], pos[2]);
	glEnd();
}

int main(int argc, char *argv[]){
	parseArguments(argc, argv);

	// Initialise GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);
	// Create Window (and context)
	GLFWwindow* window;
	window = glfwCreateWindow(windowWidth, windowHeight, "C++ and OpenGL Practice: Gas", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Set callbacks
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

	// Initalise particles for simulation
	Particle particles[particleNum];
	for (int i = 0; i < particleNum; i++){
		float x = ((float) rand()) / RAND_MAX;
		float y = ((float) rand()) / RAND_MAX;
		if (rand() % 2 == 0)
			x = -x;
		if (rand() % 2 == 0)
			y = -y;
		particles[i] = Particle(x, y, 0.0, 1.0, 5.0);
		particles[i].setVelocity(x, y, 0.0);
	}

	// Rendering and Simulation Loop
	while (!glfwWindowShouldClose(window)){

		// Clear Screen (black by default)
		glClear(GL_COLOR_BUFFER_BIT);

		// Red Particles, temporary
		float rgb[3] = {1.f, 0.f, 0.f};

		// Draw and update particles
		for (int i = 0; i < particleNum; i++){

		// TODO Use this function
		//drawParticle(particles[i], rgb);

		float *pos;
		pos = particles[i].getPosition();
		glPointSize(particles[i].getRadius());
		glBegin(GL_POINTS);
		glColor3f(rgb[0], rgb[1], rgb[2]);
		glVertex3f(pos[0], pos[1], pos[2]);
		glEnd();

		particles[i].step(0.f, -1.f, 0.f, 0.01);
		particles[i].applyBoundaries(-1.0, 1.0, 1.0, -1.0, 0.95);
	}

	// Update Screen and get events
	glfwSwapBuffers(window);
	glfwPollEvents();
}

	/* Terminate */
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
