


CONFIG(debug, debug|release){
    message(ERROR Only Build Python In Release Mode.. Debug Not Supported)
}

message(The crapy way you could copy/rename files in qt using extra targets has been broken in newer versions of qt/android builds)
message(Seems like this is a critical feature missing for large projects and cmake has its own mess of issues with qt)
message(After weeks of attempting to make it work I gave up and instead here are instructions to do it manualy)
message(for android the destination folder is bin-Android\assets\kodi\python2.7\lib\python2.7)
message(for windows the destination folder is bin-Windows\assets\kodi\system\Python\Dlls)
message(for linux the destination folder is bin-Linux\assets\kodi\system\Python\Dlls)
message(the output Build3_PythonDllLibs.pro build is typically something like F:\build-Build3_PythonDllLibs-Android_Qt_6_2_0_Clang_armeabi_v7a-Release)
message(copy all the .dll or .so files from the output of Build3_PythonDllLibs.pro build folder to the destination folder)
message(rename the dlls so they have the .pyd extension)
message(example lib_ctypes_armeabi-v7a.so should be renamed to _ctypes.pyd)



