#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Python/Utils.h"
#include "Python/PyEntity.h"
#include "Utils/Utils.h"
#include "OpenGL/OpenGLRenderer.h"
#include <Imgui.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>
#include "Core/Input.h"

namespace Cyber {
	Scene* Scene::CurrentScene = nullptr;
	static void rotate(glm::vec3 points[], int n, float x_pivot, float y_pivot,
		float angle)
	{
		for (int i = 0; i < n; i++) {

			float x_shifted = points[i].x - x_pivot;
			float y_shifted = points[i].y - y_pivot;
			
			points[i].x =
				x_pivot
				+ x_shifted * glm::cos(angle)
				- y_shifted * glm::sin(angle);

			points[i].y = y_pivot
				+ x_shifted * glm::sin(angle)
				+ y_shifted * glm::cos(angle);
		}
	}

	Scene::Scene(bool empty)
	{
		if (!empty) {
			Entity MainCamera = CreateEntity("Main Camera", "Camera");
			CameraComponent& camera = MainCamera.AddComponent<CameraComponent>();
			camera.Camera->Primary = true;
		}
	}

	Scene::~Scene()
	{
		CurrentScene = this;
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
		CurrentScene = this;
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
		CurrentScene = this;
		if (entity.HasComponent<TransformComponent>())entity.GetComponent<TransformComponent>().Destroy();
		if (entity.HasComponent<CameraComponent>())entity.GetComponent<CameraComponent>().Destroy();
		if (entity.HasComponent<SpriteRendererComponent>())entity.GetComponent<SpriteRendererComponent>().Destroy();
		if (entity.HasComponent<ScriptComponent>())entity.GetComponent<ScriptComponent>().Destroy();
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(float ts, EditorCamera& editorCamera, std::shared_ptr<Texture> cameraTexture)
	{
		CurrentScene = this;
		Renderer::BeginScene(editorCamera);
		RenderSprites();
		Renderer::DrawQuad({ 1000,1000 ,1000 }, { 1,1 }, cameraTexture.get());
		Renderer::EndScene();
		Renderer::BeginUI();
		Renderer::BeginScene(editorCamera);
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (cameraEntity) {
			SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Camera->Camera;
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
		CurrentScene = this;
		auto ScriptView = m_Registry.view<ScriptComponent>();
		for (auto et : ScriptView)
		{
			auto& script = ScriptView.get<ScriptComponent>(et);
			if (!script.Script)
				continue;
			if (!script.Script->initialized) {
				script.Script->initialized = true;

				PyObject* pEntityType = Application::Get().GetPyCyber_Entity();
				EntityObject* entity = (EntityObject*)PyObject_CallObject(pEntityType, NULL);
				entity->m_Entity = Entity(et, this);
				PyModule_AddObject(script.Script->pModule, "this", (PyObject*)entity);

				if (script.Script->onStart) {
					PyObject_CallObject(script.Script->onStart, NULL);
					if (PyErr_Occurred())
						CB_ERROR(PythonUtils::GetErrorMessage());
				}
			}
			else if (script.Script->onUpdate) {
				PyObject* pArgs, * pValue;
				pArgs = PyTuple_New(1);
				pValue = PyFloat_FromDouble(ts);
				PyTuple_SetItem(pArgs, 0, pValue);

				PyObject_CallObject(script.Script->onUpdate, pArgs);
				if (PyErr_Occurred())
					CB_ERROR(PythonUtils::GetErrorMessage());
				Py_DECREF(pArgs);
			}
		}
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (cameraEntity) {
			glm::vec2 point = Input::GetMousePositionViewport();

			Renderer::BeginScene(Camera{ cameraEntity.GetComponent<CameraComponent>().Camera->Camera.GetProjection() , glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform()) });
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
			if (sprite.Texture->UseTexture) {
				Renderer::DrawQuad(transform.GetTransform(), sprite.Texture->texture->Texture.get(), 1, sprite.Texture->Color->super_type, (int)et);
			}
			else {
				Renderer::DrawQuad(transform.GetTransform(), sprite.Texture->Color->super_type, (int)et);
			}
		}
	}

	Entity Scene::FindById(std::string id) {
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const auto& Id = view.get<TagComponent>(entity).Id;
			if (id == Id)
				return Entity{ entity, this };
		}
		return {};
	}

	static void tokenize(std::string const& str, const char delim,
		std::vector<std::string>& out)
	{
		std::stringstream ss(str);

		std::string s;
		while (std::getline(ss, s, delim)) {
			out.push_back(s);
		}
	}

	std::vector<Entity> Scene::FindByClass(std::string Class) {
		auto view = m_Registry.view<TagComponent>();
		std::vector<Entity> entities;
		for (auto entity : view)
		{
			std::vector<std::string> Classes;
			tokenize(view.get<TagComponent>(entity).Class, ' ', Classes);
			for (std::string _Class : Classes)
				if (Class == _Class) {
					entities.push_back({ entity, this });
					break;
				}

		}
		return entities;
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		ResizeCameras();

	}
	void Scene::ResizeCameras()
	{
		CurrentScene = this;
		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.Camera->FixedAspectRatio)
				cameraComponent.Camera->Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	void Scene::SetPrimaryCamera(Entity e) {
		CurrentScene = this;
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			cameraComponent.Camera->Primary = false;
		}
		e.GetComponent<CameraComponent>().Camera->Primary = true;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Camera->Primary)
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
		component.Camera->entity = entity;
		component.Camera->Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		//component.Camera->Camera.SetAspectRatio(16 / 9.0f);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}


}
