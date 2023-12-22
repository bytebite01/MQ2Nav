//
// ZonePicker.h
//

#pragma once

#include "meshgen/EQConfig.h"
#include <bgfx/bgfx.h>

#include <map>
#include <memory>
#include <string>

struct IMAGEDATA
{
	int width, height, bits;
	uint8_t* data;
	uint32_t textureId = 0;
};

class ZonePicker
{
public:
	ZonePicker(const EQConfig& eqConfig, bool batchMode = false);
	~ZonePicker();

	bool Show(bool focus);

	bool ShouldLoadNavMesh() const { return m_loadNavMesh; }
	std::string GetSelectedZone() const { return m_selectedZone; }

private:
	bool DrawExpansionGroup(const EQConfig::Expansion& expansion);

	// Mapping of Zones to Expansions
	using ZoneCollection = std::map<std::string, std::string>;
	ZoneCollection m_allMaps;
	EQConfig::MapList m_mapList;

	std::string m_eqDirectory;
	char m_filterText[64] = { 0 };

	std::string m_selectedZone;

	bool m_loadNavMesh = true;
	bool m_batchMode = false;
	int m_selectedExpansion = -1;

	std::vector<bgfx::TextureHandle> m_textures;
};
