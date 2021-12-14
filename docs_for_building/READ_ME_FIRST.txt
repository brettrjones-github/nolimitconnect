
If you are unfamilar with NoLimitConnect you might want to read NoLimitConnect_Terminology.txt

After the tools and enviroment to build in your enviroment has been setup.

paths can get to long if building in a subfolder with a long paths
Check the source code out into short path folder like F:/NoLimitConnect

As a test of your build enviroment
  If building with Visual Studio
	1.) Open $(SRC_ROOT)/SandboxApps/QtOpenglThreadTestApp/build/VS2017/QtOpenglThreadTest.sln
	2.) Build and run to confirm a working build enviroment
  If building with Qt
	1.) Open $(SRC_ROOT)/SandboxApps/QtOpenglThreadTestApp/QtOpenglThreadTest.pro
	2.) Build and run to confirm a working build enviroment


NOTE: Only Visual Studio 2017 is supported and the compiler install is required if building a windows version of NoLimitConnect
If building with Visual Studio then
	1.) Open $(SRC_ROOT)/build/VS2017/GoTvPython.sln
	2.) Rebuild All
	3.) Open $(SRC_ROOT)/build/VS2015/GoTvAll.sln
	4.) Rebuild All
	
If building with Qt
	2.) Open GoTvAll.pro with Qt creator
	3.) Click Projects and configure your kit(s)
	4.) You can uncheck Build Steps->Enable QML debugging and profiling.. there is no qml files used in project
	5.) In Build Enviroment make sure ANDROID_NDK_PLATFORM is set to android-21 .. NOT android-16 or you will get link errors
    6.) Could not get Qt to exclude the windows only subdirs
        so if building for windows you will have to uncomment them at the bottom of
        GoTvPythonLib.pro
    7.) build GoTvAll
    NOTE: python tcl and bsddb does not currently build and so is commented out

		
It will take a long time so be patient

on windows if you get error "make (e=87): The parameter is incorrect"
then the command line limit of 32k is exceeded
you may need to shorten your obj path

Some Words Of Advice
If you live in USA you should be paranoid about privacy.
In USA from the "WAR ON CRIME", "WAR ON DRUGS", "WAR OF TERROR" and whatever is the next "WAR ON _____"
we have the highest percentage of our citizens in jail of any country in the world.

In the USA you are your IP Address even if someone else used it. You can be sued for thousands of dollars using just your IP Address as your identity. 
NoLimitConnect and Kodi allow 3rd Party plugins, be paranoid that they might cause copywrite infringement.
In the USA copywrite infringement penalties can be over a $30,000.00 fine per work infringed and up to 5 years in jail

Use a VPN if possible. NordVPN and ExpressVPN can be used but do not allow port forwarding.
Port forwarding is required if hosting a network or group and recommended for fastest speed and reliability of connections.

PureVPN with the port fowarding addon seems to work well on all OS's I tested ( Windows, Linux, Android )

PrivateVPN has port forwarding and worked well on windows but does not seem to support port forwarding on Android and 
I could not get PrivateVPN to work with my Linux Mint 19.2 Virtual Box Image.

For Information about my personal setup read
NoLimitDocs/NoLimitConnetAuthorsDocs/NoLimitConnectPrivacyAndAuthorsPersonalSetup.txt
 
