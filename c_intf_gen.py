#!/usr/bin/env python
"""Generator for basic interfaces in C

Reads in a file with a description of the interface structure
and generates the C code to support the interfaces.  The interface functions
must still be implemented.

The description format is:

    INTERFACE employee
        FUNCTION set_name
            RETURN void
            INPUT char *name
        END FUNCTION
    END INTERFACE

    CLASS teacher
        IMPLEMENTS employee
        END IMPLEMENTS
    END CLASS

For each input parameter in an implementation's function, there
must be a separate "INPUT" statement.  A '\' character is used for multi-line
continuations.  A "delete" function is automatically created for all interfaces.

Most common C function inputs are handled, but there are a few that can't be
handled.  In particular, things like function pointers where the parameter name
is within parenthesis and paremeter lists follow.  Also, va_args as input
parameters would not work.  To get around the function pointer limitation, you
can use a typedef, e.g.:

typedef void
(*foo_fn)(uint32_t i, char *c);

void
my_func(foo_fn callback);

For INTERFACE, optionally you can have INCLUDE lines which will be #included.
E.g.:

    INTERFACE employee
        INCLUDE "my_file.h"
        INCLUDE <stdio.h>
        ...
    INTERFACE

Note that you must explicitly have the quotes or '<' and '>' characters
depending on what you want as the string will be added as-is after the
"#include" string.

Optionally, the following can be specified for comments in the generated files:

    AUTHOR
        NAME John Doe
        EMAIL john@doe.com
    END AUTHOR

    LICENSE
    <license text>
    END LICENSE

If these are not included, they will not appear in the comments of the generated
files.

Commented lines begin with any amount of whitespace and a '#' 
(everything after the '#' is ignored).  Lines with only whitespace are ignored.

"""
import argparse, re, sys, textwrap, os

# TODO: packaging this much better is future work

def usage (parser, exit_code=0):
    """Display the help text for the script and exit"""
    parser.print_help()
    sys.exit(exit_code)

class Function:
    """A function for an interface"""

    def __init__ (self, name):
        """Initialize with the function name"""
        self.name = name
        self.return_type = None
        self.inputs = []

    def __repr__ (self):
        return "{} (name={}, return_type={}, inputs={})".format(
                   self.__class__.__name__,
                   self.name, self.return_type, self.inputs)

    def add_input (self, input):
        """Add an input tuple or void to the function"""
        if ("void" in self.inputs):
            raise ValueError("""
                             Invalid input for function {} with void 
                             input: {}""".format(self.name, input))

        input = input.strip()
        if (input == "void" and self.get_num_inputs() > 0):
            raise ValueError("""
                             Invalid void input for function {}
                             existing parameters:
                             {}""".format(self.name, input))
        self.inputs.append(input)

    def set_return_type (self, return_type):
        """Set the return type for the function"""
        if (self.return_type is not None):
            raise ValueError("""
                             Invalid duplicate return type for function {}:
                             {}""".format(self.name, return_type))
        self.return_type = return_type

    def get_num_inputs (self):
        """Return the number of inputs for the function"""
        return len(self.inputs)

    def is_void_input (self):
        """Indicates whether the function takes a void input"""
        return ("void" in self.inputs)

class Interface:
    """An interface which consists of multiple functions declarations"""
    
    def __init__ (self, name):
        """Initialize the interface with the given name"""
        self.name = name
        self.functions = {}
        self.includes = []

    def __repr__ (self):
        return "{} (name={}, functions={}, includes={})".format(
                   self.__class__.__name__,
                   self.name, self.functions, self.includes)

    def add_function (self, function):
        """Add a function to the interface"""
        self.functions[function.name] = function

    def add_include (self, include):
        """Add a library to include to the interface"""
        self.includes.append(include)

class ClassObj:
    """Objects to store classes"""

    def __init__ (self, name):
        """Initialize the class with the given name"""
        self.name = name
        self.interfaces = []

    def __repr__ (self):
        return "{} (name={}, interfaces={})".format(
                   self.__class__.__name__,
                   self.name, self.interfaces)

    def add_interface (self, interface_name):
        """Add an interface list for the class. Duplicates are removed
           automatically."""
        self.interfaces.append(interface_name)
        self.interfaces = list(set(self.interfaces))

def get_c_indentifier (input_str):
    # Give it our best shot, won't get everything like function pointers
    # and va_args, but gets most common stuff.
    ret_val = ""
    p_c_id = re.compile(r'^.*\W([a-zA-Z]\w+)(?:\[\w*\])*\s*$')
    m = p_c_id.match(input_str)
    if (m is not None):
        ret_val = m.group(1)
    else:
        raise ValueError("""
                         Cannot parse identifier:
                         {}""".format(input_str))
    
    return ret_val


def get_log_lines (raw_data):
    """Generator to join all lines with the continuation character '\'
    
    From:
    http://code.activestate.com/recipes/66064-reading-lines-with-continuation-characters/
    """
    lines = []
    for i in raw_data:
        lines.append(i)
        if not i.endswith("\\"):
            yield "".join(lines)
            lines = []
    if len(lines) > 0:
        yield "".join(lines)

class ParsedData:
    """Stores all the data parsed from the file into appropriate data
       structures and objects."""

    def __init__ (self, intf_dict, class_dict, author=None, license=None):
        self.author = author
        self.license = license
        self.intf_dict = intf_dict
        self.class_dict = class_dict

class Author:
    """Stores author info."""

    def __init__ (self):
        self.name = None
        self.email = None

class License:
    """Stores license info."""

    def __init__ (self):
        self.license_lines = []

    def add_license_line (self, line):
        self.license_lines.append(line.rstrip("\n"))

    def get_license_str (self, prefix=""):
        return ("\n{}".format(prefix).join(self.license_lines))

class ParseError (Exception):
    """Exception for parsing errors"""

    def __init__ (self, value):
        self.value = value

    def __str__ (self):
        return textwrap.dedent(str(self.value))

def get_interface_objects (lines):
    """Get the dicts of interface and class objects from the file"""

    if_dict = {}
    class_dict = {}
    cur_if_obj = None
    cur_fn_obj = None
    cur_class_obj = None
    cur_author_obj = None
    cur_license_obj = None
    cur_impl_name = None
    author_obj = None
    license_obj = None
    in_block = False

    p_if_start = re.compile(r'\s*INTERFACE\s+(\S+)\s*$')
    p_if_end = re.compile(r'\s*END INTERFACE\s*$')
    p_fn_start = re.compile(r'\s*FUNCTION\s+(\S+)\s*$')
    p_fn_end = re.compile(r'\s*END FUNCTION\s*$')
    p_ret = re.compile(r'\s*RETURN\s+(\S+)\s*$')
    #p_input = re.compile(r'\s*INPUT\s+(\S+)(?:\s+(\S+))?\s*$')
    p_input = re.compile(r'\s*INPUT\s+(\S+.*)$')
    p_include = re.compile(r'\s*INCLUDE\s+(\S+)\s*$')
    #p_implementor = re.compile(r'\s*(\S+)\s+IMPLEMENTS\s+(\S+.*)')
    p_class_start = re.compile(r'\s*CLASS\s+(\S+)\s*$')
    p_class_end = re.compile(r'\s*END CLASS\s*$')
    p_implements_start = re.compile(r'\s*IMPLEMENTS\s+(\S+)\s*$')
    p_implements_end = re.compile(r'\s*END IMPLEMENTS\s*$')
    p_author_start = re.compile(r'\s*AUTHOR\s*$')
    p_author_end = re.compile(r'\s*END AUTHOR\s*$')
    p_license_start = re.compile(r'\s*LICENSE\s*$')
    p_license_end = re.compile(r'\s*END LICENSE\s*$')
    p_name = re.compile(r'\s*NAME\s+(\S+.*)$')
    p_email = re.compile(r'\s*EMAIL\s+(\S+.*)$')
    p_comment = re.compile(r'\s*#.*$')
    p_blank = re.compile(r'^\s*$')

    for line in lines:

        m = p_comment.match(line)
        if (m is not None):
            continue;

        m = p_blank.match(line)
        if (m is not None):
            continue;

        in_block = (cur_if_obj is not None or cur_fn_obj is not None or
                    cur_class_obj is not None or
                    cur_author_obj is not None or 
                    cur_license_obj is not None)

        m = p_if_start.match(line)
        if (m is not None):
            if (in_block):
                raise ParseError("""
                                 Invalid interface statement:
                                 {}""".format(line))
            if (m.group(1) in if_dict.viewkeys()):
                raise ParseError("""
                                 Duplicate interface statement:
                                 {}""".format(line))
            cur_if_obj = Interface(m.group(1))
            if_dict[cur_if_obj.name] = cur_if_obj
            continue

        m = p_if_end.match(line)
        if (m is not None):
            if (cur_if_obj is None):
                raise ParseError("""
                                 Invalid interface statement:
                                 {}""".format(line))
            # Add a delete function to all interfaces
            fn = Function("delete")
            fn.return_type = "void"
            fn.add_input("void")
            cur_if_obj.add_function(fn)
            cur_if_obj = None
            continue

        m = p_include.match(line)
        if (m is not None):
            if (cur_if_obj is None):
                raise ParseError("""
                                 Invalid interface statement:
                                 {}""".format(line))
            cur_if_obj.add_include(m.group(1))
            continue

        m = p_fn_start.match(line)
        if (m is not None):
            if (cur_fn_obj is not None or cur_if_obj is None):
                raise ParseError("""
                                 Invalid function statement:
                                 {}""".format(line))
            cur_fn_obj = Function(m.group(1))
            cur_if_obj.add_function(cur_fn_obj)
            continue

        m = p_fn_end.match(line)
        if (m is not None):
            if (cur_fn_obj is None):
                raise ParseError("""
                                 Invalid function statement:
                                 {}""".format(line))
            cur_fn_obj = None
            continue

        m = p_ret.match(line)
        if (m is not None):
            if (cur_fn_obj is None):
                raise ParseError("""
                                 Invalid function statement:
                                 {}""".format(line))
            try:
                cur_fn_obj.set_return_type(m.group(1))
            except Exception as e:
                raise ParseError("""
                                 Invalid function statement:
                                 {}
                                 {}""".format(e, line))
            continue

        m = p_input.match(line)
        if (m is not None):
            if (cur_fn_obj is None):
                raise ParseError("""
                                 Invalid function statement:
                                 {}""".format(line))
            cur_input = m.group(1)
            try:
                cur_fn_obj.add_input(cur_input)
            except Exception as e:
                raise ParseError("""
                                 Invalid function statement:
                                 {}
                                 {}""".format(e, line))
            continue

        m = p_class_start.match(line)
        if (m is not None):
            if (in_block):
                raise ParseError("""
                                 Invalid class statement:
                                 {}""".format(line))
            if (m.group(1) in class_dict.viewkeys()):
                raise ParseError("""
                                 Duplicate class statement:
                                 {}""".format(line))
            cur_class_obj = ClassObj(m.group(1))
            class_dict[cur_class_obj.name] = cur_class_obj
            continue

        m = p_class_end.match(line)
        if (m is not None):
            if (cur_class_obj is None):
                raise ParseError("""
                                 Invalid class statement:
                                 {}""".format(line))
            cur_class_obj = None
            continue

        m = p_implements_start.match(line)
        if (m is not None):
            if (cur_impl_name is not None or cur_class_obj is None):
                raise ParseError("""
                                 Invalid implements statement:
                                 {}""".format(line))
            cur_impl_name = m.group(1)
            cur_class_obj.add_interface(cur_impl_name)
            continue

        m = p_implements_end.match(line)
        if (m is not None):
            if (cur_impl_name is None):
                raise ParseError("""
                                 Invalid implements statement:
                                 {}""".format(line))
            cur_impl_name = None
            continue

        m = p_author_start.match(line)
        if (m is not None):
            if (in_block):
                raise ParseError("""
                                 Invalid author statement:
                                 {}""".format(line))
            if (author_obj is not None):
                raise ParseError("""
                                 Only one author block is expected:
                                 {}""".format(line))
            author_obj = Author()
            cur_author_obj = author_obj
            continue

        m = p_author_end.match(line)
        if (m is not None):
            if (cur_author_obj is None):
                raise ParseError("""
                                 Invalid author statement:
                                 {}""".format(line))
            cur_author_obj = None
            continue

        m = p_license_start.match(line)
        if (m is not None):
            if (in_block):
                raise ParseError("""
                                 Invalid license statement:
                                 {}""".format(line))
            if (license_obj is not None):
                raise ParseError("""
                                 Only one license block is expected:
                                 {}""".format(line))
            license_obj = License()
            cur_license_obj = license_obj
            continue

        m = p_license_end.match(line)
        if (m is not None):
            if (cur_license_obj is None):
                raise ParseError("""
                                 Invalid license statement:
                                 {}""".format(line))
            cur_license_obj = None
            continue

        m = p_name.match(line)
        if (m is not None):
            if (cur_author_obj is None):
                raise ParseError("""
                                 Invalid name statement:
                                 {}""".format(line))
            cur_author_obj.name = m.group(1)
            continue

        m = p_email.match(line)
        if (m is not None):
            if (cur_author_obj is None):
                raise ParseError("""
                                 Invalid email statement:
                                 {}""".format(line))
            cur_author_obj.email = m.group(1)
            continue

        if (cur_license_obj is not None):
            cur_license_obj.add_license_line(line)
            continue

        # Unknown line format
        raise ParseError("""
                         Unknown format:
                         {}""".format(line))

    # Make sure all the implemented interfaces are defined and convert
    # the list of strings to a list of Interface objects.
    undefined_ifs = [] 
    for key in class_dict.viewkeys():
        class_ifs = []
        for intf in class_dict[key].interfaces:
            if intf not in if_dict.viewkeys():
                undefined_ifs.append(intf)
            else:
                class_ifs.append(if_dict[intf])
        class_dict[key].interfaces = class_ifs

    if (len(undefined_ifs) != 0):
        raise ParseError("""
                         Non-existent interfaces specified:
                         {}""".format(undefined_ifs))

    return ParsedData(if_dict, class_dict, author_obj, license_obj)

author_template_str = "@author{name}{email}"
license_template_str ="@section LICENSE\n{license}"

header_template_str = """\
/**
 * @file
 * {author_str}{license_str}
 * @section DESCRIPTION
 *
 * {description}
 */
"""

def write_header (file, desc_str, author=None, license=None):

    author_str = ""
    if (author is not None):
        name_str = ""
        if (author.name is not None):
            name_str = " {}".format(author.name)

        email_str = ""
        if (author.email is not None):
            email_str = " <{}>".format(author.email)

        author_str = author_template_str.format(name=name_str, email=email_str)
        author_str = "\n * " + author_str + "\n * "

    license_str = ""
    if (license is not None):
        license_str = \
            license_template_str.format(license=" * " + \
                                        license.get_license_str(" * "))
        license_str = "\n * " + license_str + "\n * "

    file.write("/* THIS IS A GENERATED FILE, DO NOT EDIT!!! */\n")
    header_str = header_template_str.format(author_str=author_str,
                                            license_str=license_str,
                                            description="\n * ".join(
                                                desc_str.splitlines()))

    file.write(header_str)

def generate_interface_files (intf, parser_args, author=None, license=None):

    public_header_file_name = "{}/{}{}.h".format(parser_args.output_dir,
                                                 intf.name, 
                                                 parser_args.gen_file_suffix)
    friend_header_file_name = \
        "{}/{}_friend{}.h".format(parser_args.output_dir, intf.name,
                                  parser_args.gen_file_suffix)
    c_file_name = "{}/{}{}.c".format(parser_args.output_dir, intf.name,
                                     parser_args.gen_file_suffix)

    try:
        f = open(public_header_file_name, "w")
    except IOError:
        print "ERROR: Could not open {} for writing".format(
            public_header_file_name)
        sys.exit(1)

    # Write the public interface file
    desc_str = "This is the public interface for " + \
        "the {} class.".format(intf.name)
    write_header(f, desc_str, author, license)
    f.write("""\
#ifndef __{0}_H__
#define __{0}_H__

""".format(re.sub(".h$", "", 
        os.path.basename(public_header_file_name)).upper()))
    f.write("#include <stdlib.h>\n")
    f.write("#include <stdbool.h>\n")
    f.write("#include <stdint.h>\n")
    f.write("#include <stddef.h>\n")
    for include in intf.includes:
        f.write("#include {}\n".format(include))
    f.write("\n")
    f.write("/** Opaque pointer to reference instances of this class */\n")
    f.write("typedef struct {0}_st_ *{0}_handle;\n\n".format(intf.name))
    f.write("/* APIs below are documented in their implementation file */\n\n")
    for fn in intf.functions.viewvalues():
        f.write("extern {}\n".format(fn.return_type))
        real_name = "{}_{}".format(intf.name, fn.name)
        f.write("{1}({0}_handle {0}_h".format(intf.name, real_name))
        if (not fn.is_void_input):
            for input in fn.inputs:
                f.write(",\n{}{} {}".format(" " * (len(real_name) + 1), 
                                            input[0], input[1]))
        f.write(");\n\n")
    f.write("#endif\n")
    f.close()

    try:
        f = open(friend_header_file_name, "w")
    except IOError:
        print "ERROR: Could not open {} for writing".format(
            friend_header_file_name)
        sys.exit(1)

    # Write the friend interface file
    desc_str = "This is the friend interface for the " + \
               "{} class.\n".format(intf.name) + \
               "It should only be included by implementors of the\n" + \
               "{} interface.".format(intf.name)
    write_header(f, desc_str, author, license)
    f.write("""\
#ifndef __{0}_H__
#define __{0}_H__

""".format(re.sub(".h$", "", 
        os.path.basename(friend_header_file_name)).upper()))
    f.write("#include \"{}\"\n\n".format(
        os.path.basename(public_header_file_name)))
    f.write("/** Opaque pointer to reference private data for the class */\n")
    f.write("typedef struct {0}_private_st_ *{0}_private_handle;\n\n".format(
                intf.name))
    f.write("/** Friend accessible data for this class */\n" + \
            "typedef struct {}_st_ {{\n".format(intf.name) + \
            "    /** Reference to private data */\n" + \
            "    {}_private_handle private_h;\n".format(intf.name) + \
            "}} {}_st;\n\n".format(intf.name))
    for fn in intf.functions.viewvalues():
        f.write("/**\n" + \
                " * Virtual function declaration.\n" + \
                " */\n")
        f.write("typedef {}\n".format(fn.return_type))
        real_name = "(*{}_{}_fn)".format(intf.name, fn.name)
        f.write("{1}({0}_handle {0}_h".format(intf.name, real_name))
        if (not fn.is_void_input):
            for input in fn.inputs:
                f.write(",\n{}{} {}".format(" " * (len(real_name) + 1), 
                                            input[0], input[1]))
        f.write(");\n\n")

    f.write("/**\n" + \
            " * The virtual table to be specified by friend classes.\n" + \
            " *\n" + \
            " * @see {}_set_vtable()\n".format(intf.name) + \
            " */\n");
    f.write("typedef struct {}_vtable_st_ {{\n".format(intf.name))
    for fn in intf.functions.viewvalues():
        f.write("    /** Virtual function */\n" + \
                "    {0}_{1}_fn {1}_fn;\n".format(intf.name, fn.name))
    f.write("}} {}_vtable_st;\n\n".format(intf.name))
    f.write("/* APIs below are documented in their implementation file */\n\n")
    set_vtable_fn_name = "{}_set_vtable".format(intf.name)
    f.write("extern bool\n" + \
            "{0}({1}_handle {1}_h,\n".format(set_vtable_fn_name, intf.name) + \
            "{}{}_vtable_st *vtable);\n\n".format(
                " " * (len(set_vtable_fn_name) + 1), intf.name))
    f.write("extern void\n" + \
            "{0}_friend_delete({0}_handle {0}_h);\n\n".format(intf.name))
    f.write("extern bool\n" + \
            "{0}_init({0}_handle {0}_h);\n\n".format(intf.name))
    f.write("#endif\n")
    f.close()

    try:
        f = open(c_file_name, "w")
    except IOError:
        print "ERROR: Could not open {} for writing".format(c_file_name)
        sys.exit(1)

    # Write the implementation file
    desc_str = "This is the implementation of the " + \
               "{} interface.".format(intf.name)
    write_header(f, desc_str, author, license)
    f.write("#include <assert.h>\n")
    f.write("#include \"{}\"\n\n".format(
        os.path.basename(friend_header_file_name)))
    f.write("/**\n" + \
            " * Private variables which cannot be directly accessed by\n" + \
            " * any other class including children.\n" + \
            " */\n")
    f.write("typedef struct {}_private_st_ {{\n".format(intf.name) + \
            "    /** Virtual function table */\n" + \
            "    const {}_vtable_st *vtable;\n".format(intf.name) + \
            "}} {}_private_st;\n\n".format(intf.name))

    f.write("""\
/**
 * The internal function to delete a {0} object.  Upon return, the
 * object is not longer valid.
 *
 * @param {0}_h The object.  If NULL, then this function is a no-op.
 * @param free_{0}_h Indicates whether the base object should be freed
 * or not.
 * @see {0}_delete()
 * @see {0}_friend_delete()
 * @see {0}_private_delete()
 */
static void
{0}_delete_internal ({0}_handle {0}_h, 
    bool free_{0}_h)
{{
    if (NULL == {0}_h) {{
        return;
    }}

    if (NULL != {0}_h->private_h) {{
        free({0}_h->private_h);
        {0}_h->private_h = NULL;
    }}

    if (free_{0}_h) {{
        free({0}_h);
    }}
}}

/**
 * Allow a friend class to delete the {0} object.  It is assumed that
 * the friend class is managing the memory for the {0} object and, thus,
 * the object will not be freed.  However, members within the {0} object
 * may be freed.  This does not call the virtual function table version of
 * delete, but rather the delete specifically for type {0}.
 *
 * @param {0}_h The object.  If NULL, then this function is a no-op.
 * @see {0}_delete()
 */
void
{0}_friend_delete ({0}_handle {0}_h)
{{
    {0}_delete_internal({0}_h, false);
}}

""".format(intf.name))

    for fn in intf.functions.viewvalues():
        f.write("""\
/**
 * {0} from {1}.
 *
 * @param {1}_h The object
""".format(fn.name, intf.name))

        if (not fn.is_void_input):
            for input in fn.inputs:
                f.write(" * @param {} Input parameter\n".format(
                    get_c_indentifier(intput)))
        f.write(" * @return {}\n".format(fn.return_type) + \
                " */\n")

        f.write("{}\n".format(fn.return_type))
        real_name = "{}_{}".format(intf.name, fn.name)
        f.write("{1} ({0}_handle {0}_h".format(intf.name, real_name))
        if (not fn.is_void_input):
            for input in fn.inputs:
                f.write(",\n{}{} {}".format(" " * (len(real_name) + 2), 
                                            input[0], input[1]))
        f.write(")\n" + \
                "{\n")
        f.write("""\
    assert((NULL != {0}_h) &&
           (NULL != {0}_h->private_h) &&
           (NULL != {0}_h->private_h->vtable) &&
           (NULL != {0}_h->private_h->vtable->{1}_fn));

""".format(intf.name, fn.name))
        # Get input parameters for function call
        f.write("    return ({0}_h->private_h->vtable->{1}_fn({0}_h".format(
                    intf.name, fn.name))
        if (not fn.is_void_input):
            for input in fn.inputs:
                f.write(",\n{}".format(get_c_indentifier(input)))
        f.write("));\n")
        f.write("}\n\n")

    f.write("""\
/**
 * The virtual function table used for objects of type {0}.  As this is
 * an interface, all functions should be NULL.
 */
static const {0}_vtable_st {0}_vtable = {{
""".format(intf.name))
    f.write(",\n".join("    NULL" for fn in intf.functions.viewvalues()))
    f.write("\n};\n\n")

    f.write("""\
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
{0}_inherit_vtable (const {0}_vtable_st *parent_vtable,
    {0}_vtable_st *child_vtable,
    bool do_null_check)
{{
    if ((NULL == parent_vtable) || (NULL == child_vtable)) {{
        return (false);
    }}

""".format(intf.name))

    for fn in intf.functions.viewvalues():
        f.write("""\
    if (NULL == child_vtable->{0}_fn) {{
        child_vtable->{0}_fn = parent_vtable->{0}_fn;
        if (do_null_check && (NULL == child_vtable->{0}_fn)) {{
            return (false);
        }}
    }}

""".format(fn.name))
    f.write("    return (true);\n" + \
            "}\n\n")

    f.write("""\
/**
 * This is a function used by implementing classes to set the virtual table
 * according with their methods.
 *
 * @param {0}_h The object
 * @param vtable The virtual table specification for the implementing class.  If
 * any function pointer is NULL, an error is returned.
 * @return TRUE on success, FALSE otherwise
 */
bool
{0}_set_vtable ({0}_handle {0}_h, 
    {0}_vtable_st *vtable)
{{
    bool rc;

    if (((NULL == {0}_h) || (NULL == vtable) ||
         (NULL == {0}_h->private_h))) {{
        return (false);
    }}
    
    rc = {0}_inherit_vtable(&{0}_vtable, vtable, true);

    if (rc) {{
        {0}_h->private_h->vtable = vtable;
    }}

    return (rc);
}}

/**
 * Allows a friend class to initialize their inner {0} object.  Must be
 * called before the {0} object is used.  If an error is returned, any
 * clean-up was handled internally and there is no need to call a delete
 * function.
 *
 * @param {0}_h The object
 * @return TRUE on success, FALSE otherwise
 * @see {0}_delete()
 * @see {0}_friend_delete()
 */
bool
{0}_init ({0}_handle {0}_h)
{{
    if (NULL == {0}_h) {{
        return (false);
    }}

    {0}_h->private_h = calloc(1, sizeof(*{0}_h->private_h));
    if (NULL == {0}_h->private_h) {{
        goto err_exit;
    }}

    {0}_h->private_h->vtable = NULL;

    return (true);

err_exit:

    if (NULL != {0}_h->private_h) {{
        free({0}_h->private_h);
        {0}_h->private_h = NULL;
    }}

    return (false);
}}
""".format(intf.name))

    f.close()

def generate_class_files (class_obj, parser_args, author=None, license=None):

    header_file_name = "{}/{}{}.h".format(parser_args.output_dir,
                                          class_obj.name,
                                          parser_args.gen_file_suffix)
    c_file_name = "{}/{}{}.c".format(parser_args.output_dir, class_obj.name,
                                     parser_args.gen_file_suffix)

    try:
        f = open(header_file_name, "w")
    except IOError:
        print "ERROR: Could not open {} for writing".format(
            header_file_name)
        sys.exit(1)

    # Write the public interface file
    desc_str = "This includes the APIs for casting to interfaces the\n" + \
        "{} class implements and its opaque handle.\n".format(class_obj.name) + \
        "This file should be included in the\n" + \
        "public header file for the {} class.".format(class_obj.name)
    write_header(f, desc_str, author, license)

    f.write("""\
#ifndef __{0}_H__
#define __{0}_H__

""".format(re.sub(".h$", "", os.path.basename(header_file_name)).upper()))

    for intf in class_obj.interfaces:
        f.write("#include \"{}{}.h\"\n".format(intf.name,
                                               parser_args.gen_file_suffix))
    f.write("\n")

    f.write("""\
/** Opaque pointer to reference instances of this class */
typedef struct {0}_st_ *{0}_handle;

/* APIs below are documented in their implementation file */

extern void
{0}_delete({0}_handle {0}_h);

""".format(class_obj.name))

    for intf in class_obj.interfaces:
        f.write("""\
extern {1}_handle
{0}_cast_to_{1}({0}_handle {0}_h);

""".format(class_obj.name, intf.name))

    f.write("#endif\n")
    f.close()

    try:
        f = open(c_file_name, "w")
    except IOError:
        print "ERROR: Could not open {} for writing".format(
            c_file_name)
        sys.exit(1)

    # Write the public interface file
    desc_str = "This implements the interface related portion of the\n" + \
        "{} class.\n".format(class_obj.name) + \
        "This file should be included in the " + \
        "{} implementation file\n".format(class_obj.name) + \
        "Yes, including a C file is bizarre, " + \
        "but that's how it works here."
    write_header(f, desc_str, author, license)

    f.write("""\

/* 
 * See below for forward declarations that must be defined manually in the
 * implementing C file.
 */

""")


    f.write("#include \"{}\"\n".format(os.path.basename(header_file_name)))
    for intf in class_obj.interfaces:
        f.write("#include \"{}_friend{}.h\"\n".format(intf.name, 
                    parser_args.gen_file_suffix))
    f.write("\n")

    f.write("""\
/* Forward declarations */
/* Begin structs that must be defined manually. */

/** 
 * Forward pointer to reference non-interface data for the class.
 * This must be defined manually.
 */
typedef struct {0}_data_st_ *{0}_data_handle;

/* End structs that must be defined manually. */

""".format(class_obj.name))

    f.write("""\
/* Forward declarations */
/* Begin functions that must be defined manually. */

static void
{0}_data_delete({0}_data_handle *{0}_data_h);

static bool
{0}_data_create({0}_data_handle *{0}_data_h, void *context);

""".format(class_obj.name))

    for intf in class_obj.interfaces:
        for fn in intf.functions.viewvalues():
            if (fn.name == "delete"): continue
            f.write("""\
static {0}
{1}_{2}_{3}({2}_handle {2}_h""".format(fn.return_type, class_obj.name,
    intf.name, fn.name))
            if (not fn.is_void_input):
                for input in fn.inputs:
                    f.write(",\n    {}".format(input))
            f.write(");\n\n")

    f.write("""\
/* End functions that must be defined manually. */

""")

    f.write("/** Private data for this class */\n" + \
            "typedef struct {}_st_ {{\n".format(class_obj.name))
    for intf in class_obj.interfaces:
        f.write("""\
    /** {0} reference */
    {0}_st {0};
""".format(intf.name))
    f.write("""\
    /** Data for this class */
    {0}_data_handle {0}_data_h;
}} {0}_st;

""".format(class_obj.name))

    f.write("""\
/*
 * This is C, we need explicit casts to each of an object's parent classes.
 */

""")

    for intf in class_obj.interfaces:
        f.write("""\
/**
 * Cast the {1} object to {0}.
 *
 * @param {1}_h The {1} object
 * @return The {0} object
 */
static {0}_handle
{1}_cast_to_{0} ({1}_handle {1}_h)
{{
    {0}_handle {0}_h = NULL;

    if (NULL != {1}_h) {{
        {0}_h = ({0}_handle) ((uint8_t *) {1}_h -
            offsetof({0}_st, {1}));
    }}

    return ({0}_h);
}}

/**
 * Cast the {0} object to {1}.
 *
 * @param {0}_h The {0} object
 * @return The {1} object
 */
{1}_handle
{0}_cast_to_{1} ({0}_handle {0}_h)
{{
    {1}_handle {1}_h = NULL;

    if (NULL != {0}_h) {{
        {1}_h = &({0}_h->{1});
    }}

    return ({1}_h);
}}

 """.format(class_obj.name, intf.name))

    f.write("""\
/**
 * The function to delete a {0} object.  Upon return, the
 * object is not longer valid and the pointer is set to NULL.
 *
 * @param {0}_h A pointer to the object.  If NULL, then this function 
 * is a no-op.
 */
void
{0}_delete ({0}_handle {0}_h)
{{
    if (NULL == {0}_h) {{
        return;
    }}

    {0}_data_delete(&({0}_h->{0}_data_h));

""".format(class_obj.name))

    for intf in class_obj.interfaces:
        f.write("    {1}_friend_delete(&({0}_h->{1}));\n\n".format(
                    class_obj.name, intf.name))

    f.write("""\
    free({0}_h);
}}

""".format(class_obj.name))

    for intf in class_obj.interfaces:
        f.write("""\
/**
 * Wrapper for to call common function.
 *
 * @param {1}_h The object
 */
static void
{0}_{1}_delete ({1}_handle {1}_h)
{{
    if (NULL == {1}_h) {{
        return;
    }}

    {0}_delete({1}_cast_to_{0}({1}_h));
}}

""".format(class_obj.name, intf.name))

    for intf in class_obj.interfaces:
        f.write("""\
/**
 * The virtual function table for {0} interface.
 */
static {0}_vtable_st {0}_vtable = {{
""".format(intf.name))
        fn_names = []
        for fn in intf.functions.viewvalues():
            fn_names.append("    {}_{}_{}".format(class_obj.name, intf.name,
                                                  fn.name))
        f.write(",\n".join(fn_names) + "\n" + \
                "};\n\n")

    f.write("""\
/**
 * Initialize the {0} objects.
 *
 * @param {0}_h The object
 * @param context An opaque context passed to {0}_data_create
 * @return TRUE on success, FALSE otherwise
 */
static bool
{0}_init ({0}_handle {0}_h, void *context)
{{
    bool rc = false;
""".format(class_obj.name))

    for intf in class_obj.interfaces:
        f.write("""\
    bool {}_initialized = false;
""".format(intf.name))

    f.write("""\
    bool {0}_data_created = false;

    if (NULL == {0}_h) {{
        return (false);
    }}

""".format(class_obj.name))

    for intf in class_obj.interfaces:
        f.write("""\
    rc = {1}_init(&({0}_h->{1}));
    if (!rc) {{
        goto err_exit;
    }}
    {1}_initialized = true;

    rc = {1}_set_vtable(&({0}_h->{1}),
             &{1}_vtable);
    if (!rc) {{
        goto err_exit;
    }}

""".format(class_obj.name, intf.name))

    f.write("""\
    rc = {0}_data_create(&({0}_h->{0}_data_h), context);
    if (!rc) {{
        goto err_exit;
    }}
    {0}_data_created = true;

    return (true);

err_exit:

    if ({0}_data_created) {{
        {0}_data_delete(&({0}_h->{0}_data_h));
    }}

""".format(class_obj.name))

    for intf in class_obj.interfaces:
        f.write("""\
    if ({1}_initialized) {{
        {1}_friend_delete(&({0}_h->{1}));
    }}

""".format(class_obj.name, intf.name))

    f.write("""\
    return (rc);
}
""")

    f.close()

parser = argparse.ArgumentParser(description="""Generate basic infterfaces for
                                 C.""")

parser.add_argument("desc_file_name", metavar="description_file",
                    help="""The file containing the interface specification""")
parser.add_argument("-o", "--output-dir", dest="output_dir",
                    metavar="output directory",
                    default=".",
                    help="""The output directory for the generated files""")
parser.add_argument("-s", "--gen-file-suffix", dest="gen_file_suffix",
                    metavar="suffix for generated files",
                    default="_gen",
                    help="The suffix used for generated files.  E.g.: " + \
                         "<interface name><suffix>.c")

args = parser.parse_args()

try:
    desc_file = open(args.desc_file_name, "r")
except IOError:
    print "ERROR: Could not open {}".format(args.desc_file_name)
    usage(parser)

try:
    parsed_data = get_interface_objects(get_log_lines(desc_file))
except ParseError as e:
    print "ERROR: {}".format(e)
    sys.exit(1)
finally:
    desc_file.close()

for val in parsed_data.intf_dict.viewvalues():
    generate_interface_files(val, args, parsed_data.author, parsed_data.license)

for val in parsed_data.class_dict.viewvalues():
    generate_class_files(val, args, parsed_data.author, parsed_data.license)
