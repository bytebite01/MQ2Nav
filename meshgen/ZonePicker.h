//
// ZonePicker.h
//

#pragma once

#include "meshgen/EQConfig.h"

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

	bool Show(bool focus, std::string* selected_zone = nullptr);

	bool ShouldLoadNavMesh() const { return m_loadNavMesh; }

	bool ShouldReplaceCollidables() const { return m_replaceCollidables; }

private:
	// Mapping of Zones to Expansions
	using ZoneCollection = std::map<std::string, std::string>;
	ZoneCollection m_allMaps;
	EQConfig::MapList m_mapList;

	std::string m_eqDirectory;
	char m_filterText[64] = { 0 };

	bool m_loadNavMesh = true;
	bool m_replaceCollidables = false;
	bool m_batchMode = false;

	std::vector<IMAGEDATA> m_tgaData;
};
