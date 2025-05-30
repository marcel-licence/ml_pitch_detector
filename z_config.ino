/*
 * Copyright (c) 2025 Marcel Licence
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
 * veröffentlichten Version, weiter verteilen und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich sein wird, jedoch
 * OHNE JEDE GEWÄHR,; sogar ohne die implizite
 * Gewähr der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Einzelheiten.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
 */

/**
 * @file z_config.ino
 * @author Marcel Licence
 * @date 06.09.2023
 *
 * @brief This file contains the mapping configuration
 * Put all your project configuration here (no defines etc)
 * This file will be included at the and can access all
 * declarations and type definitions
 *
 * @see ESP32 Arduino DIY Synthesizer Projects - Little startup guide to get your MIDI synth working - https://youtu.be/ZNxGCB-d68g
 */


#ifdef __CDT_PARSER__
#include <cdt.h>
#endif


/*
 * this mapping is used for the edirol pcr-800
 * this should be changed when using another controller
 *
 * Make your own mapping:
 * The MIDI controller might send different control change messages when moving knobs,
 * sliders and pressing buttons (if available)
 *
 * Usually the controller sends message on a selected channel (configured channel).
 * The channel is represented by the first hex value.
 * In the mapping for the PCR-800 the first hex value is different because the controller is configured a bit weird.
 * MIDI channel 1 to 16 are represented by the values 0x0 to 0xF
 *
 * The second value represents the control change number.
 * Some are defined by the MIDI specification like 0x40 - "sustain"
 *
 * The third value is a free text. This might be used in future for display purposes.
 * You can just enter some text to identify the knob etc.
 *
 * The last three values are for linking a function + user parameter
 *
 * For example your controller is sending a control change message
 * - channel 0x2, control number: 0x11, value: 0x32
 * following entry will be identified as linked:
 * { 0x2, 0x11, "S3", NULL, Synth_SetParam, SYNTH_PARAM_VEL_ENV_SUSTAIN},
 *
 * The function Synth_SetParam will be called with following parameters:
 * Synth_SetParam(SYNTH_PARAM_VEL_ENV_SUSTAIN, 0x32);
 *
 * Some functions ignore the additional parameter in that case the last value can be left as zero.
 */
struct midiControllerMapping edirolMapping[] =
{
    /* general MIDI */
    { 0x0, 0x40, "sustain", NULL, NULL, 0},

    /* transport buttons */
    { 0x8, 0x52, "back", NULL, NULL, 0},
    { 0xD, 0x52, "stop", NULL, NULL, 0},
    { 0xe, 0x52, "start", NULL, NULL, 0},
    { 0xe, 0x52, "start", NULL, NULL, 0},
    { 0xa, 0x52, "rec", NULL, NULL, 0},

    /* upper row of buttons */
    { 0x0, 0x50, "A1", NULL, AppBtn, 0},
    { 0x1, 0x50, "A2", NULL, AppBtn, 1},
    { 0x2, 0x50, "A3", NULL, AppBtn, 2},
    { 0x3, 0x50, "A4", NULL, AppBtn, 3},

    { 0x4, 0x50, "A5", NULL, NULL, 4},
    { 0x5, 0x50, "A6", NULL, NULL, 5},
    { 0x6, 0x50, "A7", NULL, NULL, 6},
    { 0x7, 0x50, "A8", NULL, NULL, 7},

    { 0x0, 0x53, "A9", NULL, NULL, 8},

    { 0x1, 0x12, "S9", NULL, NULL, 0},

    /* lower row of buttons */
    { 0x0, 0x51, "B1", NULL, NULL, 0},
    { 0x1, 0x51, "B2", NULL, NULL, 1},
    { 0x2, 0x51, "B3", NULL, NULL, 2},
    { 0x3, 0x51, "B4", NULL, NULL, 3},

    { 0x4, 0x51, "B5", NULL, NULL, 4},
    { 0x5, 0x51, "B6", NULL, NULL, 5},
    { 0x6, 0x51, "B7", NULL, NULL, 6},
    { 0x7, 0x51, "B8", NULL, NULL, 7},

    { 0x1, 0x53, "B9", NULL, NULL, 8},

    /* rotary */
    { 0x0, 0x10, "R1", NULL, NULL, 0},
    { 0x1, 0x10, "R2", NULL, NULL, 0},
    { 0x2, 0x10, "R3", NULL, AppSetInputGain, 0},
    { 0x3, 0x10, "R4", NULL, NULL, 3},
    { 0x4, 0x10, "R5", NULL, NULL, 4},
    { 0x5, 0x10, "R6", NULL, NULL, 5},
    { 0x6, 0x10, "R7", NULL, NULL, 0},
    { 0x7, 0x10, "R8", NULL, NULL, 0},

    { 0x0, 0x12, "R9", NULL, NULL, 0},

    /* slider */
    { 0x0, 0x11, "S1", NULL, NULL, 0},
    { 0x1, 0x11, "S2", NULL, NULL, 0},
    { 0x2, 0x11, "S3", NULL, NULL, 0},
    { 0x3, 0x11, "S4", NULL, NULL, 0},

    { 0x4, 0x11, "S5", NULL, NULL, 0},
    { 0x5, 0x11, "S6", NULL, NULL, 0},
    { 0x6, 0x11, "S7", NULL, NULL, 0},
    { 0x7, 0x11, "S8", NULL, NULL, 0},

    { 0x1, 0x12, "S9", NULL, NULL, 0},
};

/* in this case no synthesizer module is connected, note on, off will be ignored */
struct midiMapping_s midiMapping =
{
    NULL,
    NULL,
    NULL,
    NULL, //Synth_PitchBend,
    NULL, //Synth_ModulationWheel,
    NULL, /* assign program change callback here! */
    NULL, //Synth_RealTimeMsg,
    NULL, //Synth_SongPosition,
    edirolMapping,
    sizeof(edirolMapping) / sizeof(edirolMapping[0]),
};

#ifdef MIDI_VIA_USB_ENABLED
struct usbMidiMappingEntry_s usbMidiMappingEntries[] =
{
    {
        NULL,
        App_UsbMidiShortMsgReceived,
        NULL,
        NULL,
        0xFF,
    },
};

struct usbMidiMapping_s usbMidiMapping =
{
    NULL,
    NULL,
    usbMidiMappingEntries,
    sizeof(usbMidiMappingEntries) / sizeof(usbMidiMappingEntries[0]),
};
#endif /* MIDI_VIA_USB_ENABLED */

