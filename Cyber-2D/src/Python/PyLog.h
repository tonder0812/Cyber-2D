#pragma once
#include "Python.h"
typedef struct {
    PyObject ob_base;
} LogObject;
extern PyTypeObject LogType;
