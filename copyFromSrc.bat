SET src_folder=D:\OgreSDK\media
SET tar_folder=.\gfx

xCOPY %src_folder%\*.cg %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.hlsl %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.program %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.material %tar_folder% /D:12-1-2014

SET src_folder=D:\OgreSDK\media\materials\scripts
SET tar_folder=.\gfxScripts

xCOPY %src_folder%\*.material %tar_folder% /D:7-15-2012
xCOPY %src_folder%\*.cg %tar_folder%  /D:7-15-2012
xCOPY %src_folder%\*.hlsl %tar_folder% /D:7-15-2012
xCOPY %src_folder%\*.program %tar_folder% /D:7-15-2012
xCOPY %src_folder%\*.compositor %tar_folder% /D:7-15-2012

pause > nul