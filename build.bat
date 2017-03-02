@echo off
set CompilerFlags= /Z7 /FC /nologo /Od -fp:fast -Gm- -GR- -EHa- /Oi -WX -W4 -wd4189 -wd4505 -wd4100 -wd4201 -wd4701 -wd4127 -wd4996  -wd4477 -wd4101
set LinkerFlags=-subsystem:console
set bits=x64

mkdir build > NUL 2> NUL
pushd build
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %bits%
del *.pdb > NUL 2> NUL
ctime -begin imgui.ctm
cl %CompilerFlags% /MTd ..\code\appcode.cpp -Fmappcode.map -LD /link -incremental:no -opt:ref -PDB:appcode_%RANDOM%.pdb -EXPORT:AppUpdateAndRender -EXPORT:AppGetSoundSamples -out:AppCode.dll
cl %CompilerFlags% /MD ..\code\win32layer.cpp gdi32.lib user32.lib winmm.lib opengl32.lib /link -incremental:no   %LinkerFlags%
ctime -end imgui.ctm
popd
