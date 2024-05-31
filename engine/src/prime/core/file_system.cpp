
#include "pch.h"
#include "file_system.h"
#include "prime/scene/entity.h"
#include "prime/scene/components.h"
#include "prime/core/assert.h"
#include "prime/core/resource_manager.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

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

	template<>
	struct convert<prime::Guid>
	{
		static Node encode(const prime::Guid& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, prime::Guid& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};

}

namespace prime {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

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

	static std::string RigidBody2DBodyTypeToString(BodyType bodyType)
	{
		switch (bodyType)
		{
		case BodyType::Static:    return "Static"; break;
		case BodyType::Dynamic:   return "Dynamic"; break;
		case BodyType::Kinematic: return "Kinematic"; break;
		}
		return {};
	}

	static BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static") { return BodyType::Static; }
		if (bodyTypeString == "Dynamic") { return BodyType::Dynamic; }
		if (bodyTypeString == "Kinematic") { return BodyType::Kinematic; }
		return BodyType::Static;
	}

	static void SaveEntity(YAML::Emitter& out, Entity entity)
	{
		P_ASSERT(entity.HasComponent<IDComponent>());

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().guid;

		// name component
		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap; // NameComponent

			auto& name = entity.GetComponent<NameComponent>().name;
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap; // NameComponent
		}

		// transform component
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.scale;

			out << YAML::EndMap; // TransformComponent
		}

		// camera component
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;

			out << YAML::EndMap; // CameraComponent
		}

		// sprite component
		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent

			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteComponent.color;

			if (spriteComponent.texture.get())
				out << YAML::Key << "TexturePath" << YAML::Value << spriteComponent.texture->GetPath();

			out << YAML::EndMap; // SpriteComponent
		}

		// line component
		if (entity.HasComponent<LineComponent>())
		{
			out << YAML::Key << "LineComponent";
			out << YAML::BeginMap; // LineComponent

			auto& lineComponent = entity.GetComponent<LineComponent>();
			out << YAML::Key << "Color" << YAML::Value << lineComponent.color;
			out << YAML::Key << "End" << YAML::Value << lineComponent.endPosition;

			out << YAML::EndMap; // LineComponent
		}

		// rect component
		if (entity.HasComponent<RectComponent>())
		{
			out << YAML::Key << "RectComponent";
			out << YAML::BeginMap; // RectComponent

			auto& rectComponent = entity.GetComponent<RectComponent>();
			out << YAML::Key << "Color" << YAML::Value << rectComponent.color;

			out << YAML::EndMap; // RectComponent
		}

		// rigidbody
		if (entity.HasComponent<RigidbodyComponent>())
		{
			out << YAML::Key << "RigidbodyComponent";
			out << YAML::BeginMap; // RigidbodyComponent

			auto& rb2dComponent = entity.GetComponent<RigidbodyComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.fixedRotation;
			out << YAML::Key << "Density" << YAML::Value << rb2dComponent.density;
			out << YAML::Key << "Friction" << YAML::Value << rb2dComponent.friction;
			out << YAML::Key << "Restitution" << YAML::Value << rb2dComponent.restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << rb2dComponent.restitutionThreshold;

			out << YAML::EndMap; // RigidbodyComponent
		}

		// box collider
		if (entity.HasComponent<BoxColliderComponent>())
		{
			out << YAML::Key << "BoxColliderComponent";
			out << YAML::BeginMap; // BoxColliderComponent

			auto& bc2dComponent = entity.GetComponent<BoxColliderComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.offset;
			out << YAML::Key << "Size" << YAML::Value << bc2dComponent.size;

			out << YAML::EndMap; // BoxColliderComponent
		}

		out << YAML::EndMap; // Entity
	}

	static void LoadEntity(Ref<Scene>& scene, YAML::Node data)
	{
		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				// id component
				ui64 uuid = entity["Entity"].as<ui64>();

				// Name Component
				std::string name;
				auto nameComponent = entity["NameComponent"];
				if (nameComponent) { name = nameComponent["Name"].as<std::string>(); }

				Entity deserializedEntity = scene->CreateEntityWithGuid(uuid, name);

				// Transform component
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.position = transformComponent["Position"].as<glm::vec2>();
					tc.rotation = transformComponent["Rotation"].as<float>();
					tc.scale = transformComponent["Scale"].as<glm::vec2>();
				}

				// Camera component
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.camera.SetSize(cameraProps["Size"].as<float>());
					cc.camera.SetNearClip(cameraProps["Near"].as<float>());
					cc.camera.SetFarClip(cameraProps["Far"].as<float>());

					cc.primary = cameraComponent["Primary"].as<bool>();
				}

				// Sprite Component
				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteComponent>();
					src.color = spriteComponent["Color"].as<glm::vec4>();

					if (spriteComponent["TexturePath"])
					{
						std::string texturePath = spriteComponent["TexturePath"].as<std::string>();
						src.texture = ResourceManager::LoadTexture(texturePath);
					}
				}

				// Line Component
				auto lineComponent = entity["LineComponent"];
				if (lineComponent)
				{
					auto& lc = deserializedEntity.AddComponent<LineComponent>();
					lc.color = lineComponent["Color"].as<glm::vec4>();
					lc.endPosition = lineComponent["End"].as<glm::vec2>();
				}

				// Rect Component
				auto rectComponent = entity["RectComponent"];
				if (rectComponent)
				{
					auto& rc = deserializedEntity.AddComponent<RectComponent>();
					rc.color = rectComponent["Color"].as<glm::vec4>();
				}

				auto rigidbodyComponent = entity["RigidbodyComponent"];
				if (rigidbodyComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<RigidbodyComponent>();
					rb2d.type = RigidBody2DBodyTypeFromString(rigidbodyComponent["BodyType"].as<std::string>());
					rb2d.fixedRotation = rigidbodyComponent["FixedRotation"].as<bool>();
					rb2d.density = rigidbodyComponent["Density"].as<float>();
					rb2d.friction = rigidbodyComponent["Friction"].as<float>();
					rb2d.restitution = rigidbodyComponent["Restitution"].as<float>();
					rb2d.restitutionThreshold = rigidbodyComponent["RestitutionThreshold"].as<float>();
				}

				// BoxCollider Component
				auto boxColliderComponent = entity["BoxColliderComponent"];
				if (boxColliderComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxColliderComponent>();
					bc2d.offset = boxColliderComponent["Offset"].as<glm::vec2>();
					bc2d.size = boxColliderComponent["Size"].as<glm::vec2>();
					
				}

			}
		}
	}

	void FileSystem::SaveScene(Ref<Scene>& scene, const std::string& path, const std::string& name)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		entt::basic_view entities = scene->m_registry.view<TransformComponent>();
		for (entt::entity entityID : entities)
		{
			Entity entity = { entityID, scene.get() };
			if (!entity) { return; }
			SaveEntity(out, entity);
		}
		
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	b8 FileSystem::LoadScene(Ref<Scene>& scene, const std::string& path)
	{
		YAML::Node data;
		if (!path.empty())
		{
			data = YAML::LoadFile(path);

			if (!data["Scene"]) { return false; }

			std::string sceneName = data["Scene"].as<std::string>();

			LoadEntity(scene, data);
			return true;
		}
		return false;
	}
}