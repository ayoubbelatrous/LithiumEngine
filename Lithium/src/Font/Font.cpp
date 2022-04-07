#include "lipch.h"
#include "Font.h"
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include "Core/Base.h"


using namespace msdf_atlas;


namespace Lithium
{
	static msdfgen::FreetypeHandle* freeTypeHandle = nullptr;

	Font::Font(const std::string& Filepath)
		:m_Path(Filepath)
	{
		Load();
	}

	void Font::Init()
	{
		freeTypeHandle = msdfgen::initializeFreetype();
		if (freeTypeHandle == nullptr)
		{
			CORE_LOG("[Font] : Failed to init FreeType");
			ASSERT(freeTypeHandle == nullptr);
		}
	}

	Font::Character Font::GetCharacter(uint32_t character)
	{
		return m_CharacterMap[character];
	}

	Ref<Texture> Font::GetAtlas()
	{
		return m_AtlasTexture;
	}

	glm::vec2 Font::GetAtlasSize()
	{
		return glm::vec2(m_AtlasWidth, m_AtlasHeight);
	}

	void Font::Load()
	{
		msdfgen::FontHandle* fontHandle = nullptr;
		fontHandle = msdfgen::loadFont(freeTypeHandle, m_Path.c_str());
		if (fontHandle == nullptr) {
			CORE_LOG("[Font] : Failed to load font file");
			ASSERT(fontHandle == nullptr);
		}

		std::vector<GlyphGeometry> glyphs;
		FontGeometry fontGeometry(&glyphs);


		Charset charset = Charset();
		charset.add('A');
		charset.add('B');
		charset.add('C');
		fontGeometry.loadCharset(fontHandle, 1.0f, Charset::ASCII);
		
		const double maxCornerAngle = 3.0;
		for (GlyphGeometry& glyph : glyphs)
			glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

		std::string pCachePath = std::string("cache/") + std::filesystem::path(m_Path + "-cache.png").filename().string();

		TightAtlasPacker packer;

		packer.setDimensionsConstraint(TightAtlasPacker::DimensionsConstraint::SQUARE);
		
		packer.setMinimumScale(24.0);
		packer.setPixelRange(5.0f);
		packer.setMiterLimit(1.0);
		packer.setScale(64.0f);
		packer.setPadding(1);
		packer.pack(glyphs.data(), glyphs.size());
		int width = 0, height = 0;
		packer.getDimensions(width, height);

		for (auto glyph : glyphs)
		{
			int x, y, w, h = 0;
			double left;
			double right;
			double top;
			double bottom;
			glyph.getQuadPlaneBounds(left, bottom, right, top);
			glyph.getBoxRect(x, y, w, h);
			GeometryScale = fontGeometry.getGeometryScale();
			Character currentCharacter;
			currentCharacter.PackedSize = glm::vec2(w, h);
			currentCharacter.PackedPos = glm::vec2(x, y);
			currentCharacter.CodePoint = (uint32_t)glyph.getCodepoint();
			currentCharacter.Advance = glyph.getAdvance();
			currentCharacter.Bounds = { (float)left,(float)right,(float)top,(float)bottom };
			m_CharacterMap[(uint32_t)glyph.getCodepoint()] = currentCharacter;
		}

		if (!std::filesystem::exists(pCachePath))
		{
			ImmediateAtlasGenerator<float, 3, &msdfGenerator, BitmapAtlasStorage<byte, 3>> generator(width, height);
			GeneratorAttributes attributes;

			generator.setAttributes(attributes);
			generator.setThreadCount(4);
			generator.generate(glyphs.data(), glyphs.size());
			msdfgen::BitmapConstRef<byte, 3> bitmap = generator.atlasStorage();
			msdfgen::savePng(bitmap, pCachePath.c_str());
		}
		

		m_AtlasHeight = height;
		m_AtlasWidth = width;


		m_AtlasTexture = CreateRef<Texture>(pCachePath);
		msdfgen::destroyFont(fontHandle);
	}
}