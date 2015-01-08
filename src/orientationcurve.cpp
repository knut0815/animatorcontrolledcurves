/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "orientationcurve.h"
#include <QDebug>

#define LINES_PER_SEGMENT 40
OrientationCurve::OrientationCurve() {

}

// combines CatmallRomSpline interpolation with quaternion interpolation
// to find orientation at a point on the curve parameterized [0,1] by length
OrientationCurve::OrientationCurve(SplineNodeSet *splineNodes) {
    QVector<glm::vec4> sNodes4;
    std::vector<SplineNode> sNodes3 = splineNodes->getNodes();
    for (uint i=0; i<sNodes3.size(); i++) {
        sNodes4.append(glm::vec4(sNodes3[i].getPosition(), 0.0));
    }
    spline = CatmullRomSpline(sNodes4);
    nodes = splineNodes->getNodes();
}

void OrientationCurve::drawCatmullRomSpline(){
    // Draw control points
    int n = (spline.size() - 3) * LINES_PER_SEGMENT;
    glColor4f(1.0f, 0.3f, 0.0f, 1.0f);
    glLineWidth(2.0f);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
        glm::vec3 p1 = spline.interpolateForT(0.0f);
        for (int i=1; i <= n; i++) {
            float t = (float)i / (float)n;
            glm::vec3 p2 = spline.interpolateForT(t);
            glVertex3d(p1.x, p1.y, p1.z);
            glVertex3d(p2.x, p2.y, p2.z);
            p1 = p2;
        }

    glEnd();

    // draw lines between beginning and end control points
    QVector<glm::vec4> controlPoints = spline.getControlPoints();
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
        glm::vec4 v1 = controlPoints[0];
        glm::vec4 v2 = controlPoints[1];
        glm::vec4 v3 = controlPoints[controlPoints.size() - 2];
        glm::vec4 v4 = controlPoints[controlPoints.size() - 1];
        glVertex3d(v1.x, v1.y, v1.z);
        glVertex3d(v2.x, v2.y, v2.z);
        glVertex3d(v3.x, v3.y, v3.z);
        glVertex3d(v4.x, v4.y, v4.z);
    glEnd();

}

// t is [0,1] parameterized by arc length
glm::vec3 OrientationCurve::getPositionAtT(float t) {
    return spline.interpolateForT(t);
}

// t is [0,1] parameterized by arc length
glm::mat4 OrientationCurve::getTranslationMatrixAtT(float t) {
    glm::vec3 p = spline.interpolateForT(t);
    glm::mat4 transMatrix = glm::transpose(glm::mat4(1.0, 0.0, 0.0, p.x,
                                                     0.0, 1.0, 0.0, p.y,
                                                     0.0, 0.0, 1.0, p.z,
                                                     0.0, 0.0, 0.0, 1.0));

    return transMatrix;
}

// t is [0,1] parameterized by arc length
glm::mat4 OrientationCurve::getRotationMatrixAtT(float t) {
    int segIdx = spline.getSegmentIndexAtT(t);
    float segmentT = spline.getSegmentParameterAtT(segIdx, t);
    Quaternion q1 = nodes[segIdx+1].getRotationQuaternion();
    Quaternion q2 = nodes[segIdx+2].getRotationQuaternion();
    q1 = q1.normalize();
    q2 = q2.normalize();
    Quaternion q = q1.slerp(q2, segmentT);
    q = q.normalize();

    glm::mat4 rotMatrix = q.getRotationMatrix();

    return rotMatrix;
}

// position and rotation of a point ont he curve
// t is [0,1] parameterized by arc length
glm::mat4 OrientationCurve::getOrientationMatrixAtT(float t) {
    return getTranslationMatrixAtT(t) * getRotationMatrixAtT(t);
}

// given a line with origin o and direction dir, find the closest point on the
// curve and store nearest distance to dist and t parameter [0,1] into param
glm::vec3 OrientationCurve::getNearestPointOnCurve(glm::vec3 o, glm::vec3 dir,
                                                   float *dist, float *param) {
    int numSegments = nodes.size() - 3;
    int n = numSegments * POINTS_PER_SEGMENT;
    float step = 1/(float)n;
    float minDist = 1000000000;
    glm::vec3 nearestPoint;
    float nearestT;
    for (float t=0; t<=1.0; t += step) {
        glm::vec3 p = spline.interpolateForT(t);
        float d = utils_pointToLineDistance(p, o, dir);
        if (d < minDist) {
            minDist = d;
            nearestPoint = p;
            nearestT = t;
        }
    }

    *dist = minDist;
    *param = nearestT;
    return nearestPoint;
}

// givent a point, find closest point on  on curve and store nearest distance
// to dist and t parameter [0,1] into param
glm::vec3 OrientationCurve::getNearestPointOnCurve(glm::vec3 point,
                                                   float *dist, float *param) {
    int numSegments = nodes.size() - 3;
    int n = numSegments * POINTS_PER_SEGMENT;
    float step = 1/(float)n;
    float minDist = 1000000000;
    glm::vec3 nearestPoint;
    float nearestT;
    for (float t=0; t<=1.0; t += step) {
        glm::vec3 p = spline.interpolateForT(t);
        float d = glm::distance(p, point);
        if (d < minDist) {
            minDist = d;
            nearestPoint = p;
            nearestT = t;
        }
    }

    *dist = minDist;
    *param = nearestT;
    return nearestPoint;
}

// total length of curve
float OrientationCurve::getArcLength() {
    return spline.getArcLength();
}

void OrientationCurve::draw() {
    drawCatmullRomSpline();
}














