@echo off
REM Generate Engine Solution
echo Generating Engine Solution...
call .\Libs\premake\premake5.exe --ws=engine vs2022

REM Generate Converter Solution
echo Generating Converter Solution...
call .\Libs\premake\premake5.exe --ws=converter vs2022

