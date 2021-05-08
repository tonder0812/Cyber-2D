#include "pch.h"
#include "Components.h"

namespace Cyber {
	ScriptComponent::ScriptComponent(std::string filepath) {
		PyObject * pModule;
		pModule = PyImport_ImportModule(filepath.c_str());
		onStart = nullptr;
		onUpdate = nullptr;
		onDestroy = nullptr;
		if (pModule != NULL) {
			onStart = PythonUtils::GetFuncFromModule(pModule, "Start");
			onUpdate = PythonUtils::GetFuncFromModule(pModule, "Update");
			onDestroy = PythonUtils::GetFuncFromModule(pModule, "Destroy");
			Py_DECREF(pModule);
		}
		else {
			if (PyErr_Occurred())
				CB_CORE_ERROR(PythonUtils::GetErrorMessage());
			CB_CORE_ERROR("Failed to load \"{0}\"", filepath.c_str());
		}
	}

	void ScriptComponent::Destroy() {
		if(onDestroy)
			PyObject_CallObject(onDestroy, NULL);
		Py_XDECREF(onStart);
		Py_XDECREF(onUpdate);
		Py_XDECREF(onDestroy);
	}
}