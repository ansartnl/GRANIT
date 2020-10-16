pushd %~dp0

set PROJECT_DIR=%1
rem echo %PROJECT_DIR%
set DIST_DIR=%2
rem echo %DIST_DIR%
set BUILD_DIR=%PROJECT_DIR%\release
rem echo %BUILD_DIR%

rem Prepare build directory
rd /s /q %BUILD_DIR%
md %BUILD_DIR%

cd %BUILD_DIR%

rem Start building 'stca' project
qmake %PROJECT_DIR%/supervisor.pro CONFIG+=release
mingw32-make

rem Copy built application files
copy /D /Y "%PROJECT_DIR%\gui\bin\SupervisorMonitor.exe" "%DIST_DIR%\bin"
copy /Y "%PROJECT_DIR%\gui\etc\config.xml" "%DIST_DIR%\etc\SupervisorMonitor.xml"
xcopy /Y /Q /S /I "%PROJECT_DIR%\gui\share\translations\*.qm" "%DIST_DIR%\share\translations"
copy /D /Y "%PROJECT_DIR%\server\bin\SupervisorServer.exe" "%DIST_DIR%\bin"
copy /Y "%PROJECT_DIR%\server\etc\config.xml" "%DIST_DIR%\etc\SupervisorServer.xml"
xcopy /Y /Q /S /I "%PROJECT_DIR%\server\share\translations\*.qm" "%DIST_DIR%\share\translations"

popd