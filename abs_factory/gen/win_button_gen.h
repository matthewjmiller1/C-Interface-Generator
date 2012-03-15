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
 * win_button class implements and its opaque handle.
 * This file should be included in the
 * public header file for the win_button class.
 */
#ifndef __WIN_BUTTON_GEN_H__
#define __WIN_BUTTON_GEN_H__

#include "button_gen.h"

/** Opaque pointer to reference instances of this class */
typedef struct win_button_st_ *win_button_handle;

/* APIs below are documented in their implementation file */

extern void
win_button_delete(win_button_handle win_button_h);

extern button_handle
win_button_cast_to_button(win_button_handle win_button_h);

#endif
