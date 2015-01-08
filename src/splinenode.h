/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef SPLINENODE_H
#define SPLINENODE_H

#include <glm/glm.hpp>
#include "utils.h"
#include "arcball.h"
#include "translationnode.h"
#include "camera3d.h"
#include "quaternion.h"

class SplineNode
{
public:
    SplineNode();
    SplineNode(glm::vec3 pos, camera3d *camera);

    void activate();
    void deactivate();
    void setScreenDimensions(float scrWidth, float scrHeight);
    glm::vec3 getPosition();
    glm::mat4 getRotationMatrix();
    Quaternion getRotationQuaternion();
    glm::mat4 getTranslationMatrix();
    void update(float mx, float my);
    void draw();

    void onMousePress(float mx, float my);
    void onMouseRelease(float mx, float my);
    void onMouseMove(float mx, float my);

    bool isActive;

private:

    TranslationNode tnode;
    ArcBall arcball;
    camera3d *camera;
    float radius;
};

#endif // SPLINENODE_H
