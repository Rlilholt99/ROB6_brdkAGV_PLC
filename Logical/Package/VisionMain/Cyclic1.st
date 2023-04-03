
PROGRAM _CYCLIC
    
//	/// PARAMETER//////////////////////



	gVisionImage.CFG.EthDevice := 'IF2';
	gVisionSensor[0].CFG.VisionFunction := enumBlob;
	gVisionSensor[0].CFG.PowerlinkNode := 1;
	gVisionSensor[0].CFG.DataStructure := ADR(gBlob);
	gVisionSensor[0].CFG.ComponentLink := gCamera;
//
//	
//	
	
	hwBlob.out.common.enable := TRUE;
	hwBlob.out.common.numSearchMax := 10;
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
	
	//// PARAMETER///////////////////////////
	
IF hwCam.in.status.imageAcquisitionReady AND NOT hwCam.in.status.imageProcessingActive THEN
	imgTrig.trigger := TRUE;
		cnt := 0;
		
		
		
		// Vi starter med at kigge om vores kamera har fundet nogen resultater
		// Hvis der er resultater så begynder vi at proccess dem
		IF hwBlob.in.common.numResults <> 0 AND orientationReached AND NOT awaitDecision THEN
			lineStatus.lineStatusCode := 0;
				// Først vil vi gerne gemme det index af vores resultater som er  vores linje
				// model 1 er den model der bruges til at finde linjen
				// det index som indeholder linjen bliver gemt i test
				// Ellers så tjekker vi om resultatet er fra den model som finder gulvet
			// indexet bliver gemt i et array af int's
			
			FOR i := 0 TO hwBlob.in.common.numResults BY 1 DO
				IF hwBlob.in.results[i].modelNumber = 1 THEN
					test := i;
				ELSIF hwBlob.in.results[i].modelNumber = 2 THEN
					floorIndex[cnt] := i;
					cnt := cnt+1;
					
				END_IF
			END_FOR
				// her tager vi de forskellige blob resultater af linjen som vi er intersseret i baseret på det index som vi gemte før
				minYPos := hwBlob.in.results[test].yMin/100;
				minXPos := hwBlob.in.results[test].xMin/100;
				maxYPos := hwBlob.in.results[test].yMax/100;
				maxXPos := hwBlob.in.results[test].xMax/100;
				posY := hwBlob.in.results[test].positionY/100;
				posX := hwBlob.in.results[test].positionX/100;
				middleY := (minYPos + maxYPos)/2;
				middleX := (minXPos + maxXPos)/2;
				xPosDiff := middleX-640;
				blobArea := hwBlob.in.results[test].area/100;
				
			// her resetter vi vores gulv index så vi ikke ved en fejl kommer til at tage et tidligere index fra sidste cycle
				prevFloorIndex := 10;
				// her går vi igennem alle de indexs som indeholder resultater fra gulvet
				// Bemærk at nogle index kan være 0 for alle resultater da det er baseret på hvor mange blobs kameraet finder
			FOR i := 0 TO 2 BY 1 DO
				IF floorIndex[i] <> prevFloorIndex THEN // muligvis ikke nødvendigt men bare for at være sikker så tjekker jeg at det ikke er det samme index vi gemmer flere gange
					floorBlob[i].maxXPos := hwBlob.in.results[floorIndex[i]].xMax/100;
					floorBlob[i].maxYPos := hwBlob.in.results[floorIndex[i]].yMax/100;
					floorBlob[i].minXPos := hwBlob.in.results[floorIndex[i]].xMin/100;
					floorBlob[i].minYPos := hwBlob.in.results[floorIndex[i]].yMin/100;
					floorBlob[i].circ := hwBlob.in.results[floorIndex[i]].circularity;
					floorBlob[i].area := hwBlob.in.results[floorIndex[i]].area/100;
					floorBlob[i].yPos := hwBlob.in.results[floorIndex[i]].positionY/100;
					floorBlob[i].rect := hwBlob.in.results[floorIndex[i]].rectangularity;
					floorBlob[i].xPos := hwBlob.in.results[floorIndex[i]].positionX;
					prevFloorIndex := floorIndex[i];
				END_IF
				
			END_FOR
			
				// logikken bag at finde om der er en 90 grader sving
				// essentielt så kigger den om vores gulv blob ligger imellem x og y-max + min af vores linje blob
				// tilføj illustration for dette
				// muligvis tilføj noget der tjekker hvor rektangulær gulv blobben er, hvis den ikke er rektangulær er det nok en sammenfletning istedet for
			IF floorBlob[0].rect < 70 OR cnt < 1  THEN
				conditionX := TRUE;
				conditionY := TRUE;
			ELSE
				conditionX := floorBlob[0].minXPos+100 > maxXPos-100 OR floorBlob[0].maxXPos -100 < minXPos+100; //OR floorBlob[1].minXPos +50 > maxXPos-50 OR floorBlob[1].maxXPos -50 < minXPos+50;
				
				conditionY := floorBlob[0].maxYPos -100 > minYPos +00 OR floorBlob[0].minYPos +100 < maxYPos-100; //OR floorBlob[1].maxYPos -50 > minYPos +50 OR floorBlob[1].minYPos +50 < maxYPos-50;
		
			END_IF
			
			conditionCheck := conditionX AND conditionY;
			
			IF NOT conditionCheck THEN
				lineStatus.lineStatusCode := 20;
				updateLineStatus := TRUE;
//				awaitDecision := TRUE;
			END_IF
				// Her tjekker vi om linjen bliver størrer, dette dækker tilfældet af sammenfletninger
			IF blobArea > 850000 THEN

				crossBool := TRUE;
				IF crossBool <> prevCrossBool THEN
					awaitDecision := TRUE;
					updateLineStatus := TRUE;
				END_IF
				lineStatus.lineStatusCode := 30;

				prevCrossBool := crossBool;
			ELSE
				crossBool := FALSE;
				prevCrossBool := crossBool;
			END_IF
			
				// Hvis det er tilfældet af en sammenfletning så baseret på vores condition "holdRight" så drejer robotten til højre eller venstre
				// dette burde at lede robotten til en af de linjer der sammenflettes
				// derefter vil den gå tilbage til sit sædvanelige program
			IF crossBool AND sammenEnable THEN
				IF holdRight THEN  
					angle := 0.60;
				ELSE
					angle := -0.60;
	//				angle := -1.5707;
				END_IF
				// her udregner vi vinklen i mellem midten af kameraet og centeret af linje bloben
				// vi antager at det er en retvinklet trekant 
					// tilføj illustration
					
					// i tilfældet af linjen er lige på så er vinkel super lille og dermed bliver differencen mellem vores hjul minimal
					// ellers så bliver forskellen i hastigheden i mellem hjullene størrer baseret på vinklen
					// essentielt så virker det som en simpel P-controller
					// kunne udvides med et D komponentet til at håndtere noget overshoot på justeringen
			ELSE 
					
				
				angle := ASIN((640-posX)/SQRT(EXPT((640-posX),2)+EXPT((posY),2)));
			END_IF
			IF lineStatus.lineStatusCode = 0 THEN
				evaluateStatus;
			END_IF
			// her bestemmes den mode som giver værdier til motoren
			IF NOT conditionCheck AND sharpTurn THEN
				IF floorBlob[0].yPos > 512 AND floorBlob[0].xPos > 640 THEN
					angle := 0.01;
					drejMere := angle;
					rightTurn := FALSE;
				ELSIF floorBlob[0].yPos < 512 AND floorBlob[0].xPos < 640 THEN
					rightTurn := TRUE;
					angle := -0.01;
					drejMere := angle;
				END_IF 
				orientationReached := FALSE;
				Test2 := 20;			// som kan ses så hvor meget vi drejer er baseret på vinklen
			// vores reference angle vil være 0 i dette tilfælde da det vil være samme orientation som linjen
			ELSIF (xPosDiff > 0 OR xPosDiff < 0) AND orientationReached THEN //venstre
				Test2 := 1;
				drejMere := angle;

				
//			ELSIF orientationReached THEN
//				Test2 := 10;
//				IF angle < 0 THEN
//					drejMere := -0.16;
//				ELSE
//					drejMere := 0.16;
//				END_IF
			END_IF
//		
		ELSIF awaitDecision THEN
			Test2 := 0;
		ELSIF orientationReached THEN
			lineStatus.lineStatusCode := 90;
		Test2 := 5;
		IF angle < 0 THEN
			drejMere := -0.16;
		ELSE
			drejMere := 0.16;
		END_IF
			// i tilfældet af at  vi ikke finder nogen resultater med kameraet så vil robotten rotere til den retning den sidste ville til
			// dette er for at at lave noget recovery i tilfælde af robotten mister linjen
			// den kunne ende med at rotere helt om sig selv og dermed finde linjen tilbage

		
	END_IF
		//		END_IF
		
END_IF
	

	imgTrig();
	
	

END_PROGRAM
