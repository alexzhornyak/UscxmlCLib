# Traffic Light
VCL forms application that demonstrates classical traffic light example

![Traffic](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/TrafficLight.gif)
```xml
<?xml version="1.0" encoding="UTF-8"?>
<scxml initial="working" name="TrafficLightStateMachine" version="1.0" xmlns="http://www.w3.org/2005/07/scxml">
	<state id="working" initial="red">
		<onexit>
			<cancel sendid="ID_startGoingGreen"/>
			<cancel sendid="ID_startGoingRed"/>
		</onexit>
		<transition event="switch" target="broken" type="internal"/>
		<state id="red">
			<onentry>
				<send delay="3s" event="startGoingGreen" id="ID_startGoingGreen"/>
			</onentry>
			<transition event="startGoingGreen" target="redGoingGreen" type="internal"/>
		</state>
		<state id="yellow">
			<onexit>
				<cancel sendid="ID_goGreen"/>
				<cancel sendid="ID_goRed"/>
			</onexit>
			<state id="redGoingGreen">
				<onentry>
					<send delay="1s" event="goGreen" id="ID_goGreen"/>
				</onentry>
				<transition event="goGreen" target="green" type="internal"/>
			</state>
			<state id="greenGoingRed">
				<onentry>
					<send delay="1s" event="goRed" id="ID_goRed"/>
				</onentry>
				<transition event="goRed" target="red" type="internal"/>
			</state>
		</state>
		<state id="green">
			<onentry>
				<send delay="3s" event="startGoingRed" id="ID_startGoingRed"/>
			</onentry>
			<transition event="startGoingRed" target="greenGoingRed" type="internal"/>
		</state>
	</state>
	<state id="broken" initial="blinking">
		<onexit>
			<cancel sendid="ID_blink"/>
		</onexit>
		<transition event="switch" target="working" type="internal"/>
		<state id="blinking">
			<onentry>
				<send delay="1s" event="blink" id="ID_blink"/>
			</onentry>
			<transition event="blink" target="unblinking" type="internal"/>
		</state>
		<state id="unblinking">
			<onentry>
				<send delay="1s" event="blink" id="ID_blink"/>
			</onentry>
			<transition event="blink" target="blinking" type="internal"/>
		</state>
	</state>
</scxml>
```
