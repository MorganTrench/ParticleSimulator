/**
    File: Interaction.cpp
    Author: Morgan Trench
    Date: 01/03/15
    Contributors: ...
    Language: C++
    Compiler Options: -

    Description:
        Implementation of the 'Interaction Class', which provides a (hopefully) modular way
        of applying interactions between particles whilst maintaining some efficiency.

   	TODOs:
   		Implement everything
*/

#include "Interaction.h"
#include <cmath>
#include <iostream>

#define G 6.6738e-3
#define dragCoeffient 1e0

int collisions = 0;

Interaction::Interaction(Particle *p, int *num){
	particles = p;
	n = num;
}

void Interaction::resetAccelerations(){
    for(int i = 0; i < *n; i++){
        particles[i].setAcceleration(0.0, 0.0, 0.0);
    }
}

void Interaction::constant(Particle *p, double acc[3]){
	p->addForce(acc[0], acc[1], acc[2]);
}

void Interaction::drag(Particle *p){
    if(p->getState() == dead){
        return;
    }
    // Checks / Debug
    p->checkParticle("Drag: Pre");

    double *vel = p->getVelocity();
    double vSquared = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
    double factor = dragCoeffient*vSquared*(p->getRadius()*p->getRadius());
    double v = sqrt(vSquared);
    if(v == 0)
        return;

    // Update Particle
    p->subtractForce(factor*vel[0]/v, factor*vel[1]/v, factor*vel[2]/v);

    // Checks / Debug
    p->checkParticle("Drag: Post");
}

//TODO, Tidy / Optimize
void Interaction::gravity(Particle *p1, Particle *p2){
    // If one of the particles is dead, disregard the interaction
    if((p1->getState() == dead)||(p2->getState() == dead)){
        return;
    }

    // Checks / Debug
    p1->checkParticle("Gravity: Pre P1");
    p2->checkParticle("Gravity: Pre P2");

    // Initialise Variables
    double forceVec[3];
    double * pos1; double * pos2;
    double xcomp, ycomp, zcomp, distSquared, dist;
    pos1 = p1->getPosition();
    pos2 = p2->getPosition();

    // Calculate Distances
    xcomp = (pos2[0] - pos1[0]);
    ycomp = (pos2[1] - pos1[1]);
    zcomp = (pos2[2] - pos1[2]);
    distSquared = xcomp*xcomp + ycomp*ycomp + zcomp*zcomp;
    dist = sqrt(distSquared);
    double factor = G * p1->getMass() * p2->getMass() / (distSquared * dist);

    // Sticky Collision, TODO: improve
    if(dist < (0.002*p1->getRadius() + 0.002*p2->getRadius())){
        collisions++;
        p2->setState(dead);
        // Calculate properties of combined particle
        // Conservation of momentum: m1v1 + m2v2 = mcvc
        double *v1 = p1->getVelocity();
        double *v2 = p2->getVelocity();
        double v3[3];
        double m1 = p1->getMass();
        double m2 = p2->getMass();
        double mc = m1 + m2;
        v3[0] = (m1*v1[0] + m2*v2[0])/mc;
        v3[1] = (m1*v1[1] + m2*v2[1])/mc;
        v3[2] = (m1*v1[2] + m2*v2[2])/mc;
        // Set properties
        p1->setMass(mc);
        p1->setRadius(pow(0.75*mc, 1.0/3.0));
        p1->setVelocity(v3[0], v3[1], v3[2]);
    } else {
        // Calculate forces for each component
        // F = gmM/r^2, factor = F / dist, cos(theta) = _xomp/dist
        // F * component
        forceVec[0] = factor*xcomp;
        forceVec[1] = factor*ycomp;
        forceVec[2] = factor*zcomp;

        // Apply Forces, equal and opposite
        p1->addForce(forceVec[0], forceVec[1], forceVec[2]);
        p2->subtractForce(forceVec[0], forceVec[1], forceVec[2]);
    }
    // Checks / Debug
    p1->checkParticle("Gravity: Post P1");
    p2->checkParticle("Gravity: Post P2");
}

void Interaction::interact(){
    resetAccelerations();
    // Equal and Opposite Force Optimization
    for(int i = 0; i < *n; i++){
        for(int j = i+1; j < *n; j++){
            gravity(&particles[i], &particles[j]);
        }
    }
    // Uncommenting this 'deletes random or all particles' for no apparent reason, then they sometimes magically reappear TODO fix
    // for(int i = 0; i < *n; i++){
    //     drag(&particles[i]);
    // }
}

