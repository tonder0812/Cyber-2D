#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OpenGL/OpenGLTexture.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Python/Python.h"
#include "Python/Utils.h"
#include "Core/Application.h"
#include "PyGLM/types/types.h"
#include "Python/PyComponents.h"
#include "SceneCamera.h"

namespace Cyber {

	struct TagComponent
	{
		std::string Id;
		std::string Class;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& id)
			: Id(id) {}
		TagComponent(const std::string& id, const std::string& _Class)
			: Id(id), Class(_Class) {}
	};

	struct OrderComponent
	{
		int order;
		OrderComponent(int order_)
			:order(order_) {}
	};

	struct TransformComponent
	{
		TransformComponentObject* Transform;
		TransformComponent() {
			Transform = (TransformComponentObject*)PyObject_CallObject(Application::Get().GetPyCyber_Transform(), NULL);
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
			CB_CORE_ASSERT(Transform, "Error when initializing Transform");
		}
		void Destroy() {
			Py_DECREF(Transform);
		}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Transform->Translation->super_type)
				* glm::rotate(glm::mat4(1.0f), Transform->Rotation, { 0,0,1 })
				* glm::scale(glm::mat4(1.0f), Transform->Scale->super_type);
		}

	};

	struct SpriteRendererComponent
	{
		SpriteRendererComponentObject* Texture;
		SpriteRendererComponent() {
			Texture = (SpriteRendererComponentObject*)PyObject_CallObject(Application::Get().GetPyCyber_SpriteRenderer(), NULL);
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
			CB_CORE_ASSERT(Texture, "Error when initializing Sprite");
		};
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		void Destroy() {
			Py_DECREF(Texture);
		}
	};

	struct CameraComponent
	{
		CameraComponentObject* Camera;
		CameraComponent() {
			Camera = (CameraComponentObject*)PyObject_CallObject(Application::Get().GetPyCyber_Camera(), NULL);
			if (PyErr_Occurred())
				CB_ERROR(PythonUtils::GetErrorMessage());
			CB_CORE_ASSERT(Camera, "Error when initializing Camera");
		};
		CameraComponent(const CameraComponent&) = default;
		void Destroy() {
			Py_DECREF(Camera);
		}
	};

	struct ScriptComponent
	{
		std::string name;
		ScriptComponentObject* Script;
		ScriptComponent() = default;
		ScriptComponent(const std::string& name);
		void Initialize();
		void Destroy();
		std::string GetName() { return m_name; };
	private:
		std::string m_name;
	};
}
