TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../deps/gtest/include ../src

LIBS += -L../deps/gtest/lib -static -lgtest -lgtest_main

SOURCES += \
        math/Math_test.cpp \
        math/Ray_test.cpp \
        math/Vector3_test.cpp \
        ../src/math/*.cpp
