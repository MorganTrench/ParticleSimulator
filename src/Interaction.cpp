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
#include <iostream>

#define G 6.6738e-3

Interaction::Interaction(Particle *p, int *num){
	particles = p;
	n = num;
}

void Interaction::resetAccelerations(){
    for(int i = 0; i < *n; i++){
        particles[i].setAcceleration(0.0, 0.0, 0.0);
    }
}

void Interaction::constant(Particle *p, float acc[3]){
	p->addForce(acc[0], acc[1], acc[2]);
}

//TODO, Tidy / Optimise
void Interaction::gravity(Particle *p1, Particle *p2){
            // If one of the particles is dead, disread the interaction
            if((p1->getState() == dead)||(p2->getState() == dead)){
                return;
            }
            // Initialise Variables
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

            // Sticky Collision
            if(dist < (0.001*p1->getRadius() + 0.001*p2->getRadius() )){
                p2->setState(dead);
                // Calculate properties of combined particle
                // Conservation of momentum: m1v1 + m2v2 = mcvc
                float *v1 = p1->getVelocity();
                float *v2 = p2->getVelocity();
                float v3[3];
                float m1 = p1->getMass();
                float m2 = p1->getMass();
                float mc = p1->getMass() + p2->getMass();
                v3[0] = (m1*v1[0] + m2*v2[0])/mc;
                v3[1] = (m1*v1[1] + m2*v2[1])/mc;
                v3[2] = (m1*v1[2] + m2*v2[2])/mc;
                // Set properties
                p1->setMass(mc);
                p1->setRadius(p1->getRadius() + p2->getRadius());
                p1->setVelocity(v3[0], v3[1], v3[2]);
            } else {
                // F = gmM/r^2, factor = F / dist
                float factor = G * p1->getMass() * p2->getMass() / (distSquared * dist);
                // F * component
                forceVec[0] = factor*xcomp;
                forceVec[1] = factor*ycomp;
                forceVec[2] = factor*zcomp;

                p1->addForce(forceVec[0], forceVec[1], forceVec[2]);
                p2->subtractForce(forceVec[0], forceVec[1], forceVec[2]);
            }
            
}

void Interaction::interact(){
    resetAccelerations();
    // Equal and Opposite Force Optimization
    for(int i = 0; i < *n; i++){
        for(int j = i+1; j < *n; j++){
            //std::cout << i << " " << j << std::endl;
            // Inter-Particle Gravity
            gravity(&particles[i], &particles[j]);
        }
    }


}

