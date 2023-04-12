pushd ..\
call vendor\bin\premake\premake5.exe vs2022
copy PKEngine\vendor\assimp\lib\assimp-vc140-mt.dll bin\Debug-windows-x86_64\PK-Editor\assimp-vc140-mt.dll
popd
pause