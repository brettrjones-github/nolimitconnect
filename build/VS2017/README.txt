Solution Description

=== must build python first because other solutions depend on it
GoTvPython.sln - Python only 
Build Both Release and Debug x64 builds if plan to debug else just the release build

GoTvAll.sln - Contains all projects to build and run GoTvPtoP
Set startup project to GoTvPtoP
Build either Release or Debug x64
NOTE: libkazza has a build issue. If you get error about linking asm object file 
	then rebuild it from the right click project menu
	



