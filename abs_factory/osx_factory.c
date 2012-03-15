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
 * This is the non-automated implementation of the osx_factory class.
 */

#include "osx_button.h"

/* Not an error, see comments in generated file. */
#include "gen/osx_factory_gen.c"

/**
 * The class-specific data.
 */
typedef struct osx_factory_data_st_ {
    /** The next button ID to use for a newly created button */
    uint64_t next_button_id;
} osx_factory_data_st;

/**
 * Override the gui_factory virtual function and to create a button.
 *
 * @param gui_factory_h The gui_factory object
 * @return The button object or NULL if an error occurred.
 * @see gui_factory_create_button()
 */
static button_handle
osx_factory_gui_factory_create_button (gui_factory_handle gui_factory_h)
{
    osx_button_handle osx_button_h;
    osx_factory_handle osx_factory_h;
    button_handle button_h = NULL;
    uint64_t id;

    osx_factory_h = gui_factory_cast_to_osx_factory(gui_factory_h);

    if (NULL == osx_factory_h) {
        return (NULL);
    }

    id = (osx_factory_h->osx_factory_data_h->next_button_id)++;
    osx_button_h = osx_button_new1(id);
    if (NULL != osx_button_h) {
        button_h = osx_button_cast_to_button(osx_button_h);
    }

    return (button_h);
}

/**
 * The internal function to delete osx_factory_data objects.  Upon return, the
 * object is not longer valid.
 *
 * @param osx_factory_data_h Pointer to the object.  If NULL, then this function
 * is a no-op.  The pointer is set to NULL upon return.
 * @see osx_factory_delete()
 */
static void
osx_factory_data_delete (osx_factory_data_handle *osx_factory_data_h)
{
    if ((NULL == osx_factory_data_h) || (NULL == *osx_factory_data_h)) {
        return;
    }

    free(*osx_factory_data_h);
    *osx_factory_data_h = NULL;
}

/**
 * Create a new osx_factory_data object or NULL if there is no data for
 * the object to store.
 *
 * @param osx_factory_data_h A pointer to the newly created object or NULL if
 * object creation was necessary.
 * @return TRUE on success, FALSE otherwise
 */
static bool
osx_factory_data_create (osx_factory_data_handle *osx_factory_data_h,
                         void * context)
{
    osx_factory_data_st *data;

    if (NULL == osx_factory_data_h) {
        return (false);
    }

    data = calloc(1, sizeof(*data));
    if (NULL == data) {
        return (false);
    }

    /* Initialize the data */
    data->next_button_id = 1;

    *osx_factory_data_h = data;

    return (true);
}

/**
 * Create a new osx_factory object.
 *
 * @return The object or NULL if creation failed
 */
osx_factory_handle
osx_factory_new1 (void)
{
    osx_factory_st *osx_factory = NULL;
    bool rc;

    osx_factory = calloc(1, sizeof(*osx_factory));
    if (NULL != osx_factory) {
        rc = osx_factory_init(osx_factory, NULL);
        if (!rc) {
            goto err_exit;
        }
    }

    return (osx_factory);

err_exit:

    osx_factory_delete(osx_factory);

    return (NULL);
}
