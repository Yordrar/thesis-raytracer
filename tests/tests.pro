TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -march=native -fopenmp
QMAKE_LFLAGS += -fopenmp

INCLUDEPATH += ../deps/gtest/include ../deps/stb ../src
LIBS += -L../deps/gtest/lib -static -lgtest -lgtest_main

SOURCES += \
        ../src/geometry/BVH/AxisAlignedBoundingBox.cpp \
        ../src/geometry/BVH/BVH.cpp \
        ../src/geometry/Camera.cpp \
        ../src/geometry/Entity.cpp \
        ../src/geometry/Hit.cpp \
        ../src/geometry/Intersectable.cpp \
        ../src/geometry/Scatterer.cpp \
        ../src/geometry/Scene.cpp \
        ../src/geometry/Sphere.cpp \
        ../src/geometry/mesh/Mesh.cpp \
        ../src/geometry/mesh/Triangle.cpp \
        ../src/image/Image.cpp \
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
        math/Quaternion_test.cpp \
        math/Ray_test.cpp \
        math/Vector3_test.cpp

HEADERS += \
    ../src/geometry/BVH/AxisAlignedBoundingBox.h \
    ../src/geometry/BVH/BVH.h \
    ../src/geometry/Camera.h \
    ../src/geometry/Entity.h \
    ../src/geometry/Hit.h \
    ../src/geometry/Intersectable.h \
    ../src/geometry/Scatterer.h \
    ../src/geometry/Scene.h \
    ../src/geometry/Sphere.h \
    ../src/geometry/mesh/Mesh.h \
    ../src/geometry/mesh/MeshLoader.h \
    ../src/geometry/mesh/Triangle.h \
    ../src/image/Image.h \
    ../src/material/Dielectric.h \
    ../src/material/Lambertian.h \
    ../src/material/Material.h \
    ../src/material/Metal.h \
    ../src/math/Math.h \
    ../src/math/Quaternion.h \
    ../src/math/Ray.h \
    ../src/math/Vector3.h
