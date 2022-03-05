#include "lipch.h"
#include "Font.h"
#include <msdf-atlas-gen/msdf-atlas-gen.h>

using namespace msdf_atlas;


std::tuple<unsigned char*,int> Lithium::Font::GenFonts(const char* fontFilename) {
	unsigned char* data;
	bool success = false;
	// Initialize instance of FreeType library
	if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) {
		// Load font file
		if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontFilename)) {
			// Storage for glyph geometry and their coordinates in the atlas
			
			std::vector<GlyphGeometry> glyphs;
			// FontGeometry is a helper class that loads a set of glyphs from a single font.
			// It can also be used to get additional font metrics, kerning information, etc.
			FontGeometry fontGeometry(&glyphs);
			// Load a set of character glyphs:
			// The second argument can be ignored unless you mix different font sizes in one atlas.
			// In the last argument, you can specify a charset other than ASCII.
			// To load specific glyph indices, use loadGlyphs instead.
			Charset set = Charset();
			set.add('A');
			set.add('B');
			
			fontGeometry.loadCharset(font, 1.0, set);
			// Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
			const double maxCornerAngle = 2.0;
			for (GlyphGeometry& glyph : glyphs)
				glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
			// TightAtlasPacker class computes the layout of the atlas.
			TightAtlasPacker packer;
			// Set atlas parameters:
			// setDimensions or setDimensionsConstraint to find the best value
			packer.setDimensionsConstraint(TightAtlasPacker::DimensionsConstraint::SQUARE);
			// setScale for a fixed size or setMinimumScale to use the largest that fits
			packer.setMinimumScale(24.0);
			// setPixelRange or setUnitRange
			packer.setPixelRange(4.0);
			packer.setMiterLimit(1.0);
			// Compute atlas layout - pack glyphs
			packer.pack(glyphs.data(), glyphs.size());
			// Get final atlas dimensions
			int width = 0, height = 0;
			packer.getDimensions(width, height);
			
			// The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
			ImmediateAtlasGenerator<
				float, // pixel type of buffer for individual glyphs depends on generator function
				3, // number of atlas color channels
				&msdfGenerator, // function to generate bitmaps for individual glyphs
				BitmapAtlasStorage<byte, 3> // class that stores the atlas bitmap
				// For example, a custom atlas storage class that stores it in VRAM can be used.
			> generator(width, height);
			// GeneratorAttributes can be modified to change the generator's default settings.
			GeneratorAttributes attributes;
			generator.setAttributes(attributes);
			generator.setThreadCount(2);
			// Generate atlas bitmap
			generator.generate(glyphs.data(), glyphs.size());
			
			// The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
			// The glyphs array (or fontGeometry) contains positioning data for typesetting text.
			//success = myProject::submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
			for (auto g:glyphs)
			{
				int x, y, w, h = 0;
				g.getBoxRect(x, y, w, h);
				CORE_LOG("codepoint : " << (uint32_t)g.getCodepoint() << "");
				CORE_LOG("X : " << x);
				CORE_LOG("Y : " << y);
				CORE_LOG("W : " << w);
				CORE_LOG("H : " << h);
			}
			
			
			msdfgen::savePng(generator.atlasStorage(),"assets/atlas.png");
			msdfgen::BitmapConstRef<byte, 3> bitmap = generator.atlasStorage();

			std::vector<byte> pixels(3 * bitmap.width * bitmap.height);
			for (int y = 0; y < bitmap.height; ++y)
				memcpy(&pixels[3 * bitmap.width * y], bitmap(0, bitmap.height - y - 1), 3 * bitmap.width);
			return std::make_tuple<unsigned char*,int>(&pixels[0], 3 * bitmap.width * bitmap.height);
			msdfgen::destroyFont(font);
		}
		msdfgen::deinitializeFreetype(ft);
	}
	
}
