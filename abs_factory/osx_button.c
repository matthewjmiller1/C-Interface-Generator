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
 * This is the non-automated implementation of the osx_button class.
 */

#include <stdio.h>
#include <inttypes.h>
#include "osx_button.h"

/* Not an error, see comments in generated file. */
#include "gen/osx_button_gen.c"

/**
 * The class-specific data.
 */
typedef struct osx_button_data_st_ {
    /** The ID of the button */
    uint64_t id;
} osx_button_data_st;

/**
 * Override the button virtual function to paint.
 *
 * @param button_h The button object
 * @see button_paint()
 */
static void
osx_button_button_paint (button_handle button_h)
{
    osx_button_handle osx_button_h = button_cast_to_osx_button(button_h);

    if (NULL == osx_button_h) {
        return;
    }

    printf("I'm an OSXButton with ID %"PRIu64"\n",
           osx_button_h->osx_button_data_h->id);
}

/**
 * The internal function to delete osx_button_data objects.  Upon return, the
 * object is not longer valid.
 *
 * @param osx_button_data_h Pointer to the object.  If NULL, then this function
 * is a no-op.  The pointer is set to NULL upon return.
 * @see osx_button_delete()
 */
static void
osx_button_data_delete (osx_button_data_handle *osx_button_data_h)
{
    if ((NULL == osx_button_data_h) || (NULL == *osx_button_data_h)) {
        return;
    }

    free(*osx_button_data_h);
    *osx_button_data_h = NULL;
}

/**
 * Create a new osx_button_data object or NULL if there is no data for
 * the object to store.
 *
 * @param osx_button_data_h A pointer to the newly created object or NULL if
 * object creation was necessary.
 * @return TRUE on success, FALSE otherwise
 */
static bool
osx_button_data_create (osx_button_data_handle *osx_button_data_h,
                        void *context)
{
    osx_button_data_st *data;
    uint64_t button_id;

    if ((NULL == osx_button_data_h) || (NULL == context)) {
        return (false);
    }

    button_id = *((uint64_t *) context);

    data = calloc(1, sizeof(*data));
    if (NULL == data) {
        return (false);
    }

    /* Initialize the data */
    data->id = button_id;

    *osx_button_data_h = data;

    return (true);
}

/**
 * Create a new osx_button object.
 *
 * @param id The ID given to the button by the GUI factory
 * @return The object or NULL if creation failed
 */
osx_button_handle
osx_button_new1 (uint64_t id)
{
    osx_button_st *osx_button = NULL;
    bool rc;

    osx_button = calloc(1, sizeof(*osx_button));
    if (NULL != osx_button) {
        rc = osx_button_init(osx_button, &id);
        if (!rc) {
            goto err_exit;
        }
    }

    return (osx_button);

err_exit:

    osx_button_delete(osx_button);

    return (NULL);
}
