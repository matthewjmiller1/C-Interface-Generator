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
 * This is the implementation of the gui_factory interface.
 */
#include <assert.h>
#include "gui_factory_friend_gen.h"

/**
 * Private variables which cannot be directly accessed by
 * any other class including children.
 */
typedef struct gui_factory_private_st_ {
    /** Virtual function table */
    const gui_factory_vtable_st *vtable;
} gui_factory_private_st;

/**
 * The internal function to delete a gui_factory object.  Upon return, the
 * object is not longer valid.
 *
 * @param gui_factory_h The object.  If NULL, then this function is a no-op.
 * @param free_gui_factory_h Indicates whether the base object should be freed
 * or not.
 * @see gui_factory_delete()
 * @see gui_factory_friend_delete()
 * @see gui_factory_private_delete()
 */
static void
gui_factory_delete_internal (gui_factory_handle gui_factory_h, 
    bool free_gui_factory_h)
{
    if (NULL == gui_factory_h) {
        return;
    }

    if (NULL != gui_factory_h->private_h) {
        free(gui_factory_h->private_h);
        gui_factory_h->private_h = NULL;
    }

    if (free_gui_factory_h) {
        free(gui_factory_h);
    }
}

/**
 * Allow a friend class to delete the gui_factory object.  It is assumed that
 * the friend class is managing the memory for the gui_factory object and, thus,
 * the object will not be freed.  However, members within the gui_factory object
 * may be freed.  This does not call the virtual function table version of
 * delete, but rather the delete specifically for type gui_factory.
 *
 * @param gui_factory_h The object.  If NULL, then this function is a no-op.
 * @see gui_factory_delete()
 */
void
gui_factory_friend_delete (gui_factory_handle gui_factory_h)
{
    gui_factory_delete_internal(gui_factory_h, false);
}

/**
 * create_button from gui_factory.
 *
 * @param gui_factory_h The object
 * @return button_handle
 */
button_handle
gui_factory_create_button (gui_factory_handle gui_factory_h)
{
    assert((NULL != gui_factory_h) &&
           (NULL != gui_factory_h->private_h) &&
           (NULL != gui_factory_h->private_h->vtable) &&
           (NULL != gui_factory_h->private_h->vtable->create_button_fn));

    return (gui_factory_h->private_h->vtable->create_button_fn(gui_factory_h));
}

/**
 * delete from gui_factory.
 *
 * @param gui_factory_h The object
 * @return void
 */
void
gui_factory_delete (gui_factory_handle gui_factory_h)
{
    assert((NULL != gui_factory_h) &&
           (NULL != gui_factory_h->private_h) &&
           (NULL != gui_factory_h->private_h->vtable) &&
           (NULL != gui_factory_h->private_h->vtable->delete_fn));

    return (gui_factory_h->private_h->vtable->delete_fn(gui_factory_h));
}

/**
 * The virtual function table used for objects of type gui_factory.  As this is
 * an interface, all functions should be NULL.
 */
static const gui_factory_vtable_st gui_factory_vtable = {
    NULL,
    NULL
};

/**
 * Fill in the child vtable with values inherited from the parent_vtable for all
 * functions left NULL in the child vtable.
 *
 * @param parent_vtable The parent vtable from which to inherit.
 * @param child_vtable The child vtable to which functions may be inherited.
 * @param do_null_check Indicates whether an error should be thrown if a
 * function in the child vtable is NULL after inheritance.
 * @return TRUE on success, FALSE otherwise
 */
static bool
gui_factory_inherit_vtable (const gui_factory_vtable_st *parent_vtable,
    gui_factory_vtable_st *child_vtable,
    bool do_null_check)
{
    if ((NULL == parent_vtable) || (NULL == child_vtable)) {
        return (false);
    }

    if (NULL == child_vtable->create_button_fn) {
        child_vtable->create_button_fn = parent_vtable->create_button_fn;
        if (do_null_check && (NULL == child_vtable->create_button_fn)) {
            return (false);
        }
    }

    if (NULL == child_vtable->delete_fn) {
        child_vtable->delete_fn = parent_vtable->delete_fn;
        if (do_null_check && (NULL == child_vtable->delete_fn)) {
            return (false);
        }
    }

    return (true);
}

/**
 * This is a function used by implementing classes to set the virtual table
 * according with their methods.
 *
 * @param gui_factory_h The object
 * @param vtable The virtual table specification for the implementing class.  If
 * any function pointer is NULL, an error is returned.
 * @return TRUE on success, FALSE otherwise
 */
bool
gui_factory_set_vtable (gui_factory_handle gui_factory_h, 
    gui_factory_vtable_st *vtable)
{
    bool rc;

    if (((NULL == gui_factory_h) || (NULL == vtable) ||
         (NULL == gui_factory_h->private_h))) {
        return (false);
    }
    
    rc = gui_factory_inherit_vtable(&gui_factory_vtable, vtable, true);

    if (rc) {
        gui_factory_h->private_h->vtable = vtable;
    }

    return (rc);
}

/**
 * Allows a friend class to initialize their inner gui_factory object.  Must be
 * called before the gui_factory object is used.  If an error is returned, any
 * clean-up was handled internally and there is no need to call a delete
 * function.
 *
 * @param gui_factory_h The object
 * @return TRUE on success, FALSE otherwise
 * @see gui_factory_delete()
 * @see gui_factory_friend_delete()
 */
bool
gui_factory_init (gui_factory_handle gui_factory_h)
{
    if (NULL == gui_factory_h) {
        return (false);
    }

    gui_factory_h->private_h = calloc(1, sizeof(*gui_factory_h->private_h));
    if (NULL == gui_factory_h->private_h) {
        goto err_exit;
    }

    gui_factory_h->private_h->vtable = NULL;

    return (true);

err_exit:

    if (NULL != gui_factory_h->private_h) {
        free(gui_factory_h->private_h);
        gui_factory_h->private_h = NULL;
    }

    return (false);
}
