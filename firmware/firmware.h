#ifndef _FIRMWARE_H_
#define _FIRMWARE_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <avr/power.h>
        #include <avr/interrupt.h>
        #include <util/delay.h>
        #include <stdbool.h>
        #include <string.h>

        #include "typedefs.h"
        #include "global_vars.h"
        #include "custom_hid_class.h"
        #include "descriptors.h"
        #include "definitions.h"
        #include "key_callable_functions.h"
        #include "layermaps.h"
        #include "helper.h"

        //#include <LUFA/Drivers/Board/LEDs.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include <LUFA/Platform/Platform.h>

    /* Macros: */
        /** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
        //#define LEDMASK_USB_NOTREADY      LEDS_LED1
        /** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
        //#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)
        /** LED mask for the library LED driver, to indicate that the USB interface is ready. */
        //#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)
        /** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
        //#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)

    /* Type Defines: */
        typedef struct {
            uint8_t Modifier;
            uint8_t Reserved;
            uint8_t KeyCode[14];
        } USB_ExtendedKeyboardReport_Data_t;

    /* Function Prototypes: */
        void initPins(void);
        void SetupHardware(void);

        void EVENT_USB_Device_Connect(void);
        void EVENT_USB_Device_Disconnect(void);
        void EVENT_USB_Device_ConfigurationChanged(void);
        void EVENT_USB_Device_ControlRequest(void);
        void EVENT_USB_Device_StartOfFrame(void);

        bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                                 uint8_t* const ReportID,
                                                 const uint8_t ReportType,
                                                 void* ReportData,
                                                 uint16_t* const ReportSize);
        void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                                  const uint8_t ReportID,
                                                  const uint8_t ReportType,
                                                  const void* ReportData,
                                                  const uint16_t ReportSize);

        void sendMIDIReport(void);
        void sendMIDI(uint8_t MIDICommand, uint8_t Channel, uint8_t MIDIPitch);

        void setCurrentKeyMap(void);
        void doKeyFunctionCalls(void);
        void doKeyMIDItasks(void);
        void read_from_mapping_MIDI(const key key_mapping[ROWS][COLS]);
        void read_from_mapping_FUNC(const key key_mapping[ROWS][COLS]);

#endif
