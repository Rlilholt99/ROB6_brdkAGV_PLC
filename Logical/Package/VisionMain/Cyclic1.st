
PROGRAM _CYCLIC
    
//	

//	 
//
//	
//	
//	
//	hwBlob.out.common.enable := TRUE;
//	hwBlob.out.common.numSearchMax := 1;
//	hwBlob.out.common.alignment := 0;
//	hwBlob.out.common.offsetROI.orientation := 0;
//	hwBlob.out.common.offsetROI.rotCenterX := 0;
//	hwBlob.out.common.offsetROI.rotCenterY := 0;
//	hwBlob.out.common.offsetROI.x := 0; 
//	hwBlob.out.common.offsetROI.y := 0;
	
	
//	
//	IF gVisionNettime.CMD.Activate THEN
//		//imgTrig.pCameraHw := ADR(hwCam);
//		
//		
//	
//		
//		
//		CASE visState OF
//			
//			0:
//				IF EDGEPOS(imageRequested[camIdx]) THEN
//						
//					requestTimeStamp := NettimeCurrent_us;
//						
//					resultReady[0] := FALSE;
//						
//					visState := 10;
//						
//				END_IF
//			10:
//				TriggerTimeDelta_us := (durationToCamera) - (NettimeCurrent_us - requestTimeStamp);
//				
//				IF (TriggerTimeDelta_us >= (gVisionNettime.CFG.VisionPreTrigger_Cycles) * PowerlinkCycle_us)  AND
//					(TriggerTimeDelta_us <= (gVisionNettime.CFG.VisionPreTrigger_Cycles + 1) * PowerlinkCycle_us) THEN
//					
//					gVisionNettime.DAT.NettimeNext_us	:= NettimeCurrent_us + TriggerTimeDelta_us;
//					gVisionNettime.DAT.NettimeDelta_us	:= TriggerTimeDelta_us;
//					
//					// Transfer nettime to sensor and fire trigger
//					IF hwCam.in.status.imageAcquisitionReady AND NOT hwCam.in.status.imageProcessingActive THEN
//							
//						imgTrig.trigger := TRUE;
//						imgTrig.delayNettime	:= gVisionNettime.DAT.NettimeNext_us;
//						TOF_Light.IN := TRUE;
//						
//					ELSE
//							
//						gVisionNettime.DAT.Overflow := gVisionNettime.DAT.Overflow + 1;
//						
//					END_IF
//						
//					visState := 20;
//					
//				END_IF
//			20:
//				IF EDGENEG(hwCam.in.status.imageProcessingActive) THEN
//				
//					CASE camIdx OF
//							
//						0: 
//							// Create empty svg contour of the found shape, which is used as overlay in the HMI page
//							overlay := '';
//							additionArguments := 'fill="white" stroke="red" stroke-width="8px"';
//								
//							// Check if blobs were found
//							IF hwBlob.in.common.numResults <> 0 THEN
//								// Assign bool depending on placement of detected shape
//								
//								
//								posErr := (hwBlob.in.results.xMax + hwBlob.in.results.xMin)/2;
//								
//								
//								
//									
//								//shapeCenterPoint.x := DINT_TO_LREAL(hwMatch.in.results[0].positionX/100);
//								//shapeCenterPoint.y := DINT_TO_LREAL(hwMatch.in.results[0].positionY/100);
//								
//								brdkViGeom_drawSvgCircle(shapeCenterPoint, 15, ADR(overlay), ADR(additionArguments));
//									
//							END_IF
//									
//					END_CASE
//						
//					resultReady[0] := TRUE;
//					visState := 0;
//						
//				END_IF
//					
//		END_CASE
//				
//			
//		IF imgTrig.newImage THEN
//			// Display image for camera 0 via mappView server
//			mappView.updateImage := TRUE;
//		END_IF	
//			
//		

		//mappView.pMappLink 		:= ADR(gCamera);
		//mappView();
			
		//TOF_Light();
		//imgTrig();
	//END_IF

END_PROGRAM
