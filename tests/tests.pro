TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../deps/gtest/include ../src

LIBS += -L../deps/gtest/lib -static -lgtest -lgtest_main

SOURCES += \
        ../src/geometry/Camera.cpp \
        ../src/geometry/Entity.cpp \
        ../src/geometry/Scene.cpp \
        ../src/geometry/Sphere.cpp \
        ../src/material/Material.cpp \
        geometry/Camera_test.cpp \
        geometry/Scene_test.cpp \
        geometry/Sphere_test.cpp \
        math/Math_test.cpp \
        math/Ray_test.cpp \
        math/Vector3_test.cpp \
        ../src/math/*.cpp

HEADERS += \
    ../src/geometry/Camera.h \
    ../src/geometry/Entity.h \
    ../src/geometry/Scene.h \
    ../src/geometry/Sphere.h \
    ../src/material/Material.h \
    ../src/math/Math.h \
    ../src/math/Ray.h \
    ../src/math/Vector3.h
