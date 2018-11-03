/*
  Firmware source code for Anton Lazarev's custom-designed keyboard.

  Features:
   -  68 keys
   -  compact ortholinear format
   -  carbon fiber construction
   -  full on-the-fly programmability
   -  HID Mouse functionality
   -  MIDI functionality
   -  Built-in Colemak layout
   -  14-key HID rollover
   -  n-key MIDI rollover
   -  multiple function layers

  All work on the keyboard, including mechanical design, PCB design, and
  coding was completed by Anton Lazarev     ---     https://antonok.com

  This program implements Dean Camera's LUFA Library for USB compatibility.
  Copyright information for LUFA can be found at www.lufa-lib.org.
*/

#include "firmware.h"

#define SQRT2 1.4142

//Buffer to hold the previously generated Keyboard HID report,
//for comparison purposes inside the HID class driver.
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_ExtendedKeyboardReport_Data_t)];

//Buffer to hold the previously generated Mouse HID report,
//for comparison purposes inside the HID class driver.
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_Mouse_HScrollReport_Data_t)];

/* LUFA HID Class driver interface configuration and state information.
 * This structure is passed to all HID Class driver functions, so that
 * multiple instances of the same class within a device can be
 * differentiated from one another. */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface = {
        .Config = {
                .InterfaceNumber          = INTERFACE_ID_Keyboard,
                .ReportINEndpoint         = {
                        .Address      = KEYBOARD_EPADDR,
                        .Size         = KEYBOARD_EPSIZE,
                        .Banks        = 1,
                },
                .PrevReportINBuffer       = PrevKeyboardHIDReportBuffer,
                .PrevReportINBufferSize   = sizeof(PrevKeyboardHIDReportBuffer),
        },
};

/* LUFA HID Class driver interface configuration and state information.
 * This structure is passed to all HID Class driver functions, so that
 * multiple instances of the same class within a device can be
 * differentiated from one another. */
USB_ClassInfo_HID_Device_t Mouse_HID_Interface = {
        .Config = {
                .InterfaceNumber          = INTERFACE_ID_Mouse,
                .ReportINEndpoint         = {
                        .Address        = MOUSE_IN_EPADDR,
                        .Size           = MOUSE_EPSIZE,
                        .Banks          = 1,
                },
                .PrevReportINBuffer    = PrevMouseHIDReportBuffer,
                .PrevReportINBufferSize = sizeof(PrevMouseHIDReportBuffer),
        },
};

/* LUFA MIDI Class driver interface configuration and state information.
 * This structure is passed to all MIDI Class driver functions, so that
 * multiple instances of the same class within a device can be
 * differentiated from one another. */
USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface = {
        .Config = {
                .StreamingInterfaceNumber = INTERFACE_ID_AudioStream,
                .DataINEndpoint           = {
                        .Address      = MIDI_STREAM_IN_EPADDR,
                        .Size         = MIDI_STREAM_EPSIZE,
                        .Banks        = 1,
                },
                .DataOUTEndpoint          = {
                        .Address      = MIDI_STREAM_OUT_EPADDR,
                        .Size         = MIDI_STREAM_EPSIZE,
                        .Banks        = 1,
                },
        },
};

int main(void) {
        initPins();
#ifdef ENABLE_EMERGENCY_RESET
        //Debug reset can be enabled in global_vars.h
        debugReset();
#endif
        //LUFA USB initialization
        SetupHardware();
        GlobalInterruptEnable();

        while (1) {
                setCurrentKeyMap();
                if(stopped && no_keys_pressed())
                        stopped = 0;
                checkKeyPresses();
                updateKeyChanges();
                doKeyFunctionCalls();
                doKeyMIDItasks();
                //HID keyboard/mouse reports are handled by callbacks.

                //LUFA functions for each device interface
                HID_Device_USBTask(&Keyboard_HID_Interface);
                HID_Device_USBTask(&Mouse_HID_Interface);
                MIDI_Device_USBTask(&Keyboard_MIDI_Interface);
                USB_USBTask();
        }
}

void initPins(void) {
        DDRD = 0b11111111;   //Port D is the keymatrix output port
        PORTD = 0b00000000;  //Initialize port D to be low

        DDRB &= 0;           //Ports B and F are keymatrix input ports
        DDRF &= 0;

}

// Match each keyboard layer name to a keymap
void setCurrentKeyMap(void) {
        switch(current_layer) {
                case FN_l: current_map = &FN_map; break;
                case LRDN_l: current_map = &LRDN_map; break;
                case LRUP_l: current_map = &LRUP_map; break;
                case MOUSE_l: current_map = &MOUSE_map; break;
                case MIDI_l: current_map = &MIDI_map; break;
                case QWERTY_l: current_map = &QWERTY_map; break;
                case COLEMAK_l: current_map = &COLEMAK_map; break;
        }
}

// Event handler for the library USB Connection event.
void EVENT_USB_Device_Connect(void) {
}

// Event handler for the library USB Disconnection event.
void EVENT_USB_Device_Disconnect(void) {
}

// Event handler for the library USB Configuration Changed event.
// USB configuration switching is currently unsupported.
void EVENT_USB_Device_ConfigurationChanged(void) {
        bool ConfigSuccess = true;

        ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
        ConfigSuccess &= HID_Device_ConfigureEndpoints(&Mouse_HID_Interface);
        ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&Keyboard_MIDI_Interface);

        USB_Device_EnableSOFEvents();
}

// Event handler for the library USB Control Request reception event.
void EVENT_USB_Device_ControlRequest(void) {
        HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
        HID_Device_ProcessControlRequest(&Mouse_HID_Interface);
        MIDI_Device_ProcessControlRequest(&Keyboard_MIDI_Interface);
}

// Event handler for the USB device Start Of Frame event.
void EVENT_USB_Device_StartOfFrame(void) {
        HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
        HID_Device_MillisecondElapsed(&Mouse_HID_Interface);
}

// HID class driver callback function for the creation of Mouse and Keyboard HID reports to the host.
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize) {
        if(HIDInterfaceInfo == &Keyboard_HID_Interface) {
                if(stopped) {
                        *ReportSize = sizeof(USB_KeyboardReport_Data_t);
                        return false;
                }
                else {
                        uint8_t UsedKeyCodes = 0;
                        //In report mode, up to 14 keypresses can be registered at once.
                        if (HIDInterfaceInfo->State.UsingReportProtocol) {
                                USB_ExtendedKeyboardReport_Data_t* KeyboardReport = (USB_ExtendedKeyboardReport_Data_t*) ReportData;
                                read_from_mapping_HID(&KeyboardReport->Modifier, KeyboardReport->KeyCode, &UsedKeyCodes, *current_map);
                                checkRolloverError(KeyboardReport->KeyCode, &UsedKeyCodes, 14);
                                *ReportSize = sizeof(USB_ExtendedKeyboardReport_Data_t);
                        }
                        else {  //In boot protocol mode, up to 6 keypresses can be registered at once.
                                USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*) ReportData;
                                read_from_mapping_HID(&KeyboardReport->Modifier, KeyboardReport->KeyCode, &UsedKeyCodes, *current_map);
                                checkRolloverError(KeyboardReport->KeyCode, &UsedKeyCodes, 6);
                                *ReportSize = sizeof(USB_KeyboardReport_Data_t);
                        }
                        //Returning false lets LUFA determine whether or not to send the report.
                        //Returning true forces sending.
                        return true;
                }
        }
        else {  //Mouse Report
                USB_Mouse_HScrollReport_Data_t* MouseReport = (USB_Mouse_HScrollReport_Data_t*) ReportData;
                read_from_mapping_MOUSE(&MouseReport->Button, &MouseReport->X, &MouseReport->Y,
                                        &MouseReport->Pan, &MouseReport->Scroll, *current_map);

                //Mouse movements are more natural if they are scaled to a constant speed
                if (MouseReport->X && MouseReport->Y) {
                        MouseReport->X = MouseReport->X/SQRT2;
                        MouseReport->Y = MouseReport->Y/SQRT2;
                }

                //Scaling mouse speed settings by powers of 3 provides intuitive divisions
                MouseReport->X = MouseReport->X * pow(3, mouse_speed-1);
                MouseReport->Y = MouseReport->Y * pow(3, mouse_speed-1);

                *ReportSize = sizeof(USB_Mouse_HScrollReport_Data_t);
                //Returning false sends one mouse movement per mouse keypress.
                //True enables continuous movement.
                return true;
        }
}

// HID class driver callback function for the processing of HID reports from the host.
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize) {
        // Status LEDs can be handled here, if they are available.
}

//Send a MIDI report.
void sendMIDI(uint8_t MIDICommand, uint8_t Channel, uint8_t MIDIPitch) {
        if (MIDICommand) {
                MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t) {
                                .Event       = MIDI_EVENT(0, MIDICommand),

                                .Data1       = MIDICommand | Channel,
                                .Data2       = MIDIPitch,
                                .Data3       = MIDI_STANDARD_VELOCITY,
                        };
                MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);
                MIDI_Device_Flush(&Keyboard_MIDI_Interface);
        }
}
