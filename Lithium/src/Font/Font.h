#pragma once
#include "Renderer/Texture.h"

namespace Lithium {
	struct Character {
		glm::vec2 PackedSize;
		glm::vec2 PackedPos;
		uint32_t CodePoint;
	};

	class Font
	{
		
	public:
		struct Character {
			glm::vec2 PackedSize;
			glm::vec2 PackedPos;
			uint32_t CodePoint;
		};
		Font() = default;
		Font(const std::string& Filepath);
		static void Init();

		Character GetCharacter(uint32_t character);
		Ref<Texture> GetAtlas();
	private:
		void Load();
		std::string m_Path;
		std::map<uint32_t, Character> m_CharacterMap;
		Ref<Texture> m_AtlasTexture;
		uint32_t m_AtlasWidth = 1;
		uint32_t m_AtlasHeight = 1;
	};
}
