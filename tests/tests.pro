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
        geometry/Camera_test.cpp \
        geometry/Scene_test.cpp \
        geometry/Sphere_test.cpp \
        math/Math_test.cpp \
        math/Ray_test.cpp \
        math/Vector3_test.cpp \
        ../src/math/*.cpp

HEADERS += \
    ../src/geometry/Entity.h \
    ../src/geometry/Sphere.h
