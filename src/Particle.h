#import <iostream>
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
        Header file describing the Particle Class
*/
    enum status {alive, dead};

    class Particle{

        /* Fields */
        // Particle Position, Velocity and Acceleration stored as an array of components (3 dimensions)
        double pos[3], vel[3], acc[3], rgb[3];
        // Particle size (radius) and mass
        double radius, mass;
        status state;

      public:

        /* Constructors */
        Particle();
        Particle(double x, double y, double z, double m, double r);

        /* Setters */
        void setPosition(double x, double y, double z);
        void setVelocity(double xv, double yv, double zv);
        void setAcceleration(double xa, double ya, double za);
        void setMass(double m);
        void setRadius(double m);

        /* Getters */
        double * getPosition();
        double * getVelocity();
        double * getAcceleration();
        double getVelocitySquared();
        double * getColour();
        double getMass();
        double getRadius();
        status getState();

        /* Mutators */
        void addForce(double xf, double yf, double zf);
        void subtractForce(double xf, double yf, double zf);
        void setState(status s);

        /* Function Definitions */

        /* Update Position, Velocity and reset acceleration to the passed in values (eg gravity) */
        void step(double timeStep);
        void applyBoundaries(double left, double right, double up, double down, double restitution);
        bool checkParticle(std::string message);
        void outputProperties();
    };

#endif
