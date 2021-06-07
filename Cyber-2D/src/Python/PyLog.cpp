#include "pch.h"
#include "Python.h"
#include "Python/PyLog.h"

static char* ConvertToString(PyObject* args) {
	PyObject* obj = nullptr;
	if (!PyArg_ParseTuple(args, "O", &obj)) {
		return NULL;
	}
	PyObject* repr = PyObject_Str(obj);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	char* bytes = PyBytes_AS_STRING(str);
	Py_DECREF(repr);
	Py_DECREF(str);
	return bytes;
}

static PyObject* LogTypeError(PyObject* self, PyObject* args) {
	char* str = ConvertToString(args);
	if (str) {
		CB_ERROR(str);
		Py_RETURN_NONE;
	}
	else {
		return NULL;
	}
}

static PyObject* LogTypeWarn(PyObject* self, PyObject* args) {
	char* str = ConvertToString(args);
	if (str) {
		CB_WARN(str);
		Py_RETURN_NONE;
	}
	else {
		return NULL;
	}
}

static PyObject* LogTypeInfo(PyObject* self, PyObject* args) {
	char* str = ConvertToString(args);
	if (str) {
		CB_INFO(str);
		Py_RETURN_NONE;
	}
	else {
		return NULL;
	}
}

static PyObject* LogTypeTrace(PyObject* self, PyObject* args) {
	char* str = ConvertToString(args);
	if (str) {
		CB_TRACE(str);
		Py_RETURN_NONE;
	}
	else {
		return NULL;
	}
}

static PyMethodDef LogMethods[] = {
	{"Error", (PyCFunction)LogTypeError, METH_STATIC | METH_VARARGS,
	 "Log a error message to the console"
	},
	{"Warn", (PyCFunction)LogTypeWarn, METH_STATIC | METH_VARARGS,
	 "Log a warning to the console"
	},
	{"Info", (PyCFunction)LogTypeInfo, METH_STATIC | METH_VARARGS,
	 "Log info to the console"
	},
	{"Trace", (PyCFunction)LogTypeTrace, METH_STATIC | METH_VARARGS,
	 "Log a trace to the console"
	},
	{"Log", (PyCFunction)LogTypeTrace, METH_STATIC | METH_VARARGS,
	 "Log a trace to the console"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject LogType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.Log",						// tp_name
		sizeof(LogObject),					// tp_basicsize 
		0,									// tp_itemsize 
		0,									// tp_dealloc 
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
		"Log class to group all Logging functions",	// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		LogMethods,							// tp_methods 
		0,									// tp_members 
		0,									// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		0,									// tp_init 
		0,									// tp_alloc 
		0,									// tp_new 
};