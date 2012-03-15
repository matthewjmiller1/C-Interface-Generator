/* THIS IS A GENERATED FILE, DO NOT EDIT!!! */
/**
 * @file
 * 
 * @author Matt Miller <matt@matthewmiller.net>
 * 
 * @section LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @section DESCRIPTION
 *
 * This includes the APIs for casting to interfaces the
 * osx_button class implements and its opaque handle.
 * This file should be included in the
 * public header file for the osx_button class.
 */
#ifndef __OSX_BUTTON_GEN_H__
#define __OSX_BUTTON_GEN_H__

#include "button_gen.h"

/** Opaque pointer to reference instances of this class */
typedef struct osx_button_st_ *osx_button_handle;

/* APIs below are documented in their implementation file */

extern void
osx_button_delete(osx_button_handle osx_button_h);

extern button_handle
osx_button_cast_to_button(osx_button_handle osx_button_h);

#endif
