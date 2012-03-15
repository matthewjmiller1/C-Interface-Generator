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
 * This implements the interface related portion of the
 * win_button class.
 * This file should be included in the win_button implementation file
 * Yes, including a C file is bizarre, but that's how it works here.
 */

/* 
 * See below for forward declarations that must be defined manually in the
 * implementing C file.
 */

#include "win_button_gen.h"
#include "button_friend_gen.h"

/* Forward declarations */
/* Begin structs that must be defined manually. */

/** 
 * Forward pointer to reference non-interface data for the class.
 * This must be defined manually.
 */
typedef struct win_button_data_st_ *win_button_data_handle;

/* End structs that must be defined manually. */

/* Forward declarations */
/* Begin functions that must be defined manually. */

static void
win_button_data_delete(win_button_data_handle *win_button_data_h);

static bool
win_button_data_create(win_button_data_handle *win_button_data_h, void *context);

static void
win_button_button_paint(button_handle button_h);

/* End functions that must be defined manually. */

/** Private data for this class */
typedef struct win_button_st_ {
    /** button reference */
    button_st button;
    /** Data for this class */
    win_button_data_handle win_button_data_h;
} win_button_st;

/*
 * This is C, we need explicit casts to each of an object's parent classes.
 */

/**
 * Cast the button object to win_button.
 *
 * @param button_h The button object
 * @return The win_button object
 */
static win_button_handle
button_cast_to_win_button (button_handle button_h)
{
    win_button_handle win_button_h = NULL;

    if (NULL != button_h) {
        win_button_h = (win_button_handle) ((uint8_t *) button_h -
            offsetof(win_button_st, button));
    }

    return (win_button_h);
}

/**
 * Cast the win_button object to button.
 *
 * @param win_button_h The win_button object
 * @return The button object
 */
button_handle
win_button_cast_to_button (win_button_handle win_button_h)
{
    button_handle button_h = NULL;

    if (NULL != win_button_h) {
        button_h = &(win_button_h->button);
    }

    return (button_h);
}

 /**
 * The function to delete a win_button object.  Upon return, the
 * object is not longer valid and the pointer is set to NULL.
 *
 * @param win_button_h A pointer to the object.  If NULL, then this function 
 * is a no-op.
 */
void
win_button_delete (win_button_handle win_button_h)
{
    if (NULL == win_button_h) {
        return;
    }

    win_button_data_delete(&(win_button_h->win_button_data_h));

    button_friend_delete(&(win_button_h->button));

    free(win_button_h);
}

/**
 * Wrapper for to call common function.
 *
 * @param button_h The object
 */
static void
win_button_button_delete (button_handle button_h)
{
    if (NULL == button_h) {
        return;
    }

    win_button_delete(button_cast_to_win_button(button_h));
}

/**
 * The virtual function table for button interface.
 */
static button_vtable_st button_vtable = {
    win_button_button_paint,
    win_button_button_delete
};

/**
 * Initialize the win_button objects.
 *
 * @param win_button_h The object
 * @param context An opaque context passed to win_button_data_create
 * @return TRUE on success, FALSE otherwise
 */
static bool
win_button_init (win_button_handle win_button_h, void *context)
{
    bool rc = false;
    bool button_initialized = false;
    bool win_button_data_created = false;

    if (NULL == win_button_h) {
        return (false);
    }

    rc = button_init(&(win_button_h->button));
    if (!rc) {
        goto err_exit;
    }
    button_initialized = true;

    rc = button_set_vtable(&(win_button_h->button),
             &button_vtable);
    if (!rc) {
        goto err_exit;
    }

    rc = win_button_data_create(&(win_button_h->win_button_data_h), context);
    if (!rc) {
        goto err_exit;
    }
    win_button_data_created = true;

    return (true);

err_exit:

    if (win_button_data_created) {
        win_button_data_delete(&(win_button_h->win_button_data_h));
    }

    if (button_initialized) {
        button_friend_delete(&(win_button_h->button));
    }

    return (rc);
}
