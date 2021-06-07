#pragma once
#include "Python.h"
#include "PyTexture.h"
#include "PyGLM/types/types.h"
#include "Scene/SceneCamera.h"
#include "Scene/Entity.h"

typedef struct {
	PyObject ob_base;
	vec<3, float>* Translation;
	float Rotation;
	vec<3, float>* Scale;
} TransformComponentObject;
extern PyTypeObject TransformComponentType;

typedef struct {
	PyObject ob_base;
	Cyber::SceneCamera Camera;
	Cyber::Entity entity;
	bool Primary;
	bool FixedAspectRatio;
} CameraComponentObject;
extern PyTypeObject CameraComponentType;

typedef struct {
	PyObject ob_base;
	vec<4, float>* Color;
	TextureObject* texture;
	bool UseTexture = false;
} SpriteRendererComponentObject;
extern PyTypeObject SpriteRendererComponentType;

typedef struct {
	PyObject ob_base;
	bool initialized = false;
	PyObject* Public;
	PyObject* onUpdate;
	PyObject* onStart;
	PyObject* onDestroy;
	PyObject* pModule;
} ScriptComponentObject;
extern PyTypeObject ScriptComponentType;