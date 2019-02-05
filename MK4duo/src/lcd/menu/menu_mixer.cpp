/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2013 Alberto Cotronei @MagoKimbra
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

//
// Mixer Menu
//

#include "../../../MK4duo.h"

#if HAS_LCD_MENU && ENABLED(COLOR_MIXING_EXTRUDER) && DISABLED(NEXTION)

#include "../ultralcd/dogm/ultralcd_dogm.h"
#include "../ultralcd/ultralcd.h"
#include "../ultralcd/lcdprint.h"

void lcd_mixer_gradient_z_start_edit() {
  lcdui.defer_status_screen(true);
  lcdui.encoder_direction_normal();
  ENCODER_RATE_MULTIPLY(true);
  if (lcdui.encoderPosition != 0) {
    mixer.gradient.start_z += float((int)lcdui.encoderPosition) * 0.1;
    lcdui.encoderPosition = 0;
    NOLESS(mixer.gradient.start_z, 0);
    NOMORE(mixer.gradient.start_z, Z_MAX_POS);
  }
  if (lcdui.should_draw()) {
    char tmp[21];
    sprintf_P(tmp, PSTR(MSG_START_Z ": %4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
    SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
    lcd_put_u8str(tmp);
  }

  if (lcdui.lcd_clicked) {
    if (mixer.gradient.start_z > mixer.gradient.end_z)
      mixer.gradient.end_z = mixer.gradient.start_z;
    mixer.refresh_gradient();
    lcdui.goto_previous_screen();
  }
}

void lcd_mixer_gradient_z_end_edit() {
  lcdui.defer_status_screen(true);
  lcdui.encoder_direction_normal();
  ENCODER_RATE_MULTIPLY(true);
  if (lcdui.encoderPosition != 0) {
    mixer.gradient.end_z += float((int)lcdui.encoderPosition) * 0.1;
    lcdui.encoderPosition = 0;
    NOLESS(mixer.gradient.end_z, 0);
    NOMORE(mixer.gradient.end_z, Z_MAX_POS);
  }

  if (lcdui.should_draw()) {
    char tmp[21];
    sprintf_P(tmp, PSTR(MSG_END_Z ": %4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
    SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
    lcd_put_u8str(tmp);
  }

  if (lcdui.lcd_clicked) {
    if (mixer.gradient.end_z < mixer.gradient.start_z)
      mixer.gradient.start_z = mixer.gradient.end_z;
    _lcd_mixer_commit_gradient();
    lcdui.goto_previous_screen();
  }
}

void lcd_mixer_edit_gradient_menu() {
  START_MENU();
  MENU_BACK(MSG_GRADIENT);

  MENU_ITEM_EDIT_CALLBACK(int8, MSG_START_VTOOL, &mixer.gradient.start_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
  MENU_ITEM_EDIT_CALLBACK(int8, MSG_END_VTOOL, &mixer.gradient.end_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);

  char tmp[10];

  MENU_ITEM(submenu, MSG_START_Z ":", lcd_mixer_gradient_z_start_edit);
  MENU_ITEM_ADDON_START(9);
    sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
    LCDPRINT(tmp);
  MENU_ITEM_ADDON_END();

  MENU_ITEM(submenu, MSG_END_Z ":", lcd_mixer_gradient_z_end_edit);
  MENU_ITEM_ADDON_START(9);
    sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
    LCDPRINT(tmp);
  MENU_ITEM_ADDON_END();

  END_MENU();
}

static uint8_t v_index = 0;

void _lcd_mixer_select_vtool() { mixer.T(v_index); }

void lcd_mixer_mix_edit() {
  if (lcdui.encoderPosition != 0) {
    mixer.mix[0] += (int)lcdui.encoderPosition;
    lcdui.encoderPosition = 0;
    if (mixer.mix[0] < 0) mixer.mix[0] += 100;
    if (mixer.mix[0] > 100) mixer.mix[0] -= 100;
    mixer.mix[1] = 100 - mixer.mix[0];
  }
  char tmp[21];
  sprintf_P(tmp, PSTR(MSG_MIX ":    %3d%% %3d%%"), mixer.mix[0], mixer.mix[1]);
  SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
  lcd_put_u8str(tmp);

  if (lcdui.lcd_clicked) {
    //mixer.gradient.enabled = false;
    mixer.update_vtool_from_mix();
    lcdui.goto_previous_screen();
  }
}

inline void _lcd_mixer_toggle_mix() {
  mixer.mix[0] = mixer.mix[0] == 100 ? 0 : 100;
  mixer.mix[1] = 100 - mixer.mix[0];
  mixer.update_vtool_from_mix();
  lcdui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
}

void menu_mixer() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  MENU_ITEM_EDIT_CALLBACK(int8, "V-Tool", &v_index, 0, MIXING_VIRTUAL_TOOLS - 1, _lcd_mixer_select_vtool);

  mixer.update_mix_from_vtool();

  char tmp[10];
  MENU_ITEM(submenu, MSG_MIX, lcd_mixer_mix_edit);
  MENU_ITEM_ADDON_START(10);
    sprintf_P(tmp, PSTR("%3d;%3d%%"), mixer.mix[0], mixer.mix[1]);
    lcd_put_u8str(tmp);
  MENU_ITEM_ADDON_END();

  MENU_ITEM(function, MSG_TOGGLE_MIX, _lcd_mixer_toggle_mix);

  #if ENABLED(GRADIENT_MIX)
    MENU_ITEM(submenu, MSG_GRADIENT, lcd_mixer_edit_gradient_menu);
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && ENABLED(COLOR_MIXING_EXTRUDER)
