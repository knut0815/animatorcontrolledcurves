/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H

#include <glm/glm.hpp>
#include <GL/glu.h>

class CameraModel
{
public:
    CameraModel();
    void draw();
    void setAlpha(float val);

private:
    float alpha;
};

#endif // CAMERAMODEL_H
