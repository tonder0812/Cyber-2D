#include "pch.h"
#include "PyComponents.h"
#include "Core/Application.h"

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
		"Simple transform comonent",		// tp_doc 
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