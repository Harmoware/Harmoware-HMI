@ECHO OFF
cd ..\..\bin\win32\
set FilePath=%~dp1
set FileName=%~n1
ECHO %FilePath%%FileName%
perl mkdfa.pl %FilePath%%FileName%
pause