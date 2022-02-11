#pragma once
#include "Renderer/Shader.h"



namespace Lithium
{

	enum class MaterialDataType
	{
		NONE = 0,
		Vec2,
		Vec3,
		Vec4,
		TexturePath
	};

#define DATA_TYPE(t) static MaterialDataType GetDataType() { return MaterialDataType::t; }\
								virtual MaterialDataType getType() const override { return GetDataType(); }

	class MaterialData
	{
		
	public:
		MaterialData()
		{}
		virtual ~MaterialData() = default;
		virtual MaterialDataType getType() const = 0;
	};

	class Vec2 : public MaterialData
	{
	public:

		Vec2()
		{}
		DATA_TYPE(Vec2);
		std::string name;
		glm::vec2 Value;
	};

	class Vec3 : public MaterialData
	{
	public:

		Vec3()
		{}
		DATA_TYPE(Vec3);
		std::string name;
		glm::vec3 Value;
	};

	class Vec4 : public MaterialData
	{
	public:

		Vec4()
		{}
		DATA_TYPE(Vec4);
		std::string name;
		glm::vec4 Value;
	};

	class TexturePath : public MaterialData
	{
	public:

		TexturePath()
		{}
		DATA_TYPE(TexturePath);
		std::string name;
		std::string Value;
	};

	class Material
	{
	
		std::vector<Ref<MaterialData>> _DataFields;

	public:
		
		Material();
		~Material();
		static bool MaterialToFile(const Ref<Material>& material, const std::string& path);
		static Ref<Material> MaterialFromShader(const Ref<Shader>& shader);
		std::vector<Ref<MaterialData>>& GetDataFields()
		{
			return _DataFields;
		}
		void PushDataField(const Ref<MaterialData>& datafield)
		{
			_DataFields.push_back(datafield);
		}



	};
}


