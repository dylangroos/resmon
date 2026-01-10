# Implement macOS Notifications

**Priority:** MEDIUM
**Phase:** 7 - macOS Backend

## Description
macOS notifications via NSUserNotificationCenter or UNUserNotificationCenter.

## Approach
```objc
// Modern (10.14+): UNUserNotificationCenter
// Legacy: NSUserNotificationCenter

// Using Objective-C++ (.mm file)
UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
content.title = @"resmon Alert";
content.body = @"CPU temperature critical: 95°C";
```

## Tasks
- [ ] Create src/alerts/notifications_macos.mm
- [ ] Request notification permission
- [ ] Send notification on alert
- [ ] Handle permission denied gracefully

## Acceptance Criteria
- Notification appears in Notification Center
- Works on macOS 10.14+
- Handles permission properly
