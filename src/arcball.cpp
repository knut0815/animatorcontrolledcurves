/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "arcball.h"
#include <QDebug>

ArcBall::ArcBall() {
    scrWidth = scrHeight = 1;
    radius = fmin(scrWidth, scrHeight);
}

// Width and height of screen in pixels
// camera3d used for projecting mouse position into scene
ArcBall::ArcBall(float screenWidth, float screenHeight, camera3d *cam)
{
    scrWidth = screenWidth; scrHeight = screenHeight;
    radius = fmin(scrWidth, scrHeight);
    rotMatrix = glm::mat4();
    isFocused = false;          // is object selected by mouse
    isActive = false;           // is object accepting input
    camera = cam;
    rotSpeed = 1.5;
    isAxisSelected = false;
    axisSelectionRadius = 0.25;
    isDrawPositionInitialized = false;
}

// position of the centre of the arcball in world coordinates
void ArcBall::setDrawPosition(glm::vec3 pos) {
    drawPosition = pos;
    isDrawPositionInitialized = true;
}

void ArcBall::setDrawRadius(float r) {
    drawRadius = r;
}

void ArcBall::activate() {
    isActive = true;
}

void ArcBall::deactivate() {
    isActive = false;
    isFocused = false;
}

// Width and height in pixels.
void ArcBall::setScreenDimensions(float width, float height) {
    scrWidth = width; scrHeight = height;
    radius = fmin(scrWidth, scrHeight);
}

// converts mouse coordinates to arcball position vector (centre to point on
// sphere surface)
glm::vec3 ArcBall::screenPositionToVector(float mx, float my) {
    float cx, cy;

    // find centre of arcball in screen coordinates
    if (isDrawPositionInitialized) {
        camera->set();
        glm::vec3 scrPos = camera->worldToScreenCoordinates(drawPosition);
        camera->unset();
        cx = scrPos.x;
        cy = scrPos.y;
    } else {
        cx = 0.5 * scrWidth;
        cy = 0.5 * scrHeight;
    }

    // convert screen coordinates to [-1,1]
    float x = 2*(mx - cx) / radius;
    float y = -2*(my - cy) / radius;

    // find z coordinate on surface of sphere
    float lensqr = x*x + y*y;
    if (lensqr > 1.0) {
        return glm::normalize(glm::vec3(x, y, 0.0));
    }

    return glm::vec3(x, y, sqrt(1.0 - lensqr));
}

// Givent a line with origin o and direction dir, find the closest point
// of a set of points *points and store closest distance in *dist
int ArcBall::getClosestPoint(std::vector<glm::vec3> *points,
                             glm::vec3 o, glm::vec3 dir, float *dist) {

    float minDist = 1000000000;
    int minIdx = -1;

    for (int i=0; i<(int)points->size(); i++) {
        glm::vec3 p = (*points)[i];
        float distance = utils_pointToLineDistance(p, o, dir);
        if (distance < minDist) {
            minDist = distance;
            minIdx = i;
        }
    }

    *dist = minDist;
    return minIdx;
}

// Find which rotation axis is selected/hovered over by mouse
void ArcBall::updateSelectedAxis(float mx, float my) {
    if (isFocused) { return; }

    glm::vec3 pos = camera->getPosition();
    glm::vec3 ray = camera->castRayFromScreen((double)mx, (double)my);

    // axis arcs are the set of points on the half circle facing screen
    std::vector<glm::vec3> points[3];
    utils_getHalfCirclePoints(drawPosition, drawRadius,
                              glm::vec3(1.0, 0.0, 0.0), -camera->direction,
                              &points[0]);
    utils_getHalfCirclePoints(drawPosition, drawRadius,
                              glm::vec3(0.0, 1.0, 0.0), -camera->direction,
                              &points[1]);
    utils_getHalfCirclePoints(drawPosition, drawRadius,
                              glm::normalize(glm::vec3(0.0, 0.0001, 1.0)),
                              -camera->direction,
                              &points[2]);

    // find closest position of mouse to axis arc
    int closestAxisIndex = -1;
    int closestIndex;
    float minDist = 10000000000;
    for (int i = 0; i<3; i++) {
        std::vector<glm::vec3> pointSet = points[i];

        float dist;
        int idx = getClosestPoint(&pointSet, pos, ray, &dist);
        if (dist < minDist) {
            minDist = dist;
            closestAxisIndex = i;
            closestIndex = idx;
        }
    }

    // determine whether an axis is selected
    isAxisSelected = false;
    if (minDist < axisSelectionRadius) {
        isAxisSelected = true;
        switch (closestAxisIndex)
        {
          case 0:
             selectedAxis = glm::vec3(1.0, 0.0, 0.0);
             break;
          case 1:
             selectedAxis = glm::vec3(0.0, 1.0, 0.0);
             break;
          case 2:
             selectedAxis = glm::vec3(0.0, 0.0, 1.0);
             break;
        }

        // find tangent line of the arc at closest point projected onto the screen
        // so that angle of acrball rotation can be calculated by snapping
        // mouse positions to the line
        glm::vec3 p1 = points[closestAxisIndex][closestIndex];
        glm::vec3 p2;
        if (closestIndex == 0) {
            p2 = points[closestAxisIndex][closestIndex+1];
        } else {
            p2 = points[closestAxisIndex][closestIndex-1];
        }
        p1 = camera->worldToScreenCoordinates(p1);
        p2 = camera->worldToScreenCoordinates(p2);
        screenAxisOrigin = p1;
        screenAxisDirection = glm::normalize(p2-p1);
    }
}

// initialize arcball rotation
void ArcBall::onMousePress(float mx, float my) {
    if (!isActive) { return; }

    isFocused = true;

    if (isAxisSelected) {
        glm::vec3 p = utils_closestPointOnLineFromPoint(
                                glm::vec3(mx, my, 0.0),
                                screenAxisOrigin, screenAxisDirection);
        mx = p.x;
        my = p.y;
    }

    lastVector = currVector = screenPositionToVector(mx, my);
}

// halt arcball rotation
void ArcBall::onMouseRelease(float mx, float my) {
    if (!isActive) { return; }

    lastVector = currVector = screenPositionToVector(mx, my);
    isFocused = false;
}

// update arcball rotation
void ArcBall::onMouseMove(float mx, float my) {
    if (!isFocused) {
        return;
    }

    // snap mouse position to rotation line if a specific rotation axis
    // is selected
    if (isAxisSelected) {
        glm::vec3 p = utils_closestPointOnLineFromPoint(
                                glm::vec3(mx, my, 0.0),
                                screenAxisOrigin, screenAxisDirection);
        mx = p.x;
        my = p.y;
    }

    currVector = screenPositionToVector(mx, my);
    if (currVector == lastVector) {
        return;
    }

    // find angle between vectors
    float maxdot = 0.99999;
    float dot =  fmin(maxdot, glm::dot(lastVector, currVector));
    float angle = acos(dot) * rotSpeed;

    Quaternion q;
    if (isAxisSelected) {
        // find rotation direction
        // bug: sometimes rotation is in opposite direction
        float componentValue;
        glm::vec3 cross = glm::cross(lastVector, currVector);
        if (selectedAxis.x == 1.0) {
            componentValue = cross.x;
        } else if (selectedAxis.y == 1.0) {
            componentValue = cross.y;
        } else if (selectedAxis.z == 1.0) {
            componentValue = cross.z;
        }
        float sign = componentValue > 0 ? 1.0 : -1.0;

        glm::vec3 crossVect = sign * selectedAxis;
        q = Quaternion(crossVect, 2.0*angle);  // rotate faster when axis selected
    } else {
        // find axis of rotation from perspective of camera view to build quaternion
        // and upate total rotation
        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::vec3 crossVect = glm::normalize(glm::cross(lastVector, currVector));
        glm::vec4 qVect4 = glm::vec4(crossVect, 1.0) * glm::inverse(viewMatrix);
        q = Quaternion(glm::vec3(qVect4), angle);
    }

    // update total rotation
    rotMatrix = q.getRotationMatrix() * rotMatrix;
    lastVector = currVector;
}

float ArcBall::sign(float x) {
    return (x >= 0.0f) ? +1.0f : -1.0f;
}

// total rotation matrix to quaternion
Quaternion ArcBall::getRotationQuaternion() {
    // following section of code from:
    // http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche52.html
    glm::mat4 m = glm::transpose(getRotationMatrix());
    float r11, r12, r13;
    float r21, r22, r23;
    float r31, r32, r33;

    r11 = m[0].x; r12 = m[0].y; r13 = m[0].z;
    r21 = m[1].x; r22 = m[1].y; r23 = m[1].z;
    r31 = m[2].x; r32 = m[2].y; r33 = m[2].z;

    float q0 = ( r11 + r22 + r33 + 1.0f) / 4.0f;
    float q1 = ( r11 - r22 - r33 + 1.0f) / 4.0f;
    float q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
    float q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
    if(q0 < 0.0f) q0 = 0.0f;
    if(q1 < 0.0f) q1 = 0.0f;
    if(q2 < 0.0f) q2 = 0.0f;
    if(q3 < 0.0f) q3 = 0.0f;
    q0 = sqrt(q0);
    q1 = sqrt(q1);
    q2 = sqrt(q2);
    q3 = sqrt(q3);
    if(q0 >= q1 && q0 >= q2 && q0 >= q3) {
        q0 *= +1.0f;
        q1 *= sign(r32 - r23);
        q2 *= sign(r13 - r31);
        q3 *= sign(r21 - r12);
    } else if(q1 >= q0 && q1 >= q2 && q1 >= q3) {
        q0 *= sign(r32 - r23);
        q1 *= +1.0f;
        q2 *= sign(r21 + r12);
        q3 *= sign(r13 + r31);
    } else if(q2 >= q0 && q2 >= q1 && q2 >= q3) {
        q0 *= sign(r13 - r31);
        q1 *= sign(r21 + r12);
        q2 *= +1.0f;
        q3 *= sign(r32 + r23);
    } else if(q3 >= q0 && q3 >= q1 && q3 >= q2) {
        q0 *= sign(r21 - r12);
        q1 *= sign(r31 + r13);
        q2 *= sign(r32 + r23);
        q3 *= +1.0f;
    } else {
        printf("coding error\n");
    }
    float r = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 /= r;
    q1 /= r;
    q2 /= r;
    q3 /= r;

    Quaternion q = Quaternion();
    q.set(q1, q2, q3, q0);
    return q;
}

glm::mat4 ArcBall::getRotationMatrix() {
    return rotMatrix;
}

void ArcBall::update(float mx, float my) {
    if (!isActive) { return; }

    updateSelectedAxis(mx, my);
}

void ArcBall::draw() {
    if (!isActive) { return; }

    // draw surrounding circle
    glColor4f(0.0, 0.0, 0.0, 0.5);
    glLineWidth(2.0);
    if (isActive) {
        utils_drawCircle(drawPosition, drawRadius, camera->direction);
    }

    // draw rotation axis'
    float thinLine = 2.0;
    float thickLine = 5.0;
    if (isAxisSelected && selectedAxis.x == 1.0) {
        glLineWidth(thickLine);
    } else {
        glLineWidth(thinLine);
    }
    glColor3f(1.0, 0.0, 0.0);
    utils_drawHalfCircle(drawPosition, drawRadius,
                         glm::vec3(1.0, 0.0, 0.0), -camera->direction);

    if (isAxisSelected && selectedAxis.y == 1.0) {
        glLineWidth(thickLine);
    } else {
        glLineWidth(thinLine);
    }
    glColor3f(0.0, 1.0, 0.0);
    utils_drawHalfCircle(drawPosition, drawRadius,
                         glm::vec3(0.0, 1.0, 0.0), -camera->direction);

    if (isAxisSelected && selectedAxis.z == 1.0) {
        glLineWidth(thickLine);
    } else {
        glLineWidth(thinLine);
    }
    // bug: segfault when vector is exactly <0,0,1>?
    glColor3f(0.0, 0.0, 1.0);
    utils_drawHalfCircle(drawPosition, drawRadius,
                         glm::normalize(glm::vec3(0.0, 0.0001, 1.0))
                         , -camera->direction);
}










