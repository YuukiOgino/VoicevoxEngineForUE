@echo off

SET PLUGIN_DIR=%~dp0..\Plugins

echo Plugin�̊��蓖��
mklink /j %~dp0Plugins %PLUGIN_DIR%

pause
cls


