# Implement Linux Notifications

**Priority:** MEDIUM
**Phase:** 5 - Alert System

## Description
Send desktop notifications via libnotify on Linux.

## Approach
```cpp
#include <libnotify/notify.h>

notify_init("resmon");
NotifyNotification* n = notify_notification_new(
    "resmon Alert",
    "CPU temperature critical: 95°C",
    "dialog-warning"
);
notify_notification_show(n, nullptr);
```

## Tasks
- [ ] Add libnotify to CMake (find_package)
- [ ] Create src/alerts/notifications_linux.cpp
- [ ] Initialize libnotify on startup
- [ ] Send notification on alert trigger
- [ ] Include metric name and value in message
- [ ] Respect cooldown from AlertManager

## Acceptance Criteria
- Desktop notification appears
- Shows which metric and current value
- Works on GNOME, KDE, etc.
