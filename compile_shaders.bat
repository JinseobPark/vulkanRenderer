@echo off
setlocal

set VULKAN_SDK=C:\VulkanSDK\1.4.335.0
set GLSLC=%VULKAN_SDK%\Bin\glslc.exe
set SHADER_DIR=%~dp0shaders

echo Compiling shaders...

"%GLSLC%" "%SHADER_DIR%\triangle.vert" -o "%SHADER_DIR%\triangle.vert.spv"
if %errorlevel% neq 0 (
    echo Failed to compile triangle.vert
    exit /b 1
)
echo Compiled: triangle.vert.spv

"%GLSLC%" "%SHADER_DIR%\triangle.frag" -o "%SHADER_DIR%\triangle.frag.spv"
if %errorlevel% neq 0 (
    echo Failed to compile triangle.frag
    exit /b 1
)
echo Compiled: triangle.frag.spv

echo.
echo All shaders compiled successfully!
pause
