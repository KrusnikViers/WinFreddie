#include "tray_menu.h"

#include "constants.h"
#include "registry_record.h"
#include "utils.h"

namespace {

const std::string kModuleFileName = GetModuleFilename();

}  // namespace

TrayMenu::TrayMenu(HWND window_handle) : window_handle_(window_handle) {
  handle_ = CreatePopupMenu();
  CHECK(handle_);

  AppendMenu(handle_, 0, (UINT_PTR)MenuID::Autolaunch,
             kMenuAutolaunchText.c_str());
  AppendMenu(handle_, MF_SEPARATOR, (UINT_PTR)0, nullptr);
  AppendMenu(handle_, MF_CHECKED, (UINT_PTR)MenuID::RemainActive,
             kMenuRemainActiveText.c_str());
  AppendMenu(handle_, MF_SEPARATOR, (UINT_PTR)0, nullptr);
  AppendMenu(handle_, 0u, (UINT_PTR)MenuID::About, kMenuAboutText.c_str());
  AppendMenu(handle_, MF_SEPARATOR, (UINT_PTR)0, nullptr);
  AppendMenu(handle_, 0u, (UINT_PTR)MenuID::Exit, kMenuExitText.c_str());
  SetMenuDefaultItem(handle_, (UINT)MenuID::RemainActive, 0);
}

TrayMenu::~TrayMenu() {
  if (handle_) DestroyMenu(handle_);
}

void TrayMenu::ShowMenu(int x, int y) {
  const bool is_autolaunch_set =
      RegistryRecord(kRegistryAutolaunchKey).GetStringValue(kRegistryAppPath) ==
      kModuleFileName;
  ModifyMenu(handle_, (UINT)MenuID::Autolaunch,
             is_autolaunch_set ? MF_CHECKED : 0u, (UINT_PTR)MenuID::Autolaunch,
             kMenuAutolaunchText.c_str());
  TrackPopupMenu(handle_, 0u, x, y, 0, window_handle_, nullptr);
}

void TrayMenu::SetActiveState(bool is_active) {
  if (is_active == is_active_) return;
  is_active_ = is_active;
  ModifyMenu(handle_, (UINT)MenuID::RemainActive, is_active_ ? MF_CHECKED : 0u,
             (UINT_PTR)MenuID::RemainActive, kMenuRemainActiveText.c_str());
}
