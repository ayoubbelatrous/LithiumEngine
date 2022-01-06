﻿#include "lipch.h"
#include "Font.h"
#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"

using namespace msdf_atlas;

using namespace msdfgen;
bool Lithium::Font::GenFonts()
{
	
	unicode_t cp(0xC2A4);
	Charset custom = Charset(Charset::ASCII);
	custom.add(cp);

		bool success = false;
		// Initialize instance of FreeType library
		if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) {
			// Load font file
			if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, "assets/Editor/Fonts/OpenSans-Regular.ttf")) {
				// Storage for glyph geometry and their coordinates in the atlas
				std::vector<GlyphGeometry> glyphs;
				// FontGeometry is a helper class that loads a set of glyphs from a single font.
				// It can also be used to get additional font metrics, kerning information, etc.
				FontGeometry fontGeometry(&glyphs);
				// Load a set of character glyphs:
				// The second argument can be ignored unless you mix different font sizes in one atlas.
				// In the last argument, you can specify a charset other than ASCII.
				// To load specific glyph indices, use loadGlyphs instead.
				fontGeometry.loadCharset(font, 1.0, custom);
				// Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
				const double maxCornerAngle = 3.0;
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
				packer.setPixelRange(2.0);
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
				generator.setThreadCount(4);
				// Generate atlas bitmap
				generator.generate(glyphs.data(), glyphs.size());
				// The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
				// The glyphs array (or fontGeometry) contains positioning data for typesetting text.
				savePng(generator.atlasStorage(), "assets/out.png");
				// Cleanup
				msdfgen::destroyFont(font);
			}
			msdfgen::deinitializeFreetype(ft);
		}
		return success;
	
	
}
