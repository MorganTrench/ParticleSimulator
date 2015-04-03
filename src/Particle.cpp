/**
  @file
  @author  Morgan Trench  <morgan.trench@gmail.com>

  @version 1.0

  @section LICENSE
  TODO

  @section DESCRIPTION
  Implementation of the methods within the Particle Class

 */

#include "Particle.h"
#include <cmath>
#include <iostream>

/* Constructors */
Particle::Particle(){
  pos[0] = 0; pos[1] = 0; pos[2] = 0;
  vel[0] = 0; vel[1] = 0; vel[2] = 0;
  acc[0] = 0; acc[1] = 0; acc[2] = 0;
  mass = 1; radius = 1; state = alive;
};
Particle::Particle(double x, double y, double z, double m, double r){
  pos[0] = x; pos[1] = y; pos[2] = z;
  vel[0] = 0; vel[1] = 0; vel[2] = 0;
  acc[0] = 0; acc[1] = 0; acc[2] = 0;
  mass = m; radius = r; state = alive;
};

/* Setters*/
void Particle::setPosition(double x, double y, double z){
  pos[0] = x; pos[1] = y; pos[2] = z;
};
void Particle::setVelocity(double xv, double yv, double zv){
  vel[0] = xv; vel[1] = yv; vel[2] = zv;
};
void Particle::setAcceleration(double xa, double ya, double za){
  acc[0] = xa; acc[1] = ya; acc[2] = za;
};
void Particle::setMass(double m){
  mass = m;
}
void Particle::setRadius(double r){
  radius = r;
}

/* Getters */
double * Particle::getPosition(){
  return pos;
};
double * Particle::getVelocity(){
  return vel;
};
double * Particle::getAcceleration(){
  return acc;
};
double Particle::getVelocitySquared(){
  return vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
};
double Particle::getMass(){
  return mass;
}
double Particle::getRadius(){
  return radius;
}
status Particle::getState(){
  return state;
}

/* Mutators */
void Particle::addForce(double xf, double yf, double zf){
  acc[0] += xf/mass; acc[1] += yf/mass; acc[2] += zf/mass;
}
void Particle::subtractForce(double xf, double yf, double zf){
  acc[0] -= xf*mass; acc[1] -= yf*mass; acc[2] -= zf*mass;
}
void Particle::setState(status s){
  state = s;
}
/* Function Defintions */
// Naive colour function, TODO improve
double * Particle::getColour(){
  rgb[2] = 1.0f;
  double vsq = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
  vsq = 1 - 10/vsq;
  rgb[0] = vsq; rgb[1]= vsq;
  return rgb;
}

// Updates the position and velocity of the particle, resets the acceleration to the arguments given.
void Particle::step(double timeStep){
  if(checkParticle("Pre-Step")){
    setState(dead);
    outputProperties();
  }

  // Update Positions
  pos[0] += vel[0]*timeStep;
  pos[1] += vel[1]*timeStep;
  pos[2] += vel[2]*timeStep;

  // Update Velocities
  vel[0] += acc[0]*timeStep;
  vel[1] += acc[1]*timeStep;
  vel[2] += acc[2]*timeStep;

  if(checkParticle("Post-Step")){
    setState(dead);
    outputProperties();
  }
}

// Alters the position and velocity of the particle to remain inside a 2d box
// TODO Update to a 3D Box
void Particle::applyBoundaries(double left, double right, double up, double down, double restitution){
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

void Particle::outputProperties(){
  std::cout << "Position: " <<  pos[0] << " " << pos[1] << " " << pos[2] << " " << std::endl;
  std::cout << "Velocity: " <<  vel[0] << " " << vel[1] << " " << vel[2] << " " << std::endl;
  std::cout << "Acceleration: " <<  acc[0] << " " << acc[1] << " " << acc[2] << " " << std::endl;
  std::cout << "RGB: " <<  rgb[0] << " " << rgb[1] << " " << rgb[2] << " " << std::endl;
  std::cout << "Radius: " << radius << " Mass: " << mass << " State: " << state << std::endl;
  std::cout << std::endl;
}

bool Particle::checkParticle(std::string message){
  if((pos[0] != pos[0]) || (pos[1] != pos[1]) || (pos[2] != pos[2])){
    std::cout << "Nan found - Position:" << message << std::endl;
    outputProperties();
    exit(-1);
    return true;
  }
  if((vel[0] != vel[0]) || (vel[1] != vel[1]) || (vel[2] != vel[2])){
    std::cout << "Nan found - Velocity: " << message << std::endl;
    outputProperties();
    exit(-1);
    return true;
  }
  if((acc[0] != acc[0]) || (acc[1] != acc[1]) || (acc[2] != acc[2])){
    std::cout << "Nan found - Acceleration: " << message << std::endl;
    outputProperties();
    exit(-1);
    return true;
  }
  return false;
}
