Legacy UI files moved here from active `mainform` QML surface.

Reason:
- these components are not instantiated by `AppShell.qml`
- they duplicated or conflicted with the current production UI flow
- keeping them in `components/` created noise and false-positive integration paths

Note:
- files in this directory are preserved for reference only
- do not import them from runtime QML without explicit migration work
