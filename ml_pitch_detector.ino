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
 * @file ml_pitch_detector.ino
 * @author Marcel Licence
 * @date 18.10.2024
 *
 * @brief   This is the main project file of the pitch detector
 *          Please check that the audio settings are correct
 *          and WS2812 pin is connected to for example a LED ring
 *          This project was tested on the ESP32 Audio Kit
 *
 * @see little demo: https://youtu.be/Ddy_T9LsH0A
 */


#include "config.h"


#include <Arduino.h>


/*
 * Library can be found on https://github.com/marcel-licence/ML_SynthTools
 * and you might need https://github.com/marcel-licence/ML_SynthTools_Lib too
 */
//#include <ml_types.h>
#include <ml_utils.h>

#include "ml_pitch_detector.h"


/* centralized modules */
#define ML_SYNTH_INLINE_DECLARATION
#include <ml_inline.h>
#undef ML_SYNTH_INLINE_DECLARATION


#include <Adafruit_NeoPixel.h>


#define LED_COUNT   24


/* define Adafruit NeoPixel object */
Adafruit_NeoPixel strip(LED_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);

/**
    @brief This function contains the setup routines.
 */
void setup(void)
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.print(F("ml_pitch_detector\n"));


    strip.begin();
    strip.show();
    strip.setPixelColor(0, 12, 0, 0);
    strip.setPixelColor(23, 12, 0, 12);
    strip.show();

    Notedetector_Setup(SAMPLE_RATE);

    /*
     * this code runs once
     */
#ifdef MIDI_USB_ENABLED
    Midi_Usb_Setup();
#endif

#ifdef BLINK_LED_PIN
    Blink_Setup();
    Blink_Fast(3);
#endif

    delay(250);

    Audio_Setup();


    Serial.println();
    delay(250);

    Serial.printf("Loading data\n");


    Midi_Setup();

    Serial.printf("setup done!\n");

#ifdef SOC_CPU_CORES_NUM
#if SOC_CPU_CORES_NUM > 1
    Core0TaskInit();
#else
    //#error only supported by ESP32 platform
#endif
#endif
}

#ifdef SOC_CPU_CORES_NUM
#if SOC_CPU_CORES_NUM > 1
/*
 * Core 0
 */
/* this is used to add a task to core 0 */
TaskHandle_t Core0TaskHnd;


bool setup_toggle = false;

inline
void Core0TaskInit(void)
{
    /* we need a second task for the terminal output */
    xTaskCreatePinnedToCore(Core0Task, "CoreTask0", 8000, NULL, 4, &Core0TaskHnd, 0);
}

void Core0TaskSetup(void)
{
    /*
     * init your stuff for core0 here
     */
}

void Core0Cnt1Hz(void)
{
    /*
     * something happens every second here
     */
}

void Core0TaskLoop(void)
{
    /*
     * put your loop stuff for core0 here
     */
    if (NoteDetector_Process())
    {
        float c[24];
        uint8_t c_idx = NoteDetector_GetIdx();
        NoteDetector_GetConstants24(c);

        for (uint32_t n = 0; n < 24; n++)
        {
            float f = c[n];
            f *= 255;
            uint8_t u = f;

            if (n == c_idx)
            {
                strip.setPixelColor(n, 64, 64, 64);
            }
            else
            {
                strip.setPixelColor(n, (u) / 16, 0, (255 - u) / 16);
            }
        }

        strip.show(); // Initialize all pixels to 'off'
    }
}

void Core0Task(void *parameter)
{
    Core0TaskSetup();

    while (true)
    {
        Core0TaskLoop();

        /* this seems necessary to trigger the watchdog */
        delay(1);
        yield();
    }
}
#endif /* SOC_CPU_CORES_NUM>1 */
#endif /* SOC_CPU_CORES_NUM */


/**
    @brief This function will be called every second.
        In case of an overload the function will not called so often
  */
void loop_1Hz(void)
{
#ifdef BLINK_LED_PIN
    Blink_Process();
#endif
}

float inputGain = 1.0f;


/**
    @brief This function contains the mainloop
 */
void loop(void)
{
    static int loop_cnt_1hz = 0; /*!< counter to allow 1Hz loop cycle */

    loop_cnt_1hz += SAMPLE_BUFFER_SIZE;

    if (loop_cnt_1hz >= SAMPLE_RATE)
    {
        loop_cnt_1hz -= SAMPLE_RATE;
        loop_1Hz();
    }

    /*
     * MIDI processing
     */
    Midi_Process();

#ifdef MIDI_VIA_USB_ENABLED
    UsbMidi_ProcessSync();
#endif

#ifdef MIDI_STREAM_PLAYER_ENABLED
    MidiStreamPlayer_Tick(SAMPLE_BUFFER_SIZE);
#endif

#ifdef MIDI_BLE_ENABLED
    midi_ble_loop();
#endif

#ifdef USB_HOST_ENABLED
    Usb_Host_Midi_loop();
#endif

#ifdef MIDI_USB_ENABLED
    Midi_Usb_Loop();
#endif

    /*
     * And finally the audio stuff
     */
    Q1_14 left[SAMPLE_BUFFER_SIZE], right[SAMPLE_BUFFER_SIZE];

    memset(left, 0, sizeof(left));
    memset(right, 0, sizeof(right));

    /* get samples from audio input */
    Audio_Input(left, right);

    /* adjust gain */
    mul(left, inputGain, left, SAMPLE_BUFFER_SIZE);
    mul(right, inputGain, right, SAMPLE_BUFFER_SIZE);

    float mono_f[SAMPLE_BUFFER_SIZE];
    for (int n = 0; n < SAMPLE_BUFFER_SIZE; n++)
    {
        float in;
        in = left[n].s16 + right[n].s16;
        in *= 1.0f / 32000.0f;
        mono_f[n] = in;
    }

    NoteDetector_PushSamples(mono_f, SAMPLE_BUFFER_SIZE);

    /*
     * Output the audio
     */
    Audio_Output(left, right);
}
