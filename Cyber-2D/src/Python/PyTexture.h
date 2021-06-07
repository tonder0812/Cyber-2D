#pragma once
#include "Python.h"
#include "OpenGL/OpenGLTexture.h"
#include <memory>
typedef struct {
	PyObject ob_base;
	std::shared_ptr<Cyber::Texture> Texture;
} TextureObject;
extern PyTypeObject TextureType;