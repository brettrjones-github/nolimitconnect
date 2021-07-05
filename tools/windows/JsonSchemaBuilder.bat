@ECHO OFF

SET cur_dir=%CD%

rem echo " JASON Current Directory =%cur_dir%" 
SET base_dir=%cur_dir%\..\..\..
SET builddeps_dir=%base_dir%\tools\BuildDependencies
SET bin_dir=%base_dir%\tools\bin\Win32
SET msys_bin_dir=%builddeps_dir%\msys64
IF NOT EXIST %msys_bin_dir% SET msys_bin_dir=%builddeps_dir%\msys32
SET jsonrpc_path=%base_dir%\kodi_src\xbmc\xbmc\interfaces\json-rpc
SET jsonrpc_schema_path=%jsonrpc_path%\schema
SET jsonrpc_output=ServiceDescription.h

rem echo " JASON base_dir %base_dir%" 
rem echo " JASON builddeps_dir %builddeps_dir%" 
rem echo " JASON bin_dir %bin_dir%" 
rem echo " JASON msys_bin_dir %msys_bin_dir%" 
rem echo " JASON jsonrpc_path %msys_jsonrpc_path%" 
rem echo " JASON jsonrpc_schema_path %jsonrpc_schema_path%" 
SET xbmc_json_path=%base_dir%\kodi_src\xbmc\addons\xbmc.json
SET xbmc_json_output=addon.xml
rem echo " JASON xbmc_json_path %xbmc_json_path%" 

SET /p version=<"%jsonrpc_schema_path%\version.txt"
rem echo " JASON running sed in %msys_bin_dir%\" 
"%msys_bin_dir%\usr\bin\sed.exe"  s/@jsonrpc_version@/%version%/g "%xbmc_json_path%\%xbmc_json_output%.in" > "%xbmc_json_path%\%xbmc_json_output%"
rem echo " JASON running JsonSchemaBuilder.exe in %bin_dir%\json-rpc\" 

"%bin_dir%\json-rpc\JsonSchemaBuilder.exe" "%jsonrpc_schema_path%\version.txt" "%jsonrpc_schema_path%\license.txt" "%jsonrpc_schema_path%\methods.json" "%jsonrpc_schema_path%\types.json" "%jsonrpc_schema_path%\notifications.json"
rem echo " JASON moving %jsonrpc_output% to %jsonrpc_path%\%jsonrpc_output\" 
MOVE /Y %jsonrpc_output% "%jsonrpc_path%\%jsonrpc_output%"
rem echo " JASON done"