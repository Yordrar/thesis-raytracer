QT       += core gui winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 force_debug_info

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
    geometry/BVH/AxisAlignedBoundingBox.cpp \
    geometry/BVH/BVH.cpp \
    geometry/BVH/BVHBuildStrategy.cpp \
    geometry/BVH/RandomAxis.cpp \
    geometry/BVH/SurfaceAreaHeuristic.cpp \
    geometry/Camera.cpp \
    geometry/Emitter.cpp \
    geometry/Entity.cpp \
    geometry/Hit.cpp \
    geometry/Intersectable.cpp \
    geometry/Scatterer.cpp \
    geometry/Scene.cpp \
    geometry/Sphere.cpp \
    geometry/light/AreaLight.cpp \
    geometry/light/DirectionalLight.cpp \
    geometry/light/PointLight.cpp \
    geometry/mesh/Mesh.cpp \
    geometry/mesh/MeshImporter.cpp \
    geometry/mesh/Triangle.cpp \
    image/Image.cpp \
    main.cpp \
    manager/OptionsManager.cpp \
    manager/RenderManager.cpp \
    manager/SceneManager.cpp \
    material/BlinnPhong.cpp \
    material/Dielectric.cpp \
    material/Emissive.cpp \
    material/Lambertian.cpp \
    material/Material.cpp \
    material/Metal.cpp \
    material/RefractiveLambertian.cpp \
    math/Math.cpp \
    math/Quaternion.cpp \
    math/Ray.cpp \
    math/Vector3.cpp \
    renderer/CPURenderer.cpp \
    renderer/EditModeRenderer.cpp \
    view/Inspector.cpp \
    view/MainWindow.cpp \
    view/RenderOptions.cpp \
    view/Viewport.cpp

HEADERS += \
    geometry/BVH/AxisAlignedBoundingBox.h \
    geometry/BVH/BVH.h \
    geometry/BVH/BVHBuildStrategy.h \
    geometry/BVH/RandomAxis.h \
    geometry/BVH/SurfaceAreaHeuristic.h \
    geometry/Camera.h \
    geometry/Emitter.h \
    geometry/Entity.h \
    geometry/Hit.h \
    geometry/Intersectable.h \
    geometry/Scatterer.h \
    geometry/Scene.h \
    geometry/Sphere.h \
    geometry/light/AreaLight.h \
    geometry/light/DirectionalLight.h \
    geometry/light/PointLight.h \
    geometry/mesh/Mesh.h \
    geometry/mesh/MeshImporter.h \
    geometry/mesh/Triangle.h \
    image/Image.h \
    manager/OptionsManager.h \
    manager/RenderManager.h \
    manager/SceneManager.h \
    material/BlinnPhong.h \
    material/Dielectric.h \
    material/Emissive.h \
    material/Lambertian.h \
    material/Material.h \
    material/Metal.h \
    material/RefractiveLambertian.h \
    math/Math.h \
    math/Quaternion.h \
    math/Ray.h \
    math/Vector3.h \
    renderer/CPURenderer.h \
    renderer/EditModeRenderer.h \
    view/Inspector.h \
    view/MainWindow.h \
    view/RenderOptions.h \
    view/Viewport.h

FORMS += \
    view/Inspector.ui \
    view/MainWindow.ui \
    view/RenderOptions.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS += -march=native -fopenmp
QMAKE_LFLAGS += -fopenmp
INCLUDEPATH += "../deps/opencl/include" "../deps/assimp/include" "../deps/stb"
LIBS += -static -L"../deps/opencl/lib" -L"../deps/assimp/lib" -lassimp -lIrrXML -lzlibstatic

DESTDIR = ../build

RESOURCES += \
    resources.qrc
