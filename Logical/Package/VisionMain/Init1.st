

PROGRAM _INIT
	FOR camIdx := 0 TO MAX_NUM_CAMS DO
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
	
	
	

	
	 
END_PROGRAM