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

#ifndef _DRIVER_TYPES_H_
#define _DRIVER_TYPES_H_

#pragma once

// Models
#define A4988     0
#define DRV8825   1
#define L6470     2
#define TB6560    3
#define TB6600    4
#define TMC26X    5
#define TMC2100   6
#define TMC2130   7
#define TMC2208   8
#define TMC2660   9
#define TMC5130  10

// Type
#define  X_IS_DRV(TYPE)   ( X_DRIVER_TYPE == TYPE)
#define  Y_IS_DRV(TYPE)   ( Y_DRIVER_TYPE == TYPE)
#define  Z_IS_DRV(TYPE)   ( Z_DRIVER_TYPE == TYPE)
#define X2_IS_DRV(TYPE)   (X2_DRIVER_TYPE == TYPE)
#define Y2_IS_DRV(TYPE)   (Y2_DRIVER_TYPE == TYPE)
#define Z2_IS_DRV(TYPE)   (Z2_DRIVER_TYPE == TYPE)
#define E0_IS_DRV(TYPE)   (E0_DRIVER_TYPE == TYPE)
#define E1_IS_DRV(TYPE)   (E1_DRIVER_TYPE == TYPE)
#define E2_IS_DRV(TYPE)   (E2_DRIVER_TYPE == TYPE)
#define E3_IS_DRV(TYPE)   (E3_DRIVER_TYPE == TYPE)
#define E4_IS_DRV(TYPE)   (E4_DRIVER_TYPE == TYPE)
#define E5_IS_DRV(TYPE)   (E5_DRIVER_TYPE == TYPE)

#define HAVE_DRV(TYPE) ( \
            X_IS_DRV(TYPE)  ||  Y_IS_DRV(TYPE)  ||  Z_IS_DRV(TYPE)  \
        || X2_IS_DRV(TYPE)  || Y2_IS_DRV(TYPE)  || Z2_IS_DRV(TYPE)  \
        || E0_IS_DRV(TYPE)  || E1_IS_DRV(TYPE)  || E2_IS_DRV(TYPE)  \
        || E3_IS_DRV(TYPE)  || E4_IS_DRV(TYPE)  || E5_IS_DRV(TYPE))

#define    HAS_STEALTHCHOP  ( HAVE_DRV(TMC2130) ||  HAVE_DRV(TMC2208) ||  HAVE_DRV(TMC5130))
#define  X_HAS_STEALTHCHOP  ( X_IS_DRV(TMC2130) ||  X_IS_DRV(TMC2208) ||  X_IS_DRV(TMC5130))
#define  Y_HAS_STEALTHCHOP  ( Y_IS_DRV(TMC2130) ||  Y_IS_DRV(TMC2208) ||  Y_IS_DRV(TMC5130))
#define  Z_HAS_STEALTHCHOP  ( Z_IS_DRV(TMC2130) ||  Z_IS_DRV(TMC2208) ||  Z_IS_DRV(TMC5130))
#define X2_HAS_STEALTHCHOP  (X2_IS_DRV(TMC2130) || X2_IS_DRV(TMC2208))
#define Y2_HAS_STEALTHCHOP  (Y2_IS_DRV(TMC2130) || Y2_IS_DRV(TMC2208))
#define Z2_HAS_STEALTHCHOP  (Z2_IS_DRV(TMC2130) || Z2_IS_DRV(TMC2208))
#define E0_HAS_STEALTHCHOP  (E0_IS_DRV(TMC2130) || E0_IS_DRV(TMC2208) || E0_IS_DRV(TMC5130))
#define E1_HAS_STEALTHCHOP  (E1_IS_DRV(TMC2130) || E1_IS_DRV(TMC2208))
#define E2_HAS_STEALTHCHOP  (E2_IS_DRV(TMC2130) || E2_IS_DRV(TMC2208))
#define E3_HAS_STEALTHCHOP  (E3_IS_DRV(TMC2130) || E3_IS_DRV(TMC2208))
#define E4_HAS_STEALTHCHOP  (E4_IS_DRV(TMC2130) || E4_IS_DRV(TMC2208))
#define E5_HAS_STEALTHCHOP  (E5_IS_DRV(TMC2130) || E5_IS_DRV(TMC2208))

#define    HAS_STALLGUARD   ( HAVE_DRV(TMC2130) ||  HAVE_DRV(TMC2660  ||  HAVE_DRV(TMC5130))
#define  X_HAS_STALLGUARD   ( X_IS_DRV(TMC2130) ||  X_IS_DRV(TMC2660) ||  X_IS_DRV(TMC5130))
#define  Y_HAS_STALLGUARD   ( Y_IS_DRV(TMC2130) ||  Y_IS_DRV(TMC2660) ||  Y_IS_DRV(TMC5130))
#define  Z_HAS_STALLGUARD   ( Z_IS_DRV(TMC2130) ||  Z_IS_DRV(TMC2660) ||  Z_IS_DRV(TMC5130))
#define X2_HAS_STALLGUARD   (X2_IS_DRV(TMC2130) || X2_IS_DRV(TMC2660))
#define Y2_HAS_STALLGUARD   (Y2_IS_DRV(TMC2130) || Y2_IS_DRV(TMC2660))
#define Z2_HAS_STALLGUARD   (Z2_IS_DRV(TMC2130) || Z2_IS_DRV(TMC2660))
#define E0_HAS_STALLGUARD   (E0_IS_DRV(TMC2130) || E0_IS_DRV(TMC2660) || E0_IS_DRV(TMC5130))
#define E1_HAS_STALLGUARD   (E1_IS_DRV(TMC2130) || E1_IS_DRV(TMC2660))
#define E2_HAS_STALLGUARD   (E2_IS_DRV(TMC2130) || E2_IS_DRV(TMC2660))
#define E3_HAS_STALLGUARD   (E3_IS_DRV(TMC2130) || E3_IS_DRV(TMC2660))
#define E4_HAS_STALLGUARD   (E4_IS_DRV(TMC2130) || E4_IS_DRV(TMC2660))
#define E5_HAS_STALLGUARD   (E5_IS_DRV(TMC2130) || E5_IS_DRV(TMC2660))

#define HAS_TRINAMIC        ( HAVE_DRV(TMC2130) ||  HAVE_DRV(TMC2208) ||  HAVE_DRV(TMC2660) ||  HAVE_DRV(TMC5130))
#define  X_IS_TRINAMIC      ( X_IS_DRV(TMC2130) ||  X_IS_DRV(TMC2208) ||  X_IS_DRV(TMC2660) ||  X_IS_DRV(TMC5130))
#define X2_IS_TRINAMIC      (X2_IS_DRV(TMC2130) || X2_IS_DRV(TMC2208) || X2_IS_DRV(TMC2660) || X2_IS_DRV(TMC5130))
#define  Y_IS_TRINAMIC      ( Y_IS_DRV(TMC2130) ||  Y_IS_DRV(TMC2208) ||  Y_IS_DRV(TMC2660) ||  Y_IS_DRV(TMC5130))
#define Y2_IS_TRINAMIC      (Y2_IS_DRV(TMC2130) || Y2_IS_DRV(TMC2208) || Y2_IS_DRV(TMC2660) || Y2_IS_DRV(TMC5130))
#define  Z_IS_TRINAMIC      ( Z_IS_DRV(TMC2130) ||  Z_IS_DRV(TMC2208) ||  Z_IS_DRV(TMC2660) ||  Z_IS_DRV(TMC5130))
#define Z2_IS_TRINAMIC      (Z2_IS_DRV(TMC2130) || Z2_IS_DRV(TMC2208) || Z2_IS_DRV(TMC2660) || Z2_IS_DRV(TMC5130))
#define E0_IS_TRINAMIC      (E0_IS_DRV(TMC2130) || E0_IS_DRV(TMC2208) || E0_IS_DRV(TMC2660) || E0_IS_DRV(TMC5130))
#define E1_IS_TRINAMIC      (E1_IS_DRV(TMC2130) || E1_IS_DRV(TMC2208) || E1_IS_DRV(TMC2660) || E1_IS_DRV(TMC5130))
#define E2_IS_TRINAMIC      (E2_IS_DRV(TMC2130) || E2_IS_DRV(TMC2208) || E2_IS_DRV(TMC2660) || E2_IS_DRV(TMC5130))
#define E3_IS_TRINAMIC      (E3_IS_DRV(TMC2130) || E3_IS_DRV(TMC2208) || E3_IS_DRV(TMC2660) || E3_IS_DRV(TMC5130))
#define E4_IS_TRINAMIC      (E4_IS_DRV(TMC2130) || E4_IS_DRV(TMC2208) || E4_IS_DRV(TMC2660) || E4_IS_DRV(TMC5130))
#define E5_IS_TRINAMIC      (E5_IS_DRV(TMC2130) || E5_IS_DRV(TMC2208) || E5_IS_DRV(TMC2660) || E5_IS_DRV(TMC5130))

#endif /* _DRIVER_TYPES_H_ */
