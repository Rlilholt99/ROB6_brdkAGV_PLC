(*-----------------------------------------------------------------------------------------------------------------------------------------*)
(*General vision structures*)

TYPE
	typVisionMain : 	STRUCT 
		CMD : typVisionCommand;
		CFG : typVisionConfig;
		DAT : typVisionData;
		FCT : typVisionFunction;
		DIA : typVisionDiag;
		OPT : typVisionOptics;
		HW : typVisionHW;
	END_STRUCT;
	typVisionCommand : 	STRUCT 
		ImageTrigger : BOOL;
		ImageTriggerReset : BOOL;
		AutoSetupStartStop : BOOL;
		VaSwitchApplication : BOOL;
		SaveDiagData : BOOL;
		ReadCameraInfo : BOOL;
	END_STRUCT;
	typVisionConfig : 	STRUCT 
		VisionFunction : enumVisionFunction;
		PowerlinkNode : USINT;
		DataStructure : UDINT;
		ComponentLink : ViComponentType;
		ResolutionWidth_X : UINT; (*Nr of sensor pixels in x direction (width). Is always bigger than height*)
		ResolutionHeight_Y : UINT; (*Nr of sensor pixels in y direction (height). Is always smaller than width*)
	END_STRUCT;
	typVisionFunction : 	STRUCT 
		ApplicationName : STRING[40];
		Status : DINT;
	END_STRUCT;
	typVisionDiag : 	STRUCT 
		DiagName : STRING[80] := 'diag.tgz';
		Status : DINT;
	END_STRUCT;
	typVisionData : 	STRUCT 
		Enable : BOOL := FALSE;
		MaxItemCnt : USINT := 10;
		NettimeDelay : DINT;
		Timeout : UINT := 5000;
		Gain : USINT;
		Focus : UINT;
		Exposure : UDINT;
		FlashColor : USINT := 1;
		FlashSegment : USINT := 15;
		LedColor : USINT;
		IRFilter : BOOL;
		OffsetROIX : DINT;
		OffsetROIY : DINT;
		OffsetROIRotationCenterX : DINT;
		OffsetROIRotationCenterY : DINT;
		OffsetROIOrientation : INT;
		ChromaticLock : BOOL;
		Alignment : USINT;
		AutoSetupGain : USINT;
		AutoSetupFocus : UINT;
		AutoSetupExposure : UDINT;
		AcquisitionAcceptedCnt : USINT;
		AcquisitionCompletedCnt : USINT;
		AcquisitionFailedCnt : USINT;
		LightWarningCnt : USINT;
		ImageProcessingError : UINT;
		ImageProcessingActive : BOOL;
		ImageNettime : DINT;
		ProcessingTimeCamera : UINT;
		ProcessingTimeFunction : UINT;
		ResultCnt : USINT;
		CockpitIsActive : BOOL;
		Status : UDINT;
		StatusText : STRING[30];
		Active : BOOL;
	END_STRUCT;
	typVisionOptics : 	STRUCT 
		OrderNr : STRING[40];
		Binning : BOOL;
		ResolutionX : INT;
		ResolutionY : INT;
		Lens : USINT;
		Sensor : USINT;
		ValidDistance : BOOL;
		Distance_mm : LREAL;
		DistanceLens_mm : LREAL;
		HyperFocalDistance_mm : LREAL;
		PixelSize_um : LREAL;
		PixelSizeBinning_um : LREAL;
		FocalLength_mm : LREAL;
		MinObjectDist_mm : LREAL;
		MaxObjectDist_mm : LREAL;
		Aperture : LREAL;
		CircleOfConfusion_mm : LREAL;
		DepthOfFieldNearPos_mm : LREAL;
		DepthOfFieldFarPos_mm : LREAL;
		DepthOfField_mm : LREAL;
		FieldOfView_X_mm : LREAL;
		FieldOfView_Y_mm : LREAL;
		Resolution_mmPerPx : LREAL;
	END_STRUCT;
	typVisionHW : 	STRUCT 
		Connected : BOOL;
		Ready : BOOL;
		DigitalInput01 : BOOL;
		DigitalOutput01 : BOOL;
		DigitalOutputStatus01 : BOOL;
		DigitalOutput02 : BOOL;
		DigitalOutputStatus02 : BOOL;
		SerialNumber : UDINT;
		ID : UDINT;
		Variant : UDINT;
		Firmware : UDINT;
		Undervoltage : BOOL;
		Temperature : SINT;
	END_STRUCT;
	enumVisionFunction : 
		(
		enumFunctionNone,
		enumBlob
		);
END_TYPE

(*-----------------------------------------------------------------------------------------------------------------------------------------*)
(*General Light structures*)
(*-----------------------------------------------------------------------------------------------------------------------------------------*)
(*General nettime structures*)

TYPE
	typVisionNettime : 	STRUCT 
		CMD : typNettimeCommand;
		CFG : typNettimeConfig;
		DAT : typNettimeData;
	END_STRUCT;
	typNettimeCommand : 	STRUCT 
		Activate : BOOL;
	END_STRUCT;
	typNettimeConfig : 	STRUCT 
		AxisPeriod : LREAL; (*Units when period repeats*)
		AxisPositionTrigger : LREAL; (*Position in the period where trigger is fired*)
		AxisPositionDelay_Cycles : LREAL; (*Delay in PLK cycles for the read position from drive*)
		VisionPreTrigger_Cycles : DINT; (*Number of powerlink cycles the trigger has to be send ahead*)
	END_STRUCT;
	typNettimeData : 	STRUCT 
		NettimeNext_us : DINT; (*Calculated nettime for next trigger*)
		NettimeDelta_us : DINT; (*Difference between current nettime and calculated nettime for the camera trigger*)
		AxisPosition : LREAL; (*Current axis position*)
		AxisVelocity : LREAL; (*Current axis position*)
		AxisInVelocity : BOOL; (*Indicates that the axis is moving at speed*)
		Overflow : DINT; (*Counts up when a trigger could not be set on time*)
	END_STRUCT;
END_TYPE

(*-----------------------------------------------------------------------------------------------------------------------------------------*)
(*Color detection structures*)

TYPE
	typVisionColor : 	STRUCT 
		CMD : typVisionColorCommand;
		CFG : typVisionColorConfig;
		DAT : typVisionColorData;
	END_STRUCT;
	typVisionColorCommand : 	STRUCT 
		Evaluate : BOOL; (*Evaluate new product and try to determine the color*)
		Teach : BOOL; (*Teach a new product and store color information*)
		ResetError : BOOL; (*Reset error state*)
	END_STRUCT;
	typVisionColorConfig : 	STRUCT 
		FlashColor1 : USINT := 1; (*First flash color (red)*)
		FlashColor2 : USINT := 2; (*Second flash color (green)*)
		FlashColor3 : USINT := 3; (*Third flash color (blue)*)
		FlashColor4 : USINT := 4; (*Fourth flash color (lime)*)
		ProductName : ARRAY[1..MAX_NUM_PRODUCTS]OF STRING[80]; (*Name of the product*)
		GrayValue1 : ARRAY[1..MAX_NUM_PRODUCTS]OF UINT; (*Mean gray value for first flash color*)
		GrayValue2 : ARRAY[1..MAX_NUM_PRODUCTS]OF UINT; (*Mean gray value for second flash color*)
		GrayValue3 : ARRAY[1..MAX_NUM_PRODUCTS]OF UINT; (*Mean gray value for third flash color*)
		GrayValue4 : ARRAY[1..MAX_NUM_PRODUCTS]OF UINT; (*Mean gray value for fourth flash color*)
		TeachingIndex : UINT(1..1000)  := 1; (*Index that is currently taught*)
		MaxError : UINT := 100; (*Maximum error for all gray values*)
		MinDifference : UINT := 10; (*Minimum distance to next best value*)
	END_STRUCT;
	typVisionColorData : 	STRUCT 
		GrayValue1 : UINT; (*Current mean gray value for first flash color*)
		GrayValue2 : UINT; (*Current mean gray value for second flash color*)
		GrayValue3 : UINT; (*Current mean gray value for third flash color*)
		GrayValue4 : UINT; (*Current mean gray value for fourth flash color*)
		TotalError : ARRAY[1..MAX_NUM_PRODUCTS]OF UINT; (*Total error of mean gray value for all products*)
		LowError : UINT; (*Lowest error found*)
		LowDistance : UINT; (*Distance to next best value*)
		LowIndex : UINT; (*Index of the best value*)
		LowName : STRING[80]; (*Product name of the best value*)
		Status : UINT; (*Status of color detection*)
	END_STRUCT;
END_TYPE

(*-----------------------------------------------------------------------------------------------------------------------------------------*)
(*Image upload structures*)

TYPE
	typVisionImage : 	STRUCT 
		CMD : typVisionImageCommand;
		CFG : typVisionImageConfig;
		DAT : typVisionImageData;
	END_STRUCT;
	typVisionImageCommand : 	STRUCT 
		CreateDir : BOOL;
		DeleteDir : BOOL;
		DeleteImage : BOOL;
		Refresh : BOOL;
		DownloadImage : BOOL;
		SaveImageOnPLC : BOOL;
		ResetError : BOOL;
		RefreshCrosshair : BOOL;
	END_STRUCT;
	typVisionImageConfig : 	STRUCT 
		FileDevice : STRING[80];
		DirName : STRING[80];
		PlkIPWithoutNode : STRING[80];
		EthDevice : STRING[80];
		EthIP : STRING[80];
		ConvertCycles : UDINT;
		Format : USINT; (*0: jpg. 1: bmp*)
		QualityJPG : USINT;
		UploadBmpJpg : BOOL;
		UploadSVG : BOOL;
		MainPageQualityJPG : USINT; (*quality of the jpg image on the main page. min:0, max:100, good value: 30*)
		ImageRotation_deg : UINT; (*rotation of the whole image, steps in degree. Only values 0, 90. 180, 270 make sense*)
	END_STRUCT;
	typVisionImageData : 	STRUCT 
		Images : ARRAY[0..19]OF STRING[80];
		Status : UINT;
		VisionDisabled : BOOL;
	END_STRUCT;
END_TYPE

(*-----------------------------------------------------------------------------------------------------------------------------------------*)
(*Vision specific structures*)

TYPE
	typBlobMain : 	STRUCT  (*Blob function structures*)
		RegionalFeature : BOOL;
		EnhancedBlobInformation : BOOL := FALSE;
		ModelNumber : ARRAY[1..MAX_NUM_RESULTS]OF USINT;
		Clipped : ARRAY[1..MAX_NUM_RESULTS]OF BOOL;
		Area : ARRAY[1..MAX_NUM_RESULTS]OF UDINT;
		PositionX : ARRAY[1..MAX_NUM_RESULTS]OF DINT;
		PositionY : ARRAY[1..MAX_NUM_RESULTS]OF DINT;
		Orientation : ARRAY[1..MAX_NUM_RESULTS]OF INT;
		MeanGrayValue : ARRAY[1..MAX_NUM_RESULTS]OF USINT;
		Length : ARRAY[1..MAX_NUM_RESULTS]OF UDINT;
		Width : ARRAY[1..MAX_NUM_RESULTS]OF UDINT;
	END_STRUCT;
END_TYPE

(*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*)
(*Structure that contains the recipe variables*)

TYPE
	recVariable : 	STRUCT 
		Cam : ARRAY[1..MAX_NUM_CAMS]OF recVariableCam;
		Light : ARRAY[1..10]OF recVariableLight;
	END_STRUCT;
	recVariableCam : 	STRUCT 
		ApplicationName : STRING[40];
		MaxItemCnt : USINT := 10;
		Timeout : UINT := 5000;
		Gain : USINT;
		Focus : UINT;
		Exposure : UDINT;
		FlashColor : USINT := 1;
		FlashSegment : USINT := 15;
	END_STRUCT;
	recVariableLight : 	STRUCT 
		Timeout : UINT := 5000;
		Exposure : UDINT;
		FlashColor : USINT := 1;
		FlashSegment : UINT := 15;
		SetAngle : UINT := 45;
	END_STRUCT;
END_TYPE
