/*
Nintendo Switch Fightstick - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

/** \file
 *
 *  Main source file for the posts printer demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "Joystick.h"

bool ifmonthchangee(int y, int m, int d) {
	if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
	{
		if (d == 31)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (m == 4 || m == 6 || m == 9 || m == 11)
	{
		if (d == 30)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (y % 4 == 0)//Leap year
		{
			if (d == 29)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if (d == 28
				)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}

bool ifyearchangee(int m, int d)
{
	if (m == 12 && d == 31)
	{
		return 1;
	}
	else
		return 0;
}

extern const uint8_t image_data[0x12c1] PROGMEM;

typedef enum {
	viewL,
	viewR,
	viewU,
	viewD,
	walkAround,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	X,
	Y,
	A,
	B,
	L,
	R,
	Home,
	THROW,
	NOTHING,
	PLUS,
	MINUS,
	TRIGGERS,
	ifMonth_jump2,
	ifYear_jump2,
	Loop_15
} Buttons_t;

typedef struct {
	Buttons_t button;
	uint16_t duration;
} command;

static const command step[] = {
	{ B,          5 },//wake ns
	{ NOTHING,  5 },
	{ X,          5 },//from pc
	{ NOTHING,  20 },
	{ A,          5 },//fly to field
	{ NOTHING,  120 },
	{ UP,         4 },
	{ NOTHING,  10 },
	{ RIGHT,      3 },
	{ NOTHING,  10 },
	{ A,          5 },
	{ NOTHING,  20 },
	{ A,          5 },
	{ NOTHING,  350 },
	{ viewR,    19 },
	{ NOTHING,  10 },
	{ UP,        282 },
	{ NOTHING,  10 },
	//////////////////////////////////same as watt////////////////////////////////////
	{ A,        5 },
	{ NOTHING,  10 },
	{ A,          5 },
	{ NOTHING,  10 },
	{ A,          5 },
	{ NOTHING,  100 },
	 { A,          5 },
	{ NOTHING,  120 },

	 { Home,          5 },//HOME
	{ NOTHING,  50 },
	{DOWN,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },

	{A,5},
	{ NOTHING,  50 },

	{DOWN,5}, ///setting
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING, 5 },
	{DOWN,5},
	{ NOTHING, 5 },
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING,  5},
	{DOWN,5},
	{ NOTHING,  5 },
	{DOWN,5},
	{ NOTHING, 5 },
	{DOWN,5},
	{ NOTHING,  5},
	{DOWN,5},
	{ NOTHING,  5},

	{A,5},  //system
	{ NOTHING,  10 },
	{DOWN,5},
	{ NOTHING,  10 },
	{DOWN,5},
	{ NOTHING,  10 },
	{DOWN,5},
	{ NOTHING,  10 },
	{DOWN,5},
	{ NOTHING,  10 },

	{A,5},  //date
	{ NOTHING,  10 },
	{DOWN,5},
	{ NOTHING,  10 },
	{DOWN,5},
	{ NOTHING,  10 },

	{A,5},  //change date
	{ NOTHING,  10 },

	{RIGHT,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{UP,5},
	{ NOTHING,  10 },

	{ifMonth_jump2,1},
	{LEFT,5}, //if month
	{ NOTHING,  10 },
	{UP,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },

	{ifYear_jump2,1},
	{LEFT,5}, //if year
	{ NOTHING,  10 },
	{LEFT,5},
	{ NOTHING,  10 },
	{UP,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{ NOTHING,  10 },

	{RIGHT,5}, //close
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{RIGHT,5},
	{ NOTHING,  10 },
	{A,5},
	{ NOTHING,  10 },

	 { Home,       5 },//HOME
	{ NOTHING,  50 },
	{A,5},
	{ NOTHING,  50 },
	{B,5},
	{ NOTHING,  50 },
	{ A,5 },
	{ NOTHING,  250 },

		//////////////////////////////////same as watt////////////////////////////////////

	{ X,          5 },
	{ NOTHING,  20 },
	{ A,          5 },//fly to pc
	{ NOTHING,  120 },
	{ R,         1 },
	{ NOTHING,  10 },
	{ R,      1 },
	{ NOTHING,  10 },
	{ A,          5 },
	{ NOTHING,  20 },
	{ A,          5 },
	{ NOTHING,  650 },
	{ UP,    10 },
	{ NOTHING,  100 },
	{ UP,    60 },
	{ NOTHING,  10 },
	{ LEFT,    13 },
	{ NOTHING,  10 },
	{ A,    3 }, //talk to Lomi
	{ NOTHING,  10},
	{ A,    3 },
	{ NOTHING,  15 },
	{ DOWN,    3 },
	{ NOTHING,  5 },
	{ DOWN,    3 },
	{ NOTHING,  5 },
	{ A,    3 },
	{ NOTHING,  200 },
	{ A,    3 },//recieve exp
	{ NOTHING,  15 },
	{ A,    3 },
	{ NOTHING, 450 },

	{ B,    3 }, //B loop
	{ NOTHING,  35 },
	{ Loop_15,    1 },

	{ A,    3 }, //talk to Lomi again
	{ NOTHING,  10 },
	{ A,    3 },
	{ NOTHING,  15 },
	{ DOWN,    3 },
	{ NOTHING,  5 },
	{ DOWN,    3 },
	{ NOTHING,  5 },
	{ A,    3 },
	{ NOTHING,  200 },
	{ A,    3 }, //choose a new request
	{ NOTHING,  20 },
	{ A,    3 },
	{ NOTHING,  20 },
	{ A,    3 },
	{ NOTHING,  70 },
	{ X,    3 },
	{ NOTHING,  15 },
	{ B,    3 },
	{ NOTHING,  15 },
	{ A,    3 },
	{ NOTHING,  100 },
	{ A,    3 },
	{ NOTHING,  20 },
	{ A,    3 },
	{ NOTHING,  20 },
	{ A,    3 },
	{ NOTHING,  450 },
	{ B,    3 },
	{ NOTHING,  15 },
	{ B,    3 },
	{ NOTHING,  15 },
		///
	{ B,    3 },
	{ NOTHING,  30 },
	{ B,    3 },
	{ NOTHING,  5 }, //end talk
	{ RIGHT,    13 },
	{ NOTHING,  10 },
	{ DOWN,    65 },
	{ NOTHING,  100 },
};

// Main entry point.
int main(void) {

	// We'll start by performing hardware and peripheral setup.
	SetupHardware();
	// We'll then enable global interrupts for our use.
	GlobalInterruptEnable();
	DDRC = 0x00;
	// Once that's done, we'll enter an infinite loop.
	for (;;)
	{
		// We need to run our task to process and deliver data for our IN and OUT endpoints.		
		HID_Task();
		// We also need to run the main USB management task.
		USB_USBTask();
	}
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.

#ifdef ALERT_WHEN_DONE
// Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
	#warning LED and Buzzer functionality enabled.All pins on both PORTB and \PORTD will toggle when printing is done.
		DDRD = 0xFF; //Teensy uses PORTD
	PORTD = 0x0;

	//We'll just flash all pins on both ports since the UNO R3
	DDRB = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
	PORTB = 0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
#endif
// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	// We setup the HID report endpoints.
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

	// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	// We can handle two control requests: a GetReport and a SetReport.

	// Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	// If the device isn't connected and properly configured, we can't do anything here.
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	// We'll start with the OUT endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
	// We'll check to see if we received something on the OUT endpoint.
	if (Endpoint_IsOUTReceived())
	{
		// If we did, and the packet has data, we'll react to it.
		if (Endpoint_IsReadWriteAllowed())
		{
			// We'll create a place to store our data received from the host.
			USB_JoystickReport_Output_t JoystickOutputData;
			// We'll then take in that data, setting it up in our storage.
			while (Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
			// At this point, we can react to this data.

			// However, since we're not doing anything with this data, we abandon it.
		}
		// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
		Endpoint_ClearOUT();
	}

	// We'll then move on to the IN endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
	// We first check to see if the host is ready to accept data.
	if (Endpoint_IsINReady())
	{
		/*if (PINC == 0x01)
		{
			PORTD = 0x00;
		}
		else
		{
			PORTD = 0xFF;
		}*/
		// We'll create an empty report.
		USB_JoystickReport_Input_t JoystickInputData;
		// We'll then populate this report with what we want to send to the host.

		GetNextReport(&JoystickInputData);
		// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
		while (Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
		// We then send an IN packet on this endpoint.
		Endpoint_ClearIN();

	}
}

typedef enum {
	SYNC_CONTROLLER,
	SYNC_POSITION,
	BREATHE,
	PROCESS,
	CLEANUP,
	DONE
} State_t;
State_t state = SYNC_CONTROLLER;

#define ECHOES 2
int echoes = 0;
USB_JoystickReport_Input_t last_report;

int report_count = 0;
int xpos = 0;
int ypos = 0;
int bufindex = 0;
int duration_count = 0;
int portsval = 0;
bool finn = 0;
//int xTimesManyRightbig = 0;

int year = 1;
int month = 1;
int day = 1;

int countdown_15 = 14;


// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {

	// Prepare an empty report
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;

	// Repeat ECHOES times the last report
	if (echoes > 0)
	{
		memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
		echoes--;
		return;
	}

	// States and moves management
	switch (state)
	{

	case SYNC_CONTROLLER:
		state = BREATHE;
		break;

	case SYNC_POSITION:
		bufindex = 0;


		ReportData->Button = 0;
		ReportData->LX = STICK_CENTER;
		ReportData->LY = STICK_CENTER;
		ReportData->RX = STICK_CENTER;
		ReportData->RY = STICK_CENTER;
		ReportData->HAT = HAT_CENTER;


		state = BREATHE;
		break;

	case BREATHE:
		state = PROCESS;
		break;

	case PROCESS:

		switch (step[bufindex].button)
		{
		case Home:
			ReportData->Button |= SWITCH_HOME;
			break;
		case viewU:
			ReportData->RY = STICK_MIN;
			break;

		case viewL:
			ReportData->RX = STICK_MIN;
			break;

		case viewD:
			ReportData->RY = STICK_MAX;
			break;

		case viewR:
			ReportData->RX = STICK_MAX;
			break;

		case walkAround:
			ReportData->LY = STICK_MAX;
			ReportData->LX = STICK_MIN;
			ReportData->RY = STICK_MIN;
			ReportData->RX = STICK_MAX;
			//if (PINC == 0x01) { finn = 1; PORTD = 0xFF;}

			break;

		case UP:
			ReportData->LY = STICK_MIN;
			break;

		case LEFT:
			ReportData->LX = STICK_MIN;
			break;

		case DOWN:
			ReportData->LY = STICK_MAX;
			break;

		case RIGHT:
			ReportData->LX = STICK_MAX;
			break;

		case PLUS:
			ReportData->Button |= SWITCH_PLUS;
			break;

		case MINUS:
			ReportData->Button |= SWITCH_MINUS;
			break;

		case A:
			ReportData->Button |= SWITCH_A;
			break;

		case B:
			ReportData->Button |= SWITCH_B;
			break;

		case X:
			ReportData->Button |= SWITCH_X;
			break;

		case Y:
			ReportData->Button |= SWITCH_Y;
			break;

		case R:
			ReportData->Button |= SWITCH_R;
			break;

		case L:
			ReportData->Button |= SWITCH_L;
			break;

		case THROW:
			ReportData->LY = STICK_MIN;
			ReportData->Button |= SWITCH_R;
			break;
		case TRIGGERS:
			ReportData->Button |= SWITCH_L | SWITCH_R;
			break;

		default:
			ReportData->LX = STICK_CENTER;
			ReportData->LY = STICK_CENTER;
			ReportData->RX = STICK_CENTER;
			ReportData->RY = STICK_CENTER;
			ReportData->HAT = HAT_CENTER;
			break;
		}

		duration_count++;

		if (step[bufindex].button == ifMonth_jump2 && ifmonthchangee(year, month, day) == 0)
		{
			bufindex += 18;
			duration_count = 0;
			finn = 0;
		}
		else if (step[bufindex].button == ifYear_jump2 && ifyearchangee(month, day) == 0)
		{
			bufindex += 11;
			duration_count = 0;
			finn = 0;
		}
		else if (step[bufindex].button == Loop_15 && countdown_15 != 0)
		{
			bufindex--;
			bufindex--;
			duration_count = 0;
			finn = 0;
			countdown_15--;
		}
		else
		{
			if (duration_count > step[bufindex].duration || finn == 1)
			{
				bufindex++;
				duration_count = 0;
				finn = 0;
			}
		}


		if (bufindex > (int)(sizeof(step) / sizeof(step[0])) - 1)
		{
			if (ifyearchangee(month, day) == 1)
			{
				year++;
				month = 1;
				day = 1;
			}
			else if (ifmonthchangee(year, month, day) == 1)
			{
				month++;
				day = 1;
			}
			else
			{
				day++;
			}
			// state = CLEANUP;		
			bufindex = 0;
			duration_count = 0;
			countdown_15 = 14;

			state = BREATHE;

			ReportData->LX = STICK_CENTER;
			ReportData->LY = STICK_CENTER;
			ReportData->RX = STICK_CENTER;
			ReportData->RY = STICK_CENTER;
			ReportData->HAT = HAT_CENTER;

		}

		break;

	case CLEANUP:
		state = DONE;
		break;

	case DONE:
#ifdef ALERT_WHEN_DONE
		portsval = ~portsval;
		PORTD = portsval; //flash LED(s) and sound buzzer if attached
		PORTB = portsval;
		_delay_ms(250);
#endif
		return;
	}


	// Prepare to echo this report
	memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
	echoes = ECHOES;

}