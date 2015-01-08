/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef TRANSLATIONNODE_H
#define TRANSLATIONNODE_H

#include <glm/glm.hpp>
#include <cmath>
#include "camera3d.h"
#include "utils.h"

class TranslationNode
{
public:
    TranslationNode();
    TranslationNode(glm::vec3 position, camera3d *cam);

    glm::vec3 getPosition();
    void activate();
    void deactivate();
    glm::mat4 getTranslationMatrix();
    bool isSelected();

    void onMousePress(float mx, float my);
    void onMouseRelease(float mx, float my);
    void onMouseMove(float mx, float my);
    void update(float mx, float my);
    void draw();

    bool isActive;

private:
    float pointToLineDistance(glm::vec3 p, glm::vec3 o, glm::vec3 dir);
    bool isMouseOnPoint(float mx, float my, glm::vec3 p, float radius);

    glm::vec3 position;
    camera3d *camera;

    float axisLength;
    float handleSize;
    float selectRadius;
    float mousex;
    float mousey;

    bool xHovered, yHovered, zHovered;
    bool xSelected, ySelected, zSelected;

};

#endif // TRANSLATIONNODE_H
