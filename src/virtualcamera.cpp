/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "virtualcamera.h"
#include <QDebug>

VirtualCamera::VirtualCamera()
{
}

// Virtual camera in 3d space
// Displays where an object would be on screen from perspective of camera
// screenDistance - draw distance from camera to virtual screen
// screenScale - draw scale of virtual screen
// camera3d object *cam used for retrieving perspective settings
VirtualCamera::VirtualCamera(float screenDistance, float screenScale, camera3d *cam)
{
    scrDistance = screenDistance;
    scrScale = screenScale;
    camera = cam;
    objectPosition = glm::vec3(0.0, 0.0, 0.0);

    initializeOrientation();
}

void VirtualCamera::initializeOrientation() {
    position = glm::vec3(0.0, 0.0, 0.0);
    direction = glm::vec3(1.0, 0.0, 0.0);
    up = glm::vec3(0.0, 1.0, 0.0);
}

// rotMatrix in row order
void VirtualCamera::setRotation(glm::mat4 rotMatrix) {
    initializeOrientation();
    glm::vec4 dir4 = rotMatrix * glm::vec4(direction.x,
                                           direction.y,
                                           direction.z, 1.0);
    glm::vec4 up4 = rotMatrix * glm::vec4(up.x, up.y, up.z, 1.0);

    direction = glm::vec3(dir4.x, dir4.y, dir4.z);
    up = glm::vec3(up4.x, up4.y, up4.z);
}

void VirtualCamera::setPosition(glm::vec3 pos) {
    position = pos;
}

// World position of object to be projected onto virtual screen
void VirtualCamera::setTrackingObjectPosition(glm::vec3 pos) {
    objectPosition = pos;
}

void VirtualCamera::draw() {
    glm::vec3 p = position;
    glm::vec3 dir = direction;

    // calculate screen dimensions
    float fov = camera->getFieldOfView();
    float aspect = camera->getAspectRatio();
    float height = 2 * scrDistance * tan(0.5 * fov);
    float width = aspect * height;

    glm::vec3 origin = p + scrDistance * dir;
    glm::vec3 right = glm::normalize(glm::cross(dir, up));
    glm::vec3 topLeft = origin + 0.5f*height*up - 0.5f*width*right;
    glm::vec3 topRight = origin + 0.5f*height*up + 0.5f*width*right;
    glm::vec3 bottomLeft = origin - 0.5f*height*up - 0.5f*width*right;
    glm::vec3 bottomRight = origin - 0.5f*height*up + 0.5f*width*right;

    // find where tracking object intersects screen plane
    glm::vec3 ldir = position - objectPosition;
    glm::vec3 scrpos = utils_linePlaneIntersection(objectPosition, ldir,
                                                   origin, dir);

    glm::vec3 sp = scrpos;
    glm::vec3 p1 = topLeft;
    glm::vec3 p2 = topRight;
    glm::vec3 p3 = bottomRight;
    glm::vec3 p4 = bottomLeft;
    bool isInside = glm::dot(ldir, dir) < 0.0 &&
                    glm::dot(sp-p1, p2-p1) > 0.0 &&
                    glm::dot(sp-p2, p3-p2) > 0.0 &&
                    glm::dot(sp-p3, p4-p3) > 0.0 &&
                    glm::dot(sp-p4, p1-p4) > 0.0;

    // scale screen
    topLeft = (topLeft - origin) * scrScale + origin;
    topRight = (topRight - origin) * scrScale + origin;
    bottomRight = (bottomRight - origin) * scrScale + origin;
    bottomLeft = (bottomLeft - origin) * scrScale + origin;
    scrpos = (scrpos - origin) * scrScale + origin;

    // draw object position
    if (isInside) {
        glPointSize(10.0);
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        glVertex3f(scrpos.x, scrpos.y, scrpos.z);
        glEnd();

        glLineWidth(3.0);
        glColor3f(0.0, 1.0, 0.0);
        utils_drawCircle(scrpos, 0.25, direction);
    }

    // draw borders
    glLineWidth(2.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(topLeft.x, topLeft.y, topLeft.z);
    glVertex3f(topRight.x, topRight.y, topRight.z);

    glVertex3f(topRight.x, topRight.y, topRight.z);
    glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);

    glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
    glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);

    glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
    glVertex3f(topLeft.x, topLeft.y, topLeft.z);
    glEnd();

    // draw grid
    float w = (1.0/3.0)*width*scrScale;
    float h = (1.0/3.0)*height*scrScale;

    glColor4f(0.0, 0.0, 0.0, 0.3);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glm::vec3 v1 = topLeft + right * w;
    glm::vec3 v2 = bottomLeft + right * w;
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);

    v1 = topLeft + 2.0f * right * w;
    v2 = bottomLeft + 2.0f * right * w;
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);

    v1 = topLeft - up * h;
    v2 = topRight - up * h;
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);

    v1 = topLeft - 2.0f * up * h;
    v2 = topRight - 2.0f * up * h;
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);

    glEnd();

}











