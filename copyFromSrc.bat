SET src_folder=D:\OgreSDK\media
SET tar_folder=.\gfx

COPY %src_folder%\*.cg %tar_folder%
COPY %src_folder%\*.hlsl %tar_folder%
COPY %src_folder%\*.program %tar_folder%
COPY %src_folder%\*.material %tar_folder%

pause > nul