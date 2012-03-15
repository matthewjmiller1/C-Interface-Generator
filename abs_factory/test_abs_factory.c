/**
 * @file
 * @author Matt Miller <matt@matthewjmiller.net>
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Main program to do some basic tests on C interface code.
 */

/**
 * @mainpage Example of the C Interface Generation Script
 * \author Matthew J. Miller (matt@matthewjmiller.net)
 * \date 2011
 *
 * @section sec_intro Introduction
 *
 * Interfaces give a 
 * <a href="http://www.artima.com/weblogs/viewpost.jsp?thread=274019">lot 
 * of support for OO-design in a language</a>.  Obviously, they can be done in
 * C, but it takes a fair amount of writing "boring" code to do it.  This gives
 * an example of how it can be done using the c_intf_gen.py Python script to
 * remove a lot of that "boring" code.
 *
 * This example implements the 
 * <a href=http://en.wikipedia.org/wiki/Design_Patterns_%28book%29>GoF's</a>
 * <a href="http://en.wikipedia.org/wiki/Abstract_factory">Abstract
 * Factory</a> design pattern.
 *
 * The interface structure is given via the description file 
 * \c abs_factory_def.txt.  Basically the \c gui_factory interface has a 
 * \c create_button function and is implemented by both the \c win_factory
 * and \c osx_factory classes.  The \c button interface has a \c paint
 * function and is implemented by both the \c win_button and \c osx_button
 * classes.
 *
 * In \c generate_gui in test_abs_factory.c, we specify a way of building GUIs
 * that create buttons and is OS-indepenent.  This same method of constructing
 * GUIs can then be shared by different OS-specific GUI factories that create
 * their own widgets.
 *
 * The files follow the convention:
 *    - \b my_class.h: Public API for class \c my_class.
 *    - \b my_class_friend.h: Friend API for class \c my_class.  Should only be
 *    included by classes that implement from \c my_class.
 *    - \b my_class.c: Implementation of \c my_class and includes private data
 *    not directly accessible by even friend classes.
 *
 * \section sec_usage Usage
 *
 * Assuming your c_intf_gen.py script is in the parent directory 
 * (if not, the \c GEN_SCRIPT variable in the \c Makefile can be modified), just
 * do:
 *
 * <tt>make all</tt>\n
 * <tt>./test_abs_factory</tt>
 *
 * You can edit \c test_abs_factory.c to try various things with this class
 * hierarchy.  Running <tt>make clean</tt> will remove the executable, object,
 * and generated files.
 *
 * @section sec_license GNU General Public License
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "win_factory.h"
#include "osx_factory.h"

/**
 * The factory method that builds a GUI using the given factory.
 *
 * @param gui_factory_h The factory to use for building the GUI.
 */
static void
generate_gui (gui_factory_handle gui_factory_h)
{
    button_handle button1 = NULL, button2 = NULL, button3 = NULL;

    if (NULL == gui_factory_h) {
        return;
    }

    /* 
     * The will create some buttons, call their paint functions, and then delete
     * them.  Use your imagination for much more complicated GUIs being built in
     * real life.
     */
    button1 = gui_factory_create_button(gui_factory_h);
    if (NULL == button1) {
        goto err_exit;
    }

    button2 = gui_factory_create_button(gui_factory_h);
    if (NULL == button2) {
        goto err_exit;
    }

    button3 = gui_factory_create_button(gui_factory_h);
    if (NULL == button3) {
        goto err_exit;
    }

    button_paint(button1);
    button_paint(button2);
    button_paint(button3);

    button_delete(button1);
    button_delete(button2);
    button_delete(button3);

    return;

err_exit:

    if (NULL != button1) {
        button_delete(button1);
    }

    if (NULL != button2) {
        button_delete(button2);
    }

    if (NULL != button3) {
        button_delete(button3);
    }
}

/**
 * Main function to test objects.
 */
int 
main (int argc, char *argv[])
{
    win_factory_handle win_factory_h;
    osx_factory_handle osx_factory_h;

    win_factory_h = win_factory_new1();
    if (NULL == win_factory_h) {
        return (1);
    }

    osx_factory_h = osx_factory_new1();
    if (NULL == osx_factory_h) {
        return (1);
    }

    printf("\n*** Generating Windows GUI ***\n\n");
    generate_gui(win_factory_cast_to_gui_factory(win_factory_h));
    printf("\n*** Generating OSX GUI ***\n\n");
    generate_gui(osx_factory_cast_to_gui_factory(osx_factory_h));

    win_factory_delete(win_factory_h);
    osx_factory_delete(osx_factory_h);

    printf("\n");

    return (0);
}
