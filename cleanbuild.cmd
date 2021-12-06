@echo off
:: rem #######################################################################
:: rem ## Small NT script to delete all intermediate files.
:: rem ##
:: rem ## Last modified: $Date: 2005/04/17 12:22:56 $
:: rem #######################################################################
:: rem Change to the make directory
pushd .\make
:: rem Set all files visible and read/write
attrib -r -h -s *.* /S /D
:: rem Delete solution intermediate files
del /f /q /s *.ncb
del /f /q /s *.suo
del /f /q /s *.aps
del /f /q /s *.bak
del /f /q .\common\linkcounter.exe
:: rem Delete all release subdirectories (the release of the binaries is not touched)
for /d %%i in (*) do @(
  rd /s /q ".\%%i\UnicodeRelease"
  rd /s /q ".\%%i\UnicodeDebug"
  rd /s /q ".\%%i\Release"
  rd /s /q ".\%%i\Debug"
)
:: rem Change back to the previous dir
popd
pause

