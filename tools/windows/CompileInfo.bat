rem @ECHO OFF

REM setup all paths
SET cur_dir=%CD%
echo " CompileInfo Current Directory =%cur_dir%" 

SET base_dir=%cur_dir%\..\..\..
SET builddeps_dir=%base_dir%\tools\BuildDependencies
SET bin_dir=%base_dir%\tools\bin\Win32
SET msys_bin_dir=%builddeps_dir%\msys\bin
IF NOT EXIST %msys_bin_dir% SET msys_bin_dir=%builddeps_dir%\msys32
SET awk_exe=%msys_bin_dir%\awk.exe
SET sed_exe=%msys_bin_dir%\sed.exe
SET version_info_file=%base_dir%\GoTvVersion.txt

echo " CompileInfo base_dir %base_dir%" 
echo " CompileInfo builddeps_dir %builddeps_dir%" 
echo " CompileInfo bin_dir %bin_dir%" 
echo " CompileInfo msys_bin_dir %msys_bin_dir%" 
echo " CompileInfo awk_exe %awk_exe%" 
echo " CompileInfo sed_exe %sed_exe%" 
echo " CompileInfo version_file %version_info_file%" 

REM read the version values from version.txt

FOR /f %%i IN ('%awk_exe% "/APP_NAME/ {print $2}" %version_info_file%') DO SET app_name=%%i
FOR /f %%i IN ('%awk_exe% "/COMPANY_NAME/ {print $2}" %version_info_file%') DO SET company_name=%%i
FOR /f %%i IN ('%awk_exe% "/VERSION_MAJOR/ {print $2}" %version_info_file%') DO SET major=%%i
FOR /f %%i IN ('%awk_exe% "/VERSION_MINOR/ {print $2}" %version_info_file%') DO SET minor=%%i
FOR /f %%i IN ('%awk_exe% "/VERSION_TAG/ {print $2}" %version_info_file%') DO SET tag=%%i
FOR /f %%i IN ('%awk_exe% "/ADDON_API/ {print $2}" %version_info_file%') DO SET addon_api=%%i

SET app_version=%major%.%minor%
IF NOT [%tag%] == [] (
  SET app_version=%app_version%-%tag%
)

REM XBMC_PC.rc.in requires a comma-separated version of addon_api
SET separator=,
CALL SET file_version=%%addon_api:.=%separator%%%%separator%0
echo " CompileInfo file_version %file_version%" 
echo " CompileInfo app_name %app_name%" 
echo " CompileInfo company_name %company_name%" 
echo " CompileInfo major %major%" 
echo " CompileInfo minor %minor%" 
echo " CompileInfo tag %tag%" 
echo " CompileInfo addon_api %addon_api%" 
REM create the files with the proper version information
"%sed_exe%" -e s/@APP_NAME@/%app_name%/g -e s/@APP_VERSION_MAJOR@/%major%/g -e s/@APP_VERSION_MINOR@/%minor%/g -e s/@APP_VERSION_TAG@/%tag%/g "%base_dir%\kodi_src\xbmc\xbmc\CompileInfo.cpp.in" > "%base_dir%\kodi_src\xbmc\xbmc\CompileInfo.cpp"
echo " CompileInfo setting addon api version" 
"%sed_exe%" s/@APP_ADDON_API@/%addon_api%/g "%base_dir%\kodi_src\xbmc\addons\xbmc.addon\addon.xml.in" > "%base_dir%\kodi_src\xbmc\addons\xbmc.addon\addon.xml"
echo " CompileInfo creating app info resource"
"%sed_exe%" -e s/@APP_NAME@/%app_name%/g -e s/@COMPANY_NAME@/%company_name%/g -e s/@APP_VERSION_MAJOR@/%major%/g -e s/@APP_VERSION_MINOR@/%minor%/g -e s/@APP_VERSION_TAG@/%tag%/g -e s/@FILE_VERSION@/%file_version%/g -e s/@APP_VERSION@/%app_version%/g "%base_dir%\kodi_src\xbmc\xbmc\platform\win32\XBMC_PC.rc.in" > "%base_dir%\kodi_src\xbmc\xbmc\platform\win32\XBMC_PC.rc"
echo " CompileInfo done"
