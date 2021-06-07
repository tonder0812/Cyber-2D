#include "pch.h"
#include "PyComponents.h"
#include "Scene/Components.h"
#include "Utils.h"
#include "Core/Application.h"
#include <glm/gtc/matrix_transform.hpp>

/////////////////////////////////////////////////////////////////////////////
// TransformComponent ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static int TransformComponentTypeInit(TransformComponentObject* self, PyObject* args, PyObject* kwds) {
	PyObject* pVec3Type = Cyber::Application::Get().GetPyGLM_Vec3();
	self->Translation = (vec<3, float> *)PyObject_CallObject(pVec3Type, NULL);
	self->Translation->super_type = { 0,0,0 };
	self->Scale = (vec<3, float> *)PyObject_CallObject(pVec3Type, NULL);
	self->Scale->super_type = { 1,1,1 };
	return 0;
}

static int TransformComponentTypeDel(TransformComponentObject* self, PyObject* args, PyObject* kwds) {
	Py_DECREF(self->Translation);
	Py_DECREF(self->Scale);
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

static PyMemberDef TransformComponentMembers[] = {
	{"Translation", T_OBJECT_EX, offsetof(TransformComponentObject, Translation), 0,
	 "Translation"},
	{"Rotation", T_FLOAT, offsetof(TransformComponentObject, Rotation), 0,
	 "Rotation"},
	{"Scale", T_OBJECT_EX, offsetof(TransformComponentObject, Scale), 0,
	 "Scale"},
	{NULL}  /* Sentinel */
};

PyTypeObject TransformComponentType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.TransformComponent",			// tp_name
		sizeof(TransformComponentObject),	// tp_basicsize 
		0,									// tp_itemsize 
		(destructor)TransformComponentTypeDel,// tp_dealloc 
		0,									// tp_print 
		0,									// tp_getattr 
		0,									// tp_setattr 
		0,									// tp_reserved 
		0,									// tp_repr 
		0,									// tp_as_number 
		0,									// tp_as_sequence 
		0,									// tp_as_mapping 
		0,									// tp_hash  
		0,									// tp_call 
		0,									// tp_str 
		0,									// tp_getattro 
		0,									// tp_setattro 
		0,									// tp_as_buffer 
		Py_TPFLAGS_DEFAULT ,				// tp_flags 
		"Simple transform component",		// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		0,									// tp_methods 
		TransformComponentMembers,			// tp_members 
		0,									// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		(initproc)TransformComponentTypeInit,// tp_init 
		0,									// tp_alloc 
		PyType_GenericNew,					// tp_new 
};

/////////////////////////////////////////////////////////////////////////////
// SpriteRendererComponent //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static int SpriteRendererComponentTypeInit(SpriteRendererComponentObject* self, PyObject* args, PyObject* kwds) {
	PyObject* pVec4Type = Cyber::Application::Get().GetPyGLM_Vec4();
	self->Color = (vec<4, float> *)PyObject_CallObject(pVec4Type, NULL);
	self->Color->super_type = { 1,1,1,1 };
	self->texture = (TextureObject*)PyObject_CallObject(Cyber::Application::Get().GetPyCyber_Texture(), NULL);
	self->UseTexture = false;
	return 0;
}

static int SpriteRendererComponentTypeDel(SpriteRendererComponentObject* self, PyObject* args, PyObject* kwds) {
	Py_DECREF(self->Color);
	Py_DECREF(self->texture);
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

static PyObject* SpriteRendererTypeTextureGet(SpriteRendererComponentObject* self) {
	Py_INCREF(self->texture);
	return (PyObject*)self->texture;
}

static int SpriteRendererTypeTextureSet(SpriteRendererComponentObject* self, PyObject* value) {
	if (!PyObject_IsInstance(value, Cyber::Application::Get().GetPyCyber_Texture())) {
		PyErr_SetString(PyExc_TypeError, "Invalid Type - value must be a Cyber.Texture");
		return -1;
	}
	self->texture->Texture = ((TextureObject*)value)->Texture;
	return 0;
}

static PyObject* SpriteRendererTypeUseTextureGet(SpriteRendererComponentObject* self) {
	return PyBool_FromLong(self->UseTexture);
}

static int SpriteRendererTypeUseTextureSet(SpriteRendererComponentObject* self, PyObject* value) {
	if (!self->texture->Texture.get()) {
		PyErr_SetString(PyExc_RuntimeError, "Cannot use texture without setting it first");
		return -1;
	}
	self->UseTexture = PyObject_IsTrue(value);
	return 0;
}

static PyMemberDef SpriteRendererComponentMembers[] = {
	{"Color", T_OBJECT_EX, offsetof(SpriteRendererComponentObject, Color), 0,
	 "Color"},
	{NULL}  /* Sentinel */
};

static PyGetSetDef SpriteRendererProperties[] = {
	{"Texture",(getter)SpriteRendererTypeTextureGet,(setter)SpriteRendererTypeTextureSet,"SpriteRenderer's texture"},
	{"UseTexture",(getter)SpriteRendererTypeUseTextureGet,(setter)SpriteRendererTypeUseTextureSet,"If SpriteRenderer uses its texture"},
	{NULL}  /* Sentinel */
};

PyTypeObject SpriteRendererComponentType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.SpriteRendererComponent",	// tp_name
		sizeof(SpriteRendererComponentObject),// tp_basicsize 
		0,									// tp_itemsize 
		(destructor)SpriteRendererComponentTypeDel,// tp_dealloc 
		0,									// tp_print 
		0,									// tp_getattr 
		0,									// tp_setattr 
		0,									// tp_reserved 
		0,									// tp_repr 
		0,									// tp_as_number 
		0,									// tp_as_sequence 
		0,									// tp_as_mapping 
		0,									// tp_hash  
		0,									// tp_call 
		0,									// tp_str 
		0,									// tp_getattro 
		0,									// tp_setattro 
		0,									// tp_as_buffer 
		Py_TPFLAGS_DEFAULT ,				// tp_flags 
		"Simple Sprite Renderer component",	// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		0,									// tp_methods 
		SpriteRendererComponentMembers,		// tp_members 
		SpriteRendererProperties,			// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		(initproc)SpriteRendererComponentTypeInit,// tp_init 
		0,									// tp_alloc 
		PyType_GenericNew,					// tp_new 
};

/////////////////////////////////////////////////////////////////////////////
// CameraComponent //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static int CameraComponentTypeInit(CameraComponentObject* self, PyObject* args, PyObject* kwds) {
	self->Camera = Cyber::SceneCamera();
	self->FixedAspectRatio = false;
	self->Primary = false;
	return 0;
}

static int CameraComponentTypeDel(CameraComponentObject* self, PyObject* args, PyObject* kwds) {
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

static PyObject* CameraTypeSizeGet(CameraComponentObject* self) {
	return PyFloat_FromDouble(self->Camera.GetSize());
}

static int CameraTypeSizeSet(CameraComponentObject* self, PyObject* value) {
	float v = PyFloat_AsDouble(value);
	if (PyErr_Occurred()) {
		Cyber::PythonUtils::GetErrorMessage();
		PyErr_SetString(PyExc_TypeError, "Invalid Type - Size must be a number");
		return -1;
	}
	self->Camera.SetSize(v);
	return 0;
}

static PyObject* CameraTypeNearClipGet(CameraComponentObject* self) {
	return PyFloat_FromDouble(self->Camera.GetNearClip());
}

static int CameraTypeNearClipSet(CameraComponentObject* self, PyObject* value) {
	float v = PyFloat_AsDouble(value);
	if (PyErr_Occurred()) {
		Cyber::PythonUtils::GetErrorMessage();
		PyErr_SetString(PyExc_TypeError, "Invalid Type - Near must be a number");
		return -1;
	}
	self->Camera.SetNearClip(v);
	return 0;
}

static PyObject* CameraTypeFarClipGet(CameraComponentObject* self) {
	return PyFloat_FromDouble(self->Camera.GetFarClip());
}

static int CameraTypeFarClipSet(CameraComponentObject* self, PyObject* value) {
	float v = PyFloat_AsDouble(value);
	if (PyErr_Occurred()) {
		Cyber::PythonUtils::GetErrorMessage();
		PyErr_SetString(PyExc_TypeError, "Invalid Type - Far must be a number");
		return -1;
	}
	self->Camera.SetFarClip(v);
	return 0;
}

static PyObject* CameraTypePrimaryGet(CameraComponentObject* self) {
	return PyBool_FromLong(self->Primary);
}

static int CameraTypePrimarySet(CameraComponentObject* self, PyObject* value) {
	bool v = PyObject_IsTrue(value);
	self->Primary = v;
	if (v)
		Cyber::Scene::CurrentScene->SetPrimaryCamera(self->entity);
	return 0;
}

static PyObject* CameraTypeScreenToWorld(CameraComponentObject* self, PyObject* args, PyObject* kwds) {
	PyObject* obj = nullptr;
	if (!PyArg_ParseTuple(args, "O", &obj)) {
		return NULL;
	}

	if (!PyObject_IsInstance(obj, Cyber::Application::Get().GetPyGLM_Vec2())) {
		PyErr_SetString(PyExc_TypeError, "Invalid Type - Point must be a glm.vec2");
		return NULL;
	}

	vec<2, float>* world = (vec<2, float>*) PyObject_CallObject(Cyber::Application::Get().GetPyGLM_Vec2(), NULL);
	glm::vec2 point = ((vec<2, float>*)obj)->super_type;
	glm::vec2 pt = self->Camera.Normalize(point);
	world->super_type = (Cyber::Camera{ self->Camera.GetProjection() , glm::inverse(self->entity.GetComponent<Cyber::TransformComponent>().GetTransform()) }).NormalizedToWorld(pt);
	return (PyObject*)world;
}

static PyGetSetDef CameraProperties[] = {
	{"Size",(getter)CameraTypeSizeGet,(setter)CameraTypeSizeSet,"Camera's vertical size"},
	{"Near",(getter)CameraTypeNearClipGet,(setter)CameraTypeNearClipSet,"Camera's Near clip"},
	{"Far",(getter)CameraTypeFarClipGet,(setter)CameraTypeFarClipSet,"Camera's Far clip"},
	{"Primary",(getter)CameraTypePrimaryGet,(setter)CameraTypePrimarySet ,"Is this Camera the primary camera of the Scene"},
	{NULL}  /* Sentinel */
};

static PyMethodDef CameraMethods[] = {
	{"ScreenToWorld", (PyCFunction)CameraTypeScreenToWorld, METH_VARARGS,
	 "Return world coordinates correspondent with screen coordinates passed"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject CameraComponentType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.CameraComponent",			// tp_name
		sizeof(CameraComponentObject),		// tp_basicsize 
		0,									// tp_itemsize 
		(destructor)CameraComponentTypeDel,	// tp_dealloc 
		0,									// tp_print 
		0,									// tp_getattr 
		0,									// tp_setattr 
		0,									// tp_reserved 
		0,									// tp_repr 
		0,									// tp_as_number 
		0,									// tp_as_sequence 
		0,									// tp_as_mapping 
		0,									// tp_hash  
		0,									// tp_call 
		0,									// tp_str 
		0,									// tp_getattro 
		0,									// tp_setattro 
		0,									// tp_as_buffer 
		Py_TPFLAGS_DEFAULT ,				// tp_flags 
		"Simple camera component",			// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		CameraMethods,						// tp_methods 
		0,									// tp_members 
		CameraProperties,					// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		(initproc)CameraComponentTypeInit,	// tp_init 
		0,									// tp_alloc 
		PyType_GenericNew,					// tp_new 
};


/////////////////////////////////////////////////////////////////////////////
// ScriptComponent //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static int ScriptComponentTypeInit(ScriptComponentObject* self, PyObject* args, PyObject* kwds) {
	return 0;
}

static int ScriptComponentTypeDel(ScriptComponentObject* self, PyObject* args, PyObject* kwds) {
	Py_XDECREF(self->onStart);
	Py_XDECREF(self->onUpdate);
	Py_XDECREF(self->onDestroy);
	Py_XDECREF(self->Public);
	Py_XDECREF(self->pModule);
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

static PyMemberDef ScriptComponentMembers[] = {
	{"Public", T_OBJECT_EX, offsetof(ScriptComponentObject, Public), 0,
	 "Public script variables"},
	{NULL}  /* Sentinel */
};
PyTypeObject ScriptComponentType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.ScriptComponent",			// tp_name
		sizeof(ScriptComponentObject),		// tp_basicsize 
		0,									// tp_itemsize 
		(destructor)ScriptComponentTypeDel,	// tp_dealloc 
		0,									// tp_print 
		0,									// tp_getattr 
		0,									// tp_setattr 
		0,									// tp_reserved 
		0,									// tp_repr 
		0,									// tp_as_number 
		0,									// tp_as_sequence 
		0,									// tp_as_mapping 
		0,									// tp_hash  
		0,									// tp_call 
		0,									// tp_str 
		0,									// tp_getattro 
		0,									// tp_setattro 
		0,									// tp_as_buffer 
		Py_TPFLAGS_DEFAULT ,				// tp_flags 
		"Simple script component",			// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		0,									// tp_methods 
		ScriptComponentMembers,				// tp_members 
		0,									// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		(initproc)ScriptComponentTypeInit,	// tp_init 
		0,									// tp_alloc 
		PyType_GenericNew,					// tp_new 
};