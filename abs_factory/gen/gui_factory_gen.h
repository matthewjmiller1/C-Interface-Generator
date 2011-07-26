/* THIS IS A GENERATED FILE, DO NOT EDIT!!! */
/**
 * @file
 * 
 * @author Matt Miller <matt@matthewmiller.net>
 * 
 * @section LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * This is the public interface for the gui_factory class.
 */
#ifndef __GUI_FACTORY_GEN_H__
#define __GUI_FACTORY_GEN_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "button_gen.h"

/** Opaque pointer to reference instances of this class */
typedef struct gui_factory_st_ *gui_factory_handle;

/* APIs below are documented in their implementation file */

extern button_handle
gui_factory_create_button(gui_factory_handle gui_factory_h);

extern void
gui_factory_delete(gui_factory_handle gui_factory_h);

#endif
