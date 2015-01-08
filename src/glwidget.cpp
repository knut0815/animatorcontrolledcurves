/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtOpenGL>
#include <QCursor>
#include <time.h>
#include "glwidget.h"
#include <cmath>
#include "glm/glm.hpp"

#include "quaternion.h"
#include "spaceship.h"
#include "cameramodel.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#define LINES_PER_SEGMENT 40

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

    screenWidth = 1200;
    screenHeight = 600;

    //initializealize update/draw timers
    float updatesPerSecond = 30;
    float drawsPerSecond = 30;

    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    drawTimer->start(1000.0/drawsPerSecond);

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
    updateTimer->start(1000.0/updatesPerSecond);

    deltaTimer = new QTime();
    deltaTimer->start();

    // Initialize camera
    glm::vec3 pos = glm::vec3(16.88, 11.82, 11.41);
    glm::vec3 dir = glm::vec3(-0.828, -0.472, -0.302);
    camera = camera3d(pos, dir, screenWidth, screenHeight,
                      60.0, 0.5, 100.0);

    // for updating camera movement
    isMovingForward = false;
    isMovingBackward = false;
    isMovingRight = false;
    isMovingLeft = false;
    isMovingUp = false;
    isMovingDown = false;
    isRotatingRight = false;
    isRotatingLeft = false;
    isRotatingUp = false;
    isRotatingDown = false;
    isRollingRight = false;
    isRollingLeft = false;

    // slider ranges and values
    minVelocityValue = 0.5;
    maxVelocityValue = 20.0;
    minAccelerationValue = 0.001;
    maxAccelerationValue = 0.990;

    velocityValue = minVelocityValue;
    accelerationValue = minAccelerationValue;

    // simulation system
    minDeltaTimeModifier = 0.125;
    maxDeltaTimeModifier = 1.0;
    deltaTimeModifier = maxDeltaTimeModifier;
    runningTime = 0.0;
    isAnimatingFromCamera = false;
    isAnimating = false;

    // initialize minimum number of nodes for a catmullrom spline
    splineNodes = SplineNodeSet(&camera);
    splineNodes.addNode(glm::vec3(10.1, 1.1, 1.1));
    splineNodes.addNode(glm::vec3(10.1, 3.1, 4.1));
    splineNodes.addNode(glm::vec3(10.1, 5.1, 9.1));
    splineNodes.addNode(glm::vec3(10.1, 7.1, 13.1));

    cameraNodes = SplineNodeSet(&camera);
    cameraNodes.addNode(glm::vec3(5.1, 1.1, 1.1));
    cameraNodes.addNode(glm::vec3(5.1, 3.1, 4.1));
    cameraNodes.addNode(glm::vec3(5.1, 5.1, 9.1));
    cameraNodes.addNode(glm::vec3(5.1, 7.1, 13.1));



    updateSplines();

    std::srand(time(NULL));
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(screenWidth, screenHeight);
}

void GLWidget::initializeGL()
{
    static const GLfloat lightPos[4] = { 20.0f, 20.0f, 20.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


// move and rotate camera
void GLWidget::updateCameraMovement(float dt) {
    if (isAnimating && isAnimatingFromCamera) {
        // camera view from perspective of camera path
        float t = cameraAnimationProgress;
        glm::mat4 rotMatrix = cameraCurve.getRotationMatrixAtT(t);
        glm::vec3 pos = cameraCurve.getPositionAtT(t);
        camera.setRotation(rotMatrix);
        camera.setPosition(pos);
    } else {
        float camSpeed = 10.0;
        float camRotSpeed = 2.0;

        if (isMovingForward) { camera.moveForward(camSpeed * dt); }
        if (isMovingBackward) { camera.moveBackward(camSpeed * dt); }
        if (isMovingRight) { camera.moveRight(camSpeed * dt); }
        if (isMovingLeft) { camera.moveLeft(camSpeed * dt); }
        if (isMovingUp) { camera.moveUp(camSpeed * dt); }
        if (isMovingDown) { camera.moveDown(camSpeed * dt); }
        if (isRotatingRight) { camera.rotateRight(camRotSpeed * dt); }
        if (isRotatingLeft) { camera.rotateLeft(camRotSpeed * dt); }
        if (isRotatingUp) { camera.rotateUp(camRotSpeed * dt); }
        if (isRotatingDown) { camera.rotateDown(camRotSpeed * dt); }
        if (isRollingRight) { camera.rollRight(camRotSpeed * dt); }
        if (isRollingLeft) { camera.rollLeft(camRotSpeed * dt); }
    }
}

void GLWidget::initializeSimulation() {
    if (!isAnimating) {
        // save state of camera before switching to camera path
        cameraPositionState = camera.position;
        cameraDirectionState = camera.direction;
        cameraUpState = camera.up;
    }

    animationProgress = 0.0;
    cameraAnimationProgress = 0.0;
    rocketAnimationProgress = 0.0;

    isAnimating = true;
}

void GLWidget::stopSimulation() {
    isAnimating = false;

    // restore state of camera after animation ends
    camera.position = cameraPositionState;
    camera.direction = cameraDirectionState;
    camera.up = cameraUpState;
}

float GLWidget::getRocketAnimationProgress(float t) {
    return utils_easeInOut(t, accelerationValue, 0.9999);
}

float GLWidget::getCameraAnimationProgress(float t) {
    return t;
}

void GLWidget::updateSimulation() {
    // find delta time
    float dt = (float) deltaTimer->elapsed() / 1000;
    runningTime = runningTime + dt;
    deltaTimer->restart();
    updateCameraMovement(dt);

    dt *= deltaTimeModifier;  // speed of simulation

    if (!isAnimating) { return; }

    // update animation progress
    float rocketPathLen = rocketCurve.getArcLength();
    float tVelocity = velocityValue / rocketPathLen;
    animationProgress += tVelocity * dt;
    if (animationProgress > 1.0) {
        animationProgress = 1.0;
    }

    cameraAnimationProgress = getCameraAnimationProgress(animationProgress);
    rocketAnimationProgress = getRocketAnimationProgress(animationProgress);

    if (!isAnimating) {
        stopSimulation();
    }
}

// reset splines to splinenode positions
void GLWidget::updateSplines() {
    rocketCurve = OrientationCurve(&splineNodes);
    cameraCurve = OrientationCurve(&cameraNodes);
}

// Draws coordinate axis' and floor grid
void GLWidget::drawGrid() {
    // draw axis'
    float len = 10.0;
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);   // x
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(len, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);   // y
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, len, 0.0);
    glColor3f(0.0, 0.0, 1.0);   // z
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, len);
    glEnd();

    // draw outline around xy, zy planes
    glLineWidth(2.0);
    glColor4f(0.0, 0.0, 0.0, 0.3);
    glBegin(GL_LINES);
    glVertex3f(0.0, len, 0.0);
    glVertex3f(len, len, 0.0);
    glVertex3f(len, len, 0.0);
    glVertex3f(len, 0.0, 0.0);
    glVertex3f(0.0, len, 0.0);
    glVertex3f(0.0, len, len);
    glVertex3f(0.0, len, len);
    glVertex3f(0.0, 0.0, len);
    glEnd();


    // draw xz plane grid
    float spacing = 0.25;
    int yLines = 120;
    int zLines = 120;
    float height = (float)yLines * spacing;
    float width = (float)zLines * spacing;

    float z = spacing;
    glLineWidth(1.0);
    glColor4f(0.0, 0.0, 0.0, 0.2);
    glBegin(GL_LINES);
    for (int i=0; i < yLines; i++) {
        glVertex3f(0.0, 0.0, z);
        glVertex3f(width, 0.0, z);
        z += spacing;
    }

    float x = spacing;
    for (int i=0; i < zLines; i++) {
        glVertex3f(x, 0.0, 0.0);
        glVertex3f(x, 0.0, height);
        x += spacing;
    }
    glEnd();

}

void GLWidget::drawSplineInterface() {
    updateSplines();
    rocketCurve.draw();
    cameraCurve.draw();

    QPoint mpos = QCursor::pos();
    mpos = QWidget::mapFromGlobal(mpos);

    cameraNodes.update((float)mpos.x(), (float)mpos.y());
    cameraNodes.draw();

    splineNodes.update((float)mpos.x(), (float)mpos.y());
    splineNodes.draw();
}

// Camera moves linearly on path while rocket moves with acceleration
// Path lengths my differ, so this function uses the bisection method to
// find what [0,1] parameter camera is at from [0,1] rocket parameter
float GLWidget::getCameraTFromRocketT(float t) {
    if (t == 0.0 || t == 1.0) { return t; }

    float eps = 0.001;
    float low = 0.0;
    float high = 1.0;

    int max = 100;
    for (int i=0; i<max; i++) {
        float mid = 0.5*(low + high);
        float camT = getRocketAnimationProgress(getCameraAnimationProgress(mid));
        float f = camT - t;
        if (f == 0.0 || 0.5 * (high - low) < eps) {
            return mid;
        }

        int fSign = f > 0 ? 1 : -1;
        float fLow = getRocketAnimationProgress(getCameraAnimationProgress(low)) - t;
        int lowSign = fLow > 0 ? 1 : -1;

        if (fSign == lowSign) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return 0.0;
}

// draws models on spline nodes displaying their orientation
// draws camera screen preview
void GLWidget::drawPreviewModels() {
    float scrDist = 3.0;
    float scrScale = 1.0;
    VirtualCamera virtualCam = VirtualCamera(scrDist, scrScale, &camera);

    // draw rockets on nodes
    SpaceShip rocketModel = SpaceShip();
    std::vector<SplineNode> nodes = splineNodes.getNodes();
    for (uint i=1; i<nodes.size()-1; i++) {
        SplineNode n = nodes[i];

        glm::mat4 matrix = n.getRotationMatrix();
        glm::mat4 transMatrix = n.getTranslationMatrix();
        glPushMatrix();
        glMultMatrixf((GLfloat*)&transMatrix);
        glMultMatrixf((GLfloat*)&matrix);
        if (n.isActive) {
            rocketModel.setAlpha(1.0);
        } else {
            rocketModel.setAlpha(0.2);
        }
        rocketModel.draw();
        glPopMatrix();
    }

    // draw camera and virtual camera screen on nodes
    CameraModel camModel = CameraModel();
    nodes = cameraNodes.getNodes();
    for (uint i=1; i<nodes.size()-1; i++) {
        SplineNode n = nodes[i];

        glm::mat4 matrix = n.getRotationMatrix();
        glm::mat4 transMatrix = n.getTranslationMatrix();
        glPushMatrix();
        glMultMatrixf((GLfloat*)&transMatrix);
        glMultMatrixf((GLfloat*)&matrix);
        if (n.isActive) {
            camModel.setAlpha(1.0);
        } else {
            camModel.setAlpha(0.2);
        }
        camModel.draw();
        glPopMatrix();

        // if camera node is active, show rocket at cooresponding position
        if (n.isActive) {
            float camT;
            float dist;
            glm::vec3 cpos = n.getPosition();
            cameraCurve.getNearestPointOnCurve(cpos, &dist, &camT);
            float rt = getRocketAnimationProgress(camT);
            glm::vec3 opos = rocketCurve.getPositionAtT(rt);

            glm::mat4 rot = rocketCurve.getRotationMatrixAtT(rt);
            glm::mat4 trans = rocketCurve.getTranslationMatrixAtT(rt);
            glPushMatrix();
            glMultMatrixf((GLfloat*)&trans);
            glMultMatrixf((GLfloat*)&rot);
            rocketModel.setAlpha(1.0);
            rocketModel.draw();
            glPopMatrix();

            virtualCam.setRotation(matrix);
            virtualCam.setPosition(cpos);
            virtualCam.setTrackingObjectPosition(opos);
            virtualCam.draw();
        }
    }


    // if mouse is hovering over a curve, display models in their orientations
    // at that point
    QPoint mpos = QCursor::pos();
    mpos = QWidget::mapFromGlobal(mpos);
    glm::vec3 dir = camera.castRayFromScreen((double)mpos.x(), (double)mpos.y());
    glm::vec3 campos = camera.getPosition();

    // display only when user is not editing the nodes
    if (!splineNodes.isSelected() && !cameraNodes.isSelected()) {
        float tRocket, tCamera;
        float distRocket, distCamera;
        rocketCurve.getNearestPointOnCurve(campos, dir, &distRocket, &tRocket);
        cameraCurve.getNearestPointOnCurve(campos, dir, &distCamera, &tCamera);

        if (distRocket < distCamera) {
            tCamera = getCameraTFromRocketT(tRocket);
        } else {
            tRocket = getRocketAnimationProgress(tCamera);
        }

        if (fmin(distCamera, distRocket) < 0.5) {
            glm::mat4 rot = rocketCurve.getRotationMatrixAtT(tRocket);
            glm::mat4 trans = rocketCurve.getTranslationMatrixAtT(tRocket);
            glPushMatrix();
            glMultMatrixf((GLfloat*)&trans);
            glMultMatrixf((GLfloat*)&rot);
            rocketModel.setAlpha(1.0);
            rocketModel.draw();
            glPopMatrix();

            rot = cameraCurve.getRotationMatrixAtT(tCamera);
            trans = cameraCurve.getTranslationMatrixAtT(tCamera);
            glPushMatrix();
            glMultMatrixf((GLfloat*)&trans);
            glMultMatrixf((GLfloat*)&rot);
            camModel.setAlpha(1.0);
            camModel.draw();
            glPopMatrix();

            glm::vec3 pos = cameraCurve.getPositionAtT(tCamera);
            virtualCam.setRotation(rot);
            virtualCam.setPosition(pos);
            virtualCam.setTrackingObjectPosition(rocketCurve.getPositionAtT(tRocket));
            virtualCam.draw();
        }

    }
}

// drawn when animation is running
void GLWidget::drawAnimation() {
    SpaceShip rocketModel = SpaceShip();
    rocketModel.setAlpha(1.0);
    float rt = rocketAnimationProgress;
    glm::mat4 rocketMatrix = rocketCurve.getOrientationMatrixAtT(rt);
    glPushMatrix();
    glMultMatrixf((GLfloat*)&rocketMatrix);
    rocketModel.draw();
    glPopMatrix();

    // draw camera and virtual screen only in 3rd person mode
    if (!isAnimatingFromCamera) {
        CameraModel camModel = CameraModel();
        camModel.setAlpha(1.0);
        float ct = cameraAnimationProgress;
        glm::mat4 cameraMatrix = cameraCurve.getOrientationMatrixAtT(ct);
        glPushMatrix();
        glMultMatrixf((GLfloat*)&cameraMatrix);
        camModel.draw();
        glPopMatrix();

        float scrDist = 3.0;
        float scrScale = 1.5;
        VirtualCamera virtualCam = VirtualCamera(scrDist, scrScale, &camera);
        virtualCam.setRotation(cameraCurve.getRotationMatrixAtT(ct));
        virtualCam.setPosition(cameraCurve.getPositionAtT(ct));
        virtualCam.setTrackingObjectPosition(rocketCurve.getPositionAtT(rt));
        virtualCam.draw();
    }

}

void GLWidget::paintGL()
{
    camera.set();
    drawGrid();

    if (isAnimating) {
        drawAnimation();
    } else {
        drawSplineInterface();
        drawPreviewModels();
    }

    camera.unset();
}

void GLWidget::resizeGL(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    camera.resize((float)width, (float)height);
    splineNodes.setScreenDimensions(screenWidth, screenHeight);
    cameraNodes.setScreenDimensions(screenWidth, screenHeight);
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // turn on camera movement
    isMovingForward = event->key()  == Qt::Key_W;
    isMovingBackward = event->key() == Qt::Key_S;
    isMovingRight = event->key()    == Qt::Key_D;
    isMovingLeft = event->key()     == Qt::Key_A;
    isMovingUp = event->key()       == Qt::Key_T;
    isMovingDown = event->key()     == Qt::Key_G;

    isRotatingRight = event->key()  == Qt::Key_E;
    isRotatingLeft = event->key()   == Qt::Key_Q;
    isRotatingUp = event->key()     == Qt::Key_F;
    isRotatingDown = event->key()   == Qt::Key_R;
    isRollingRight = event->key()   == Qt::Key_X;
    isRollingLeft = event->key()    == Qt::Key_Z;

    // slow down simulation
    if (event->key() == Qt::Key_C) {
        deltaTimeModifier = minDeltaTimeModifier;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // keyboard input halts running simulation/animation
    if (isAnimating) {
        stopSimulation();
    }

    // turn off camera movement
    isMovingForward = event->key()  == Qt::Key_W ? false : isMovingForward;
    isMovingBackward = event->key() == Qt::Key_S ? false : isMovingBackward;
    isMovingRight = event->key()    == Qt::Key_D ? false : isMovingRight;
    isMovingLeft = event->key()     == Qt::Key_A ? false : isMovingLeft;
    isMovingUp = event->key()       == Qt::Key_T ? false : isMovingUp;
    isMovingDown = event->key()     == Qt::Key_G ? false : isMovingDown;

    isRotatingRight = event->key()  == Qt::Key_E ? false : isRotatingRight;
    isRotatingLeft = event->key()   == Qt::Key_Q ? false : isRotatingLeft;
    isRotatingDown = event->key()   == Qt::Key_R ? false : isRotatingDown;
    isRotatingUp = event->key()     == Qt::Key_F ? false : isRotatingUp;
    isRollingRight = event->key()   == Qt::Key_X ? false : isRollingRight;
    isRollingLeft = event->key()    == Qt::Key_Z ? false : isRollingLeft;

    if (event->key() == Qt::Key_C) {
        deltaTimeModifier = maxDeltaTimeModifier;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    // mouse input halts running animation/simulation
    if (isAnimating) {
        stopSimulation();
    }

    if (event->button() == Qt::RightButton) {
        splineNodes.deactivateNodes();
        cameraNodes.deactivateNodes();
        stopAnimation();
        return;
    }

    // only allow user to edit one spline at a time
    bool splineSelectedBefore = splineNodes.isSelected();
    bool cameraSelectedBefore = cameraNodes.isSelected();

    splineNodes.onMousePress((float)event->x(), (float)event->y());
    cameraNodes.onMousePress((float)event->x(), (float)event->y());

    bool splineSelectedAfter = splineNodes.isSelected();
    bool cameraSelectedAfter = cameraNodes.isSelected();

    if (!splineSelectedBefore && splineSelectedAfter) {
        cameraNodes.deactivateNodes();
    } else if (!cameraSelectedBefore && cameraSelectedAfter) {
        splineNodes.deactivateNodes();
    }

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    splineNodes.onMouseRelease((float)event->x(), (float)event->y());
    cameraNodes.onMouseRelease((float)event->x(), (float)event->y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    splineNodes.onMouseMove((float)event->x(), (float)event->y());\
    cameraNodes.onMouseMove((float)event->x(), (float)event->y());
}

// determines which direction next splinenode will be appended to the spline
// when user adds a node
glm::vec3 GLWidget::getNextSplineNodeDirection(std::vector<SplineNode> nodes) {
    glm::vec3 p1 = nodes[nodes.size()-2].getPosition();
    glm::vec3 p2 = nodes[nodes.size()-1].getPosition();

    return glm::normalize(p2 - p1);
}

// slot when add rocket point button is pressed
void GLWidget::addRocketPoint() {
    if (isAnimating) { return; }

    std::vector<SplineNode> nodes = splineNodes.getNodes();
    glm::vec3 randVect = getNextSplineNodeDirection(nodes);
    float len = 4.0;
    glm::vec3 p = nodes[nodes.size()-1].getPosition();
    glm::vec3 nextPoint = p + len*randVect;
    splineNodes.addNode(glm::vec3(nextPoint.x, nextPoint.y, nextPoint.z));
}

// slot when add camera point button is pressed
void GLWidget::addCameraPoint() {
    if (isAnimating) { return; }

    std::vector<SplineNode> nodes = cameraNodes.getNodes();
    glm::vec3 randVect = getNextSplineNodeDirection(nodes);
    float len = 4.0;
    glm::vec3 p = nodes[nodes.size()-1].getPosition();
    glm::vec3 nextPoint = p + len*randVect;
    cameraNodes.addNode(glm::vec3(nextPoint.x, nextPoint.y, nextPoint.z));
}

void GLWidget::startAnimation() {
    qDebug() << "start animation";
    splineNodes.deactivateNodes();
    cameraNodes.deactivateNodes();
    initializeSimulation();
}

void GLWidget::stopAnimation() {
    isAnimating = false;
}

void GLWidget::setAnimationState(int state) {
    qDebug() << "state" << state;
    if (state == 2) {
        isAnimatingFromCamera = true;
    } else {
        isAnimatingFromCamera = false;
    }
}

void GLWidget::setVelocity(int percent)
{
    float t = (float)percent / 100.0;
    velocityValue = utils_lerp(minVelocityValue, maxVelocityValue, t);
    qDebug() << "Set velocity" << velocityValue;
    emit velocityChanged(percent);
}

void GLWidget::setAcceleration(int percent)
{
    float t = (float)percent / 100.0;
    accelerationValue = utils_lerp(maxAccelerationValue, minAccelerationValue, t);
    qDebug() << "Set acceleration" << accelerationValue;
    emit accelerationChanged(percent);
}










