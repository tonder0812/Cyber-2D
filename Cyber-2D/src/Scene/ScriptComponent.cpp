#include "pch.h"
#include "Components.h"
#include "Python/PyEntity.h"

namespace Cyber {
	ScriptComponent::ScriptComponent(const std::string& name_) :
		name(name_) {
		Initialize();
	}

	void ScriptComponent::Initialize() {
		m_name = name;
		initialized = false;
		pModule = PyImport_ImportModule(name.c_str());
		onStart = nullptr;
		onUpdate = nullptr;
		onDestroy = nullptr;
		if (pModule != NULL) {
			onStart = PythonUtils::GetFuncFromModule(pModule, "Start");
			onUpdate = PythonUtils::GetFuncFromModule(pModule, "Update");
			onDestroy = PythonUtils::GetFuncFromModule(pModule, "Destroy");
		}
		else {
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
			CB_ERROR("Failed to load \"{0}\"", name.c_str());
		}
	}

	void ScriptComponent::Destroy() {
		if (initialized && onDestroy) {
			PyObject_CallObject(onDestroy, NULL);
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
		}
		Py_XDECREF(onStart);
		Py_XDECREF(onUpdate);
		Py_XDECREF(onDestroy);
		Py_DECREF(pModule);
		PyObject* key = PyUnicode_FromString(m_name.c_str());
		PyDict_DelItem(PySys_GetObject("modules"), key);
		Py_DECREF(key);
		if (PyErr_Occurred())
			PythonUtils::GetErrorMessage();
	}
}