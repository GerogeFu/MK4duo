/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2019 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * mcode
 *
 * Copyright (C) 2019 Alberto Cotronei @MagoKimbra
 */

#if HAS_CHDK || HAS_PHOTOGRAPH

#define CODE_M240

#if ENABLED(PHOTO_RETRACT_MM)
  inline void e_move_m240(const float length, const float fr_mm_s) {
    if (length && thermalManager.hotEnoughToExtrude(ACTIVE_HOTEND)) {
      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        advancedpause.do_pause_e_move(length, fr_mm_s);
      #else
        current_position[E_AXIS] += length / planner.e_factor[tools.active_extruder];
        planner.buffer_line(mechanics.current_position, fr_mm_s, tools.active_extruder);
      #endif
    }
  }
#endif

#if HAS_PHOTOGRAPH
  constexpr uint8_t NUM_PULSES = 16;
  constexpr float PULSE_LENGTH = 0.01524;
  inline void set_photo_pin(const uint8_t state) { WRITE(PHOTOGRAPH_PIN, state); HAL::delayMilliseconds(PULSE_LENGTH); }
  inline void tweak_photo_pin() { set_photo_pin(HIGH); set_photo_pin(LOW); }
  inline void spin_photo_pin() { for (uint8_t i = NUM_PULSES; i--;) tweak_photo_pin(); }
#endif

/**
 * M240: Trigger a camera by...
 *
 *  - CHDK                  : Emulate a Canon RC-1 with a configurable ON duration.
 *                            http://captain-slow.dk/2014/03/09/3d-printing-timelapses/
 *  - PHOTOGRAPH_PIN        : Pulse a digital pin 16 times.
 *                            See http://www.doc-diy.net/photo/rc-1_hacked/
 *  - PHOTO_SWITCH_POSITION : Bump a physical switch with the X-carriage using a
 *                            configured position, delay, and retract length.
 *
 * PHOTO_POSITION parameters:
 *    R - Retract/recover length (current units)
 *    S - Retract/recover feedrate (mm/m)
 *    X - Move to X before triggering the shutter
 *    Y - Move to Y before triggering the shutter
 *    Z - Raise Z by a distance before triggering the shutter
 *
 * PHOTO_SWITCH_POSITION parameters:
 *    D - Duration (ms) to hold down switch (Requires PHOTO_SWITCH_MS)
 *    P - Delay (ms) after triggering the shutter (Requires PHOTO_SWITCH_MS)
 *    I - Switch trigger position override X
 *    J - Switch trigger position override Y
 */
inline void gcode_M240(void) {

  #if ENABLED(PHOTO_POSITION)

    if (!printer.isHomedAll()) return;

    const float old_pos[XYZ] = { mechanics.current_position[X_AXIS], mechanics.current_position[Y_AXIS], mechanics.current_position[Z_AXIS] };

    #if ENABLED(PHOTO_RETRACT_MM)
      constexpr float rfr = (MMS_TO_MMM(
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          PAUSE_PARK_RETRACT_FEEDRATE
        #elif ENABLED(FWRETRACT)
          RETRACT_FEEDRATE
        #else
          45
        #endif
      ));
      const float rval = parser.seenval('R') ? parser.value_linear_units() : PHOTO_RETRACT_MM,
                  sval = parser.seenval('S') ? MMM_TO_MMS(parser.value_feedrate()) : rfr;
      e_move_m240(-rval, sval);
    #endif

    constexpr float photo_position[XYZ] = PHOTO_POSITION;
    float raw[XYZ] = {
       parser.seenval('X') ? RAW_X_POSITION(parser.value_linear_units()) : photo_position[X_AXIS],
       parser.seenval('Y') ? RAW_Y_POSITION(parser.value_linear_units()) : photo_position[Y_AXIS],
      (parser.seenval('Z') ? parser.value_linear_units() : photo_position[Z_AXIS]) + mechanics.current_position[Z_AXIS]
    };
    endstops.clamp_to_software(raw);
    mechanics.do_blocking_move_to(raw);

    #if ENABLED(PHOTO_SWITCH_POSITION)
      constexpr float photo_switch_position[2] = PHOTO_SWITCH_POSITION;
      const float sraw[] = {
         parser.seenval('I') ? RAW_X_POSITION(parser.value_linear_units()) : photo_switch_position[X_AXIS],
         parser.seenval('J') ? RAW_Y_POSITION(parser.value_linear_units()) : photo_switch_position[Y_AXIS]
      };
      mechanics.do_blocking_move_to_xy(sraw[X_AXIS], sraw[Y_AXIS], get_homing_bump_feedrate(X_AXIS));
      #if PHOTO_SWITCH_MS > 0
        printer.safe_delay(parser.intval('D', PHOTO_SWITCH_MS));
      #endif
      mechanics.do_blocking_move_to(raw);
    #endif

  #endif

  #if HAS_CHDK

    OUT_WRITE(CHDK_PIN, HIGH);
    printer.chdk_watch.start();

  #elif HAS_PHOTOGRAPH

    spin_photo_pin();
    delay(7.33);
    spin_photo_pin();

  #endif

  #if ENABLED(PHOTO_POSITION)
    #if PHOTO_DELAY_MS > 0
      printer.safe_delay(parser.intval('P', PHOTO_DELAY_MS));
    #endif
    mechanics.do_blocking_move_to(old_pos);
    #if ENABLED(PHOTO_RETRACT_MM)
      e_move_m240(rval, sval);
    #endif
  #endif
}

#endif // HAS_CHDK || PHOTOGRAPH_PIN
