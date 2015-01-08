/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <glm/glm.hpp>
#include <GL/glu.h>

class SpaceShip
{
public:
    SpaceShip();
    void draw();
    void setAlpha(float val);

private:
    float alpha;
};

#endif // SPACESHIP_H
