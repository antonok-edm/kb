#ifndef _HID_CLASS_CUSTOM_H_
#define _HID_CLASS_CUSTOM_H_
    //Custom HID report descriptor which adds horizontal scroll, or "pan", to the standard LUFA HID Mouse implementation.
    #define HID_DESCRIPTOR_MOUSE_HSCROLL(MinAxisVal, MaxAxisVal, MinPhysicalVal, MaxPhysicalVal, Buttons, AbsoluteCoords) \
        HID_RI_USAGE_PAGE(8, 0x01),                                            \
        HID_RI_USAGE(8, 0x02),                                                 \
        HID_RI_COLLECTION(8, 0x01),                                            \
            HID_RI_USAGE(8, 0x01),                                             \
            HID_RI_COLLECTION(8, 0x00),                                        \
                HID_RI_USAGE_PAGE(8, 0x09),                                    \
                HID_RI_USAGE_MINIMUM(8, 0x01),                                 \
                HID_RI_USAGE_MAXIMUM(8, Buttons),                              \
                HID_RI_LOGICAL_MINIMUM(8, 0x00),                               \
                HID_RI_LOGICAL_MAXIMUM(8, 0x01),                               \
                HID_RI_REPORT_COUNT(8, Buttons),                               \
                HID_RI_REPORT_SIZE(8, 0x01),                                   \
                HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
                HID_RI_REPORT_COUNT(8, 0x01),                                  \
                HID_RI_REPORT_SIZE(8, (Buttons % 8) ? (8 - (Buttons % 8)) : 0),\
                HID_RI_INPUT(8, HID_IOF_CONSTANT),                             \
                HID_RI_USAGE_PAGE(8, 0x01),                                    \
                HID_RI_USAGE(8, 0x30),                                         \
                HID_RI_USAGE(8, 0x31),                                         \
                HID_RI_USAGE(8, 0x38),                                         \
                HID_RI_LOGICAL_MINIMUM(16, MinAxisVal),                        \
                HID_RI_LOGICAL_MAXIMUM(16, MaxAxisVal),                        \
                HID_RI_PHYSICAL_MINIMUM(16, MinPhysicalVal),                   \
                HID_RI_PHYSICAL_MAXIMUM(16, MaxPhysicalVal),                   \
                HID_RI_REPORT_COUNT(8, 0x03),                                  \
                HID_RI_REPORT_SIZE(8, (((MinAxisVal >= -128) && (MaxAxisVal <= 127)) ? 8 : 16)), \
                HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | (AbsoluteCoords ? HID_IOF_ABSOLUTE : HID_IOF_RELATIVE)), \
                HID_RI_USAGE_PAGE(8, 0x0c),                                    \
                0x0a, 0x38, 0x02,                                              \
                HID_RI_LOGICAL_MINIMUM(16, MinAxisVal),                        \
                HID_RI_LOGICAL_MAXIMUM(16, MaxAxisVal),                        \
                HID_RI_PHYSICAL_MINIMUM(16, MinAxisVal),                       \
                HID_RI_PHYSICAL_MAXIMUM(16, MaxAxisVal),                       \
                HID_RI_REPORT_COUNT(8, 0x01),                                  \
                HID_RI_REPORT_SIZE(8, (((MinAxisVal >= -128) && (MaxAxisVal <= 127)) ? 8 : 16)), \
                HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | (AbsoluteCoords ? HID_IOF_ABSOLUTE : HID_IOF_RELATIVE)), \
            HID_RI_END_COLLECTION(0),                                          \
        HID_RI_END_COLLECTION(0)
    /** \brief Standard HID Boot Protocol Mouse Report (customized with Horizontal scroll support).
     *  Type define for a standard Boot Protocol Mouse report
     */
    typedef struct {
        uint8_t Button; /**< Button mask for currently pressed buttons in the mouse. */
        int8_t  X;      /**< Current delta X movement of the mouse. */
        int8_t  Y;      /**< Current delta Y movement on the mouse. */
        int8_t  Scroll; /**< Current vertical scroll. */
        int8_t  Pan;    /**< Current horizontal scroll. */
    } __attribute__ ((packed)) USB_Mouse_HScrollReport_Data_t;
#endif
