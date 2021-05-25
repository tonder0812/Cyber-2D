#pragma once
#include "Python.h"
#include "PyGLM/types/types.h"
typedef struct {
	PyObject ob_base;
	vec<3, float>* Translation;
	float Rotation;
	vec<3, float>* Scale;
} TransformComponentObject;
extern PyTypeObject TransformComponentType;

typedef struct {
	PyObject ob_base;
	bool initialized = false;
	PyObject* exports;
	PyObject* onUpdate;
	PyObject* onStart;
	PyObject* onDestroy;
	PyObject* pModule;
} ScriptComponentObject;
extern PyTypeObject ScriptComponentType;