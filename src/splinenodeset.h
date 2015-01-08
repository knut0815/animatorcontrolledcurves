/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef SPLINENODESET_H
#define SPLINENODESET_H

#include <glm/glm.hpp>
#include <GL/glu.h>
#include <vector>
#include "camera3d.h"
#include "splinenode.h"
#include "utils.h"

class SplineNodeSet
{
public:
    SplineNodeSet();
    SplineNodeSet(camera3d *cam);

    void addNode(glm::vec3 position);
    std::vector<SplineNode> getNodes();
    void setScreenDimensions(float scrWidth, float scrHeight);

    void onMousePress(float mx, float my);
    void onMouseRelease(float mx, float my);
    void onMouseMove(float mx, float my);
    void deactivateNodes();
    bool isSelected();
    void update(float mx, float my);
    void draw();

private:
    bool isMouseOnPoint(float mx, float my, glm::vec3 p, float radius);
    float pointToLineDistance(glm::vec3 p, glm::vec3 o, glm::vec3 dir);

    camera3d *camera;
    std::vector<SplineNode> nodes;

    bool isHovering;
    SplineNode *hoverNode;

    bool isNodeSelected;
    SplineNode *selectedNode;


    float selectRadius;
};

#endif // SPLINENODESET_H
