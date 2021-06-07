#pragma once
#include "Python.h"
typedef struct {
    PyObject ob_base;
} SceneObject;
extern PyTypeObject SceneType;