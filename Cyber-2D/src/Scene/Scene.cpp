#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Python/Utils.h"
#include "Utils/Utils.h"
#include "OpenGL/OpenGLRenderer.h"
#include <Imgui.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>

namespace Cyber {

	static void rotate(glm::vec3 a[], int n, float x_pivot, float y_pivot,
		float angle)
	{
		int i = 0;
		while (i < n) {
			int x_shifted = a[i].x - x_pivot;
			int y_shifted = a[i].y - y_pivot;
			a[i].x = x_pivot
				+ (x_shifted * glm::cos(angle)
					- y_shifted * glm::sin(angle));
			a[i].y = y_pivot
				+ (x_shifted * glm::sin(angle)
					+ y_shifted * glm::cos(angle));
			i++;
		}
	}

	Scene::Scene(bool empty)
	{
		if (!empty) {
			Entity MainCamera = CreateEntity("Main Camera","Camera");
			MainCamera.AddComponent<CameraComponent>();
		}
	}

	Scene::~Scene()
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , this };
				DestroyEntity(entity);
			});
	}

	Entity Scene::CreateEntity(const std::string& id)
	{
		return CreateEntity(id, "");
	}
	Entity Scene::CreateEntity(const std::string& id, const std::string& Class)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Id = id.empty() ? "Entity" : id;
		tag.Class = Class;
		entity.AddComponent<OrderComponent>(++m_Nentities);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		if (entity.HasComponent<TransformComponent>())entity.GetComponent<TransformComponent>().Destroy();
		if (entity.HasComponent<ScriptComponent>())entity.GetComponent<ScriptComponent>().Destroy();
		if (entity.HasComponent<SpriteRendererComponent>())entity.GetComponent<SpriteRendererComponent>().Destroy();
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(float ts, EditorCamera& editorCamera, std::shared_ptr<Texture> cameraTexture)
	{
		Renderer::BeginScene(editorCamera);
		RenderSprites();
		Renderer::DrawQuad({ 1000,1000 ,1000 }, { 1,1 }, cameraTexture.get());
		Renderer::EndScene();
		Renderer::BeginUI();
		Renderer::BeginScene(editorCamera);
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (cameraEntity) {
			SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			glm::vec3& position = cameraEntity.GetComponent<TransformComponent>().Transform->Translation->super_type;
			glm::vec3& scale = cameraEntity.GetComponent<TransformComponent>().Transform->Scale->super_type;
			glm::mat4 trans = glm::scale(glm::translate(glm::mat4(1), position), { 120 / 120.0f ,90 / 120.0f,1 });
			float farClip = editorCamera.GetPosition().z + editorCamera.GetFar() - 0;
			trans[3].z = farClip;
			Renderer::DrawQuad(trans, cameraTexture.get(), 1, glm::vec4(1), cameraEntity);
			glm::vec3 verts[] = {
				{position.x - camera.GetSize() * scale.x * camera.GetAspectRatio() * 0.5f, position.y - camera.GetSize() * scale.y * 0.5f, farClip},
				{position.x - camera.GetSize() * scale.x * camera.GetAspectRatio() * 0.5f, position.y + camera.GetSize() * scale.y * 0.5f, farClip},
				{position.x + camera.GetSize() * scale.x * camera.GetAspectRatio() * 0.5f, position.y + camera.GetSize() * scale.y * 0.5f, farClip},
				{position.x + camera.GetSize() * scale.x * camera.GetAspectRatio() * 0.5f, position.y - camera.GetSize() * scale.y * 0.5f, farClip},
			};

			rotate(verts, 4, position.x, position.y, cameraEntity.GetComponent<TransformComponent>().Transform->Rotation);
			Renderer::DrawLines(verts, 4);
		}
		Renderer::EndScene();
	}


	void Scene::OnUpdateRuntime(float ts)
	{
		auto ScriptView = m_Registry.view<ScriptComponent>();
		for (auto et : ScriptView)
		{
			auto& script = ScriptView.get<ScriptComponent>(et);
			if (!script.initialized) {
				script.initialized = true;
				if (script.onStart) {
					PyObject_CallObject(script.onStart, NULL);
					if (PyErr_Occurred())
						CB_ERROR(PythonUtils::GetErrorMessage());
				}
			}
			if (script.onUpdate) {
				PyObject* pArgs, * pValue;
				pArgs = PyTuple_New(2);
				pValue = PyFloat_FromDouble(ts);
				PyTuple_SetItem(pArgs, 0, pValue);

				PyObject* transform = (PyObject*)Entity(et, this).GetComponent<TransformComponent>().Transform;
				Py_INCREF(transform);
				PyTuple_SetItem(pArgs, 1, transform);

				PyObject_CallObject(script.onUpdate, pArgs);
				if (PyErr_Occurred())
					CB_ERROR(PythonUtils::GetErrorMessage());
				Py_DECREF(pArgs);
			}
		}
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (cameraEntity) {

			Renderer::BeginScene(Camera{ cameraEntity.GetComponent<CameraComponent>().Camera.GetProjection() , glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform()) });
			RenderSprites();
			Renderer::EndScene();
		}
	}

	void Scene::RenderSprites() {
		auto SpriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		SpriteGroup.sort<TransformComponent>([](const auto& lhs, const auto& rhs) {
			return lhs.Transform->Translation->super_type.z < rhs.Transform->Translation->super_type.z;
			});
		for (auto et : SpriteGroup)
		{
			auto [transform, sprite] = SpriteGroup.get<TransformComponent, SpriteRendererComponent>(et);
			if (sprite.UseTexture) {
				Renderer::DrawQuad(transform.GetTransform(), sprite.texture.get(), 1, sprite.Color->super_type, (int)et);
			}
			else {
				Renderer::DrawQuad(transform.GetTransform(), sprite.Color->super_type, (int)et);
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		ResizeCameras();

	}
	void Scene::ResizeCameras()
	{
		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	void Scene::SetPrimaryCamera(Entity e) {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			cameraComponent.Primary = false;
		}
		e.GetComponent<CameraComponent>().Primary = true;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}


	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<OrderComponent>(Entity entity, OrderComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}


}
