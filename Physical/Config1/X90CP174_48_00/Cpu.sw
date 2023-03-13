<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1">
    <Task Name="motorCtrl" Source="ros_topics_typ.motorCtrl.prg" Memory="UserROM" Language="IEC" Debugging="true" />
    <Task Name="VisionMain" Source="Package.VisionMain.prg" Memory="UserROM" Language="IEC" Debugging="true" Disabled="true" />
    <Task Name="WaterTank_" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#2" />
  <TaskClass Name="Cyclic#3" />
  <TaskClass Name="Cyclic#4" />
  <TaskClass Name="Cyclic#5" />
  <TaskClass Name="Cyclic#6" />
  <TaskClass Name="Cyclic#7" />
  <TaskClass Name="Cyclic#8" />
  <NcDataObjects>
    <NcDataObject Name="mcdrivelog" Source="" Memory="UserROM" Language="Binary" />
  </NcDataObjects>
  <Binaries>
    <BinaryObject Name="udbdef" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="FWRules" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="TCData" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="arcoal" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arflatprv" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arsvcreg" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="mvLoader" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visionUnit" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="mCoWebSc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="ashwac" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Role" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="User" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="ashwd" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="target" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="components" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Settings" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="viApp" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="viComp" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="ViLine" Source="" Memory="UserROM" Language="Binary" />
  </Binaries>
  <Libraries>
    <LibraryObject Name="brsystem" Source="Libraries.brsystem.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="MTTypes" Source="Libraries.MTTypes.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="sys_lib" Source="Libraries.sys_lib.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="runtime" Source="Libraries.runtime.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="ROS" Source="Libraries.ROS.lby" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="standard" Source="Libraries.standard.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="ExOS" Source="Libraries.ExOS.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="ExDeploy" Source="Libraries.ExDeploy.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="ExData" Source="Libraries.ExData.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="FileIO" Source="Libraries.FileIO.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsIOTime" Source="Libraries.AsIOTime.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="ArEventLog" Source="Libraries.ArEventLog.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsTCP" Source="Libraries.AsTCP.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="ros_topics" Source="ros_topics_typ.ros_topics.lby" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="astime" Source="Libraries.astime.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="ViBase" Source="Libraries.ViBase.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsZip" Source="Libraries.AsZip.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsARCfg" Source="Libraries.AsARCfg.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="powerlnk" Source="Libraries.powerlnk.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsEPL" Source="Libraries.AsEPL.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsIO" Source="Libraries.AsIO.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="MTBasics" Source="Libraries.MTBasics.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="MTFilter" Source="Libraries.MTFilter.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsIecCon" Source="Libraries.AsIecCon.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsIODiag" Source="Libraries.AsIODiag.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsETH" Source="Libraries.AsETH.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsBrWStr" Source="Libraries.AsBrWStr.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsHttp" Source="Libraries.AsHttp.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsMem" Source="Libraries.AsMem.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="AsBrStr" Source="Libraries.AsBrStr.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="CoTrace" Source="Libraries.CoTrace.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="McBase" Source="Libraries.McBase.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="brdkViBase" Source="Libraries.brdkViBase.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkViCal" Source="Libraries.brdkViCal.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkViGeom" Source="Libraries.brdkViGeom.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkViImg" Source="Libraries.brdkViImg.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkViMapp" Source="Libraries.brdkViMapp.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkViVF" Source="Libraries.brdkViVF.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkWS" Source="Libraries.brdkWS.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="MTLinAlg" Source="Libraries.MTLinAlg.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brdkMU" Source="Libraries.brdkMU.lby" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="brdkSTR" Source="Libraries.brdkSTR.lby" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="AsSem" Source="Libraries.AsSem.lby" Memory="UserROM" Language="binary" Debugging="true" />
    <LibraryObject Name="mpbase" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="arssl" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>