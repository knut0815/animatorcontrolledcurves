#    CSC 486 Assignment 1
#    Ryan Guy
#    V00484803

HEADERS       = glwidget.h \
                window.h \
    camera3d.h \
    quaternion.h \
    arcball.h \
    splinenode.h \
    translationnode.h \
    spaceship.h \
    splinenodeset.h \
    catmullromspline.h \
    utils.h \
    cameramodel.h \
    orientationcurve.h \
    virtualcamera.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
    camera3d.cpp \
    quaternion.cpp \
    arcball.cpp \
    splinenode.cpp \
    translationnode.cpp \
    spaceship.cpp \
    splinenodeset.cpp \
    catmullromspline.cpp \
    utils.cpp \
    cameramodel.cpp \
    orientationcurve.cpp \
    virtualcamera.cpp
QT           += opengl widgets


contains(QT_CONFIG, opengles.) {
    contains(QT_CONFIG, angle): \
        warning("Qt was built with ANGLE, which provides only OpenGL ES 2.0 on top of DirectX 9.0c")
    error("This example requires Qt to be configured with -opengl desktop")
}
