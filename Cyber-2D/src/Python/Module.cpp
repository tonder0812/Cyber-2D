#include "pch.h"
#include "Python/Python.h"
#include "Python/PyInput.h"
#include "Python/PyComponents.h"
#include "Python/PyLog.h"

PyModuleDef CyberModule = {
	PyModuleDef_HEAD_INIT,
	"Cyber",
	"Cyber module.",
	-1,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

extern "C" {
	PyObject* PyInit_Cyber(void)
	{
		PyObject* m;

		if (PyType_Ready(&InputType) < 0) return NULL;
		if (PyType_Ready(&LogType) < 0) return NULL;
		if (PyType_Ready(&TransformComponentType) < 0) return NULL;


		m = PyModule_Create(&CyberModule);
		if (m == NULL)
			return NULL;

		Py_INCREF(&InputType);
		if (PyModule_AddObject(m, "Input", (PyObject*)&InputType) < 0) {
			Py_DECREF(&InputType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&LogType);
		if (PyModule_AddObject(m, "Log", (PyObject*)&LogType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&TransformComponentType);
		if (PyModule_AddObject(m, "TransformComponent", (PyObject*)&TransformComponentType) < 0) {
			Py_DECREF(&TransformComponentType);
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(m);
			return NULL;
		}

		return m;
	}
}