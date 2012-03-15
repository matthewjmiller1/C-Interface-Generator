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
 * This is the friend interface for the button class.
 * It should only be included by implementors of the
 * button interface.
 */
#ifndef __BUTTON_FRIEND_GEN_H__
#define __BUTTON_FRIEND_GEN_H__

#include "button_gen.h"

/** Opaque pointer to reference private data for the class */
typedef struct button_private_st_ *button_private_handle;

/** Friend accessible data for this class */
typedef struct button_st_ {
    /** Reference to private data */
    button_private_handle private_h;
} button_st;

/**
 * Virtual function declaration.
 */
typedef void
(*button_paint_fn)(button_handle button_h);

/**
 * Virtual function declaration.
 */
typedef void
(*button_delete_fn)(button_handle button_h);

/**
 * The virtual table to be specified by friend classes.
 *
 * @see button_set_vtable()
 */
typedef struct button_vtable_st_ {
    /** Virtual function */
    button_paint_fn paint_fn;
    /** Virtual function */
    button_delete_fn delete_fn;
} button_vtable_st;

/* APIs below are documented in their implementation file */

extern bool
button_set_vtable(button_handle button_h,
                  button_vtable_st *vtable);

extern void
button_friend_delete(button_handle button_h);

extern bool
button_init(button_handle button_h);

#endif
