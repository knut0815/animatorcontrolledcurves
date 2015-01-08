/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef VIRTUALCAMERA_H
#define VIRTUALCAMERA_H

#include <glm/glm.hpp>
#include "camera3d.h"
#include "utils.h"

class VirtualCamera
{
public:
    VirtualCamera();
    VirtualCamera(float scrDist, float scrScale, camera3d *camera);

    void draw();
    void setRotation(glm::mat4 rotMatrix);
    void setPosition(glm::vec3 pos);
    void setTrackingObjectPosition(glm::vec3 pos);

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

private:
    void initializeOrientation();

    float scrDistance;
    float scrScale;
    camera3d *camera;
    glm::vec3 objectPosition;

};

#endif // VIRTUALCAMERA_H
