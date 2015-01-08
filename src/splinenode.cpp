/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "splinenode.h"
#include <QDebug>

SplineNode::SplineNode() {

}

// interface for editing position and rotation of a control point
// pos is position in world coordinates
// camera3d object *cam is used by arcball and translation node objects
SplineNode::SplineNode(glm::vec3 pos, camera3d *cam)
{
    camera = cam;
    isActive = false;    // is accepting input
    radius = 2.5;        // draw radius

    arcball = ArcBall(camera->screenWidth, camera->screenHeight, camera);
    tnode = TranslationNode(pos, camera);
    arcball.setDrawRadius(radius);
    arcball.setDrawPosition(pos);
}

void SplineNode::setScreenDimensions(float scrWidth, float scrHeight) {
    arcball.setScreenDimensions(scrWidth, scrHeight);
}

void SplineNode::activate() {
    isActive = true;
    arcball.activate();
    tnode.activate();
}

void SplineNode::deactivate() {
    isActive = false;
    arcball.deactivate();
    tnode.deactivate();
}

glm::mat4 SplineNode::getRotationMatrix() {
    return arcball.getRotationMatrix();
}

Quaternion  SplineNode::getRotationQuaternion() {
    return arcball.getRotationQuaternion();
}

glm::mat4 SplineNode::getTranslationMatrix() {
    return tnode.getTranslationMatrix();
}

glm::vec3 SplineNode::getPosition() {
    return tnode.getPosition();
}

void SplineNode::update(float mx, float my) {
    if (!isActive) { return; }

    tnode.update(mx, my);
    arcball.update(mx, my);
}


void SplineNode::draw() {
    glm::vec3 p = tnode.getPosition();
    tnode.draw();
    arcball.setDrawPosition(p);
    arcball.draw();
}

void SplineNode::onMouseMove(float mx, float my) {
    if (!isActive) { return; }
    tnode.onMouseMove(mx, my);

    if (!tnode.isSelected()) {
        arcball.onMouseMove(mx, my);
    }
}

void SplineNode::onMousePress(float mx, float my) {
    if (!isActive) { return; }
    tnode.onMousePress(mx, my);

    if (!tnode.isSelected()) {
        arcball.onMousePress(mx, my);
    }
}

void SplineNode::onMouseRelease(float mx, float my) {
    if (!isActive) { return; }
    tnode.onMouseRelease(mx, my);

    if (!tnode.isSelected()) {
        arcball.onMouseRelease(mx, my);
    }
}







