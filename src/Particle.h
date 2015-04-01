#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER
/**
    File: Particle.h
    Author: Morgan Trench
    Date: 30/03/15
    Contributors: ...
    Language: C++
    Compiler Options: -
    
    Description:
        Header file describing the Partcle Class
*/
    enum status {alive, dead};

    class Particle{

        /* Fields */
        // Particle Position, Velocity and Acceleration stored as an array of components (3 dimensions)
        float pos[3], vel[3], acc[3], rgb[3];
        // Particle size (radius) and mass
        float radius, mass;
        status state;

      public:

        /* Constructors */
        Particle();
        Particle(float x, float y, float z, float m, float r);

        /* Setters */
        void setPosition(float x, float y, float z);
        void setVelocity(float xv, float yv, float zv);
        void setAcceleration(float xa, float ya, float za);
        void setMass(float m);
        void setRadius(float m);

        /* Getters */
        float * getPosition();
        float * getVelocity();
        float * getAcceleration();
        float getVelocitySquared();
        float * getColour();
        float getMass();
        float getRadius();
        status getState();

        /* Mutators */
        void addForce(float xf, float yf, float zf);
        void subtractForce(float xf, float yf, float zf);
        void setState(status s);

        /* Function Definitions */

        /* Update Position, Velocitiy and reset acceleration to the passed in values (eg gravity) */
        void step(float timeStep);
        void applyBoundaries(float left, float right, float up, float down, float restitution);
    };

#endif
