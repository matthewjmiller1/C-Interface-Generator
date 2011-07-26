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
 * osx_factory class.
 * This file should be included in the osx_factory implementation file
 * Yes, including a C file is bizarre, but that's how it works here.
 */

/* 
 * See below for forward declarations that must be defined manually in the
 * implementing C file.
 */

#include "osx_factory_gen.h"
#include "gui_factory_friend_gen.h"

/* Forward declarations */
/* Begin structs that must be defined manually. */

/** 
 * Forward pointer to reference non-interface data for the class.
 * This must be defined manually.
 */
typedef struct osx_factory_data_st_ *osx_factory_data_handle;

/* End structs that must be defined manually. */

/* Forward declarations */
/* Begin functions that must be defined manually. */

static void
osx_factory_data_delete(osx_factory_data_handle *osx_factory_data_h);

static bool
osx_factory_data_create(osx_factory_data_handle *osx_factory_data_h, void *context);

static button_handle
osx_factory_gui_factory_create_button(gui_factory_handle gui_factory_h);

/* End functions that must be defined manually. */

/** Private data for this class */
typedef struct osx_factory_st_ {
    /** gui_factory reference */
    gui_factory_st gui_factory;
    /** Data for this class */
    osx_factory_data_handle osx_factory_data_h;
} osx_factory_st;

/*
 * This is C, we need explicit casts to each of an object's parent classes.
 */

/**
 * Cast the gui_factory object to osx_factory.
 *
 * @param gui_factory_h The gui_factory object
 * @return The osx_factory object
 */
static osx_factory_handle
gui_factory_cast_to_osx_factory (gui_factory_handle gui_factory_h)
{
    osx_factory_handle osx_factory_h = NULL;

    if (NULL != gui_factory_h) {
        osx_factory_h = (osx_factory_handle) ((uint8_t *) gui_factory_h -
            offsetof(osx_factory_st, gui_factory));
    }

    return (osx_factory_h);
}

/**
 * Cast the osx_factory object to gui_factory.
 *
 * @param osx_factory_h The osx_factory object
 * @return The gui_factory object
 */
gui_factory_handle
osx_factory_cast_to_gui_factory (osx_factory_handle osx_factory_h)
{
    gui_factory_handle gui_factory_h = NULL;

    if (NULL != osx_factory_h) {
        gui_factory_h = &(osx_factory_h->gui_factory);
    }

    return (gui_factory_h);
}

 /**
 * The function to delete a osx_factory object.  Upon return, the
 * object is not longer valid and the pointer is set to NULL.
 *
 * @param osx_factory_h A pointer to the object.  If NULL, then this function 
 * is a no-op.
 */
void
osx_factory_delete (osx_factory_handle osx_factory_h)
{
    if (NULL == osx_factory_h) {
        return;
    }

    osx_factory_data_delete(&(osx_factory_h->osx_factory_data_h));

    gui_factory_friend_delete(&(osx_factory_h->gui_factory));

    free(osx_factory_h);
}

/**
 * Wrapper for to call common function.
 *
 * @param gui_factory_h The object
 */
static void
osx_factory_gui_factory_delete (gui_factory_handle gui_factory_h)
{
    if (NULL == gui_factory_h) {
        return;
    }

    osx_factory_delete(gui_factory_cast_to_osx_factory(gui_factory_h));
}

/**
 * The virtual function table for gui_factory interface.
 */
static gui_factory_vtable_st gui_factory_vtable = {
    osx_factory_gui_factory_create_button,
    osx_factory_gui_factory_delete
};

/**
 * Initialize the osx_factory objects.
 *
 * @param osx_factory_h The object
 * @param context An opaque context passed to osx_factory_data_create
 * @return TRUE on success, FALSE otherwise
 */
static bool
osx_factory_init (osx_factory_handle osx_factory_h, void *context)
{
    bool rc = false;
    bool gui_factory_initialized = false;
    bool osx_factory_data_created = false;

    if (NULL == osx_factory_h) {
        return (false);
    }

    rc = gui_factory_init(&(osx_factory_h->gui_factory));
    if (!rc) {
        goto err_exit;
    }
    gui_factory_initialized = true;

    rc = gui_factory_set_vtable(&(osx_factory_h->gui_factory),
             &gui_factory_vtable);
    if (!rc) {
        goto err_exit;
    }

    rc = osx_factory_data_create(&(osx_factory_h->osx_factory_data_h), context);
    if (!rc) {
        goto err_exit;
    }
    osx_factory_data_created = true;

    return (true);

err_exit:

    if (osx_factory_data_created) {
        osx_factory_data_delete(&(osx_factory_h->osx_factory_data_h));
    }

    if (gui_factory_initialized) {
        gui_factory_friend_delete(&(osx_factory_h->gui_factory));
    }

    return (rc);
}
