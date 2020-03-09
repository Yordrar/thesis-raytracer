TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -march=native -fopenmp
QMAKE_LFLAGS += -fopenmp

INCLUDEPATH += ../deps/gtest/include ../src
LIBS += -L../deps/gtest/lib -static -lgtest -lgtest_main

SOURCES += \
        ../src/geometry/Camera.cpp \
        ../src/geometry/Entity.cpp \
        ../src/geometry/Intersectable.cpp \
        ../src/geometry/Scene.cpp \
        ../src/geometry/Sphere.cpp \
        ../src/image/Framebuffer.cpp \
        ../src/material/Dielectric.cpp \
        ../src/material/Lambertian.cpp \
        ../src/material/Material.cpp \
        ../src/material/Metal.cpp \
        ../src/math/Math.cpp \
        ../src/math/Quaternion.cpp \
        ../src/math/Vector3.cpp \
        ../src/math/Ray.cpp \
        geometry/Camera_test.cpp \
        geometry/Scene_test.cpp \
        geometry/Sphere_test.cpp \
        math/Math_test.cpp \
        math/Ray_test.cpp \
        math/Vector3_test.cpp

HEADERS += \
    ../src/geometry/Camera.h \
    ../src/geometry/Entity.h \
    ../src/geometry/Intersectable.h \
    ../src/geometry/Scene.h \
    ../src/geometry/Sphere.h \
    ../src/image/Framebuffer.h \
    ../src/material/Dielectric.h \
    ../src/material/Lambertian.h \
    ../src/material/Material.h \
    ../src/material/Metal.h \
    ../src/math/Math.h \
    ../src/math/Quaternion.h \
    ../src/math/Ray.h \
    ../src/math/Vector3.h
