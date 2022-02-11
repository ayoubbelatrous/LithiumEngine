#include "lipch.h"
#include "Material.h"
#include "Utils/OpenglUtils.h"

namespace Lithium
{
	Material::Material()
	{

	}
	Material::~Material()
	{

	}

	bool Material::MaterialToFile(const Ref<Material>& material, const std::string& path)
	{
		for (Ref<MaterialData> field : material->GetDataFields())
		{
			if (field->getType() == MaterialDataType::Vec2)
			{
				Vec2* data = (Vec2*)field.get();
				CORE_LOG("VEC2 : x = " << data->Value.x << " y = " << data->Value.y);
			}
		}

		return false;
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
				vec3->name = info.Name;
				material->PushDataField(vec3);
			}

			else if (type == MaterialDataType::Vec4)
			{
				Ref<Vec4> vec4 = CreateRef<Vec4>();
				vec4->name = info.Name;
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