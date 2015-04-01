/**
    File: Interaction.cpp
    Author: Morgan Trench
    Date: 01/03/15
    Contributors: ...
    Language: C++
    Compiler Options: -
    
    Description:
        Implementation of the 'Interaction Class', which provides a (hopefully) modular way
        of applying interactions bewteen particles whilst maintaining some efficiency.

   	TODOs:
   		Implement everything 
*/

#include "Interaction.h"
#include <cmath>

#define G 6.6738e-3

Interaction::Interaction(Particle *p, int *num){
	particles = p;
	n = num;
}

void Interaction::resetAccelerations(){
    for(int i = 0; i < *n; i++){
        particles[i].setAcceleration(0.0, -9.8, 0.0);
    }
}

void Interaction::constant(Particle *p, float acc[3]){
	p->addForce(acc[0], acc[1], acc[2]);
}

//TODO, Tidy / Optimise
void Interaction::gravity(Particle *p1, Particle *p2){
            float forceVec[3];
            float * pos1; float * pos2;
            float xcomp, ycomp, zcomp, distSquared, dist;
            pos1 = p1->getPosition();
            pos2 = p2->getPosition();

            // Calculate Distances
            xcomp = (pos2[0] - pos1[0]);
            ycomp = (pos2[1] - pos1[1]);
            zcomp = (pos2[2] - pos1[2]);
            distSquared = xcomp*xcomp + ycomp*ycomp + zcomp*zcomp;
            dist = sqrt(distSquared);

            // F = gmM/r^2, factor = F / dist
            float factor = G * p1->getMass() * p2->getMass() / (distSquared * dist);
            // F * component
            forceVec[0] = factor*xcomp;
            forceVec[1] = factor*ycomp;
            forceVec[2] = factor*zcomp;

            p1->addForce(forceVec[0], forceVec[1], forceVec[2]);
            p2->subtractForce(forceVec[0], forceVec[1], forceVec[2]);
            
}

void Interaction::interact(){
    resetAccelerations();
    // Equal and Opposite Force Optimization
    for(int i = 0; i < *n; i++){
        for(int j = i+1; j < *n; j++){
            // Inter-Particle Gravity
            gravity(&particles[i], &particles[j]);
        }
    }


}

