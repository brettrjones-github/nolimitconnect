
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


NOTE: Only Visual Studio 2019 is supported and the compiler install is required if building a windows version of NoLimitConnect
If building with Visual Studio then
	1.) Open $(SRC_ROOT)/build/VS2017/NoLimitConnectAll.sln
	2.) Rebuild All
	
If building with Qt for Andorid
	1.) Repeat the following steps for BuildNlc1_NlcDependLibs.pro, BuildNlc2_NlcCore.pro and BuildNlc3_NlcAppOnly.pro
	2.) Open BuildXXX.pro with Qt creator
	3.) Click Projects and configure your kit(s)
	4.) You can uncheck Build Steps->Enable QML debugging and profiling.. there is no qml files used in project
	5.) For BuildNlc1_NlcDependLibs.pro, BuildNlc2_NlcCore.pro which are static libraries you can disable Build Android APK step to avoid
		the apk build failed error (The error does not cause it to not build but is annoying)
	6.) Build

If building with Qt for Linux
	1.) Open and build BuildNlc1_NlcDependLibs.pro, BuildNlc2_NlcCore.pro and BuildNlc3_NlcAppOnly.pro
		
Builds may take a long time so be patient

On windows if you get error "make (e=87): The parameter is incorrect"
then the command line limit of 32k is exceeded.
You may need to shorten your obj path.

Some Words Of Advice
If you live in USA you should be paranoid about privacy.
In USA from the "WAR ON CRIME", "WAR ON DRUGS", "WAR OF TERROR" and whatever is the next "WAR ON _____"
we have the highest percentage of our citizens in jail of any country in the world.

In the USA you are your IP Address even if someone else used it. You can be sued for thousands of dollars using just your IP Address as your identity. 
NoLimitConnect and Kodi allow 3rd Party plugins, be paranoid that they might cause copywrite infringement.
In the USA copywrite infringement penalties can be over a $30,000.00 fine per work infringed and up to 5 years in jail

Use a VPN if possible. NordVPN and ExpressVPN can be used but do not allow port forwarding.
Port forwarding is required if hosting a network or group or chat room or random connect and is recommended for fastest speed and reliability of connections.
See VPN_Setup_For_NoLimitConnect.txt for more information about VPNs.

 
