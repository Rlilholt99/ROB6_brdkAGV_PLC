

PROGRAM _INIT
	FOR camIdx := 0 TO MAX_NUM_CAMS DO
		imageRequested := FALSE;
		requestTimeStamp := 0;
		durationToCamera := 0;
		resultReady := FALSE;
		imgProcessed := TRUE;
	END_FOR
	
	gVisionNettime.CMD.Activate := TRUE;
	gVisionNettime.CFG.AxisPeriod := 360;
	gVisionNettime.CFG.AxisPositionDelay_Cycles := 1;
	gVisionNettime.CFG.VisionPreTrigger_Cycles:= 8;
	
	

	
	
	runOnce := TRUE;
	
//	
//	mappView.enable := TRUE;
//	mappView.getImageArray := FALSE;
//	mappView.imageFormat := BRDKVIIMG_IMG_TYPE_JPEG;
//	mappView.jpegQuality := 50;
//	mappView.timeout := 3000;
//	mappView.wsPort := 18000;
//	mappView();
//	
	
	
	 
END_PROGRAM