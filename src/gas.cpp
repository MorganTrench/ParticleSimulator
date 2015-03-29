/* gas.cpp - Morgan Trench
Basic bunch of particles in a box simulation

Usage ./programName

Compile Instructions
- OSX: g++ -o gas Particle.cpp gas.cpp -lglew -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo 0

Depedencies: GLFW3, GLEW

TODO
*/

#define particleNum 50
#define reportNum 100

/* Imports */
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "time.h"
#include "Particle.h"
/* Graphics */
//#define GLEW_STATIC
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

/* Global Variables */
// Number of Particles
int n;
// Size of timestep
float timeStep;
// Window Dimensions
int windowWidth;
int windowHeight;

/* Parses commandline arguments into variables */
void parseArguments(int argc, char *argv[]){
	if (argc != 5){
		cout << "Usage ./ProgramName ParticleNumber TimeStep WindowWidth WindowHeight" << endl;
		exit(-1);
	}
	n = atoi(argv[1]);
	timeStep = atof(argv[2]);
	windowWidth = atoi(argv[3]);
	windowHeight = atoi(argv[4]);
	// Debug Output
	cout 	<< "Arguments: \n"
			<< "\tParticles: " << n  << '\n'
			<< "\tTime Step: " << timeStep << " seconds" << '\n'
			<< "\tResoluion: " << windowWidth << "*" << windowHeight << endl;
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

/* Dedicated Function for drawing a particle */
// Allows easier edits / readability, currently unused
inline void drawParticle(Particle p, float rgb[3]){
	float *pos;
	pos = p.getPosition();
	glPointSize(p.getRadius());
	glBegin(GL_POINTS);
	glColor3f(rgb[0], rgb[1], rgb[2]);
	glVertex3f(pos[0], pos[1], pos[2]);
	glEnd();
}

int main(int argc, char *argv[]){
	cout << "Particles in a box Simulations" << endl;
	parseArguments(argc, argv);
	cout << "Setting up: " << endl;
	srand(time(NULL));

	// Initialise GLFW
	cout << "\tInitialising GLFW..." << endl;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Window (and context)
	cout << "\tCreating Window and Context..." << endl;
	GLFWwindow* window;
	window = glfwCreateWindow(windowWidth, windowHeight, "C++ and OpenGL Practice: Gas", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Change Argument to 1 for 60fps cap

	// Set callbacks
	cout << "\tSetting Callbacks / Event Handlers..." << endl;
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

	// Initalise particles for simulation
	cout << "\tReserving memory and creating particles..." << endl;
	Particle *particles = (Particle *) malloc( sizeof(Particle) * n );
	for (int i = 0; i < n; i++){
		float x = ((float) rand()) / RAND_MAX;
		float y = ((float) rand()) / RAND_MAX;
		float vx = ((float) rand()) / RAND_MAX;
		float vy = ((float) rand()) / RAND_MAX;
		if (rand() % 2 == 0)
			x = -x;
		if (rand() % 2 == 0)
			y = -y;
		if (rand() % 2 == 0)
			vx = -vx;
		if (rand() % 2 == 0)
			vy = -vy;
		particles[i] = Particle(x, y, 0.0, 1.0, 1.0);
		particles[i].setVelocity(vx, vy, 0.0);
	}

	// Rendering and Simulation Loop
	float simulationTime = 0.0;
	float prevReportTime = 0.0;
	float realTime = 0.0;
	float prevRealTime = 0.0; 
	cout << "Beginning Simulation..." << endl;
	while (!glfwWindowShouldClose(window)){
		if((simulationTime - prevReportTime) >= reportNum*timeStep){

			cout << "\r\tTime: " << simulationTime << " seconds";
			prevRealTime = realTime;
			realTime = glfwGetTime();
			cout << " - " << reportNum/(realTime - prevRealTime) << " fps";
			cout << " - " << timeStep*reportNum/(realTime - prevRealTime) << " time ratio";
			cout.flush();
			prevReportTime = simulationTime;
		}
		//cout << simulationTime << " - " << prevReportTime << endl;
		// Clear Screen (black by default)
		glClear(GL_COLOR_BUFFER_BIT);

		// Red Particles, temporary
		float * pi;
		float * pj;
		float distSquared, xcomp, ycomp, zcomp;

		// Draw and update particles
		for (int i = 0; i < n; i++){
			//Draw Particle
			drawParticle(particles[i], particles[i].getColour());

			// Temp Gravity
			float forces[3] = {0.0f, 0.0f, 0.0f};
			for(int j = 0; j < n; j++){
				if(i != j){
					pi = particles[i].getPosition();
					pj = particles[j].getPosition();
					xcomp = (pj[0] - pi[0]);
					ycomp = (pj[1] - pi[1]);
					zcomp = (pj[2] - pi[2]);
					distSquared = xcomp*xcomp + ycomp*ycomp + zcomp*zcomp;
					forces[0] += (0.01*xcomp / distSquared) + (-0.001*particles[i].getVelocity()[0]);
					forces[1] += (0.01*ycomp / distSquared) + (-0.001*particles[i].getVelocity()[1]);
					forces[2] += (0.01*zcomp / distSquared) + (-0.001*particles[i].getVelocity()[2]);
				}
			}

			// Update Particle Position
			particles[i].step(forces[0], forces[1] - 100.0, forces[2], timeStep);
			particles[i].applyBoundaries(-1.0, 1.0, 1.0, -1.0, 0.8);
		}
		simulationTime += timeStep;

		// Update Screen and get events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	cout << endl;

	/* Terminate */
	cout << "Ending Simulation..." << endl;
	free(particles);
	glfwDestroyWindow(window);
	glfwTerminate();
	cout << "Done" << endl;
	exit(EXIT_SUCCESS);
}
