//============================================================================
// Copyright (C) 2016 Brett R. Jones
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett1900@usa.com
// http://www.gotvp2p.net
//============================================================================
package com.p2pgotv.gotvstation;

public interface ToGuiHardwareControlInterface 
{
	public void 				doGuiShutdownHardware();
	public void 				doGuiWantMicrophoneRecording( boolean wantMicInput );
	public void 				doGuiWantSpeakerOutput( boolean wantSpeakerOutput );
	public void					doGuiWantVideoCapture( boolean wantVideoCapture );
};

