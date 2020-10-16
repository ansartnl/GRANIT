pushd %~dp0

rd /S /Q ..\release

mkdir ..\release\

rem gui

mkdir ..\release\gui
mkdir ..\release\gui\bin
mkdir ..\release\gui\etc
mkdir ..\release\gui\share
mkdir ..\release\gui\share\translations

copy /Y /B .\gui\bin\SupervisorMonitor.exe ..\release\gui\bin\SupervisorMonitor.exe
copy /Y /B .\gui\etc\config.xml ..\release\gui\etc\SupervisorMonitor.xml
copy /Y /B .\gui\share\translations\qt_ru.qm ..\release\gui\share\translations\qt_ru.qm
copy /Y /B .\gui\share\translations\SupervisorMonitor_ru.qm ..\release\gui\share\translations\SupervisorMonitor_ru.qm

rem server

mkdir ..\release\server
mkdir ..\release\server\bin
mkdir ..\release\server\etc
mkdir ..\release\server\etc\examples

copy /Y /B .\server\bin\SupervisorServer.exe ..\release\server\bin\SupervisorServer.exe
copy /Y /B .\server\etc\config.xml ..\release\server\etc\SupervisorServer.xml
copy /Y /B .\server\etc\config_aftnws.xml ..\release\server\etc\examples\config_aftnws.xml
copy /Y /B .\server\etc\config_cwp.xml ..\release\server\etc\examples\config_cwp.xml
copy /Y /B .\server\etc\config_default.xml ..\release\server\etc\examples\config_default.xml
copy /Y /B .\server\etc\config_fdp.xml ..\release\server\etc\examples\config_fdp.xml

popd