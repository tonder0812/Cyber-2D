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
		Script = (ScriptComponentObject*)PyObject_CallObject(Application::Get().GetPyCyber_Script(), NULL);
		if (PyErr_Occurred())
			CB_CORE_ERROR(PythonUtils::GetErrorMessage());
		CB_CORE_ASSERT(Script, "Error when initializing Transform");
		Script->initialized = false;
		Script->pModule = PyImport_ImportModule(name.c_str());
		Script->onStart = nullptr;
		Script->onUpdate = nullptr;
		Script->onDestroy = nullptr;
		if (Script->pModule != NULL) {
			Script->onStart = PythonUtils::GetFuncFromModule(Script->pModule, "Start", true);
			Script->onUpdate = PythonUtils::GetFuncFromModule(Script->pModule, "Update", true);
			Script->onDestroy = PythonUtils::GetFuncFromModule(Script->pModule, "Destroy", true);
			Script->Public = PythonUtils::GetVarFromModule(Script->pModule, "Public", true);
			PyObject* key = PyUnicode_FromString(m_name.c_str());
			PyDict_DelItem(PySys_GetObject("modules"), key);
			Py_DECREF(key);
			if (PyErr_Occurred())
				PythonUtils::GetErrorMessage();
		}
		else {
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
			//CB_ERROR("Failed to load \"{0}\"", name.c_str());
			Py_DECREF(Script);
			Script = nullptr;
		}
	}

	void ScriptComponent::Destroy() {
		if (!Script) {
			return;
		}
		if (Script->initialized && Script->onDestroy) {
			PyObject_CallObject(Script->onDestroy, NULL);
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
		}
		Py_DECREF(Script);
	}
}