//
// MQ2Nav_Settings.h
//

#pragma once

#include "MQ2Navigation.h"

namespace mq2nav
{
	struct SettingsData
	{
		bool autobreak = false;;
		bool autoreload = false;
	};
	SettingsData& GetSettings();

	// Load settings from the .ini file
	void LoadSettings(bool showMessage = true);

	// Save settings to the .ini file
	void SaveSettings(bool showMessage = true);

} // namespace mq2nav
