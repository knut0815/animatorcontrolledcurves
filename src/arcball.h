/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef ARCBALL_H
#define ARCBALL_H

#include <cmath>
#include <glm/glm.hpp>
#include "quaternion.h"
#include "camera3d.h"
#include "utils.h"

class ArcBall
{
public:
    ArcBall();
    ArcBall(float screenWidth, float screenHeight, camera3d *camera);

    void activate();
    void deactivate();
    void setDrawPosition(glm::vec3 pos);
    void setDrawRadius(float r);
    void setScreenDimensions(float width, float height);
    void onMousePress(float mx, float my);
    void onMouseRelease(float mx, float my);
    void onMouseMove(float mx, float my);
    float sign(float x);
    Quaternion getRotationQuaternion();
    glm::mat4 getRotationMatrix();
    void update(float mx, float my);
    void draw();

private:
    float scrWidth, scrHeight;
    float radius;
    camera3d *camera;
    float rotSpeed;

    glm::vec3 screenPositionToVector(float mx, float my);

    glm::vec3 lastVector;
    glm::vec3 currVector;
    glm::mat4 rotMatrix;
    bool isFocused;
    bool isActive;

    glm::vec3 drawPosition;
    float drawRadius;
    bool isDrawPositionInitialized;

    void updateSelectedAxis(float mx, float my);
    int getClosestPoint(std::vector<glm::vec3> *points,
                              glm::vec3 o, glm::vec3 dir, float *dist);
    glm::vec3 selectedAxis;
    bool isAxisSelected;
    glm::vec3 screenAxisOrigin;
    glm::vec3 screenAxisDirection;
    float axisSelectionRadius;
};

#endif // ARCBALL_H
