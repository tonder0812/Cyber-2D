#include "pch.h"
#include "PyTexture.h"

static int TextureTypeInit(TextureObject* self, PyObject* args, PyObject* kwds) {
	
	const char* str=NULL;
	if (!PyArg_ParseTuple(args, "|s", &str)) {
		return NULL;
	}
	if (str) {
		self->Texture = std::make_shared<Cyber::Texture>(str);
		if (self->Texture->GetWidth() == 0) {
			PyErr_Format(PyExc_FileNotFoundError, "File %s not found", str);
			return -1;
		}
	}
	return 0;
}

static int TextureTypeDel(TextureObject* self, PyObject* args, PyObject* kwds) {
	
	self->Texture = nullptr;
	Py_TYPE(self)->tp_free((PyObject*)self);
	return 0;
}

PyTypeObject TextureType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.Texture",					// tp_name
		sizeof(TextureObject),				// tp_basicsize 
		0,									// tp_itemsize 
		(destructor)TextureTypeDel,			// tp_dealloc 
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
		"Texture",							// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		0,									// tp_methods 
		0,									// tp_members 
		0,									// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		(initproc)TextureTypeInit,			// tp_init 
		0,									// tp_alloc 
		PyType_GenericNew,					// tp_new 
};