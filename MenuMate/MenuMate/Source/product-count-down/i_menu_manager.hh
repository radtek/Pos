#ifndef __hh_i_menu_manager__
#define __hh_i_menu_manager__

#include <string_type.hh>
#include <key_types.hh>
#include <vector>
#include <set>

namespace item_management {

class i_item_definition;
class i_item_definition_factory;
class i_item_manager;
class i_size_definition;
class i_size_definition_factory;

class i_menu_manager {
public:
        /*
         * An attempt to ensure that those who implement
         * a new menu manager know that they MUST take a
         * item_manager at some point!
         */
        i_menu_manager(i_item_manager *item_manager);
        virtual ~i_menu_manager();

        virtual void add_menu(menu_key_type key) = 0;

        virtual void add_course(
            course_key_type course_key,
            menu_key_type menu_key) = 0;

        virtual void add_item_definition(
            menu_key_type menu_key,
            course_key_type course_key,
            i_item_definition *definition) = 0;

        virtual void add_item_definitions(
            menu_key_type menu_key,
            course_key_type course_key,
            std::vector<i_item_definition *> &definitions) = 0;

        virtual void remove_menu(menu_key_type key) = 0;

        virtual bool is_menu_loaded(menu_key_type key) const = 0;

        virtual std::set<menu_key_type> get_loaded_menus() const = 0;

        virtual const i_item_definition_factory &
            get_item_definition_factory() const = 0;
        virtual const i_size_definition_factory &
            get_size_definition_factory() const = 0;

private:
        i_menu_manager(const i_menu_manager &initializer);
        i_menu_manager &operator=(const i_menu_manager &rhs);
};

} /* item_management */

#endif /* ! __hh_i_menu_manager__ */

