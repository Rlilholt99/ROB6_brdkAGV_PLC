

PROGRAM _INIT
	FOR camIdx := 0 TO MAX_NUM_CAMS DO
		imageRequested[camIdx] := FALSE;
		requestTimeStamp := 0;
		durationToCamera := 0;
		resultReady[camIdx] := FALSE;
	END_FOR
	
	gVisionNettime.CMD.Activate := TRUE;
	gVisionNettime.CFG.AxisPeriod := 360;
	gVisionNettime.CFG.AxisPositionDelay_Cycles := 1;
	gVisionNettime.CFG.VisionPreTrigger_Cycles:= 8;
	
	
	hwCam.out.par.gainLevel := 1;
	hwCam.out.par.flashColor := BRDKVIBASE_CAM_LED_GREEN;
	hwCam.out.par.exposureTime := 1000;
	hwCam.out.par.focus := 5000;
	
		gVisionImage.CFG.EthDevice := 'IF2';
		gVisionSensor[0].CFG.VisionFunction := enumBlob;
		gVisionSensor[0].CFG.PowerlinkNode := 1;
		gVisionSensor[0].CFG.DataStructure := ADR(gBlob);
		gVisionSensor[0].CFG.ComponentLink := gCamera;
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