#pragma once
#include "Python.h"
#include "Scene/Entity.h"
typedef struct {
	PyObject ob_base;
	Cyber::Entity m_Entity;
} EntityObject;
extern PyTypeObject EntityType;