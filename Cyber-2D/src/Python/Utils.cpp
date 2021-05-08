#include "pch.h"
#include "Python.h"
#include "Utils.h"

namespace Cyber::PythonUtils {

	PyObject* GetFuncFromModule(PyObject* pModule, const char* name) {
		PyObject* pFunc = PyObject_GetAttrString(pModule, name);
		if (pFunc && PyCallable_Check(pFunc)) {
			return pFunc;
		}

		if (PyErr_Occurred())
			CB_CORE_ERROR(PythonUtils::GetErrorMessage());

		CB_CORE_WARN("Cannot find function \"{0}\"", name);
		return nullptr;
	}

	std::string	GetErrorMessage() {
		std::string message;
		PyObject* type, * val, * tb;
		PyErr_Fetch(&type, &val, &tb);
		PyErr_NormalizeException(&type, &val, &tb);
		if (tb != NULL) {
			PyException_SetTraceback(val, tb);
		}
		PyObject* pModule = PyImport_ImportModule("traceback");
		PyObject* pTracebackFunc = PythonUtils::GetFuncFromModule(pModule, "format_exception");
		PyObject* args = PyTuple_New(3);
		PyTuple_SetItem(args, 0, type);
		PyTuple_SetItem(args, 1, val);
		if (tb) {
			PyTuple_SetItem(args, 2, tb);
		}
		else {
			PyTuple_SetItem(args, 2, Py_None);
		}
		PyObject* pValue = PyObject_CallObject(pTracebackFunc, args);
		int len = PyList_GET_SIZE(pValue);
		for (int i = 0; i < len; i++) {
			PyObject* tmp = PyUnicode_AsEncodedString(PyList_GetItem(pValue, i), "utf-8", "~E~");
			message.append(PyBytes_AS_STRING(tmp));
		}
		Py_DECREF(pValue);
		Py_DECREF(pModule);
		Py_DECREF(pTracebackFunc);
		Py_DECREF(args);
		message.pop_back();
		return message;
	}
}