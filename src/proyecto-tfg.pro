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
    geometry/BVH/AxisAlignedBoundingBox.cpp \
    geometry/BVH/BVH.cpp \
    geometry/Camera.cpp \
    geometry/Emitter.cpp \
    geometry/Entity.cpp \
    geometry/Hit.cpp \
    geometry/Intersectable.cpp \
    geometry/Scatterer.cpp \
    geometry/Scene.cpp \
    geometry/Sphere.cpp \
    geometry/light/DirectionalLight.cpp \
    geometry/light/PointLight.cpp \
    geometry/mesh/Mesh.cpp \
    geometry/mesh/MeshImporter.cpp \
    geometry/mesh/Triangle.cpp \
    image/Image.cpp \
    main.cpp \
    manager/RenderManager.cpp \
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
    view/Viewport.cpp

HEADERS += \
    geometry/BVH/AxisAlignedBoundingBox.h \
    geometry/BVH/BVH.h \
    geometry/Camera.h \
    geometry/Emitter.h \
    geometry/Entity.h \
    geometry/Hit.h \
    geometry/Intersectable.h \
    geometry/Scatterer.h \
    geometry/Scene.h \
    geometry/Sphere.h \
    geometry/light/DirectionalLight.h \
    geometry/light/PointLight.h \
    geometry/mesh/Mesh.h \
    geometry/mesh/MeshImporter.h \
    geometry/mesh/Triangle.h \
    image/Image.h \
    manager/RenderManager.h \
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
    view/Viewport.h

FORMS += \
    view/Inspector.ui \
    view/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS += -march=native -fopenmp
QMAKE_LFLAGS += -fopenmp
INCLUDEPATH += "../deps/opencl/include" "../deps/assimp/include" "../deps/stb"
LIBS += -static -L"../deps/opencl/lib" -L"../deps/assimp/lib" -lassimp -lIrrXML -lzlibstatic

DISTFILES +=

RESOURCES += \
    resources.qrc
