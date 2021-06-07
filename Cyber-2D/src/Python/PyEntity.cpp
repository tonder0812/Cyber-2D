#include "pch.h"
#include "PyEntity.h"
#include <structmember.h>
#include "Scene/Components.h"
#include "Scene/Scene.h"

static int EntityTypeInit(EntityObject* self, PyObject* args, PyObject* kwds) {
	return 0;
}

static int EntityTypeDel(EntityObject* self, PyObject* args, PyObject* kwds) {
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

static PyObject* EntityTypeGetTransform(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (self->m_Entity && self->m_Entity.HasComponent<Cyber::TransformComponent>()) {
		Cyber::TransformComponent& transform = self->m_Entity.GetComponent<Cyber::TransformComponent>();
		Py_INCREF(transform.Transform);
		return (PyObject*)transform.Transform;
	}
	else {
		Py_RETURN_NONE;
	}
}

static PyObject* EntityTypeGetCamera(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (self->m_Entity && self->m_Entity.HasComponent<Cyber::CameraComponent>()) {
		Cyber::CameraComponent& camera = self->m_Entity.GetComponent<Cyber::CameraComponent>();
		Py_INCREF(camera.Camera);
		return (PyObject*)camera.Camera;
	}
	else {
		Py_RETURN_NONE;
	}
}
static PyObject* EntityTypeAddCamera(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (!self->m_Entity) {
		Py_RETURN_NONE;
	}
	if (self->m_Entity.HasComponent<Cyber::CameraComponent>()) {
		PyErr_SetString(PyExc_RuntimeError, "Invalid operation - Entities cannot have more than one Camera component at the same time");
		return NULL;
	}
	else {
		Cyber::CameraComponent& camera = self->m_Entity.AddComponent<Cyber::CameraComponent>();
		Py_INCREF(camera.Camera);
		return (PyObject*)camera.Camera;
	}
}

static PyObject* EntityTypeGetSpriteRenderer(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (self->m_Entity && self->m_Entity.HasComponent<Cyber::SpriteRendererComponent>()) {
		Cyber::SpriteRendererComponent& spriteRenderer = self->m_Entity.GetComponent<Cyber::SpriteRendererComponent>();
		Py_INCREF(spriteRenderer.Texture);
		return (PyObject*)spriteRenderer.Texture;
	}
	else {
		Py_RETURN_NONE;
	}
}
static PyObject* EntityTypeAddSpriteRenderer(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (!self->m_Entity) {
		Py_RETURN_NONE;
	}
	if (self->m_Entity.HasComponent<Cyber::SpriteRendererComponent>()) {
		PyErr_SetString(PyExc_RuntimeError, "Invalid operation - Entities cannot have more than one SpriteRenderer component at the same time");
		return NULL;
	}
	else {
		Cyber::SpriteRendererComponent& spriteRenderer = self->m_Entity.AddComponent<Cyber::SpriteRendererComponent>();
		Py_INCREF(spriteRenderer.Texture);
		return (PyObject*)spriteRenderer.Texture;
	}
}

static PyObject* EntityTypeGetScript(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (self->m_Entity && self->m_Entity.HasComponent<Cyber::ScriptComponent>()) {
		Cyber::ScriptComponent& script = self->m_Entity.GetComponent<Cyber::ScriptComponent>();
		Py_INCREF(script.Script);
		return (PyObject*)script.Script;
	}
	else {
		Py_RETURN_NONE;
	}
}
static PyObject* EntityTypeAddScript(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (!self->m_Entity) {
		Py_RETURN_NONE;
	}
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str)) {
		return NULL;
	}
	if (self->m_Entity.HasComponent<Cyber::ScriptComponent>()) {
		PyErr_SetString(PyExc_RuntimeError, "Invalid operation - Entities cannot have more than one Script component at the same time");
		return NULL;
	}
	else {
		Cyber::ScriptComponent& script = self->m_Entity.AddComponent<Cyber::ScriptComponent>(str);
		if (!script.Script) {
			PyErr_Format(PyExc_ValueError, "Invalid value - Module %s doesn't exist", str);
			return NULL;
		}
		Py_INCREF(script.Script);
		return (PyObject*)script.Script;
	}
}

static PyObject* EntityTypeDestroy(EntityObject* self, PyObject* args, PyObject* kwds) {
	Cyber::Scene::CurrentScene->DestroyEntity(self->m_Entity);
	self->m_Entity = Cyber::Entity();
	Py_RETURN_NONE;
}

static PyObject* EntityTypeIdGet(EntityObject* self) {
	if (!self->m_Entity) {
		Py_RETURN_NONE;
	}
	std::string& id = self->m_Entity.GetComponent<Cyber::TagComponent>().Id;
	return PyUnicode_FromString(id.c_str());
}

static int EntityTypeIdSet(EntityObject* self, PyObject* value) {
	if (!self->m_Entity) {
		return 0;
	}
	PyObject* repr = PyObject_Str(value);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	char* bytes = PyBytes_AS_STRING(str);
	self->m_Entity.GetComponent<Cyber::TagComponent>().Id = std::string(bytes);
	Py_DECREF(str);
	Py_DECREF(repr);
	return 0;
}

static PyObject* EntityTypeClassGet(EntityObject* self) {
	if (!self->m_Entity) {
		Py_RETURN_NONE;
	}
	std::string& Class = self->m_Entity.GetComponent<Cyber::TagComponent>().Class;
	return PyUnicode_FromString(Class.c_str());
}

static int EntityTypeClassSet(EntityObject* self, PyObject* value) {
	if (!self->m_Entity) {
		return 0;
	}
	PyObject* repr = PyObject_Str(value);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	char* bytes = PyBytes_AS_STRING(str);
	self->m_Entity.GetComponent<Cyber::TagComponent>().Class = std::string(bytes);
	Py_DECREF(str);
	Py_DECREF(repr);
	return 0;
}

static PyMethodDef EntityMethods[] = {
	{"GetTransform", (PyCFunction)EntityTypeGetTransform, METH_NOARGS,
	 "Return entity's transform"
	},
	{"GetCamera", (PyCFunction)EntityTypeGetCamera, METH_NOARGS,
	 "Return entity's camera"
	},
	{"AddCamera", (PyCFunction)EntityTypeAddCamera, METH_NOARGS,
	 "Adds and returns a new Camera attached to the entity"
	},
	{"GetSpriteRenderer", (PyCFunction)EntityTypeGetSpriteRenderer, METH_NOARGS,
	 "Return entity's SpriteRenderer"
	},
	{"AddSpriteRenderer", (PyCFunction)EntityTypeAddSpriteRenderer, METH_NOARGS,
	 "Adds and returns a new SpriteRenderer attached to the entity"
	},
	{"GetScript", (PyCFunction)EntityTypeGetScript, METH_NOARGS,
	 "Return entity's script"
	},
	{"AddScript", (PyCFunction)EntityTypeAddScript, METH_VARARGS,
	 "Adds and returns a new Script attached to the entity"
	},
	{"Destroy", (PyCFunction)EntityTypeDestroy, METH_NOARGS,
	 "Remove entity from Scene"
	},
	{NULL}  /* Sentinel */
};

static PyGetSetDef EntityProperties[] = {
	{"Id",(getter)EntityTypeIdGet,(setter)EntityTypeIdSet,"Entity ID"},
	{"Class",(getter)EntityTypeClassGet,(setter)EntityTypeClassSet,"Entity Class"},
	{NULL}  /* Sentinel */
};

PyTypeObject EntityType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.Entity",						// tp_name
		sizeof(EntityObject),				// tp_basicsize 
		0,									// tp_itemsize 
		(destructor)EntityTypeDel,			// tp_dealloc 
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
		"Entity",							// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		EntityMethods,						// tp_methods 
		0,									// tp_members 
		EntityProperties,					// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		(initproc)EntityTypeInit,			// tp_init 
		0,									// tp_alloc 
		PyType_GenericNew,					// tp_new 
};