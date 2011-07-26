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
 * This implements the interface related portion of the
 * win_factory class.
 * This file should be included in the win_factory implementation file
 * Yes, including a C file is bizarre, but that's how it works here.
 */

/* 
 * See below for forward declarations that must be defined manually in the
 * implementing C file.
 */

#include "win_factory_gen.h"
#include "gui_factory_friend_gen.h"

/* Forward declarations */
/* Begin structs that must be defined manually. */

/** 
 * Forward pointer to reference non-interface data for the class.
 * This must be defined manually.
 */
typedef struct win_factory_data_st_ *win_factory_data_handle;

/* End structs that must be defined manually. */

/* Forward declarations */
/* Begin functions that must be defined manually. */

static void
win_factory_data_delete(win_factory_data_handle *win_factory_data_h);

static bool
win_factory_data_create(win_factory_data_handle *win_factory_data_h, void *context);

static button_handle
win_factory_gui_factory_create_button(gui_factory_handle gui_factory_h);

/* End functions that must be defined manually. */

/** Private data for this class */
typedef struct win_factory_st_ {
    /** gui_factory reference */
    gui_factory_st gui_factory;
    /** Data for this class */
    win_factory_data_handle win_factory_data_h;
} win_factory_st;

/*
 * This is C, we need explicit casts to each of an object's parent classes.
 */

/**
 * Cast the gui_factory object to win_factory.
 *
 * @param gui_factory_h The gui_factory object
 * @return The win_factory object
 */
static win_factory_handle
gui_factory_cast_to_win_factory (gui_factory_handle gui_factory_h)
{
    win_factory_handle win_factory_h = NULL;

    if (NULL != gui_factory_h) {
        win_factory_h = (win_factory_handle) ((uint8_t *) gui_factory_h -
            offsetof(win_factory_st, gui_factory));
    }

    return (win_factory_h);
}

/**
 * Cast the win_factory object to gui_factory.
 *
 * @param win_factory_h The win_factory object
 * @return The gui_factory object
 */
gui_factory_handle
win_factory_cast_to_gui_factory (win_factory_handle win_factory_h)
{
    gui_factory_handle gui_factory_h = NULL;

    if (NULL != win_factory_h) {
        gui_factory_h = &(win_factory_h->gui_factory);
    }

    return (gui_factory_h);
}

 /**
 * The function to delete a win_factory object.  Upon return, the
 * object is not longer valid and the pointer is set to NULL.
 *
 * @param win_factory_h A pointer to the object.  If NULL, then this function 
 * is a no-op.
 */
void
win_factory_delete (win_factory_handle win_factory_h)
{
    if (NULL == win_factory_h) {
        return;
    }

    win_factory_data_delete(&(win_factory_h->win_factory_data_h));

    gui_factory_friend_delete(&(win_factory_h->gui_factory));

    free(win_factory_h);
}

/**
 * Wrapper for to call common function.
 *
 * @param gui_factory_h The object
 */
static void
win_factory_gui_factory_delete (gui_factory_handle gui_factory_h)
{
    if (NULL == gui_factory_h) {
        return;
    }

    win_factory_delete(gui_factory_cast_to_win_factory(gui_factory_h));
}

/**
 * The virtual function table for gui_factory interface.
 */
static gui_factory_vtable_st gui_factory_vtable = {
    win_factory_gui_factory_create_button,
    win_factory_gui_factory_delete
};

/**
 * Initialize the win_factory objects.
 *
 * @param win_factory_h The object
 * @param context An opaque context passed to win_factory_data_create
 * @return TRUE on success, FALSE otherwise
 */
static bool
win_factory_init (win_factory_handle win_factory_h, void *context)
{
    bool rc = false;
    bool gui_factory_initialized = false;
    bool win_factory_data_created = false;

    if (NULL == win_factory_h) {
        return (false);
    }

    rc = gui_factory_init(&(win_factory_h->gui_factory));
    if (!rc) {
        goto err_exit;
    }
    gui_factory_initialized = true;

    rc = gui_factory_set_vtable(&(win_factory_h->gui_factory),
             &gui_factory_vtable);
    if (!rc) {
        goto err_exit;
    }

    rc = win_factory_data_create(&(win_factory_h->win_factory_data_h), context);
    if (!rc) {
        goto err_exit;
    }
    win_factory_data_created = true;

    return (true);

err_exit:

    if (win_factory_data_created) {
        win_factory_data_delete(&(win_factory_h->win_factory_data_h));
    }

    if (gui_factory_initialized) {
        gui_factory_friend_delete(&(win_factory_h->gui_factory));
    }

    return (rc);
}
