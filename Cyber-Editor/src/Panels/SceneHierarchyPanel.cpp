#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Components.h"
#include "Utils/Utils.h"
#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Cyber {

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(Scene* context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		std::vector<Entity> entities;
		m_Context->m_Registry.each([&](auto entityID)
			{
				entities.push_back({ entityID , m_Context });
			});
		std::sort(entities.begin(), entities.end(), [](Entity& a, Entity& b) {
			return a.GetComponent<OrderComponent>().order < b.GetComponent<OrderComponent>().order;
			});
		for (Entity entity : entities)
			DrawEntityNode(entity);
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};


		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& ID = entity.GetComponent<TagComponent>().Id;
		bool selected = false;
		if (m_SelectionContext == entity) {
			selected = true;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		}
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0,0.5 });
		ImGui::PushID((void*)&ID);
		if (ImGui::Button(ID.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			m_SelectionContext = entity;
		}
		ImGui::PopStyleVar();
		if (selected) {
			ImGui::PopStyleColor(3);
		}
		if (ImGui::BeginPopupContextItem())
		{
			int& myOrder = entity.GetComponent<OrderComponent>().order;
			if (myOrder > 1 && ImGui::MenuItem("Move Up")) {
				auto view = m_Context->m_Registry.view<OrderComponent>();
				for (auto other : view) {
					int& otherOrder = view.get<OrderComponent>(other).order;
					if (otherOrder == myOrder - 1) {
						otherOrder++;
						myOrder--;
						break;
					}
				}
			}
			if (myOrder < m_Context->m_Nentities && ImGui::MenuItem("Move Down")) {
				auto view = m_Context->m_Registry.view<OrderComponent>();
				for (auto other : view) {
					int& otherOrder = view.get<OrderComponent>(other).order;
					if (otherOrder == myOrder + 1) {
						otherOrder--;
						myOrder++;
						break;
					}
				}
			}
			if (ImGui::MenuItem("Delete Entity")) {
				m_Context->DestroyEntity(entity);
				if (m_SelectionContext == entity)
					m_SelectionContext = {};
			}
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize * io.FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawFloatControl(const std::string& label, const std::string& btnLabel, float& value, float resetValue = 0.0f, bool expandAll = false, float width = 100)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);


		ImGui::SetColumnWidth(0, width);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		if (!expandAll) {
			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		}
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize * io.FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button(btnLabel.c_str(), buttonSize))
			value = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
		if (!expandAll) {
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();
		}
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool deletable = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			ImGuiIO& io = ImGui::GetIO();
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushID((void*)typeid(T).hash_code());
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize * io.FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }) && deletable)
			{
				ImGui::OpenPopup("ComponentSettings");
			}


			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();

			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();

			ImGuiStyle& style = ImGui::GetStyle();
			float width = ImGui::CalcTextSize("Class").x + style.ItemSpacing.x * 2;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tagComponent.Id.c_str(), sizeof(buffer));

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, width);
			ImGui::Text("ID");
			ImGui::NextColumn();

			if (ImGui::InputText("##ID", buffer, sizeof(buffer)))
			{
				tagComponent.Id = std::string(buffer);
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");
			ImGui::Columns(1);

			char buffer2[256];
			memset(buffer2, 0, sizeof(buffer2));
			std::strncpy(buffer2, tagComponent.Class.c_str(), sizeof(buffer2));

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, width);
			ImGui::Text("Class");
			ImGui::NextColumn();
			if (ImGui::InputText("##Class", buffer2, sizeof(buffer2)))
			{
				tagComponent.Class = std::string(buffer2);
			}
			ImGui::Columns(1);

		}



		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				if (!m_SelectionContext.HasComponent<CameraComponent>())
					m_SelectionContext.AddComponent<CameraComponent>();
				else {
					CB_CORE_WARN("This entity already has the Camera Component!");
					MessageBoxA(NULL, "This entity already has the Camera Component!", "Message", MB_OK | MB_ICONWARNING);
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				if (!entity.HasComponent<SpriteRendererComponent>())
					entity.AddComponent<SpriteRendererComponent>();
				else {
					CB_CORE_WARN("This entity already has the Sprite Renderer Component!");
					MessageBoxA(NULL, "This entity already has the Sprite Renderer Component!", "Message", MB_OK | MB_ICONWARNING);
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Script"))
			{
				if (!entity.HasComponent<ScriptComponent>())
					entity.AddComponent<ScriptComponent>();
				else {
					CB_CORE_WARN("This entity already has the Script Component!");
					MessageBoxA(NULL, "This entity already has the Script Component!", "Message", MB_OK | MB_ICONWARNING);

				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				ImGuiStyle& style = ImGui::GetStyle();
				float width = ImGui::CalcTextSize("Translation").x + style.ItemSpacing.x * 3;
				if (width < 100)
					width = 100;
				DrawVec3Control("Translation", component.Transform->Translation->super_type, 0, width);
				float rotation = glm::degrees(component.Transform->Rotation);
				DrawFloatControl("Rotation", "R", rotation, 0, false, width);
				component.Transform->Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Transform->Scale->super_type, 1.0f, width);
			}, false);

		DrawComponent<CameraComponent>("Camera", entity, [&](CameraComponent& component)
			{
				auto& camera = component.Camera;

				if (ImGui::Checkbox("Primary", &component.Camera->Primary) && component.Camera->Primary) {
					m_Context->SetPrimaryCamera(entity);
				}


				float Size = camera->Camera.GetSize();

				ImGuiStyle& style = ImGui::GetStyle();
				float width = ImGui::CalcTextSize("Size").x + style.ItemSpacing.x * 3;
				if (width < 100)
					width = 100;
				DrawFloatControl("Size", "S", Size, 10, true, width);
				camera->Camera.SetSize(Size);

				float Near = camera->Camera.GetNearClip();
				DrawFloatControl("Near", "N", Near, -1, true, width);
				camera->Camera.SetNearClip(Near);

				float Far = camera->Camera.GetFarClip();
				DrawFloatControl("Far", "F", Far, 1, true, width);
				camera->Camera.SetFarClip(Far);

				/*if (ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio))
					m_Context->ResizeCameras();

				if (component.FixedAspectRatio) {
					float AspectRatio = camera.GetAspectRatio();
					DrawFloatControl("Aspect Ratio", "AR", AspectRatio, 1);
					camera.SetAspectRatio(AspectRatio);
				}*/

			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", &component.Texture->Color->super_type.r);
				bool disabled = component.Texture->texture->Texture.get() == nullptr;
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (disabled ? 0.5f : 1.0f));
				ImGui::Checkbox("Use texture", &component.Texture->UseTexture);
				ImGui::PopStyleVar();
				ImGui::PopItemFlag();
				if (ImGui::Button("Load Image"))
				{
					std::string path = FileDialogs::OpenFile("Image Files(*.BMP;*.JPG;*.PNG)\0*.BMP;*.JPG;*.PNG\0All files (*.*)\0*.*\0");
					if (!path.empty()) {
						std::string relativePath = std::filesystem::relative(path, Application::Get().getCWD()).string();
						auto texture = std::make_shared<Texture>(relativePath);
						component.Texture->texture->Texture = texture;
					}
				}
			});


		DrawComponent<ScriptComponent>("Script", entity, [&](auto& component)
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, component.name.c_str(), sizeof(buffer));
				if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
				{
					component.name = std::string(buffer);
				}
				if (ImGui::Button("Refresh"))
				{
					auto view = m_Context->m_Registry.view<ScriptComponent>();
					for (auto entity : view)
					{
						auto& script = view.get<ScriptComponent>(entity);
						if (script.name == component.name) {
							script.Destroy();
							script.Initialize();
						}
					}
				}
			});
	}

}
