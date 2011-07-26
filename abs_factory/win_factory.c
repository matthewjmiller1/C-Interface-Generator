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
 * This is the non-automated implementation of the win_factory class.
 */

#include "win_button.h"

/* Not an error, see comments in generated file. */
#include "gen/win_factory_gen.c"

/* Don't need a definition since we don't use it */
//typedef struct win_factory_data_st_ *win_factory_data_handle;

/**
 * Override the gui_factory virtual function and to create a button.
 *
 * @param gui_factory_h The gui_factory object
 * @return The button object or NULL if an error occurred.
 * @see gui_factory_create_button()
 */
static button_handle
win_factory_gui_factory_create_button (gui_factory_handle gui_factory_h)
{
    win_button_handle win_button_h;
    button_handle button_h = NULL;

    if (NULL == gui_factory_h) {
        return (NULL);
    }

    win_button_h = win_button_new1();
    if (NULL != win_button_h) {
        button_h = win_button_cast_to_button(win_button_h);
    }

    return (button_h);
}

/**
 * The internal function to delete win_factory_data objects.  Upon return, the
 * object is not longer valid.
 *
 * @param win_factory_data_h Pointer to the object.  If NULL, then this function
 * is a no-op.  The pointer is set to NULL upon return.
 * @see win_factory_delete()
 */
static void
win_factory_data_delete (win_factory_data_handle *win_factory_data_h)
{
    if ((NULL == win_factory_data_h) || (NULL == *win_factory_data_h)) {
        return;
    }

    free(*win_factory_data_h);
    *win_factory_data_h = NULL;
}

/**
 * Create a new win_factory_data object or NULL if there is no data for
 * the object to store.
 *
 * @param win_factory_data_h A pointer to the newly created object or NULL if
 * object creation was necessary.
 * @return TRUE on success, FALSE otherwise
 */
static bool
win_factory_data_create (win_factory_data_handle *win_factory_data_h,
                         void *context)
{
    if (NULL == win_factory_data_h) {
        return (false);
    }

    /* No data is stored for this class */
    *win_factory_data_h = NULL;

    return (true);
}

/**
 * Create a new win_factory object.
 *
 * @return The object or NULL if creation failed
 */
win_factory_handle
win_factory_new1 (void)
{
    win_factory_st *win_factory = NULL;
    bool rc;

    win_factory = calloc(1, sizeof(*win_factory));
    if (NULL != win_factory) {
        rc = win_factory_init(win_factory, NULL);
        if (!rc) {
            goto err_exit;
        }
    }

    return (win_factory);

err_exit:

    win_factory_delete(win_factory);

    return (NULL);
}
