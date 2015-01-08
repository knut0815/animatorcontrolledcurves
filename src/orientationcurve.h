/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#ifndef ORIENTATIONCURVE_H
#define ORIENTATIONCURVE_H

#include <glm/glm.hpp>
#include <vector>
#include <QVector>
#include <GL/glu.h>
#include "quaternion.h"
#include "splinenodeset.h"
#include "splinenode.h"
#include "catmullromspline.h"

class OrientationCurve
{
public:
    OrientationCurve();
    OrientationCurve(SplineNodeSet *splineNodes);

    glm::vec3 getPositionAtT(float t);
    glm::mat4 getTranslationMatrixAtT(float t);
    glm::mat4 getRotationMatrixAtT(float t);
    glm::mat4 getOrientationMatrixAtT(float t);
    glm::vec3 getNearestPointOnCurve(glm::vec3 o, glm::vec3 dir,
                                     float *dist, float *param);
    glm::vec3 getNearestPointOnCurve(glm::vec3 point,
                                     float *dist, float *param);

    float getArcLength();
    void drawCatmullRomSpline();
    void draw();

private:
    std::vector<SplineNode> nodes;
    CatmullRomSpline spline;

};

#endif // ORIENTATIONCURVE_H
