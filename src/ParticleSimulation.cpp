/**
    File: gas.cpp
    Author: Morgan Trench
    Date: 01/04/15
    Contributors: ...
    Language: C++
    Compiler Options: -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
    
    Compile Instructions
		- OSX: g++ -o gas src/Particle.cpp src/gas.cpp -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
	Usage
		- ./ProgramName ParticleNumber TimeStep WindowWidth WindowHeight

    Description:
        Basic bunch of particles in a box simulation
*/
#define reportNum 100

/* Imports */
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "Particle.h"
#include "Interaction.h"
/* Graphics */
// #define GLEW_STATIC
// #include <GL/glew.h>

// using namespace std;

/* Global Variables */
// Number of Particles
int n;
// Size of timestep
float timeStep;
// Window Dimensions
int windowWidth;
int windowHeight;

/* Parses commandline arguments into variables */
void parseArguments (int argc, char *argv[]) {
	if (argc != 5){
		std::cout << "Usage ./ProgramName ParticleNumber TimeStep WindowWidth WindowHeight" << std::endl;
		exit(-1);
	}
	n = atoi(argv[1]);
	timeStep = atof(argv[2]);
	windowWidth = atoi(argv[3]);
	windowHeight = atoi(argv[4]);
	// Debug Output
	std::cout 	<< "Arguments: \n"
			<< "\tParticles: " << n  << '\n'
			<< "\tTime Step: " << timeStep << " seconds" << '\n'
			<< "\tResoluion: " << windowWidth << "*" << windowHeight << std::endl;
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
	if(p.getState() == alive){
		float *pos;
		pos = p.getPosition();
		glPointSize(p.getRadius());
		glBegin(GL_POINTS);
		glColor3f(rgb[0], rgb[1], rgb[2]);
		glVertex3f(pos[0], pos[1], pos[2]);
		glEnd();
	}
}

int main(int argc, char *argv[]){
	std::cout << "Particles in a box Simulations" << std::endl;
	parseArguments(argc, argv);
	std::cout << "Setting up: " << std::endl;
	srand(time(NULL));

	// Initialise GLFW
	std::cout << "\tInitialising GLFW..." << std::endl;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Window (and context)
	std::cout << "\tCreating Window and Context..." << std::endl;
	GLFWwindow* window;
	window = glfwCreateWindow(windowWidth, windowHeight, "C++ and OpenGL Practice: Gas", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Change Argument to 1 for 60fps cap

	// Set callbacks
	std::cout << "\tSetting Callbacks / Event Handlers..." << std::endl;
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

	// Initalise particles for simulation
	std::cout << "\tReserving memory and creating particles..." << std::endl;
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
		particles[i].setVelocity(0.0, 0.0, 0.0);
		// particles[i].setVelocity(vx, vy, 0.0);
	}
	// Initalise interaction class
	Interaction interactions = Interaction(particles, &n);

	// Rendering and Simulation Loop
	float simulationTime = 0.0;
	float prevReportTime = 0.0;
	float realTime = 0.0;
	float prevRealTime = 0.0; 
	std::cout << "Beginning Simulation..." << std::endl;
	while (!glfwWindowShouldClose(window)){
		if((simulationTime - prevReportTime) >= reportNum*timeStep){

			std::cout << "\r\tTime: " << simulationTime << " seconds";
			prevRealTime = realTime;
			realTime = glfwGetTime();
			std::cout << " - " << reportNum/(realTime - prevRealTime) << " fps";
			std::cout << " - " << timeStep*reportNum/(realTime - prevRealTime) << " time ratio";
			std::cout.flush();
			prevReportTime = simulationTime;
		}
		//std::cout << simulationTime << " - " << prevReportTime << std::endl;
		// Clear Screen (black by default)
		glClear(GL_COLOR_BUFFER_BIT);

		interactions.interact();

		// Draw and update particles
		for (int i = 0; i < n; i++){
			//Draw Particle
			drawParticle(particles[i], particles[i].getColour());

			// Update Particle Position
			particles[i].step(timeStep);
			particles[i].applyBoundaries(-1.0, 1.0, 1.0, -1.0, 0.8);
		}
		simulationTime += timeStep;

		// Update Screen and get events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	std::cout << std::endl;

	/* Terminate */
	std::cout << "Ending Simulation..." << std::endl;
	free(particles);
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Done" << std::endl;
	exit(EXIT_SUCCESS);
}
