#pragma once
#include <string>
#include "Python.h"

namespace Cyber::PythonUtils {
	std::string	GetErrorMessage();
	PyObject* GetFuncFromModule(PyObject* pModule, const char* name);
}