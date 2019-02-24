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

#if HAS_SD_SUPPORT

  #define CODE_M20
  #define CODE_M21
  #define CODE_M22
  #define CODE_M23
  #define CODE_M24
  #define CODE_M25
  #define CODE_M26
  #define CODE_M27
  #define CODE_M28
  #define CODE_M29
  #define CODE_M30
  #define CODE_M32
  #define CODE_M33
  #define CODE_M524

  /**
   * M20: List SD card to serial output
   */
  inline void gcode_M20(void) {
    SERIAL_EM(MSG_BEGIN_FILE_LIST);
    card.ls();
    SERIAL_EM(MSG_END_FILE_LIST);
  }

  /**
   * M21: Init SD Card
   */
  inline void gcode_M21(void) { card.mount(); }

  /**
   * M22: Release SD Card
   */
  inline void gcode_M22(void) { card.unmount(); }

  /**
   * M23: Select a file
   */
  inline void gcode_M23(void) {
    #if HAS_SD_RESTART
      card.delete_restart_file();
    #endif
    // Simplify3D includes the size, so zero out all spaces (#7227)
    // Questa funzione blocca il nome al primo spazio quindi file con spazio nei nomi non funziona da rivedere
    //for (char *fn = parser.string_arg; *fn; ++fn) if (*fn == ' ') *fn = '\0';
    card.selectFile(parser.string_arg);
    lcdui.set_status(card.fileName);
  }

  /**
   * M24: Start or Resume SD Print
   */
  inline void gcode_M24(void) {

    if (parser.seenval('S')) card.setIndex(parser.value_long());
    if (parser.seenval('T')) print_job_counter.resume(parser.value_long());

    #if ENABLED(PARK_HEAD_ON_PAUSE)
      if (advancedpause.did_pause_print) {
        advancedpause.resume_print();
        return;
      }
    #endif

    if (card.isFileOpen()) {
      card.startFileprint();
      print_job_counter.start();
      host_action.prompt_open(PROMPT_INFO, PSTR("Resume SD"));
      host_action.resume();
    }

    lcdui.reset_status();

  }

  /**
   * M25: Pause SD Print
   */
  void gcode_M25(void) {

    // Set initial pause flag to prevent more commands from landing in the queue while we try to pause
    #if ENABLED(SDSUPPORT)
      if (IS_SD_PRINTING()) card.pauseSDPrint();
    #endif

    #if ENABLED(PARK_HEAD_ON_PAUSE)
      gcode_M125();
    #else
      print_job_counter.pause();
      lcdui.reset_status();
      host_action.prompt_open(PROMPT_PAUSE_RESUME, PSTR("Pause SD"), PSTR("Resume"));
      host_action.pause();
    #endif
  }

  /**
   * M26: Set SD Card file index
   */
  inline void gcode_M26(void) {
    if (card.isDetected() && parser.seenval('S'))
      card.setIndex(parser.value_long());
  }

  /**
   * M27: Get SD Card status or set the SD status auto-report interval.
   */
  inline void gcode_M27(void) {
    if (parser.seen('C'))
      SERIAL_EMT("Current file: ", card.fileName);
    else if (parser.seenval('S'))
      card.setAutoreportSD(parser.value_bool());
    else
      card.printStatus();
  }

  /**
   * M28: Start SD Write
   */
  inline void gcode_M28(void) { card.startWrite(parser.string_arg, false); }

  /**
   * M29: Stop SD Write
   * Processed in write to file routine above
   */
  inline void gcode_M29(void) { card.setSaving(false); }

  /**
   * M30 <filename>: Delete SD Card file
   */
  inline void gcode_M30(void) {
    if (card.isDetected()) {
      card.closeFile();
      card.deleteFile(parser.string_arg);
    }
  }

  /**
   * M32: Select file and start SD print
   */
  inline void gcode_M32(void) {
    if (IS_SD_PRINTING()) planner.synchronize();

    if (card.isDetected()) {
      card.closeFile();

      char* namestartpos = parser.string_arg ; // default name position

      SERIAL_MV("Open file: ", namestartpos);
      SERIAL_EM(" and start print.");
      card.selectFile(namestartpos);
      if (parser.seenval('S')) card.setIndex(parser.value_long());

      mechanics.feedrate_mm_s       = 20.0; // 20 units/sec
      mechanics.feedrate_percentage = 100;  // 100% mechanics.feedrate_mm_s
      card.startFileprint();
      print_job_counter.start();
    }
  }

  /**
   * M33: Stop printing, close file and save restart.gcode
   */
  inline void gcode_M33(void) {
    if (card.isDetected() && IS_SD_PRINTING())
      card.setAbortSDprinting(true);
  }

  #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)

    #define CODE_M34

    /**
     * M34: Set SD Card Sorting Options
     */
    inline void gcode_M34(void) {
      if (parser.seen('S')) card.setSortOn(parser.value_bool());
      if (parser.seenval('F')) {
        const int v = parser.value_long();
        card.setSortFolders(v < 0 ? -1 : v > 0 ? 1 : 0);
      }
      //if (parser.seen('R')) card.setSortReverse(parser.value_bool());
    }

  #endif // SDCARD_SORT_ALPHA && SDSORT_GCODE

  #if ENABLED(ADVANCED_SD_COMMAND)

    #define CODE_M39

    /**
     * M39: SD Card Info and formatting
     *
     *    F1 for formatting card.
     *
     */
    inline void gcode_M39(void) {
      if (parser.seen('F')) {
        card.formatSD();
        card.mount();
      }
      card.infoSD();
    }

  #endif // ADVANCED_SD_COMMAND

  /**
   * M524: Abort the current SD print job (started with M24)
   */
  inline void gcode_M524(void) {
    if (IS_SD_PRINTING()) card.setAbortSDprinting(true);
  }

#endif // HAS_SD_SUPPORT
