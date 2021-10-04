/*
 Name:		TestUserMCU.ino
 Created:	2/10/2020 2:56:49 PM
 Author:	POEM Technology

 Description:   A quick and dirty test project to make sure the User MCU library builds
				and can run some (very) simple code.

 Usage: Build and download into the test device. Attached a serial monitor (e.g. PuTTY) to the serial
		port put up by the device. Hit <ENTER>. Follow along in the monitor window.
*/

typedef enum {
	ShowVersion,
	FlashWhiteLED,
	ColorCycleLED
} TestState;

void clearInputBuffer()
{
	while (Serial.read() != -1) {
		; // read as fast as possible
	}
}

void waitForAnyKey()
{
	while (!Serial.available()) {
		Dash.snooze(250);
	}

	clearInputBuffer();
}

void showMessage(String msg, bool outputNewline = false)
{
	msg += outputNewline ? "\r\n" : "";
	Serial.print(msg);
}

void issueWaitPrompt()
{
	Serial.print(" Press any key to continue.");
	waitForAnyKey();
}

String colorSelector[] = { "BLUE", "RED", "YELLOW", "GREEN", "ORANGE", "TURQUOISE" }; // color to test
const uint8_t NUM_COLORS = sizeof(colorSelector) / sizeof(String);

void cycleColorLED()
{
	uint8_t c = 0;
	while (!Serial.available()) {
		bool setResult = HologramCloud.setRGB(colorSelector[c % NUM_COLORS]);
		Dash.snooze(500); // let the use enjoy :)
		c++;
	}

	HologramCloud.setRGB("BLACK"); // "BLACK" == off
}

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(115200);
	HologramCloud.begin();
	waitForAnyKey();
}

TestState state = ShowVersion;

// the loop function runs over and over again until power down or reset
void loop()
{
	switch (state) {
	case ShowVersion:
		showMessage("\f---------- DASH USER MCU TEST HARNESS ----------", true);
		showMessage("System version: " + HologramCloud.systemVersion(), true);
		state = FlashWhiteLED;
		break;

	case FlashWhiteLED:
		showMessage("Dash white LED flash test.");
		Dash.pulseLED(500, 250);
		issueWaitPrompt();
		Dash.offLED();
		state = ColorCycleLED;
		break;

	case ColorCycleLED:
		showMessage("Dash cycle color LED test. Press <Enter> to stop test.");
		cycleColorLED();
		issueWaitPrompt();
		state = ShowVersion;
		break;

	default:
		state = ShowVersion;
	}
}
