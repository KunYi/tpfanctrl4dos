@ECHO OFF

set inc=.\inc
set optcompile=-i=%inc% -d3
set optcompile=-i=%inc%

wcc %optcompile% fan.c
wcc %optcompile% ec.c
wcc %optcompile% portio.c


wlink debug all File fan.obj,ec.obj,portio.obj Name fan.exe
set inc=
set optcompile=
