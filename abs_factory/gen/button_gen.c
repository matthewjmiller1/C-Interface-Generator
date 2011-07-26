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
 * This is the implementation of the button interface.
 */
#include <assert.h>
#include "button_friend_gen.h"

/**
 * Private variables which cannot be directly accessed by
 * any other class including children.
 */
typedef struct button_private_st_ {
    /** Virtual function table */
    const button_vtable_st *vtable;
} button_private_st;

/**
 * The internal function to delete a button object.  Upon return, the
 * object is not longer valid.
 *
 * @param button_h The object.  If NULL, then this function is a no-op.
 * @param free_button_h Indicates whether the base object should be freed
 * or not.
 * @see button_delete()
 * @see button_friend_delete()
 * @see button_private_delete()
 */
static void
button_delete_internal (button_handle button_h, 
    bool free_button_h)
{
    if (NULL == button_h) {
        return;
    }

    if (NULL != button_h->private_h) {
        free(button_h->private_h);
        button_h->private_h = NULL;
    }

    if (free_button_h) {
        free(button_h);
    }
}

/**
 * Allow a friend class to delete the button object.  It is assumed that
 * the friend class is managing the memory for the button object and, thus,
 * the object will not be freed.  However, members within the button object
 * may be freed.  This does not call the virtual function table version of
 * delete, but rather the delete specifically for type button.
 *
 * @param button_h The object.  If NULL, then this function is a no-op.
 * @see button_delete()
 */
void
button_friend_delete (button_handle button_h)
{
    button_delete_internal(button_h, false);
}

/**
 * paint from button.
 *
 * @param button_h The object
 * @return void
 */
void
button_paint (button_handle button_h)
{
    assert((NULL != button_h) &&
           (NULL != button_h->private_h) &&
           (NULL != button_h->private_h->vtable) &&
           (NULL != button_h->private_h->vtable->paint_fn));

    return (button_h->private_h->vtable->paint_fn(button_h));
}

/**
 * delete from button.
 *
 * @param button_h The object
 * @return void
 */
void
button_delete (button_handle button_h)
{
    assert((NULL != button_h) &&
           (NULL != button_h->private_h) &&
           (NULL != button_h->private_h->vtable) &&
           (NULL != button_h->private_h->vtable->delete_fn));

    return (button_h->private_h->vtable->delete_fn(button_h));
}

/**
 * The virtual function table used for objects of type button.  As this is
 * an interface, all functions should be NULL.
 */
static const button_vtable_st button_vtable = {
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
button_inherit_vtable (const button_vtable_st *parent_vtable,
    button_vtable_st *child_vtable,
    bool do_null_check)
{
    if ((NULL == parent_vtable) || (NULL == child_vtable)) {
        return (false);
    }

    if (NULL == child_vtable->paint_fn) {
        child_vtable->paint_fn = parent_vtable->paint_fn;
        if (do_null_check && (NULL == child_vtable->paint_fn)) {
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
 * @param button_h The object
 * @param vtable The virtual table specification for the implementing class.  If
 * any function pointer is NULL, an error is returned.
 * @return TRUE on success, FALSE otherwise
 */
bool
button_set_vtable (button_handle button_h, 
    button_vtable_st *vtable)
{
    bool rc;

    if (((NULL == button_h) || (NULL == vtable) ||
         (NULL == button_h->private_h))) {
        return (false);
    }
    
    rc = button_inherit_vtable(&button_vtable, vtable, true);

    if (rc) {
        button_h->private_h->vtable = vtable;
    }

    return (rc);
}

/**
 * Allows a friend class to initialize their inner button object.  Must be
 * called before the button object is used.  If an error is returned, any
 * clean-up was handled internally and there is no need to call a delete
 * function.
 *
 * @param button_h The object
 * @return TRUE on success, FALSE otherwise
 * @see button_delete()
 * @see button_friend_delete()
 */
bool
button_init (button_handle button_h)
{
    if (NULL == button_h) {
        return (false);
    }

    button_h->private_h = calloc(1, sizeof(*button_h->private_h));
    if (NULL == button_h->private_h) {
        goto err_exit;
    }

    button_h->private_h->vtable = NULL;

    return (true);

err_exit:

    if (NULL != button_h->private_h) {
        free(button_h->private_h);
        button_h->private_h = NULL;
    }

    return (false);
}
