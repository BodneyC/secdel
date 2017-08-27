@echo off
setlocal enabledelayedexpansion

IF NOT "%~1" == "" (
	set arg1="%~1"
) else (
	set arg1="."
)

for  %%x in (%arg1%\*.*) do (
  set "firstFile=%%x"
	main.exe !firstfile!
)
