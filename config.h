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
 * Dieses Programm ist Freie Software: Sie k�nnen es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
 * ver�ffentlichten Version, weiter verteilen und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es n�tzlich sein wird, jedoch
 * OHNE JEDE GEW�HR,; sogar ohne die implizite
 * Gew�hr der MARKTF�HIGKEIT oder EIGNUNG F�R EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License f�r weitere Einzelheiten.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
 */

/**
 * @file config.h
 * @author Marcel Licence
 * @date 06.09.2023
 *
 * @brief This file contains the project configuration
 * - Audio configuration: SAMPLE_RATE, SAMPLE_BUFFER_SIZE
 * - Communication Settings: SERIAL_BAUDRATE
 */


#ifdef __CDT_PARSER__
#include <cdt.h>
#endif


#ifndef CONFIG_H_
#define CONFIG_H_


#define WS2812_PIN  21


//#define BOARD_ML_SYNTH_V2 /* my second DIY PCB */
//#define BOARD_ESP32_AUDIO_KIT_AC101 /* this configures the project to run on the ESP32 audio kit with the AC101 codec */
#define BOARD_ESP32_AUDIO_KIT_ES8388 /* this configures the project to run on the ESP32 audio kit with the ES8388 codec - you may configure the board file as-well */

#include <ml_boards.h>


/* use the following to generate a test sound to test your setup */
//#define OUTPUT_SAW_TEST
//#define OUTPUT_SINE_TEST


#define MIDI_FMT_INT
#define SAMPLE_BUFFER_SIZE  48
#define SAMPLE_RATE 48000
#define SAMPLE_SIZE_16BIT
#define SERIAL_BAUDRATE 115200
#define STATUS_SIMPLE


#endif /* CONFIG_H_ */

