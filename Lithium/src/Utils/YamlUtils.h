#pragma once
#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"


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

}


namespace Lithium
{
	YAML::Emitter& operator <<(YAML::Emitter& emitter, const glm::vec4& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
		return emitter;
	}
	YAML::Emitter& operator <<(YAML::Emitter& emitter, const glm::vec3& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return emitter;
	}

	YAML::Emitter& operator <<(YAML::Emitter& emitter, const glm::vec2& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << YAML::EndSeq;
		return emitter;
	}
}