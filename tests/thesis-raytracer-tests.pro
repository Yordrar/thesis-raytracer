TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt core

QMAKE_CXXFLAGS += -march=native -fopenmp
QMAKE_LFLAGS += -fopenmp

INCLUDEPATH += ../deps/gtest/include ../deps/stb ../src "../deps/assimp/include"
LIBS += -L../deps/gtest/lib -L../deps/assimp/lib -static -lgtest -lgtest_main -lassimp -lIrrXML -lzlibstatic

SOURCES += \
        ../src/geometry/BVH/AxisAlignedBoundingBox.cpp \
        ../src/geometry/BVH/BVH.cpp \
        ../src/geometry/BVH/BVHBuildStrategy.cpp \
        ../src/geometry/BVH/RandomAxis.cpp \
        ../src/geometry/BVH/SurfaceAreaHeuristic.cpp \
        ../src/geometry/Camera.cpp \
        ../src/geometry/Emitter.cpp \
        ../src/geometry/Entity.cpp \
        ../src/geometry/Hit.cpp \
        ../src/geometry/Intersectable.cpp \
        ../src/geometry/Scatterer.cpp \
        ../src/geometry/Scene.cpp \
        ../src/geometry/Sphere.cpp \
        ../src/geometry/light/AreaLight.cpp \
        ../src/geometry/light/DirectionalLight.cpp \
        ../src/geometry/light/PointLight.cpp \
        ../src/geometry/mesh/Mesh.cpp \
        ../src/geometry/mesh/MeshImporter.cpp \
        ../src/geometry/mesh/Triangle.cpp \
        ../src/image/Image.cpp \
        ../src/manager/SceneManager.cpp \
        ../src/material/BlinnPhong.cpp \
        ../src/material/Dielectric.cpp \
        ../src/material/Emissive.cpp \
        ../src/material/Lambertian.cpp \
        ../src/material/Material.cpp \
        ../src/material/Metal.cpp \
        ../src/material/RefractiveLambertian.cpp \
        ../src/math/Math.cpp \
        ../src/math/Quaternion.cpp \
        ../src/math/Vector3.cpp \
        ../src/math/Ray.cpp \
        ../src/manager/RenderManager.cpp \
        ../src/manager/OptionsManager.cpp \
        ../src/renderer/CPURenderer.cpp \
        ../src/renderer/EditModeRenderer.cpp \
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
    ../src/geometry/BVH/BVHBuildStrategy.h \
    ../src/geometry/BVH/RandomAxis.h \
    ../src/geometry/BVH/SurfaceAreaHeuristic.h \
    ../src/geometry/Camera.h \
    ../src/geometry/Emitter.h \
    ../src/geometry/Entity.h \
    ../src/geometry/Hit.h \
    ../src/geometry/Intersectable.h \
    ../src/geometry/Scatterer.h \
    ../src/geometry/Scene.h \
    ../src/geometry/Sphere.h \
    ../src/geometry/light/AreaLight.h \
    ../src/geometry/light/DirectionalLight.h \
    ../src/geometry/light/PointLight.h \
    ../src/geometry/mesh/Mesh.h \
    ../src/geometry/mesh/MeshImporter.h \
    ../src/geometry/mesh/MeshLoader.h \
    ../src/geometry/mesh/Triangle.h \
    ../src/image/Image.h \
    ../src/manager/SceneManager.h \
    ../src/material/BlinnPhong.h \
    ../src/material/Dielectric.h \
    ../src/material/Emissive.h \
    ../src/material/Lambertian.h \
    ../src/material/Material.h \
    ../src/material/Metal.h \
    ../src/material/RefractiveLambertian.h \
    ../src/math/Math.h \
    ../src/math/Quaternion.h \
    ../src/math/Ray.h \
    ../src/math/Vector3.h \
    ../src/manager/RenderManager.h \
    ../src/manager/OptionsManager.h\ \
    ../src/renderer/CPURenderer.h \
    ../src/renderer/EditModeRenderer.h
