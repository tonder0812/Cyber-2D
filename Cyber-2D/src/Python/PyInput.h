#pragma once
#include "Python.h"
typedef struct {
    PyObject ob_base;
} InputObject;
extern PyTypeObject InputType;