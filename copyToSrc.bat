SET src_folder=.\gfx
SET tar_folder=D:\OgreSDK\media

COPY %src_folder%\*.cg %tar_folder%
COPY %src_folder%\*.hlsl %tar_folder%
COPY %src_folder%\*.program %tar_folder%
COPY %src_folder%\*.material %tar_folder%

pause > nul