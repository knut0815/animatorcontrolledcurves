/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "translationnode.h"
#include <QDebug>

// Translation node is an interface for controlling the position of a point
// in a 3d world
// uses x, y, z axis align handles to move node
TranslationNode::TranslationNode() {
    xHovered = yHovered = zHovered = false;
    xSelected = ySelected = zSelected = false;
    isActive = false;
}

// position  pos in world coordinates
// camera3d object *cam used for mouse projection
TranslationNode::TranslationNode(glm::vec3 pos, camera3d* cam) {
    position = pos;
    camera = cam;
    axisLength = 2.0;
    handleSize = 10.0;
    selectRadius = 0.5;

    xHovered = yHovered = zHovered = false;      // axis handle hovered by mouse
    xSelected = ySelected = zSelected = false;   // axis handle selction
    isActive = false;                            // is accepting input
}

glm::vec3 TranslationNode::getPosition() {
    return position;
}

// is a handle selected
bool TranslationNode::isSelected() {
    return xSelected || ySelected || zSelected;
}

// translation matrix in row order
glm::mat4 TranslationNode::getTranslationMatrix() {
    return glm::transpose(glm::mat4(1.0, 0.0, 0.0, position.x,
                                    0.0, 1.0, 0.0, position.y,
                                    0.0, 0.0, 1.0, position.z,
                                    0.0, 0.0, 0.0, 1.0));
}

void TranslationNode::activate() {
    isActive = true;
}

void TranslationNode::deactivate() {
    isActive = false;
    xHovered = yHovered = zHovered = false;
    xSelected = ySelected = zSelected = false;
    isActive = false;
}

void TranslationNode::onMouseMove(float mx, float my) {
    if (!isActive) { return; }
    if (!xSelected && !ySelected && !zSelected) { return; }

    // find closest point on axis to where mouse position is
    // axis line
    glm::vec3 p1 = position;
    glm::vec3 dir1;
    if (xSelected) { dir1 = glm::vec3(1.0, 0.0, 0.0); }
    if (ySelected) { dir1 = glm::vec3(0.0, 1.0, 0.0); }
    if (zSelected) { dir1 = glm::vec3(0.0, 0.0, 1.0); }

    // mouse ray line
    glm::vec3 p2 = camera->getPosition();
    camera->set();
    glm::vec3 dir2 = camera->castRayFromScreen((double)mx, (double)my);
    camera->unset();

    glm::vec3 p21 = p2 - p1;
    glm::vec3 cross = glm::cross(dir2, dir1);
    float dot = glm::dot(cross, cross);
    glm::vec3 r = glm::cross(p21, cross) / dot;

    float t = glm::dot(r, dir2);
    glm::vec3 p = p1 + t*dir1;

    // translate position
    glm::vec3 trans;
    if (xSelected) {
        trans = glm::vec3(p.x - position.x - axisLength, 0.0, 0.0);
    }
    if (ySelected) {
        trans = glm::vec3(0.0, p.y - position.y - axisLength, 0.0);
    }
    if (zSelected) {
        trans = glm::vec3(0.0, 0.0, p.z - position.z - axisLength);
    }

    position += trans;
}

void TranslationNode::onMousePress(float mx, float my) {
    if (!isActive) { return; }

    // check if mouse if over a handle for selection
    camera->set();
    glm::vec3 xHandle = position + glm::vec3(axisLength, 0.0, 0.0);
    glm::vec3 yHandle = position + glm::vec3(0.0, axisLength, 0.0);
    glm::vec3 zHandle = position + glm::vec3(0.0, 0.0, axisLength);

    if (isMouseOnPoint(mx, my, xHandle, selectRadius)) {
        xSelected = true;
    } else if (isMouseOnPoint(mx, my, yHandle, selectRadius)) {
        ySelected = true;
    } else if (isMouseOnPoint(mx, my, zHandle, selectRadius)) {
        zSelected = true;
    }
    camera->unset();

}

void TranslationNode::onMouseRelease(float mx, float my) {
    (void)mx;
    (void)my;
    xSelected = ySelected = zSelected = false;
}

// is mouse at (mx, my) hovering over point p within radius
bool TranslationNode::isMouseOnPoint(float mx, float my, glm::vec3 p, float radius) {
    glm::vec3 pos = camera->getPosition();
    glm::vec3 dir = camera->castRayFromScreen((double)mx, (double)my);
    float dist = utils_pointToLineDistance(p, pos, dir);

    return dist < radius ? true : false;
}

void TranslationNode::update(float mx, float my) {
    if (!isActive) { return; }

    mousex = mx;
    mousey = my;

    // find all handles that mouse is hovering over
    std::vector<glm::vec3> hoverPoints;
    glm::vec3 xHandle = position + glm::vec3(axisLength, 0.0, 0.0);
    glm::vec3 yHandle = position + glm::vec3(0.0, axisLength, 0.0);
    glm::vec3 zHandle = position + glm::vec3(0.0, 0.0, axisLength);

    xHovered = yHovered = zHovered = false;
    if (isMouseOnPoint(mx, my, xHandle, selectRadius)) {
        hoverPoints.push_back(xHandle);
    }
    if (isMouseOnPoint(mx, my, yHandle, selectRadius)) {
        hoverPoints.push_back(yHandle);
    }
    if (isMouseOnPoint(mx, my, zHandle, selectRadius)) {
        hoverPoints.push_back(zHandle);
    }

    // find closest axis handle
    float min = 1000000000;
    for (uint i=0; i<hoverPoints.size(); i++) {
        glm::vec3 p = hoverPoints[i];
        float dist = glm::distance(camera->getPosition(), p);
        if (dist < min) {
            min = dist;
            xHovered = p == xHandle ? true : false;
            yHovered = p == yHandle ? true : false;
            zHovered = p == zHandle ? true : false;
        }
    }
}

void TranslationNode::draw() {

    // draw axis'
    float x = position.x;
    float y = position.y;
    float z = position.z;

    glPointSize(10.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(x, y, z);
    glEnd();

    if (isActive) {
        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(x, y, z);
        glVertex3f(x + axisLength, y, z);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(x, y, z);
        glVertex3f(x, y + axisLength, z);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + axisLength);
        glEnd();

        // draw handles
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(x + axisLength, y, z);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(x, y + axisLength, z);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(x, y, z + axisLength);
        glEnd();
    }

    // draw circle on hover or selection
    float r = 0.3;
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    if (xHovered && !xSelected) {
        utils_drawCircle(glm::vec3(x + axisLength, y, z), r, camera->direction);
    }
    if (yHovered && !ySelected) {
        utils_drawCircle(glm::vec3(x, y + axisLength, z), r, camera->direction);
    }
    if (zHovered && !zSelected) {
        utils_drawCircle(glm::vec3(x, y, z + axisLength), r, camera->direction);
    }

    glLineWidth(3.0);
    float len = 10.0;
    if (xSelected) {
        glColor3f(1.0, 0.0, 0.0);
        utils_drawCircle(glm::vec3(x + axisLength, y, z), r, camera->direction);

        glLineWidth(1.0);
        glColor4f(0.0, 0.0, 0.0, 0.2);
        glBegin(GL_LINES);
        glVertex3f(x + axisLength, y, z);
        glVertex3f(x + axisLength + len, y, z);
        glVertex3f(x, y, z);
        glVertex3f(x - len, y, z);
        glEnd();
    }
    if (ySelected) {
        glColor3f(0.0, 1.0, 0.0);
        utils_drawCircle(glm::vec3(x, y + axisLength, z), r, camera->direction);

        glLineWidth(1.0);
        glColor4f(0.0, 0.0, 0.0, 0.2);
        glBegin(GL_LINES);
        glVertex3f(x, y + axisLength, z);
        glVertex3f(x, y + axisLength + len, z);
        glVertex3f(x, y, z);
        glVertex3f(x, y - len, z);
        glEnd();
    }
    if (zSelected) {
        glColor3f(0.0, 0.0, 1.0);
        utils_drawCircle(glm::vec3(x, y, z + axisLength), r, camera->direction);

        glLineWidth(1.0);
        glColor4f(0.0, 0.0, 0.0, 0.2);
        glBegin(GL_LINES);
        glVertex3f(x, y, z + axisLength);
        glVertex3f(x, y, z + axisLength + len);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z - len);
        glEnd();
    }

    if (xSelected || ySelected || zSelected) {
        glLineWidth(2.0);
        glColor4f(0.0, 0.0, 0.0, 0.3);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, 0.0, z);
        glEnd();

        glPointSize(10.0);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_POINTS);
        glVertex3f(x, 0.0, z);
        glEnd();
    }
}










