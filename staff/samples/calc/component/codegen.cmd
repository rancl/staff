setlocal
set arch=%1%
if "%arch%" == "" (
  echo.
  echo Error: Arch is not set.
  exit 1
)
SET STAFF_HOME=%cd%\..\..\..\deploy\win_%arch%\staff
set PATH=..\%STAFF_HOME%\bin;%STAFF_HOME%\lib;%PATH%
cd src
staff_codegen -u -tcomponent CalcService.h SubService.h
staff_codegen -u -twsdl CalcService.h SubService.h
endlocal
