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
 * This is the non-automated implementation of the win_button class.
 */

#include <stdio.h>
#include "win_button.h"

/* Not an error, see comments in generated file. */
#include "gen/win_button_gen.c"

/* Don't need a definition since we don't use it */
//typedef struct win_button_data_st_ *win_button_data_handle;

/**
 * Override the button virtual function to paint.
 *
 * @param button_h The button object
 * @see button_paint()
 */
static void
win_button_button_paint (button_handle button_h)
{
    if (NULL == button_h) {
        return;
    }

    printf("I'm a WinButton\n");
}

/**
 * The internal function to delete win_button_data objects.  Upon return, the
 * object is not longer valid.
 *
 * @param win_button_data_h Pointer to the object.  If NULL, then this function
 * is a no-op.  The pointer is set to NULL upon return.
 * @see win_button_delete()
 */
static void
win_button_data_delete (win_button_data_handle *win_button_data_h)
{
    if ((NULL == win_button_data_h) || (NULL == *win_button_data_h)) {
        return;
    }

    free(*win_button_data_h);
    *win_button_data_h = NULL;
}

/**
 * Create a new win_button_data object or NULL if there is no data for
 * the object to store.
 *
 * @param win_button_data_h A pointer to the newly created object or NULL if
 * object creation was necessary.
 * @return TRUE on success, FALSE otherwise
 */
static bool
win_button_data_create (win_button_data_handle *win_button_data_h,
                        void *context)
{
    if (NULL == win_button_data_h) {
        return (false);
    }

    /* No data is stored for this class */
    *win_button_data_h = NULL;

    return (true);
}

/**
 * Create a new win_button object.
 *
 * @return The object or NULL if creation failed
 */
win_button_handle
win_button_new1 (void)
{
    win_button_st *win_button = NULL;
    bool rc;

    win_button = calloc(1, sizeof(*win_button));
    if (NULL != win_button) {
        rc = win_button_init(win_button, NULL);
        if (!rc) {
            goto err_exit;
        }
    }

    return (win_button);

err_exit:

    win_button_delete(win_button);

    return (NULL);
}
