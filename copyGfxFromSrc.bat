SET src_folder=D:\OgreSDK\media
SET tar_folder=.\gfx\media

xCOPY %src_folder%\*.cg %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.hlsl %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.program %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.material %tar_folder% /D:12-1-2014

SET src_folder=D:\OgreSDK\media\materials
SET tar_folder=.\gfx\materials

xCOPY %src_folder%\*.cg %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.hlsl %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.program %tar_folder% /D:12-1-2014
xCOPY %src_folder%\*.material %tar_folder% /D:12-1-2014

SET src_folder=D:\OgreSDK\media\general
SET tar_folder=.\gfx\general

xCOPY %src_folder%\*.material %tar_folder% /D:12-1-2014

SET src_folder=D:\OgreSDK\media\materials\core
SET tar_folder=.\gfx\materials\core

xCOPY %src_folder%\*.material %tar_folder% /D:7-15-2012
xCOPY %src_folder%\*.cg %tar_folder%  /D:7-15-2012
xCOPY %src_folder%\*.hlsl %tar_folder% /D:7-15-2012
xCOPY %src_folder%\*.program %tar_folder% /D:7-15-2012
xCOPY %src_folder%\*.compositor %tar_folder% /D:7-15-2012

pause > nul