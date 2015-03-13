#include "Particle.h"
#include <cmath>

/* Constructors */
Particle::Particle(){
  pos[0] = 0; pos[1] = 0; pos[2] = 0;
  vel[0] = 0; vel[1] = 0; vel[2] = 0;
  acc[0] = 0; acc[1] = 0; acc[2] = 0;
  mass = 1; radius = 1;
};
Particle::Particle(float x, float y, float z, float m, float r){
  pos[0] = x; pos[1] = y; pos[2] = z;
  vel[0] = 0; vel[1] = 0; vel[2] = 0;
  acc[0] = 0; acc[1] = 0; acc[2] = 0;
  mass = m; radius = r;
};

/* Setters*/
void Particle::setPosition(float x, float y, float z){
  pos[0] = x; pos[1] = y; pos[2] = z;
};
void Particle::setVelocity(float xv, float yv, float zv){
  vel[0] = xv; vel[1] = yv; vel[2] = zv;
};
void Particle::setAcceleration(float xa, float ya, float za){
  acc[0] = xa; acc[1] = ya; acc[2] = za;
};
void Particle::setMass(float m){
  mass = m;
}
void Particle::setRadius(float r){
  radius = r;
}

/* Getters */
float * Particle::getPosition(){
  return pos;
};
float * Particle::getVelocity(){
  return vel;
};
float * Particle::getAcceleration(){
  return acc;
};
float Particle::getMass(){
  return mass;
}
float Particle::getRadius(){
  return radius;
}

/* Function Defintions */
void Particle::step(float xa, float ya, float za, float timeStep){
  //Update Positions
  pos[0] += vel[0]*timeStep;
  pos[1] += vel[1]*timeStep;
  pos[2] += vel[2]*timeStep;

  //Update Velocities
  vel[0] += acc[0]*timeStep;
  vel[1] += acc[1]*timeStep;
  vel[2] += acc[2]*timeStep;

  //Reset Accelarations
  acc[0] = xa;
  acc[1] = ya;
  acc[2] = za;
}

void Particle::applyBoundaries(float left, float right, float up, float down, float restitution){
    if (pos[0] < left) {
        pos[0] = std::abs(pos[0] - left) + left;
        vel[0] = -vel[0]*restitution;
    }
    if (pos[0] > right) {
        pos[0] = right - std::abs(pos[0] - right);
        vel[0] = -vel[0]*restitution;
    }
    if (pos[1] < down) {
        pos[1] = std::abs(pos[1] - down) + down;
        vel[1] = -vel[1]*restitution;
    }
    if (pos[1] > up) {
        pos[1] = up - std::abs(pos[1] - up);
        vel[1] = -vel[1]*restitution;
    }
}
