@echo OFF

if "%1"=="" (
    set bType=Debug
)

set PWD=%cd%

rmdir %pwd%\binary_out /q /s
mkdir binary_out
copy %PWD%\bin\%bType%\OpenWindow.exe		 			%PWD%\binary_out\OpenWindow.exe
copy %PWD%\bin\%bType%\OpenWindow.pdb 					%PWD%\binary_out\OpenWindow.pdb
copy %PWD%\bin\%bType%\TriangleTest.exe		 			%PWD%\binary_out\TriangleTest.exe
copy %PWD%\bin\%bType%\TriangleTest.pdb 				%PWD%\binary_out\TriangleTest.pdb
copy %PWD%\bin\src\%bType%\Dragon.dll 					%PWD%\binary_out\Dragon.dll
copy %PWD%\bin\src\%bType%\Dragon.lib 					%PWD%\binary_out\Dragon.lib
copy %PWD%\bin\src\%bType%\Dragon.exp 					%PWD%\binary_out\Dragon.exp
copy %PWD%\bin\src\%bType%\Dragon.pdb 					%PWD%\binary_out\Dragon.pdb
copy %PWD%\bin\lib\glfw\src\%bType%\glfw3.lib 			%PWD%\binary_out\glfw3.lib
copy %PWD%\bin\lib\glfw\src\%bType%\glfw3.pdb			%PWD%\binary_out\glfw3.pdb
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\alrecord.exe		%PWD%\binary_out\alrecord.exe
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\alrecord.pdb  	%PWD%\binary_out\alrecord.pdb
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\altonegen.exe 	%PWD%\binary_out\altonegen.exe
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\altonegen.pdb	%PWD%\binary_out\altonegen.pdb
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\common.lib		%PWD%\binary_out\common.lib
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\common.pdb		%PWD%\binary_out\common.pdb
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\ex-common.lib    %PWD%\binary_out\ex-common.lib
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\ex-common.pdb	%PWD%\binary_out\ex-common.pdb
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\openal-info.exe	%PWD%\binary_out\openal-info.exe
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\openal-info.pdb	%PWD%\binary_out\openal-info.pdb
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\OpenAL32.dll		%PWD%\binary_out\OpenAL32.dll
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\OpenAL32.exp		%PWD%\binary_out\OpenAL32.exp
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\OpenAL32.lib		%PWD%\binary_out\OpenAL32.lib
copy %PWD%\bin\lib\OpenAL-Soft\%bType%\OpenAL32.pdb		%PWD%\binary_out\OpenAL32.pdb