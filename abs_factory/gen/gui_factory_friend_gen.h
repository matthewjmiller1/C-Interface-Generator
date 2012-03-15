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
 * This is the friend interface for the gui_factory class.
 * It should only be included by implementors of the
 * gui_factory interface.
 */
#ifndef __GUI_FACTORY_FRIEND_GEN_H__
#define __GUI_FACTORY_FRIEND_GEN_H__

#include "gui_factory_gen.h"

/** Opaque pointer to reference private data for the class */
typedef struct gui_factory_private_st_ *gui_factory_private_handle;

/** Friend accessible data for this class */
typedef struct gui_factory_st_ {
    /** Reference to private data */
    gui_factory_private_handle private_h;
} gui_factory_st;

/**
 * Virtual function declaration.
 */
typedef button_handle
(*gui_factory_create_button_fn)(gui_factory_handle gui_factory_h);

/**
 * Virtual function declaration.
 */
typedef void
(*gui_factory_delete_fn)(gui_factory_handle gui_factory_h);

/**
 * The virtual table to be specified by friend classes.
 *
 * @see gui_factory_set_vtable()
 */
typedef struct gui_factory_vtable_st_ {
    /** Virtual function */
    gui_factory_create_button_fn create_button_fn;
    /** Virtual function */
    gui_factory_delete_fn delete_fn;
} gui_factory_vtable_st;

/* APIs below are documented in their implementation file */

extern bool
gui_factory_set_vtable(gui_factory_handle gui_factory_h,
                       gui_factory_vtable_st *vtable);

extern void
gui_factory_friend_delete(gui_factory_handle gui_factory_h);

extern bool
gui_factory_init(gui_factory_handle gui_factory_h);

#endif
