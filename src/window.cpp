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

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidget = new GLWidget;

    // Initialize UI sliders to control program
    QLabel *velocityLabel = new QLabel("Velocity");
    QSlider *velocitySlider = new QSlider(Qt::Horizontal);
    velocitySlider->setRange(0, 100);
    velocitySlider->setSingleStep(5);
    velocitySlider->setPageStep(5);
    velocitySlider->setTickInterval(5);
    velocitySlider->setTickPosition(QSlider::TicksRight);

    QLabel *accelerationLabel = new QLabel("Acceleration");
    QSlider *accelerationSlider = new QSlider(Qt::Horizontal);
    accelerationSlider->setRange(0, 100);
    accelerationSlider->setSingleStep(5);
    accelerationSlider->setPageStep(5);
    accelerationSlider->setTickInterval(5);
    accelerationSlider->setTickPosition(QSlider::TicksRight);

    // buttons for adding points to rocket and camera spline
    QPushButton *addPoint1 = new QPushButton("Add Point to Rocket Spline");
    QPushButton *addPoint2 = new QPushButton("Add Point to Camera Spline");
    QPushButton *animateButton = new QPushButton("Animate");

    QCheckBox *cameraCheckBox = new QCheckBox("Animate from camera path");


    // add ui elements to layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(addPoint1);
    mainLayout->addWidget(addPoint2);
    mainLayout->addWidget(animateButton);
    mainLayout->addWidget(cameraCheckBox);
    mainLayout->addWidget(velocityLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(velocitySlider, 0, Qt::AlignTop);
    mainLayout->addWidget(accelerationLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(accelerationSlider, 0, Qt::AlignTop);
    setLayout(mainLayout);
    setWindowTitle(tr("Interpolation Demo"));

    // Connect signals to slots
    connect(addPoint1, SIGNAL(pressed()), glWidget, SLOT(addRocketPoint()));
    connect(addPoint2, SIGNAL(pressed()), glWidget, SLOT(addCameraPoint()));
    connect(animateButton, SIGNAL(pressed()), glWidget, SLOT(startAnimation()));
    connect(cameraCheckBox, SIGNAL(stateChanged(int)), glWidget, SLOT(setAnimationState(int)));

    connect(velocitySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setVelocity(int)));
    connect(glWidget, SIGNAL(velocityChanged(int)), velocitySlider, SLOT(setValue(int)));
    connect(accelerationSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setAcceleration(int)));
    connect(glWidget, SIGNAL(accelerationChanged(int)), accelerationSlider, SLOT(setValue(int)));

    // initialide slider values
    velocitySlider->setValue(20);
    accelerationSlider->setValue(20);
}

void Window::drawOpenGL()
{
    glWidget->repaint();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);

    glWidget->keyPressEvent(e);

}

void Window::keyReleaseEvent(QKeyEvent *e)
{
    glWidget->keyReleaseEvent(e);
}
















