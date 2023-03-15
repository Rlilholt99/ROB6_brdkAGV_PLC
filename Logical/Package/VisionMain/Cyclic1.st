
PROGRAM _CYCLIC
    
//	

	positionPID();


	gVisionImage.CFG.EthDevice := 'IF2';
	gVisionSensor[0].CFG.VisionFunction := enumBlob;
	gVisionSensor[0].CFG.PowerlinkNode := 1;
	gVisionSensor[0].CFG.DataStructure := ADR(gBlob);
	gVisionSensor[0].CFG.ComponentLink := gCamera;
//
//	
//	
	
	hwBlob.out.common.enable := TRUE;
	hwBlob.out.common.numSearchMax := 1;
	hwBlob.out.enhancedBlobInformation := TRUE;
	hwBlob.out.regionFeatures := TRUE;
	hwBlob.out.common.alignment := 0;
	hwBlob.out.common.offsetROI.orientation := 0;
	hwBlob.out.common.offsetROI.rotCenterX := 0;
	hwBlob.out.common.offsetROI.rotCenterY := 0;
	hwBlob.out.common.offsetROI.x := 0; 
	hwBlob.out.common.offsetROI.y := 0;
	hwCam.out.par.focus := 11000;
	hwCam.out.par.flashColor := 99;
	hwCam.out.par.flashSegment := 4;
	hwCam.out.par.exposureTime := 1000;
	imgTrig.pCameraHw := ADR(hwCam);
	
	// alt dette burde køre under en specific condiotion
//	
	IF hwCam.in.status.imageAcquisitionReady AND imgProcessed AND NOT hwCam.in.status.imageProcessingActive THEN
		imgTrig.trigger := TRUE;

//		IF NOT hwCam.in.status.imageProcessingActive THEN
				
			IF hwBlob.in.common.numResults <> 0 THEN
				minYPos := hwBlob.in.results.yMin/100;
				minXPos := hwBlob.in.results.xMin/100;
				maxYPos := hwBlob.in.results.yMax/100;
				maxXPos := hwBlob.in.results.xMax/100;
				testMean := hwBlob.in.results.meanGreyValue;
				middleY := (minYPos + maxYPos)/2;
				middleX := (minXPos + maxXPos)/2;
				middleErr := middleX - 640;
				
				positionPID.ActValue := DINT_TO_REAL(middleErr);
				positionPID.SetValue := 0;
				IF (middleErr > 50) THEN
				Test2 := 1;
				drejMere := positionPID.Out;
//				drejMere := (middleErr/640);
				ELSIF (middleErr < -50) THEN
				Test2 := -1;
				drejMere := positionPID.Out;
//				drejMere := (middleErr/640);
				ELSE
					Test2 := 0;
				END_IF
			
			ELSE
			Test2 := 10;
			END_IF
		//		END_IF
		
	END_IF
	

	imgTrig();
	
	

END_PROGRAM
