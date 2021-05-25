#include "pch.h"
#include "PyEntity.h"
#include <structmember.h>
#include "Scene/Components.h"

static int EntityTypeInit(EntityObject* self, PyObject* args, PyObject* kwds) {
	CB_CORE_INFO("Entity init");
	return 0;
}

static int EntityTypeDel(EntityObject* self, PyObject* args, PyObject* kwds) {
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

static PyObject* EntityTypeGetTransform(EntityObject* self, PyObject* args, PyObject* kwds) {
	if (self->m_Entity.HasComponent<Cyber::TransformComponent>()) {
		Cyber::TransformComponent& transform = self->m_Entity.GetComponent<Cyber::TransformComponent>();
		Py_INCREF(transform.Transform);
		return (PyObject*)transform.Transform;
	}
	else {
		Py_RETURN_NONE;
	}
}

static PyObject* EntityTypeIdGet(EntityObject* self) {
	std::string& id = self->m_Entity.GetComponent<Cyber::TagComponent>().Id;
	return PyUnicode_FromString(id.c_str());
}

static int EntityTypeIdSet(EntityObject* self, PyObject* value) {
	PyObject* repr = PyObject_Str(value);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	char* bytes = PyBytes_AS_STRING(str);
	self->m_Entity.GetComponent<Cyber::TagComponent>().Id = std::string(bytes);
	Py_DECREF(str);
	Py_DECREF(repr);
	return 0;
}

static PyObject* EntityTypeClassGet(EntityObject* self) {
	std::string& Class = self->m_Entity.GetComponent<Cyber::TagComponent>().Class;
	return PyUnicode_FromString(Class.c_str());
}

static int EntityTypeClassSet(EntityObject* self, PyObject* value) {
	PyObject* repr = PyObject_Str(value);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	char* bytes = PyBytes_AS_STRING(str);
	self->m_Entity.GetComponent<Cyber::TagComponent>().Class = std::string(bytes);
	Py_DECREF(str);
	Py_DECREF(repr);
	return 0;
}

PyMethodDef EntityMethods[] = {
	{"GetTransform", (PyCFunction)EntityTypeGetTransform, METH_NOARGS,
	 "Return entities transform"
	},
	{NULL}  /* Sentinel */
};

PyGetSetDef EntityProperties[] = {
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