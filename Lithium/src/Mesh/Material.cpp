#include "lipch.h"
#include "Material.h"
#include "Utils/OpenglUtils.h"
#include "yaml-cpp/yaml.h"
#include "Utils/YamlUtils.h"

namespace Lithium
{
	typedef uint32_t Asset;

	Material::Material()
	{

	}
	Material::~Material()
	{

	}

	bool Material::MaterialToFile(const Ref<Material>& material, const std::string& path)
	{
		YAML::Emitter emitter;
	
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginSeq;

		for (Ref<MaterialData> field : material->GetDataFields())
		{
			emitter << YAML::BeginMap;
			if (field->getType() == MaterialDataType::Vec2)
			{
				Vec3* vec2 = static_cast<Vec3*>(field.get());
				emitter << YAML::Key << "attr" << YAML::Value << vec2->name;
				emitter << YAML::Key << "value" << YAML::Value << vec2->Value;
				emitter << YAML::Key << "type" << YAML::Value << "Vec2";
			}

			else
			if (field->getType() == MaterialDataType::Vec3)
			{

				Vec3* vec3 = static_cast<Vec3*>(field.get());
				emitter << YAML::Key << "attr" << YAML::Value << vec3->name;
				emitter << YAML::Key << "value" << YAML::Value << vec3->Value;
				emitter << YAML::Key << "type" << YAML::Value << "Vec3";

			}
			else
			if (field->getType() == MaterialDataType::Vec4)
			{
				Vec4* vec4 = static_cast<Vec4*>(field.get());
				emitter << YAML::Key << "attr" << YAML::Value << vec4->name;
				emitter << YAML::Key << "value" << YAML::Value << vec4->Value;
				emitter << YAML::Key << "type" << YAML::Value << "Vec4";

			}
			else
			if (field->getType() == MaterialDataType::TexturePath)
			{
				TexturePath* path = static_cast<TexturePath*>(field.get());
				emitter << YAML::Key << "attr" << YAML::Value << path->name;
				emitter << YAML::Key << "value" << YAML::Value << path->Value;
				emitter << YAML::Key << "type" << YAML::Value << "Path";

			}
			emitter << YAML::EndMap;
		}
		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
		std::ofstream output(path);
		output << emitter.c_str();
		return true;
	}

	Ref<Material> Material::MaterialFromFile(const std::string& path)
	{
		Ref<Material> material = CreateRef<Material>();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::ParserException e)
		{
			
			CORE_LOG("failed to load material");
			return material;
		}

		for (auto d : data["values"])
		{
			std::string name = d["attr"].as<std::string>();
			
			std::string type = d["type"].as<std::string>();
			if (type == "Vec2")
			{
				glm::vec2 value = d["value"].as<glm::vec2>();
				Ref<Vec2> vec2 = CreateRef<Vec2>();
				vec2->name = name;
				vec2->Value = value;
				material->PushDataField(vec2);
			}
			else
			if (type == "Vec3")
			{
				glm::vec3 value = d["value"].as<glm::vec3>();
				Ref<Vec3> vec3 = CreateRef<Vec3>();
				vec3->name = name;
				vec3->Value = value;

				material->PushDataField(vec3);

			}
			else
			if (type == "Vec4")
			{
				glm::vec4 value = d["value"].as<glm::vec4>();
				Ref<Vec4> vec4 = CreateRef<Vec4>();
				vec4->name = name;
				vec4->Value = value;

				material->PushDataField(vec4);

			}
			else
			if (type == "Path")
			{
				std::string value = d["value"].as<std::string>();
				Ref<TexturePath> path = CreateRef<TexturePath>();
				path->name = name;
				path->Value = value;
				material->PushDataField(path);
			}
		
		}


		return material;

	}

	Ref<Material> Material::MaterialFromShader(const Ref<Shader>& shader)
	{
		
		Ref<Material> material = CreateRef<Material>();
		for (size_t i = 0; i < shader->GetAttribCount(); i++)
		{
			ShaderDataInfo info = shader->GetAttribute(i);
			uint32_t t = info.DataType;
			MaterialDataType type = Utils::Opengl::GltToMdt(t);
			

			if (type == MaterialDataType::Vec2)
			{
				Ref<Vec2> vec2 = CreateRef<Vec2>();
				vec2->name = info.Name;
				material->PushDataField(vec2);
			}

			else if (type == MaterialDataType::Vec3)
			{
				Ref<Vec3> vec3 = CreateRef<Vec3>();
				vec3->Value = glm::vec3(0.0);
				vec3->name = info.Name;
				material->PushDataField(vec3);
			}

			else if (type == MaterialDataType::Vec4)
			{
				Ref<Vec4> vec4 = CreateRef<Vec4>();
				vec4->name = info.Name;
				vec4->Value = glm::vec4(0,0,0,1);

				material->PushDataField(vec4);
			}

			else if (type == MaterialDataType::TexturePath)
			{
				Ref<TexturePath> texpath = CreateRef<TexturePath>();
				texpath->name = info.Name;
				material->PushDataField(texpath);
			}
		}

		return material;
	}

	

}