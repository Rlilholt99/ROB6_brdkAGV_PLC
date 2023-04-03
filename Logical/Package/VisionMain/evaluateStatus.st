
ACTION evaluateStatus: 

	IF lineFollow.lineMode THEN
		lineStatus.lineStatusCode := lineStatus.lineStatusCode +1;
	END_IF
	IF lineFollow.lineSharpTurn THEN
		lineStatus.lineStatusCode := lineStatus.lineStatusCode + 12;
	END_IF
	IF lineFollow.lineForward THEN
		lineStatus.lineStatusCode := lineStatus.lineStatusCode + 10;
	END_IF
	IF lineFollow.lineHoldRight THEN
		lineStatus.lineStatusCode := lineStatus.lineStatusCode + 116;
	END_IF
	IF lineFollow.lineInterSection THEN
		lineStatus.lineStatusCode := lineStatus.lineStatusCode + 14;
	END_IF
	
	updateLineStatus := TRUE;
	
	
END_ACTION
