@echo off

SET PLUGIN_DIR=%~dp0..\Plugins

echo Plugin‚ÌŠ„‚è“–‚Ä
mklink /j %~dp0Plugins %PLUGIN_DIR%

pause
cls


