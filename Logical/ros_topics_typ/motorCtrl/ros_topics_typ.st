
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
		this.accLimiter.MaxOut := 1;
		this.accLimiter.MinOut := -1;
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
//	IF counter2 > 5 THEN
//		counter2 := 0;
	ros_topics_typCyclic_0(Handle := ros_topics_typInit_0.Handle, pros_topics_typ := ADR(ros.topics));
//	ELSE
//		counter2 := counter2 + 1;
//	END_IF
	ExComponentInfo_0(ExTargetLink := ADR(gTarget_0), ExComponentLink := ADR(gros_topics_typ_0), Enable := TRUE);
    
	ExDatamodelInfo_0(ExTargetLink := ADR(gTarget_0), Enable := TRUE, InstanceName := 'ros_topics_typ_0');
	//--------------- ex OS done ----------------
	ExSyncInfo_0(ExTargetLink := ADR(gTarget_0), Enable := TRUE);
	
	
	ExDataConnectionInfo_0(ExTargetLink := ADR(gTarget_0),Enable := TRUE);
	
	forwardTimer();
	moveForwardTimer();
	kp := 0.7;
	kd := 2500;
	suction.motorEnable := ros.topics.vaccumTopic.vaccumMotor;
	suction.valveEnable := ros.topics.vaccumTopic.vaccumValve;
	lineFollow.lineForward := ros.topics.lineFollow.lineForward;
	lineFollow.lineHoldRight := ros.topics.lineFollow.lineHoldRight;
	lineFollow.lineMode := ros.topics.lineFollow.lineMode;
	lineFollow.lineSharpTurn := ros.topics.lineFollow.lineSharpTurn;
	lineFollow.lineInterSection := ros.topics.lineFollow.lineInterSection;
	lineFollow.lineDecision := ros.topics.lineFollow.lineDecision;
	ros.topics.twist.linear.x := MAX(MIN(ros.topics.twist.linear.x,ros.topics.config.maxSpeed),ros.topics.config.minSpeed);
	lineFollow.lineFast := ros.topics.lineFollow.lineFast;
	lineFollow.lineSlow := ros.topics.lineFollow.lineSlow;
	
	
	mathCheck1 :=  LREAL_TO_REAL(0.5*(drejMere - errOld)*ros.topics.config.baseWidth/2.0);//LREAL_TO_REAL(0.3-(drejMere*0.35 + 0.01*(drejMere - errOld))*ros.topics.config.baseWidth/2.0);
	mathCheck2 := LREAL_TO_REAL(0.3+(drejMere*0.35 + 0.01*(drejMere-errOld))*ros.topics.config.baseWidth/2.0);
	
	IF localVariables[0].hmi.output.overload OR localVariables[1].hmi.output.overload OR NOT ros_topics_typCyclic_0.Operational THEN
		IF lineMode THEN
			
			CASE Test2 OF
				0:	
					IF tempDec <> FALSE THEN
						awaitDecision := FALSE;
						tempDec := FALSE;
						orientationReached := TRUE;
					ELSE
						tempDec := lineFollow.lineDecision;
//						sharpTurn  := lineFollow.lineSharpTurn;
//						sammenEnable := lineFollow.lineInterSection;
						

					END_IF
	
					
					localVariables[0].PID.SetValue := 0;
					localVariables[1].PID.SetValue := 0;
				1:
					
						IF lineFollow.lineSlow THEN
							localVariables[0].PID.SetValue := LREAL_TO_REAL(0.1-((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
							localVariables[1].PID.SetValue := LREAL_TO_REAL(0.1+((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
						ELSIF lineFollow.lineFast THEN
							localVariables[0].PID.SetValue := LREAL_TO_REAL(0.5-((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
							localVariables[1].PID.SetValue := LREAL_TO_REAL(0.5+((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
						ELSE
							localVariables[0].PID.SetValue := LREAL_TO_REAL(0.3-((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
							localVariables[1].PID.SetValue := LREAL_TO_REAL(0.3+((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
						END_IF
					errOld := LREAL_TO_REAL(ros.cur_theta - oldOrientation);
					oldOrientation := LREAL_TO_REAL(ros.cur_theta);
					drejMere := drejMere + errOld;
					
				2:
					iAlive := iAlive +1;
					orientationReached := FALSE;
					sammenEnable := FALSE;
					lineStatus.lineStatusCode := 70;
					moveForwardTimer.PT := T#3000ms;
					moveForwardTimer.IN := TRUE;
					localVariables[0].PID.SetValue := LREAL_TO_REAL(-((drejMere)*kp*ros.topics.config.baseWidth/2.0));
					localVariables[1].PID.SetValue := LREAL_TO_REAL(((drejMere)*kp*ros.topics.config.baseWidth/2.0));
					IF moveForwardTimer.Q THEN
						
						moveForwardTimer.IN := FALSE;
						orientationReached := TRUE;
						Test2 := 1;

					END_IF
				-1:
					CASE turnSwitch OF
						0:
							orientationReached := FALSE;
							startOrientation := LREAL_TO_REAL(ros.cur_theta);

							goalOrientation := startOrientation - 0.8929;
							turnSwitch := 5;
							
						5:
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							IF ros.vel_x = 0 THEN
								forwardTimer.PT := T#1000ms;
								forwardTimer.IN := TRUE;
								IF forwardTimer.Q THEN
									forwardTimer.IN := FALSE;
									turnSwitch := 7;
								END_IF
							END_IF
						
						7:
							forwardTimer.PT := T#3000ms;
							localVariables[0].PID.SetValue := 0.05;
							localVariables[1].PID.SetValue := 0.05;
							forwardTimer.IN := TRUE;
							IF forwardTimer.Q THEN
								localVariables[0].PID.SetValue := 0;
								localVariables[1].PID.SetValue := 0;
								turnSwitch := 10;
								forwardTimer.IN := FALSE;
							END_IF
						10:
							
						
							IF ros.cur_theta + 2.0 < lastOrientation OR ros.cur_theta - 2.0 > lastOrientation THEN
								iAlive := iAlive +1;
								goalOrientation := LREAL_TO_REAL(ros.cur_theta + remainingOrientation);
							ELSIF ros.cur_theta < goalOrientation THEN
								turnSwitch := 20;
							END_IF
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							remainingOrientation := LREAL_TO_REAL(goalOrientation - ros.cur_theta);
							localVariables[0].PID.SetValue := -0.05;
							localVariables[1].PID.SetValue := 0.05;
						
							
						20:
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;

							turnSwitch := 21;		
	
						21:
							ros.topics.lineStatus.lineStatusCode := 50;
							IF moveForward <> TRUE THEN
								orientationReached := TRUE;
								turnSwitch := 0;
								Test2 := 10;
							END_IF
					END_CASE
				5:
					errOld := 0;
					oldOrientation := 0;
					IF moveForward THEN
						lineStatus.lineStatusCode := 90;
						localVariables[0].PID.SetValue := 0.1;
						localVariables[1].PID.SetValue := 0.1;
						moveForwardTimer.PT := T#5000ms;
						moveForwardTimer.IN := TRUE;
						IF moveForwardTimer.Q THEN
							localVariables[0].PID.SetValue := 0.1;
							localVariables[1].PID.SetValue := 0.1;
							//Test2 := 10;
							moveForwardTimer.IN := FALSE;
						END_IF
					ELSE
						Test2 := 10;
					END_IF
						
				10:
					lineStatus.lineStatusCode := 90;
					localVariables[0].PID.SetValue := LREAL_TO_REAL(-(drejMere*0.45)*ros.topics.config.baseWidth/2.0);
					localVariables[1].PID.SetValue := LREAL_TO_REAL((drejMere*0.45)*ros.topics.config.baseWidth/2.0);
					moveForwardTimer.IN := FALSE;
				20:
					CASE turnSwitch OF
						0:
							orientationReached := FALSE;
							startOrientation := LREAL_TO_REAL(ros.cur_theta);
							IF rightTurn THEN
								goalOrientation := startOrientation - 1.4929;
								turnSwitch := 5;
							ELSE
								goalOrientation := startOrientation + 1.4929;
								turnSwitch := 5;
							END_IF
							
						5:
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							IF ros.vel_x = 0 THEN
								turnSwitch := 7;
							END_IF
						
						7:
							forwardTimer.PT := T#5000ms;
							localVariables[0].PID.SetValue := 0.05;
							localVariables[1].PID.SetValue := 0.05;
							forwardTimer.IN := TRUE;
							IF forwardTimer.Q THEN
								localVariables[0].PID.SetValue := 0;
								localVariables[1].PID.SetValue := 0;
								turnSwitch := 10;
								forwardTimer.IN := FALSE;
							END_IF
						10:
							
						
							IF ros.cur_theta + 2.0 < lastOrientation OR ros.cur_theta - 2.0 > lastOrientation THEN
								iAlive := iAlive +1;
								goalOrientation := LREAL_TO_REAL(ros.cur_theta + remainingOrientation);
							ELSIF rightTurn THEN
								IF ros.cur_theta < goalOrientation THEN
									turnSwitch := 20;
								END_IF
							ELSIF ros.cur_theta > goalOrientation THEN
								turnSwitch := 20;
							END_IF
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							remainingOrientation := LREAL_TO_REAL(goalOrientation - ros.cur_theta);
							IF rightTurn THEN
								localVariables[0].PID.SetValue := -0.05;
								localVariables[1].PID.SetValue := 0.05;
							ELSE 
								localVariables[0].PID.SetValue := 0.05;
								localVariables[1].PID.SetValue := -0.05;
								END_IF
						20:
							orientationReached := TRUE;
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							sharpTurn := FALSE;
							Test2 := 10;
							turnSwitch := 0;		
					END_CASE
					
			END_CASE
		ELSE
		localVariables[0].PID.SetValue := 0;
		localVariables[1].PID.SetValue := 0;
		END_IF
	/////////////////////////////// EXOS DEAKTIVERET ///////////////////////////////////////////////////////////////	
	/////////////////////////////// EXOS AKTIVERET//////////////////////////////////////////////////////////////////	
		
		
	ELSE
		IF ros.topics.lineFollow.lineMode THEN
			
			CASE Test2 OF
				0:	
					iAlive := iAlive +1;
					IF tempDec <> FALSE THEN
						awaitDecision := FALSE;
						tempDec := FALSE;
						orientationReached := TRUE;
						lineStatus.lineStatusCode := 20;
						

					ELSE
						
						tempDec := lineFollow.lineDecision;
						sharpTurn  := lineFollow.lineSharpTurn;
						sammenEnable := lineFollow.lineInterSection;
						lineStatus.lineStatusCode := 20;

					END_IF
	
					
					localVariables[0].PID.SetValue := 0;
					localVariables[1].PID.SetValue := 0;
				1:
					//iAlive := iAlive +1;
					IF lineFollow.lineSlow THEN
						localVariables[0].PID.SetValue := LREAL_TO_REAL(0.1-((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
						localVariables[1].PID.SetValue := LREAL_TO_REAL(0.1+((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
					ELSIF lineFollow.lineFast THEN
						localVariables[0].PID.SetValue := LREAL_TO_REAL(0.5-((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
						localVariables[1].PID.SetValue := LREAL_TO_REAL(0.5+((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
					ELSE
						localVariables[0].PID.SetValue := LREAL_TO_REAL(0.3-((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
						localVariables[1].PID.SetValue := LREAL_TO_REAL(0.3+((drejMere-errOld)*kp + kd*(errOld)*ros.topics.config.baseWidth/2.0));
					END_IF
					errOld := LREAL_TO_REAL(ros.cur_theta - oldOrientation);
					oldOrientation := LREAL_TO_REAL(ros.cur_theta);
					drejMere := drejMere + errOld;
					
					
				2:
					orientationReached := FALSE;
					lineStatus.lineStatusCode := 70;
					moveForwardTimer.PT := T#3000ms;
					moveForwardTimer.IN := TRUE;
					localVariables[0].PID.SetValue := LREAL_TO_REAL(0.1-((drejMere)*kp*ros.topics.config.baseWidth/2.0));
					localVariables[1].PID.SetValue := LREAL_TO_REAL(0.1+((drejMere)*kp*ros.topics.config.baseWidth/2.0));
					
					IF moveForwardTimer.Q THEN
						orientationReached := TRUE;
						moveForwardTimer.IN := FALSE;
						

					END_IF
				-1:
					
					CASE turnSwitch OF
						0:
							orientationReached := FALSE;
							startOrientation := LREAL_TO_REAL(ros.cur_theta);

							goalOrientation := startOrientation - 1.4929;
							turnSwitch := 5;
							
						5:
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							IF ros.vel_x = 0 THEN
								forwardTimer.PT := T#1000ms;
								forwardTimer.IN := TRUE;
								IF forwardTimer.Q THEN
									forwardTimer.IN := FALSE;
									turnSwitch := 7;
								END_IF
							END_IF
						
						7:
							forwardTimer.PT := T#3000ms;
							localVariables[0].PID.SetValue := 0.05;
							localVariables[1].PID.SetValue := 0.05;
							forwardTimer.IN := TRUE;
							IF forwardTimer.Q THEN
								localVariables[0].PID.SetValue := 0;
								localVariables[1].PID.SetValue := 0;
								turnSwitch := 10;
								forwardTimer.IN := FALSE;
							END_IF
						10:
							
						
							IF ros.cur_theta + 2.0 < lastOrientation OR ros.cur_theta - 2.0 > lastOrientation THEN
								goalOrientation := LREAL_TO_REAL(ros.cur_theta + remainingOrientation);
							ELSIF ros.cur_theta < goalOrientation THEN
								turnSwitch := 20;
							END_IF
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							remainingOrientation := LREAL_TO_REAL(goalOrientation - ros.cur_theta);
							localVariables[0].PID.SetValue := -0.05;
							localVariables[1].PID.SetValue := 0.05;
						
							
						20:
							
							lineStatus.lineStatusCode := 50;
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							
							turnSwitch := 21;	
						21:
							lineStatus.lineStatusCode := 50;
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							IF TRUE <> ros.topics.lineFollow.lineForward THEN
								orientationReached := TRUE;
								turnSwitch := 0;
								Test2 := 10;
							END_IF
					END_CASE
						

				5:
					errOld := 0;
					oldOrientation := 0;
					IF lineFollow.lineForward THEN
						lineStatus.lineStatusCode := 90;
						localVariables[0].PID.SetValue := 0.1;
						localVariables[1].PID.SetValue := 0.1;
						moveForwardTimer.PT := T#5000ms;
						moveForwardTimer.IN := TRUE;
						IF moveForwardTimer.Q THEN
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							Test2 := 10;
							moveForwardTimer.IN := FALSE;
						END_IF
					
					ELSE
						Test2 := 10;
					END_IF
						
				10:
					lineStatus.lineStatusCode := 90;
					localVariables[0].PID.SetValue := LREAL_TO_REAL(-(drejMere*0.45)*ros.topics.config.baseWidth/2.0);
					localVariables[1].PID.SetValue := LREAL_TO_REAL((drejMere*0.45)*ros.topics.config.baseWidth/2.0);
					moveForwardTimer.IN := FALSE;
				20:
					CASE turnSwitch OF
						0:
							lineStatus.lineStatusCode := 20;
							orientationReached := FALSE;
							startOrientation := LREAL_TO_REAL(ros.cur_theta);
							IF rightTurn THEN
								goalOrientation := startOrientation - 1.4929;
								turnSwitch := 5;
							ELSE
								goalOrientation := startOrientation + 1.4929;
								turnSwitch := 5;
							END_IF
							
						5:
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							IF ros.vel_x = 0 THEN
								turnSwitch := 7;
							END_IF
						
						7:
							forwardTimer.PT := T#5000ms;
							localVariables[0].PID.SetValue := 0.05;
							localVariables[1].PID.SetValue := 0.05;
							forwardTimer.IN := TRUE;
							IF forwardTimer.Q THEN
								localVariables[0].PID.SetValue := 0;
								localVariables[1].PID.SetValue := 0;
								turnSwitch := 10;
								forwardTimer.IN := FALSE;
							END_IF
						10:
							
						
							IF ros.cur_theta + 2.0 < lastOrientation OR ros.cur_theta - 2.0 > lastOrientation THEN
								goalOrientation := LREAL_TO_REAL(ros.cur_theta + remainingOrientation);
							ELSIF rightTurn THEN
								IF ros.cur_theta < goalOrientation THEN
									turnSwitch := 20;
								END_IF
							ELSIF ros.cur_theta > goalOrientation THEN
								turnSwitch := 20;
							END_IF
							lastOrientation := LREAL_TO_REAL(ros.cur_theta);
							remainingOrientation := LREAL_TO_REAL(goalOrientation - ros.cur_theta);
							IF rightTurn THEN
								localVariables[0].PID.SetValue := -0.05;
								localVariables[1].PID.SetValue := 0.05;
							ELSE 
								localVariables[0].PID.SetValue := 0.05;
								localVariables[1].PID.SetValue := -0.05;
							END_IF
						20:
							orientationReached := TRUE;
							localVariables[0].PID.SetValue := 0;
							localVariables[1].PID.SetValue := 0;
							sharpTurn := FALSE;
							Test2 := 10;
							turnSwitch := 0;		
					END_CASE
					
			END_CASE	
		ELSE
			
			localVariables[0].PID.SetValue := LREAL_TO_REAL(ros.topics.twist.linear.x + ros.topics.twist.angular.z * ros.topics.config.baseWidth / 2.0) * ros.currentLimiter;  // m/s
			localVariables[1].PID.SetValue := LREAL_TO_REAL(ros.topics.twist.linear.x - ros.topics.twist.angular.z * ros.topics.config.baseWidth / 2.0) * ros.currentLimiter;
		END_IF
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
	
	IF lineStatus.lineStatusCode = 0 THEN
		evaluateStatus;
	END_IF
	
//	IF counter > 5  THEN
//		counter := 0;
	ros.topics.odemetry.pose.pose.position.x 	:= ros.cur_x;
	ros.topics.odemetry.pose.pose.position.y	:= ros.cur_y;
	ros.topics.odemetry.pose.pose.orientation.z	:= ros.cur_theta;
		
	ros.topics.odemetry.twist.twist.linear.x  	:= ros.vel_x;
	ros.topics.odemetry.twist.twist.angular.z 	:= ros.vel_theta;
	IF ros.topics.lineStatus.lineStatusCode <> lineStatus.lineStatusCode THEN //AND lineFollow.lineMode THEN	
		ros.topics.lineStatus.lineStatusCode := lineStatus.lineStatusCode;
	END_IF
//	ELSE
//		counter := counter +1;
//	END_IF
	lineStatus.lineStatusCode := 0;
	setupWatchPointers;
	

	
	
	
END_PROGRAM

PROGRAM _EXIT

	ros_topics_typExit_0(Handle := ros_topics_typInit_0.Handle);

END_PROGRAM
