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
 * This includes the APIs for casting to interfaces the
 * osx_factory class implements and its opaque handle.
 * This file should be included in the
 * public header file for the osx_factory class.
 */
#ifndef __OSX_FACTORY_GEN_H__
#define __OSX_FACTORY_GEN_H__

#include "gui_factory_gen.h"

/** Opaque pointer to reference instances of this class */
typedef struct osx_factory_st_ *osx_factory_handle;

/* APIs below are documented in their implementation file */

extern void
osx_factory_delete(osx_factory_handle osx_factory_h);

extern gui_factory_handle
osx_factory_cast_to_gui_factory(osx_factory_handle osx_factory_h);

#endif
