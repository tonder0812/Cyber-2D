#include "pch.h"
#include "Python.h"
#include "Python/PyScene.h"
#include "Core/Application.h"
#include "Scene/Scene.h"
#include "Python/PyEntity.h"

static PyObject* SceneTypeFindByID(PyObject* self, PyObject* args) {
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str)) {
		return NULL;
	}
	Cyber::Entity e = Cyber::Scene::CurrentScene->FindById(str);
	if (e) {
		EntityObject* et = (EntityObject*)PyObject_CallObject(Cyber::Application::Get().GetPyCyber_Entity(), NULL);
		et->m_Entity = e;
		return (PyObject*)et;
	}
	else {
		Py_RETURN_NONE;
	}
}

static PyObject* SceneTypeFindByClass(PyObject* self, PyObject* args) {
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str)) {
		return NULL;
	}
	std::vector<Cyber::Entity> entities = Cyber::Scene::CurrentScene->FindByClass(str);
	PyListObject* list = (PyListObject*)PyList_New(entities.size());
	for (int i = 0; i < entities.size(); i++) {
		EntityObject* et = (EntityObject*)PyObject_CallObject(Cyber::Application::Get().GetPyCyber_Entity(), NULL);
		et->m_Entity = entities[i];
		PyList_SetItem((PyObject*)list, i, (PyObject*)et);
	}
	return (PyObject*)list;
}

static PyObject* SceneTypeCreateEntity(PyObject* self, PyObject* args) {
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str)) {
		return NULL;
	}
	Cyber::Entity e = Cyber::Scene::CurrentScene->CreateEntity(str);
	EntityObject* et = (EntityObject*)PyObject_CallObject(Cyber::Application::Get().GetPyCyber_Entity(), NULL);
	et->m_Entity = e;
	return (PyObject*)et;
}


PyMethodDef SceneMethods[] = {
	{"FindByID", (PyCFunction)SceneTypeFindByID, METH_STATIC | METH_VARARGS,
	 "Find a entity with ID in the current Scene"
	},
	{"FindByClass", (PyCFunction)SceneTypeFindByClass, METH_STATIC | METH_VARARGS,
	 "Find a entities with Class in the current Scene"
	},
	{"CreateEntity", (PyCFunction)SceneTypeCreateEntity , METH_STATIC | METH_VARARGS,
	 "Creates a new entity with ID in the current Scene"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject SceneType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.Scene",						// tp_name
		sizeof(SceneObject),				// tp_basicsize 
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
		"Scene class to interact with the current scene",// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		SceneMethods,						// tp_methods 
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