/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "splinenodeset.h"
#include <QDebug>

SplineNodeSet::SplineNodeSet() {

}

// A set of spline nodes
SplineNodeSet::SplineNodeSet(camera3d *cam)
{
    camera = cam;
    selectRadius = 0.5;
    isHovering = false;           // is mouse hovering over a node
    isNodeSelected = false;       // has a mouse clicked on a node

}

std::vector<SplineNode> SplineNodeSet::getNodes() {
    return nodes;
}

// add next node to set at world position pos
void SplineNodeSet::addNode(glm::vec3 pos) {
    SplineNode n = SplineNode(pos, camera);
    nodes.push_back(n);
}

void SplineNodeSet::setScreenDimensions(float scrWidth, float scrHeight) {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].setScreenDimensions(scrWidth, scrHeight);
    }
}

// Set is selected if any splinenodes in set are selected
bool SplineNodeSet::isSelected() {
    bool r = false;
    for (int i=0; i<(int)nodes.size(); i++) {
        r = r || nodes[i].isActive;
    }
    return r;
}

void SplineNodeSet::deactivateNodes() {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].deactivate();
    }

    isNodeSelected = false;
}

void SplineNodeSet::onMousePress(float mx, float my) {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].onMousePress(mx, my);
    }

    // switch selection from one node to the other
    if (isHovering && (!isNodeSelected || (selectedNode != hoverNode))) {
        selectedNode = hoverNode;
        deactivateNodes();
        selectedNode->activate();
        isNodeSelected = true;
    }
}

void SplineNodeSet::onMouseRelease(float mx, float my) {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].onMouseRelease(mx, my);
    }
}

void SplineNodeSet::onMouseMove(float mx, float my) {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].onMouseMove(mx, my);
    }
}

// checks whether mouse position (mx, my) is over a point within radius
bool SplineNodeSet::isMouseOnPoint(float mx, float my, glm::vec3 p, float radius) {
    glm::vec3 pos = camera->getPosition();
    glm::vec3 dir = camera->castRayFromScreen((double)mx, (double)my);
    float dist = utils_pointToLineDistance(p, pos, dir);

    return dist < radius ? true : false;
}

// find closest node that mouse is hovering over
void SplineNodeSet::update(float mx, float my) {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].update(mx, my);
    }

    // all points that mouse is hovering over
    std::vector<glm::vec3> hoverPoints;
    for (int i=0; i<(int)nodes.size(); i++) {
        glm::vec3 p = nodes[i].getPosition();
        if (isMouseOnPoint(mx, my, p, selectRadius)) {
            hoverPoints.push_back(p);
        }
    }

    isHovering = (int)hoverPoints.size() > 0 ? true : false;

    // find closest point that mouse if hovering over
    glm::vec3 campos = camera->getPosition();
    glm::vec3 hoverPoint;
    float min = 1000000000;
    for (int i=0; i<(int)hoverPoints.size(); i++) {
        glm::vec3 p = hoverPoints[i];
        float dist = glm::distance(p, campos);
        if (dist < min) {
            min = dist;
            hoverPoint = p;
        }
    }

    if (isHovering) {
        for (int i=0; i<(int)nodes.size(); i++) {
            if (hoverPoint == nodes[i].getPosition()) {
                hoverNode = &nodes[i];
                break;
            }
        }
    }

}

void SplineNodeSet::draw() {
    for (int i=0; i<(int)nodes.size(); i++) {
        nodes[i].draw();
    }

    // draw circle around node if mouse is hovering over
    if (isHovering) {
        glm::vec3 p = hoverNode->getPosition();
        glColor4f(0.0, 0.0, 0.0, 0.5);
        glLineWidth(1.0);
        utils_drawCircle(p, selectRadius, camera->direction);
    }
}






