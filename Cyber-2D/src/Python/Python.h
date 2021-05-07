#pragma once
#pragma once
#define PY_SSIZE_T_CLEAN

#ifdef _DEBUG
#define RESTORE_DEBUG
#undef _DEBUG
#endif
#include <Python.h>
#ifdef RESTORE_DEBUG
#define _DEBUG
#undef RESTORE_DEBUG
#endif

//Python related Macros
#define Py_Import_Module(X) PyImport_AppendInittab(#X,&PyInit_##X)