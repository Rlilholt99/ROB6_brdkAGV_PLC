
TYPE
	ros_topic_point_typ : 	STRUCT  (*this expresses velocity in free space broken into its linear and angular parts.*)
		y : LREAL;
		z : LREAL;
		x : LREAL;
	END_STRUCT;
	ros_topic_vector3_typ : 	STRUCT  (*this expresses velocity in free space broken into its linear and angular parts.*)
		y : LREAL;
		z : LREAL;
		x : LREAL;
	END_STRUCT;
	ros_topic_twist_typ : 	STRUCT  (*This expresses velocity in free space broken into its linear and angular parts.*)
		angular : ros_topic_vector3_typ;
		linear : ros_topic_vector3_typ;
	END_STRUCT;
	ros_topic_twistCor_typ : 	STRUCT  (*This expresses velocity in free space with uncertainty.*)
		twist : ros_topic_twist_typ;
	END_STRUCT;
	ros_topic_quaternion_typ : 	STRUCT  (*This represents an orientation in free space in quaternion form.*)
		y : LREAL;
		z : LREAL;
		w : LREAL;
		x : LREAL;
	END_STRUCT;
	ros_topic_odemety_point_typ : 	STRUCT  (*This contains the position of a point in free space*)
		y : LREAL;
		z : LREAL;
		x : LREAL;
	END_STRUCT;
	ros_topic_pose_typ : 	STRUCT  (*A representation of pose in free space, composed of position and orientation. *)
		position : ros_topic_point_typ;
		orientation : ros_topic_quaternion_typ;
	END_STRUCT;
	ros_topic_poseCor_typ : 	STRUCT  (*This represents a pose in free space with uncertainty.*)
		pose : ros_topic_pose_typ;
	END_STRUCT;
	ros_topic_time_typ : 	STRUCT 
		nsec : UDINT;
		sec : UDINT;
	END_STRUCT;
	ros_topic_header_typ : 	STRUCT 
		stamp : ros_topic_time_typ;
		seq : UDINT;
		frame_id : STRING[20];
	END_STRUCT;
	ros_topic_odemety_typ : 	STRUCT 
		pose : ros_topic_poseCor_typ;
		twist : ros_topic_twistCor_typ;
		header : ros_topic_header_typ;
	END_STRUCT;
	ros_config_typ : 	STRUCT 
		maxSpeed : LREAL := 2;
		minSpeed : LREAL := -2;
		baseWidth : LREAL := 0.53;
	END_STRUCT;
	ros_topics_typ : 	STRUCT 
		odemetry : ros_topic_odemety_typ; (*PUB*)
		twist : ros_topic_twist_typ; (*SUB*)
		config : ros_config_typ; (*SUB*)
		encoder : ros_encoder; (*PUB*)
		vaccumTopic : ros_armBools; (*SUB*)
		lineFollow : ros_lineMode; (*SUB*)
		lineStatus : ros_lineStatus; (*PUB*)
	END_STRUCT;
	ros_motorCtrl : 	STRUCT 
		twist : ros_topic_twist_typ; (*SUB*)
		config : ros_config_typ; (*SUB*)
	END_STRUCT;
	ros_encoder : 	STRUCT 
		encoder1 : INT;
		encoder2 : INT;
	END_STRUCT;
	ros_armBools : 	STRUCT 
		vaccumValve : BOOL;
		vaccumMotor : BOOL;
	END_STRUCT;
	ros_lineMode : 	STRUCT 
		lineMode : BOOL;
		lineSharpTurn : BOOL;
		lineHoldRight : BOOL;
		lineForward : BOOL;
		lineInterSection : BOOL;
	END_STRUCT;
	ros_lineStatus : 	STRUCT 
		lineStatusCode : INT;
	END_STRUCT;
END_TYPE
