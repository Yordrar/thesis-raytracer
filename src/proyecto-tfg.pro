QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    geometry/Camera.cpp \
    geometry/Entity.cpp \
    geometry/Scene.cpp \
    geometry/Sphere.cpp \
    main.cpp \
    manager/RenderManager.cpp \
    material/Diffuse.cpp \
    material/Material.cpp \
    material/Metal.cpp \
    math/Math.cpp \
    math/Ray.cpp \
    math/Vector3.cpp \
    view/MainWindow.cpp

HEADERS += \
    geometry/Camera.h \
    geometry/Entity.h \
    geometry/Scene.h \
    geometry/Sphere.h \
    manager/RenderManager.h \
    material/Diffuse.h \
    material/Material.h \
    material/Metal.h \
    math/Math.h \
    math/Ray.h \
    math/Vector3.h \
    view/MainWindow.h

FORMS += \
    view/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
