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
 * @file midi_callbacks.ino
 * @author Marcel Licence
 * @date 06.09.2023
 *
 * @brief This file contains the MIDI callbacks
 * please link the functions in z_config.ino (in your MIDI mapping)
 */


/*
 * MIDI callbacks
 */
inline void AppBtn(uint8_t param, uint8_t value)
{
    if (value > 0)
    {
        switch (param)
        {
#ifdef BOARD_ESP32_AUDIO_KIT_AC101

        case 0:
            ac101_setSourceMic();
            break;
        case 1:
            ac101_setSourceLine();
            break;
#endif
        }
    }
}

void AppSetInputGain(uint8_t unused __attribute__((unused)), uint8_t value)
{
    inputGain = log2fromU7(value, -16, 5);
    Status_ValueChangedFloat("InputGain", inputGain);
}
