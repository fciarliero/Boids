//
// Created by fran on 01/05/20.
//
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "glm/glm.hpp"

#ifndef CINDER_BOIDS_H
#define CINDER_BOIDS_H
using namespace ci;
using namespace ci::app;

class Boid {
public:
    Boid(int x, int y, vec2 size);
    void show();
    void update();
    void windowsize(vec2 newsize);
    vec2 align(std::vector<Boid> boids);
    vec2 cohesion(std::vector<Boid> boids);
    vec2 separation(std::vector<Boid> boids);
    vec2 getPosition ()const;
    vec2 getVelocity()const;
    void applyBehaviour(std::vector<Boid> boids);
    bool operator ==(const Boid &boid2);
    bool operator !=(const Boid &boid2);
    void addObstacle();
    std::vector<vec2> getObstacles()const;
private:
    void keepBounded();
    vec2 _position;
    int _width;
    int _height;
    vec2 _velocity;
    vec2 _acceleration;
    float _max_force;
    float _max_speed;
    int _perception;
    std::vector<vec2> _obstacles;

};


#endif //CINDER_BOIDS_H
