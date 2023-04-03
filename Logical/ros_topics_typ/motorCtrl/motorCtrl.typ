
TYPE
	ros_typ : 	STRUCT 
		vel_theta : LREAL;
		vel_x : LREAL;
		r : LREAL;
		cur_theta : LREAL;
		dist : LREAL;
		cur_y : LREAL;
		cur_x : LREAL;
		d_theta : LREAL;
		topics : ros_topics_typ;
		diffLowpassDist : REAL;
		diffDist : LREAL;
		currentLimiter : REAL;
	END_STRUCT;
	local_typ : 	STRUCT 
		hw : hw_typ;
		hmi : hmi_typ;
		speed : LREAL;
		encoderOld : INT;
		accLimiter : MTBasicsLimiter;
		PIDCurrent : MTBasicsPID;
		PID : MTBasicsPID;
		distance : LREAL;
		diffEncoder : LREAL;
		filterCurrent : MTFilterMovingAverage;
		filterSpeed : MTFilterMovingAverage;
		diffTime : LREAL;
		timeOld : DINT;
		PIDtune : MTBasicsOscillationTuning;
		lowPass : MTFilterLowPass;
		ton_overcurrent : TON;
		currentLimiter : REAL;
	END_STRUCT;
	hw_typ : 	STRUCT 
		input : hw_input_typ;
		output : hw_output_typ;
	END_STRUCT;
	hw_output_typ : 	STRUCT 
		pwm : INT;
		clear : BOOL;
	END_STRUCT;
	hw_input_typ : 	STRUCT 
		encoder : INT;
		overload : BOOL;
		current : ARRAY[0..7]OF INT;
		nettime : DINT;
	END_STRUCT;
	hmi_typ : 	STRUCT 
		config : config_typ;
		output : hmi_output_typ;
		input : hmi_input_typ;
	END_STRUCT;
	hmi_output_typ : 	STRUCT 
		current : ARRAY[0..7]OF REAL;
		overload : BOOL;
		distance : LREAL;
		ticks : LREAL;
		currentTotal : REAL;
	END_STRUCT;
	hmi_input_typ : 	STRUCT 
		clear : BOOL;
		tune : BOOL;
		resetDistance : BOOL;
	END_STRUCT;
	config_typ : 	STRUCT 
		ppm : UDINT := 105860;
		maxCurrent : REAL := 8;
	END_STRUCT;
	common_typ : 	STRUCT 
	END_STRUCT;
	suction_typ : 	STRUCT 
		valveEnable : BOOL;
		motorEnable : BOOL;
	END_STRUCT;
END_TYPE
