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
 * osx_button class.
 * This file should be included in the osx_button implementation file
 * Yes, including a C file is bizarre, but that's how it works here.
 */

/* 
 * See below for forward declarations that must be defined manually in the
 * implementing C file.
 */

#include "osx_button_gen.h"
#include "button_friend_gen.h"

/* Forward declarations */
/* Begin structs that must be defined manually. */

/** 
 * Forward pointer to reference non-interface data for the class.
 * This must be defined manually.
 */
typedef struct osx_button_data_st_ *osx_button_data_handle;

/* End structs that must be defined manually. */

/* Forward declarations */
/* Begin functions that must be defined manually. */

static void
osx_button_data_delete(osx_button_data_handle *osx_button_data_h);

static bool
osx_button_data_create(osx_button_data_handle *osx_button_data_h, void *context);

static void
osx_button_button_paint(button_handle button_h);

/* End functions that must be defined manually. */

/** Private data for this class */
typedef struct osx_button_st_ {
    /** button reference */
    button_st button;
    /** Data for this class */
    osx_button_data_handle osx_button_data_h;
} osx_button_st;

/*
 * This is C, we need explicit casts to each of an object's parent classes.
 */

/**
 * Cast the button object to osx_button.
 *
 * @param button_h The button object
 * @return The osx_button object
 */
static osx_button_handle
button_cast_to_osx_button (button_handle button_h)
{
    osx_button_handle osx_button_h = NULL;

    if (NULL != button_h) {
        osx_button_h = (osx_button_handle) ((uint8_t *) button_h -
            offsetof(osx_button_st, button));
    }

    return (osx_button_h);
}

/**
 * Cast the osx_button object to button.
 *
 * @param osx_button_h The osx_button object
 * @return The button object
 */
button_handle
osx_button_cast_to_button (osx_button_handle osx_button_h)
{
    button_handle button_h = NULL;

    if (NULL != osx_button_h) {
        button_h = &(osx_button_h->button);
    }

    return (button_h);
}

 /**
 * The function to delete a osx_button object.  Upon return, the
 * object is not longer valid and the pointer is set to NULL.
 *
 * @param osx_button_h A pointer to the object.  If NULL, then this function 
 * is a no-op.
 */
void
osx_button_delete (osx_button_handle osx_button_h)
{
    if (NULL == osx_button_h) {
        return;
    }

    osx_button_data_delete(&(osx_button_h->osx_button_data_h));

    button_friend_delete(&(osx_button_h->button));

    free(osx_button_h);
}

/**
 * Wrapper for to call common function.
 *
 * @param button_h The object
 */
static void
osx_button_button_delete (button_handle button_h)
{
    if (NULL == button_h) {
        return;
    }

    osx_button_delete(button_cast_to_osx_button(button_h));
}

/**
 * The virtual function table for button interface.
 */
static button_vtable_st button_vtable = {
    osx_button_button_paint,
    osx_button_button_delete
};

/**
 * Initialize the osx_button objects.
 *
 * @param osx_button_h The object
 * @param context An opaque context passed to osx_button_data_create
 * @return TRUE on success, FALSE otherwise
 */
static bool
osx_button_init (osx_button_handle osx_button_h, void *context)
{
    bool rc = false;
    bool button_initialized = false;
    bool osx_button_data_created = false;

    if (NULL == osx_button_h) {
        return (false);
    }

    rc = button_init(&(osx_button_h->button));
    if (!rc) {
        goto err_exit;
    }
    button_initialized = true;

    rc = button_set_vtable(&(osx_button_h->button),
             &button_vtable);
    if (!rc) {
        goto err_exit;
    }

    rc = osx_button_data_create(&(osx_button_h->osx_button_data_h), context);
    if (!rc) {
        goto err_exit;
    }
    osx_button_data_created = true;

    return (true);

err_exit:

    if (osx_button_data_created) {
        osx_button_data_delete(&(osx_button_h->osx_button_data_h));
    }

    if (button_initialized) {
        button_friend_delete(&(osx_button_h->button));
    }

    return (rc);
}
