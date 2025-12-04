@echo off
setlocal

:: Define source files
set FILE1=mod_info.json
set FILE2=favico.ico
set FILE3=Release\Ty2AP-Client.dll
set FILE4=Patch_PC.rkv

:: Define destination directory
set DEST=Publish

:: Create destination directory if it doesn't exist
if not exist "%DEST%" (
    mkdir "%DEST%"
)

:: Copy files
echo Copying files to %DEST%...
copy "%FILE1%" "%DEST%"
copy "%FILE2%" "%DEST%"
copy "%FILE3%" "%DEST%"
copy "%FILE4%" "%DEST%"

echo All files copied successfully!
endlocal
