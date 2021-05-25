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
		vec<4, float>* Color;
		std::shared_ptr<Texture> texture;
		bool UseTexture = false;
		SpriteRendererComponent() {
			PyObject* pVec4Type = Application::Get().GetPyGLM_Vec4();
			Color = (vec<4, float> *)PyObject_CallObject(pVec4Type, NULL);
			Color->super_type = { 1,1,1,1 };
		};
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: texture(nullptr), UseTexture(false) {
			PyObject* pVec4Type = Application::Get().GetPyGLM_Vec4();
			Color = (vec<4, float> *)PyObject_CallObject(pVec4Type, NULL);
			Color->super_type = color;
		}
		SpriteRendererComponent(std::shared_ptr<Texture> tex)
			: texture(tex), UseTexture(true) {
			PyObject* pVec4Type = Application::Get().GetPyGLM_Vec4();
			Color = (vec<4, float> *)PyObject_CallObject(pVec4Type, NULL);
			Color->super_type = { 1,1,1,1 };
		}
		SpriteRendererComponent(const glm::vec4& color, std::shared_ptr<Texture> tex)
			: texture(tex), UseTexture(true) {
			PyObject* pVec4Type = Application::Get().GetPyGLM_Vec4();
			Color = (vec<4, float> *)PyObject_CallObject(pVec4Type, NULL);
			Color->super_type = color;
		}
		void Destroy() {
			Py_DECREF(Color);
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
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
