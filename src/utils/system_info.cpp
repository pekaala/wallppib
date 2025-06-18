#include "../../include/utils/system_info.h"

bool SystemInfo::is_wayland()
{
    const char *wayland_display = std::getenv("WAYLAND_DISPLAY");
    return (wayland_display != nullptr);
}