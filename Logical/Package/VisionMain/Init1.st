

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
	
	positionPID.Enable := TRUE;
	positionPID.PIDParameters.Gain := 0.005;
	positionPID.PIDParameters.FilterTime :=  0.000228000456;
	positionPID.PIDParameters.DerivativeTime :=  0.00228000456;
	positionPID.PIDParameters.IntegrationTime :=  0.02500019;
	
	
	positionPID.MinOut := 0;
	positionPID.MaxOut := 1;
	positionPID.Update := TRUE;
	
	
	
	
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