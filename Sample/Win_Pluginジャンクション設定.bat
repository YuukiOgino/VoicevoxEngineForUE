@echo off

SET PLUGIN_DIR=%~dp0..\Plugins

echo Pluginの割り当て
mklink /j %~dp0Plugins %PLUGIN_DIR%

pause
cls


