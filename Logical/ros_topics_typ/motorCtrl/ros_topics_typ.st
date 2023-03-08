
PROGRAM _INIT

	ros_topics_typInit_0();

	(* Insert code here *)
	FOR i:=0 TO
		i < 2 BY 1 DO
		setupPointers;
	
		this.PID.Enable := TRUE;
		this.PID.MinOut := -100;
		this.PID.MaxOut := 100;
		this.PID.PIDParameters.DerivativeTime := 0.00228000456;
		this.PID.PIDParameters.Gain := 78;
		this.PID.PIDParameters.IntegrationTime := 0.02500019;
		this.PID.PIDParameters.FilterTime := 0.000228000456;
		
		this.PID.Update := TRUE;
		
		
		
		this.PIDCurrent.Enable := TRUE;
		this.PIDCurrent.MinOut := 0;
		this.PIDCurrent.MaxOut := 100;
		this.PIDCurrent.PIDParameters.DerivativeTime := 10;
		this.PIDCurrent.PIDParameters.Gain := 1;
		this.PIDCurrent.PIDParameters.IntegrationTime := 10;
		this.PIDCurrent.PIDParameters.FilterTime := 20;
		this.PIDCurrent.Update := TRUE;
		this.PIDCurrent.SetValue := 8;
		
		
		this.filterSpeed.Enable := TRUE;
		this.filterSpeed.WindowLength := 100; 
		this.filterSpeed.Update := TRUE;
	
		this.filterCurrent.Enable := TRUE;
		this.filterCurrent.WindowLength := 3; 
		this.filterCurrent.Update := TRUE;
	
		
		this.accLimiter.Enable := TRUE;
		this.accLimiter.MaxNegSlewRate := 1;
		this.accLimiter.MaxPosSlewRate := 1;
		this.accLimiter.MaxOut := 2;
		this.accLimiter.MinOut := -2;
		this.accLimiter.Update := TRUE;
		
		this.PIDtune.SetValue:= 1.0;
		this.PIDtune.MinOut:= 0.0;
		this.PIDtune.MaxOut:= 100.0;
		this.PIDtune.Update := TRUE;
		
		
		this.lowPass.Enable := TRUE;
		this.lowPass.CutOffFrequency := 10;
		this.lowPass.Order := 1;
		this.lowPass.Update := TRUE;
		
		
		this.encoderOld := this.hw.input.encoder;
		this.timeOld := this.hw.input.nettime;
		
		this.ton_overcurrent.PT := T#1s;
		
	END_FOR
	
	
END_PROGRAM

PROGRAM _CYCLIC
    //--------------- ex OS ----------------
    //Auto connect:
	ros_topics_typCyclic_0.Enable := ExComponentInfo_0.Operational AND NOT ros_topics_typCyclic_0.Aborted; // Component has been deployed and started up successfully
    
	//Auto connect when deployment is off and manually started in GPOS:
	//ros_topics_typCyclic_0.Enable := TRUE;
	ros_topics_typCyclic_0.Start := ros_topics_typCyclic_0.Connected;
    
	ros_topics_typCyclic_0(Handle := ros_topics_typInit_0.Handle, pros_topics_typ := ADR(ros.topics));
    
	ExComponentInfo_0(ExTargetLink := ADR(gTarget_0), ExComponentLink := ADR(gros_topics_typ_0), Enable := TRUE);
    
	ExDatamodelInfo_0(ExTargetLink := ADR(gTarget_0), Enable := TRUE, InstanceName := 'ros_topics_typ_0');
	//--------------- ex OS done ----------------
	ExSyncInfo_0(ExTargetLink := ADR(gTarget_0), Enable := TRUE);
	
	

	ros.topics.twist.linear.x := MAX(MIN(ros.topics.twist.linear.x,ros.topics.config.maxSpeed),ros.topics.config.minSpeed);
	
	IF localVariables[0].hmi.output.overload OR localVariables[1].hmi.output.overload OR NOT ros_topics_typCyclic_0.Operational THEN
		localVariables[0].PID.SetValue := 0;
		localVariables[1].PID.SetValue := 0;
	ELSE
		localVariables[0].PID.SetValue := LREAL_TO_REAL(ros.topics.twist.linear.x + ros.topics.twist.angular.z * ros.topics.config.baseWidth / 2.0) * ros.currentLimiter;  // m/s
		localVariables[1].PID.SetValue := LREAL_TO_REAL(ros.topics.twist.linear.x - ros.topics.twist.angular.z * ros.topics.config.baseWidth / 2.0) * ros.currentLimiter;
	END_IF
		
	
		
	FOR i:=0 TO 1 BY 1 DO
		setupPointers;
	
		hmi.output.overload := FALSE;
			
		hmi.output.currentTotal := 0;
			
		FOR k:=0 TO 7 DO
				
			hmi.output.current[k] := INT_TO_REAL(hw.input.current[k])*10.0/32767.0;
			hmi.output.currentTotal := hmi.output.currentTotal + hmi.output.current[k];
		END_FOR
			
		hmi.output.currentTotal := hmi.output.currentTotal /2.0;
			
		
		IF this.filterCurrent.UpdateDone THEN
			this.filterSpeed.Update := FALSE;
		END_IF
			
		this.filterCurrent(In := hmi.output.currentTotal);
			
		IF this.PIDCurrent.UpdateDone THEN
			this.PIDCurrent.Update := FALSE;
		END_IF
			
		this.PIDCurrent(ActValue := this.filterCurrent.Out);
			
		this.currentLimiter := 1;//this.PIDCurrent.Out/100;
			
		hmi.output.overload := hw.input.overload;
		
		this.ton_overcurrent(IN := hmi.output.overload);
		IF this.ton_overcurrent.Q THEN
			this.hmi.input.clear := TRUE;
		END_IF
			
		hw.output.clear := hmi.input.clear;
			
		IF NOT hmi.output.overload THEN
			hmi.input.clear := FALSE;
		END_IF
			
		this.diffTime := DINT_TO_LREAL(hw.input.nettime -this.timeOld);
		IF this.diffTime < -100000.0 THEN
			this.diffTime := this.diffTime + 2*2147483647;
		END_IF
		this.diffTime := this.diffTime /1000.0;
		this.timeOld := hw.input.nettime;
			
		this.diffEncoder := INT_TO_LREAL(hw.input.encoder)-INT_TO_LREAL(this.encoderOld);
			
		this.encoderOld := hw.input.encoder;
			
		IF this.diffEncoder > 32727 THEN
			
			this.diffEncoder := this.diffEncoder - 2*32767;
				
		ELSIF this.diffEncoder < -32727 THEN
			
			this.diffEncoder := this.diffEncoder + 2*32767;
				
		END_IF
			
		hmi.output.ticks := hmi.output.ticks + this.diffEncoder;
		hmi.output.distance := hmi.output.ticks / UDINT_TO_LREAL(config.ppm);
			
		IF hmi.input.resetDistance THEN
			hmi.input.resetDistance := FALSE;
			hmi.output.ticks := 0;
			hmi.output.distance := 0;
		END_IF
			
		this.distance 	:= this.diffEncoder/UDINT_TO_LREAL(config.ppm);
			
		this.lowPass.In := LREAL_TO_REAL(this.distance);
		this.lowPass();
			
		this.speed 		:= this.distance * 1000/this.diffTime; 
			
		IF this.filterSpeed.UpdateDone THEN
			this.filterSpeed.Update := FALSE;
		END_IF
			
		this.filterSpeed(In := LREAL_TO_REAL(this.speed));
			
		IF this.accLimiter.UpdateDone THEN
			this.accLimiter.Update := FALSE;
		END_IF
			
		this.accLimiter(In := this.PID.SetValue);
			
			
		IF this.PID.UpdateDone THEN
			this.PID.Update := FALSE;
		END_IF
			
		this.PID.ActValue := LREAL_TO_REAL(this.speed);
		this.PID.SetValue := this.accLimiter.Out;
		// PID TUNE
			
		IF hmi.input.tune THEN
			this.PIDtune.Enable := TRUE;
		END_IF
			
	
		this.PIDtune.ActValue := LREAL_TO_REAL(this.speed);
			
		
		IF this.PIDtune.TuningDone THEN
			this.PIDtune.Start := FALSE;
			this.PID.IntegrationPartPresetValue := this.PIDtune.Out;
			this.PID.SetIntegrationPart := TRUE;
		ELSIF this.PIDtune.Active AND NOT this.PIDtune.Update THEN
			//	this.PIDtune.Start := TRUE;
		END_IF
			
		IF this.PIDtune.UpdateDone THEN
			this.PIDtune.Update := FALSE;
		END_IF
			
		(* call function block *)
		this.PID();
		this.PIDtune();
			
		this.PID.SetIntegrationPart := FALSE;
			
		IF hmi.input.tune THEN
			hw.output.pwm := REAL_TO_INT(this.PIDtune.Out*327.67);
		ELSE
			hw.output.pwm := REAL_TO_INT(this.PID.Out*327.67);
		END_IF
				
	END_FOR
		
	ros.currentLimiter := MIN(localVariables[0].currentLimiter,localVariables[1].currentLimiter);
		
	ros.diffDist := localVariables[1].distance-localVariables[0].distance;
	ros.diffLowpassDist := localVariables[1].lowPass.Out-localVariables[0].lowPass.Out;
		
	ros.dist := (localVariables[1].distance+localVariables[0].distance)/2;
		
	IF localVariables[0].diffEncoder = localVariables[1].diffEncoder THEN
		ros.d_theta := 0.0;
		ros.cur_x := ros.cur_x + ros.dist * COS(ros.cur_theta);
		ros.cur_y := ros.cur_y + ros.dist * SIN(ros.cur_theta);
	ELSE
		ros.d_theta := -(localVariables[1].distance - localVariables[0].distance) / ros.topics.config.baseWidth;
		ros.dist := (localVariables[1].distance+localVariables[0].distance)/2;
		ros.r := ros.dist / ros.d_theta;
		ros.cur_x := ros.cur_x + ros.r * (SIN(ros.d_theta + ros.cur_theta) - SIN(ros.cur_theta));
		ros.cur_y := ros.cur_y - ros.r * (COS(ros.d_theta + ros.cur_theta) - COS(ros.cur_theta));
		ros.cur_theta := normalizeAngle(ros.cur_theta + ros.d_theta);
	END_IF
	
		
	ros.vel_x := ros.dist * (1000.0/localVariables[0].diffTime);
	ros.vel_theta := ros.d_theta * (1000.0/localVariables[0].diffTime);
	
	// set ros data
	ros.topics.odemetry.pose.pose.position.x 	:= ros.cur_x;
	ros.topics.odemetry.pose.pose.position.y	:= ros.cur_y;
	ros.topics.odemetry.pose.pose.orientation.z	:= ros.cur_theta;
		
	ros.topics.odemetry.twist.twist.linear.x  	:= ros.vel_x;
	ros.topics.odemetry.twist.twist.angular.z 	:= ros.vel_theta;
			
	setupWatchPointers;
	

	
	
	
END_PROGRAM

PROGRAM _EXIT

	ros_topics_typExit_0(Handle := ros_topics_typInit_0.Handle);

END_PROGRAM
