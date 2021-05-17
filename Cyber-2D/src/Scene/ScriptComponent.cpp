#include "pch.h"
#include "Components.h"

namespace Cyber {
	ScriptComponent::ScriptComponent(const std::string& name_) :
		name(name_) {
		Initialize();
	}

	void ScriptComponent::Initialize() {
		m_name = name;
		initialized = false;
		PyObject* pModule;
		pModule = PyImport_ImportModule(name.c_str());
		onStart = nullptr;
		onUpdate = nullptr;
		onDestroy = nullptr;
		if (pModule != NULL) {
			PyModule_AddObject(pModule, "testt", PyFloat_FromDouble(10));
			onStart = PythonUtils::GetFuncFromModule(pModule, "Start");
			onUpdate = PythonUtils::GetFuncFromModule(pModule, "Update");
			onDestroy = PythonUtils::GetFuncFromModule(pModule, "Destroy");
			Py_DECREF(pModule);
		}
		else {
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
			CB_ERROR("Failed to load \"{0}\"", name.c_str());
		}
	}

	void ScriptComponent::Destroy() {
		if (initialized && onDestroy)
			PyObject_CallObject(onDestroy, NULL);
		Py_XDECREF(onStart);
		Py_XDECREF(onUpdate);
		Py_XDECREF(onDestroy);
		PyObject* key = PyUnicode_FromString(m_name.c_str());
		PyDict_DelItem(PySys_GetObject("modules"), key);
		Py_DECREF(key);
		if (PyErr_Occurred())
			PythonUtils::GetErrorMessage();
	}
}