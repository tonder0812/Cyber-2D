@echo off
pushd %~dp0\..\
mkdir Build 2> nul
mkdir Build\temp 2> nul
set x=0

:start
set /A x+=1

if exist "bin\Dist-windows-x86_64\Cyber-Editor\Cyber-Editor.exe" (
if exist "bin\Dist-windows-x86_64\Cyber-Editor\assets" (
if exist "bin\Dist-windows-x86_64\Sandbox\Sandbox.exe" (
    goto exists
)))
goto notExists

:notExists
if %x% EQU 2 (
  echo Failed to compile project
  pause
  goto end
)
echo Compiling
msbuild Cyber-2D.sln /p:Configuration=Dist > nul
goto start

:exists
echo Copying Editor
copy /b "bin\Dist-windows-x86_64\Cyber-Editor\Cyber-Editor.exe" "Build\Cyber-Editor.exe" > nul

echo Copying Sandbox
copy /b "bin\Dist-windows-x86_64\Sandbox\Sandbox.exe" "Build\Sandbox.exe" > nul

echo Copying configuration
copy /b "imgui.ini" "Build\imgui.ini" > nul

echo Copying assets
Xcopy /E /I /Y "bin\Dist-windows-x86_64\Cyber-Editor\assets" "Build\assets" > nul

echo Copying Python
copy /b "packages\python.3.9.4\tools\python39.dll" "Build\python39.dll" > nul
Xcopy /E /I /Y "packages" "Build\packages" > nul

echo Done
start Build

:end