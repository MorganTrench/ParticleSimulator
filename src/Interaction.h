#include "Particle.h"

#ifndef INTERACTION_HEADER
#define INTERACTION_HEADER
/**
    File: Interaction.h
    Author: Morgan Trench
    Date: 01/03/15
    Contributors: ...
    Language: C++
    Compiler Options: -
    
    Description:
        Header file describing the Interaction Class
*/

	class Interaction{
		Particle *particles;
		int *n;

	public:
		Interaction(Particle *p, int *n);
		void interact();

	private:
		void resetAccelerations();
		void constant(Particle *p, float acc[3]);
		void gravity(Particle *p1, Particle *p2);
		void interParticleGravity(int index);
	};

#endif