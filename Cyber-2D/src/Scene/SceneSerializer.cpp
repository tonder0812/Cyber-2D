#include "pch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}
namespace Cyber {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(Scene& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity"; // TODO: Entity ID goes here

		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();
			out << YAML::Value << tagComponent.Id;

			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			out << YAML::Key << "Class" << YAML::Value << tagComponent.Class;

			out << YAML::EndMap; // TagComponent
		}
		else {
			out << YAML::Value << "";
		}


		if (entity.HasComponent<OrderComponent>())
		{
			out << YAML::Key << "OrderComponent";
			out << YAML::BeginMap; // OrderComponent

			auto& order = entity.GetComponent<OrderComponent>().order;
			out << YAML::Key << "Order" << YAML::Value << order;

			out << YAML::EndMap; // OrderComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Transform->Translation->super_type;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Transform->Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Transform->Scale->super_type;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera;
			out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();
			out << YAML::Key << "AspectRatio" << YAML::Value << cameraComponent.Camera.GetAspectRatio();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color->super_type;
			out << YAML::Key << "UseTexture" << YAML::Value << spriteRendererComponent.UseTexture;
			out << YAML::Key << "Texture" << YAML::Value << (spriteRendererComponent.texture.get() ? spriteRendererComponent.texture->GetPath() : std::string(""));

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // ScriptComponent

			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "Name" << YAML::Value << scriptComponent.GetName();

			out << YAML::EndMap; // ScriptComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene.m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, &m_Scene };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath, std::fstream::out);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CB_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				std::string id = entity["Entity"].as<std::string>();

				std::string Class;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					Class = tagComponent["Class"].as<std::string>();

				CB_CORE_TRACE("Deserialized entity with ID = {0}, Class = {1}", id, Class);

				Entity deserializedEntity = m_Scene.CreateEntity(id, Class);

				auto orderComponent = entity["OrderComponent"];
				if (orderComponent)
					deserializedEntity.GetComponent<OrderComponent>().order = orderComponent["Order"].as<int>();

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Transform->Translation->super_type = transformComponent["Translation"].as<glm::vec3>();
					tc.Transform->Rotation = transformComponent["Rotation"].as<float>();
					tc.Transform->Scale->super_type = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];

					cc.Camera.SetSize(cameraProps["Size"].as<float>());
					cc.Camera.SetNearClip(cameraProps["Near"].as<float>());
					cc.Camera.SetFarClip(cameraProps["Far"].as<float>());
					cc.Camera.SetAspectRatio(cameraProps["AspectRatio"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color->super_type = spriteRendererComponent["Color"].as<glm::vec4>();
					src.UseTexture = spriteRendererComponent["UseTexture"].as<bool>();
					std::string path = spriteRendererComponent["Texture"].as<std::string>();
					if (!path.empty())
						src.texture = std::make_shared<Texture>(path);
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent) {
					deserializedEntity.AddComponent<ScriptComponent>(scriptComponent["Name"].as<std::string>());
				}
			}
		}

		return true;
	}
}
