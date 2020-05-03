//
// Created by fran on 01/05/20.
//

#include "Boid.h"
Boid::Boid(int x, int y, vec2 size){
    _position.x = x;
    _position.y = y;
    _width = size.x;
    _height = size.y;
    _velocity = vec2(rand() % 5 - 2.5, rand() % 5 -2.5);
    _acceleration = vec2(0,0); //(rand() %2 *-1)* 0.01,(rand() %2 *-1)*0.01);//(double) rand() / (RAND_MAX)*10 , ((double) rand() / (RAND_MAX)*10) );
    _max_force = 0.3;
    _max_speed = 7;
    _perception = 50;
}
void drawPaperPlanes(const vec2 posicion, const vec2 velocidad){
    vec2 vert_a = velocidad;
    vert_a = glm::normalize(vert_a);
    vert_a.operator*=(4);
    vec2 vert_b = vec2(vert_a.x,-vert_a.y);
    vec2 vert_c = vec2(-vert_a.x, vert_a.y);
    vert_a.operator*=(4);
    vert_a.operator+=(posicion);
    vert_b.operator+=(posicion);
    vert_c.operator+=(posicion);
    /*gl::drawLine( vert_a,vert_b);
    gl::drawLine(vert_a, vert_c);
    gl::drawLine(vert_c,vert_b);
    //gl::drawLine(vert_b,posicion);*/
    //gl::drawLine(vert_a,posicion);
    gl::drawSolidTriangle(vert_a,vert_b,vert_c);
}
void Boid::show() {
    //gl::drawSolidCircle(_position,2,5);
    drawPaperPlanes(_position, _velocity);
}

void Boid::update() {
    _position.operator+=(_velocity);
    _velocity.operator+=(_acceleration);
    this->keepBounded();
    if (glm::length(_velocity) > _max_speed){
        _velocity = glm::normalize(_velocity).operator*=(_max_speed);
        //velocidad.operator*=(_max_speed);
        //velocidad.operator/=(velocidad.operator*=(_max_speed));
    }
    _acceleration.operator*=(0);
}

void Boid::keepBounded() {
    /*if (_position.x >= _width or _position.x <= 0) {
        velocidad.x = -velocidad.x;
    }
    if (_position.y >= _height or _position.y <= 0) {
        velocidad.y = -velocidad.y;
    }
    */
    if (_position.x > _width){
        _position.x = 0;
    }else if (_position.x < 0){
        _position.x = _width;
    }
    if (_position.y > _height){
        _position.y = 0;
    } else if (_position.y < 0){
        _position.y = _height;
    }


}

void Boid::windowsize(vec2 newsize) {
    _width = newsize.x;
    _height = newsize.y;
}

vec2 Boid::align(std::vector<Boid> boids) {
    vec2 steering = vec2(0,0);
    int total = 0;
    vec2 avg = vec2(0,0);
    float perceptionRadius = _perception / 2;
    for (auto &boid : boids) {
        vec2 d = this->getPosition();
        vec2 diff = d.operator-=(boid.getPosition());
        //d = glm::length(diff);
        if(this->operator!=(boid) and glm::length(diff) <= perceptionRadius) {
            steering.operator+=(boid.getVelocity());
            total++;
        }
    }
    if (total >0) {
        steering.operator/=(total);
        steering = glm::normalize(steering).operator*=(_max_speed);
        steering.operator-=(this->getVelocity());
        if (glm::length(steering) > _max_force) {
            steering = glm::normalize(steering).operator*=(_max_force);
        }
    }
    return steering;

}


vec2 Boid::getPosition()const {
    return vec2(_position);
}

vec2 Boid::getVelocity()const {
    return vec2(_velocity);
}


void Boid::applyBehaviour(std::vector<Boid> boids) {
    vec2 alignment = this->align(boids);
    vec2 cohesion = this->cohesion(boids);
    vec2 separation = this->separation(boids);
    separation.operator*=(1.8);
    alignment.operator*=(1.5);
    this->_acceleration.operator+=(separation);
    this->_acceleration.operator+=(cohesion);
    this->_acceleration.operator+=(alignment);

}

vec2 Boid::cohesion(std::vector<Boid> boids) {
    int total = 0;
    vec2 steering = vec2(0,0);
    float perceptionRadius = _perception;
    //como todos los boids pesan lo mismo, el CenterofMass es el promedio de las posiciones
    //F = m*A → F = A
    for (Boid &boid : boids) {
        vec2 d = this->getPosition();
        vec2 diff = d.operator-=(boid.getPosition());
        //d = glm::length(diff);
        if(this->operator!=(boid) and glm::length(diff) <= perceptionRadius) {
                total++;
                steering.operator+=(boid.getPosition());
            }
        }

    if (total > 0){
        steering.operator/=(total);
        steering = steering.operator-=(_position);
        steering = glm::normalize(steering).operator*=(_max_speed);
        steering = steering.operator-=(_velocity);
        if (glm::length(steering)>_max_force){
            steering = glm::normalize(steering).operator*=(_max_force);
        }
    }

    return glm::vec2(steering);
}

bool Boid::operator==(const Boid &boid2) {
    vec2 pos1 = this->getPosition();
    vec2 pos2 = boid2.getPosition();
    bool same_pos = pos1.x == pos2.x and pos1.y == pos2.y;
    vec2 vel1 = this->getVelocity();
    vec2 vel2 = boid2.getVelocity();
    bool same_vel = vel1.x == vel2.x and vel1.y == vel2.y;
    return same_pos and same_vel;
}

bool Boid::operator!=(const Boid &boid2) {
    return (not this->operator==( boid2));
}

vec2 Boid::separation(std::vector<Boid> boids) {
    int percepRadius = _perception / 2 -1;
    vec2 steering=vec2(0,0);
    int total = 0;
    //avoid other shit
    //walls
    int x = this->getPosition().x;
    int y = this->getPosition().y;

    Boid leftWall = Boid(0, y, vec2(_width,_height));
    Boid rightWall = Boid(this->_width -x, y, vec2(_width,_height));
    Boid floor = Boid(x, 0, vec2(_width,_height));
    Boid ceiling = Boid(x, this->_height - y, vec2(_width,_height));
    std::vector<Boid> shit2avoid {leftWall, rightWall, floor, ceiling};
    shit2avoid.insert( shit2avoid.end(), boids.begin(), boids.end() );

    for(Boid &boid : boids) {
        float d = glm::distance(this->getPosition(),boid.getPosition());
        if (this->operator!=(boid) and d < percepRadius){
            vec2 diff = this->getPosition();
            diff.operator-=(boid.getPosition());
            diff.operator/=(d*d);
            steering.operator+=(diff);
            total++;
        }
    }

    if (total >0){
        steering.operator/=(total);
        steering = glm::normalize(steering).operator*=(_max_speed);
        steering.operator-=(this->getVelocity());
        if (glm::length(steering)>_max_force){
            steering = glm::normalize(steering).operator*=(_max_force);
        }
    }
    return steering;
}

void Boid::addObstacle() {


}

std::vector<vec2> Boid::getObstacles() const {
    return _obstacles;
}

