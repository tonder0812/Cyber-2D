#include "pch.h"
#include "Python/Python.h"
#include "Python/PyInput.h"
#include "Python/PyComponents.h"
#include "Python/PyEntity.h"
#include "Python/PyScene.h"
#include "Python/PyLog.h"
#include "Python/PyTexture.h"
#include "Utils.h"

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

		if (PyType_Ready(&TextureType) < 0) return NULL;
		if (PyType_Ready(&InputType) < 0) return NULL;
		if (PyType_Ready(&LogType) < 0) return NULL;
		if (PyType_Ready(&EntityType) < 0) return NULL;
		if (PyType_Ready(&SceneType) < 0) return NULL;
		if (PyType_Ready(&TransformComponentType) < 0) return NULL;
		if (PyType_Ready(&SpriteRendererComponentType) < 0) return NULL;
		if (PyType_Ready(&CameraComponentType) < 0) return NULL;
		if (PyType_Ready(&ScriptComponentType) < 0) return NULL;


		m = PyModule_Create(&CyberModule);
		if (m == NULL)
			return NULL;

		Py_INCREF(&TextureType);
		if (PyModule_AddObject(m, "Texture", (PyObject*)&TextureType) < 0) {
			Py_DECREF(&TextureType);
			Py_DECREF(m);
			return NULL;
		}

		Py_INCREF(&InputType);
		if (PyModule_AddObject(m, "Input", (PyObject*)&InputType) < 0) {
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(m);
			return NULL;
		}

		Py_INCREF(&LogType);
		if (PyModule_AddObject(m, "Log", (PyObject*)&LogType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&EntityType);
		if (PyModule_AddObject(m, "Entity", (PyObject*)&EntityType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(&EntityType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&SceneType);
		if (PyModule_AddObject(m, "Scene", (PyObject*)&SceneType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(&EntityType);
			Py_DECREF(&SceneType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&TransformComponentType);
		if (PyModule_AddObject(m, "TransformComponent", (PyObject*)&TransformComponentType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(&EntityType);
			Py_DECREF(&SceneType);
			Py_DECREF(&TransformComponentType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&CameraComponentType);
		if (PyModule_AddObject(m, "CameraComponent", (PyObject*)&CameraComponentType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(&EntityType);
			Py_DECREF(&SceneType);
			Py_DECREF(&TransformComponentType);
			Py_DECREF(&CameraComponentType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&SpriteRendererComponentType);
		if (PyModule_AddObject(m, "SpriteRendererComponent", (PyObject*)&SpriteRendererComponentType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(&EntityType);
			Py_DECREF(&SceneType);
			Py_DECREF(&TransformComponentType);
			Py_DECREF(&CameraComponentType);
			Py_DECREF(&SpriteRendererComponentType);
			Py_DECREF(m);
			return NULL;
		}
		Py_INCREF(&ScriptComponentType);
		if (PyModule_AddObject(m, "ScriptComponent", (PyObject*)&ScriptComponentType) < 0) {
			Py_DECREF(&LogType);
			Py_DECREF(&InputType);
			Py_DECREF(&TextureType);
			Py_DECREF(&EntityType);
			Py_DECREF(&SceneType);
			Py_DECREF(&TransformComponentType);
			Py_DECREF(&CameraComponentType);
			Py_DECREF(&SpriteRendererComponentType);
			Py_DECREF(&ScriptComponentType);
			Py_DECREF(m);
			return NULL;
		}

		return m;
	}
}