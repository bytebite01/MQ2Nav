
#pragma once

#include "meshgen/Hash.h"

#include <imgui/imgui.h>
#include <unordered_map>


class PanelWindow
{
public:
	PanelWindow(std::string_view panelName, std::string_view settingsName);
	virtual ~PanelWindow() = default;

	void Initialize();

	virtual void OnImGuiRender(bool* p_open) = 0;

	void SetIsOpen(bool open);

	// ID of the panel. Used for things like lookups and settings.
	std::string panelName;

	// Named Used by settings
	std::string settingsName;

	// Indicates if panel is currently being displayed;
	bool isOpen = true;

	// Indicates that panel visibility should be persistent
	bool isOpenPersistent = true;

	// Indicates that panel can be closed by the user
	bool isClosable = true;

	// Focus this panel on the next frame
	bool focusNextFrame = false;
};

struct DockingSplit
{
	std::string initialDock;
	std::string newDock;
	ImGuiDir_ direction;
	float ratio = .25f;
	ImGuiDockNodeFlags nodeFlags = 0;
};

struct PanelDockAssignment
{
	std::string panelName;
	std::string dockName;
	bool open;
};

struct DockingLayout
{
	std::vector<DockingSplit> splits;
	std::vector<PanelDockAssignment> assignments;
	std::string layoutName;

	bool isDefault = false;
};

class PanelManager
{
public:
	PanelManager();
	~PanelManager();

	void PrepareDockSpace();

	void OnImGuiRender();

	void AddDockingLayout(DockingLayout&& layout);

	template <typename T, typename... Args>
	std::shared_ptr<T> AddPanel(Args&&... args)
	{
		static_assert(std::is_base_of_v<PanelWindow, T>, "AddPanel requries a type that inherits from PanelWindow");

		std::shared_ptr<T> panel = std::make_shared<T>(std::forward<Args>(args)...);
		size_t hash = hash::fnv_1a()(panel->panelName);

		assert(!m_panels.contains(hash));
		m_panelsSorted.push_back(hash);
		m_panels.emplace(hash, panel);

		return panel;
	}

	// Retrieve panel by its ID
	template <typename T>
	std::shared_ptr<T> GetPanel(std::string_view panelName) const
	{
		static_assert(std::is_base_of_v<PanelWindow, T>, "AddPanel requries a type that inherits from PanelWindow");

		size_t hash = hash::fnv_1a()(panelName);

		auto iter = m_panels.find(hash);
		if (iter == m_panels.end())
			return nullptr;

		return iter->second;
	}

	ImGuiID GetMainDockSpaceID() const { return m_dockspaceID; }

	// Focus panel by its ID.
	bool FocusPanel(std::string_view panelName);

	void Shutdown();

	// TODO: Temporary menu to get things working
	void DoWindowMenu();
	void DoLayoutsMenu();

private:
	void ApplyDockLayout();

	// List of all of our panels
	std::unordered_map<size_t, std::shared_ptr<PanelWindow>> m_panels;
	std::vector<size_t> m_panelsSorted;

	// List of docking splits in order of creation
	std::vector<DockingLayout> m_dockingLayouts;

	DockingLayout* m_activeLayout = nullptr;     // The currently active docking layout
	bool m_resetLayout = false;                  // Reset layout on next frame.
	ImGuiID m_dockspaceID = 0;                   // Id of the main dockspace

	// IDs of all docknodes
	std::unordered_map<std::string, ImGuiID> m_dockspaceIDs;
};
