class Particle{

    float pos[3], vel[3], acc[3];
    float radius, mass;

  public:

    /* Constructors */
    Particle();
    Particle(float x, float y, float z, float m, float r);

    /* Setters */
    void setPosition(float x, float y, float z);
    void setVelocity(float xv, float yv, float zv);
    void setAcceleration(float xa, float ya, float za);
    void setMass(float m); void setRadius(float m);

    /* Getters */
    float * getPosition();
    float * getVelocity();
    float * getAcceleration();
    float getMass(); float getRadius();

    /* Function Definitions */

    /* Update Position, Velocitiy and reset acceleration to the passed in values (eg gravity) */
    void step(float xa, float ya, float za, float timeStep);
    void applyBoundaries(float left, float right, float up, float down, float restitution);

};
