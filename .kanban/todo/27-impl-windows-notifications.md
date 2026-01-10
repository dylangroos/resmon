# Implement Windows Notifications

**Priority:** MEDIUM
**Phase:** 6 - Windows Backend

## Description
Windows Toast Notifications for alerts.

## Approach
```cpp
// Using WinToast library or raw Windows API
// ToastNotificationManager for Windows 10+
```

## Tasks
- [ ] Create src/alerts/notifications_windows.cpp
- [ ] Integrate WinToast library (or similar)
- [ ] Send toast notification on alert
- [ ] Include app icon
- [ ] Handle Windows 7 fallback (balloon tips)

## Acceptance Criteria
- Toast notification appears in Action Center
- Shows metric and value
- Works on Windows 10/11
