#ifndef __hh_item_management_exceptions__
#define __hh_item_management_exceptions__

#include <stdexcept>

#include <string_type.hh>
#include <key_types.hh>

namespace item_management {

class item_management_exception : public std::exception {
public:
        item_management_exception() { };
        virtual ~item_management_exception() throw() { };

        const char *what() const throw() { return "No message."; }
};

class item_exception : public item_management_exception {
public:
        item_exception(item_key_type key)
            : key_(key) { };
        virtual ~item_exception() throw() { };

        item_key_type get_key() const { return key_; };

private:
        item_key_type key_;
};

class menu_exception : public item_management_exception {
public:
        menu_exception(menu_key_type key)
            : menu_key_(key) { };
        virtual ~menu_exception() throw() { };

        menu_key_type get_menu_key() const { return menu_key_; };

private:
        menu_key_type menu_key_;
};

class menu_already_present_exception : public menu_exception {
public:
        menu_already_present_exception(menu_key_type key)
            : menu_exception(key) { };
        ~menu_already_present_exception() throw() { };
};

class menu_not_present_exception : public menu_exception {
public:
        menu_not_present_exception(menu_key_type key)
            : menu_exception(key) { };
        ~menu_not_present_exception() throw() { };
};

class course_exception : public menu_exception {
public:
        course_exception(menu_key_type menu_key, course_key_type key)
            : menu_exception(menu_key),
              course_key_(key) { };
        virtual ~course_exception() throw() { };

        course_key_type get_course_key() const { return course_key_; };

private:
        course_key_type course_key_;
};

class course_not_present_exception : public course_exception {
public:
        course_not_present_exception(menu_key_type menu_key,
                                     course_key_type course_key)
            : course_exception(menu_key, course_key) { };
        ~course_not_present_exception() throw() { };
};

class course_already_present_exception : public course_exception {
public:
        course_already_present_exception(menu_key_type menu_key,
                                         course_key_type course_key)
            : course_exception(menu_key, course_key) { };
        ~course_already_present_exception() throw() { };
};

class item_already_present_exception : public item_exception {
public:
        item_already_present_exception(item_key_type key)
            : item_exception(key) { };
        ~item_already_present_exception() throw() { };
};

class item_not_present_exception : public item_exception {
public:
        item_not_present_exception(item_key_type key)
            : item_exception(key) { };
        ~item_not_present_exception() throw() { };
};

class no_sizes_exception : public item_exception {
public:
        no_sizes_exception(item_key_type key)
            : item_exception(key) { };
        ~no_sizes_exception() throw() { }
};

class size_not_present_exception : public item_exception {
public:
        size_not_present_exception(item_key_type item_key,
                                   size_key_type size_key)
            : item_exception(item_key),
              size_key_(size_key),
              size_name_(L"No name specified.") { };
        size_not_present_exception(item_key_type item_key,
                                   const string_type &size_name)
            : item_exception(item_key),
              size_key_(0),
              size_name_(size_name) { }
        ~size_not_present_exception() throw() { };

private:
        size_key_type size_key_;
        string_type size_name_;
};

} /* item_management */

#endif /* ! __hh_item_management_exceptions__ */

