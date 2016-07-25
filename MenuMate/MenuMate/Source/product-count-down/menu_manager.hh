#ifndef __hh_menu_manager__
#define __hh_menu_manager__

#include <map>

#include <i_menu_manager.hh>

namespace utilities {
namespace locking {

class i_spinlock;

} /* locking */
} /* utilities */

namespace item_management {

class menu_manager : public i_menu_manager {
public:
        menu_manager(i_item_manager *item_manager);
        ~menu_manager();

        void add_menu(menu_key_type key);

        void add_course(course_key_type course_key,
                        menu_key_type menu_key);

        void add_item_definition(menu_key_type menu,
                                 course_key_type course,
                                 i_item_definition *item);

        void add_item_definitions(menu_key_type menu,
                                  course_key_type course,
                                  std::vector<i_item_definition *> &items);

        void remove_menu(menu_key_type key);

        bool is_menu_loaded(menu_key_type key) const;

        const i_item_definition_factory &
            get_item_definition_factory() const;
        const i_size_definition_factory &
            get_size_definition_factory() const;

        std::set<menu_key_type> get_loaded_menus() const;

private:
        typedef std::set<course_key_type> course_key_set;
        typedef std::set<item_key_type> item_key_set;

        typedef std::map<course_key_type, item_key_set> course_to_item_map;
        typedef std::map<menu_key_type, course_key_set> menu_to_course_map;

        menu_manager(const menu_manager &initializer);
        menu_manager &operator=(const menu_manager &rhs);

        void ensure_course_present(menu_key_type menu_key,
                                   course_key_type course_key) const;
        void ensure_menu_present(menu_key_type key) const;

        void remove_course(course_key_type key);

        i_item_manager *item_manager_;

        menu_to_course_map menu_to_course_;
        course_to_item_map course_to_items_;

        utilities::locking::i_spinlock *manager_lock_;
};

} /* item_management */

#endif /* ! __hh_menu_manager__ */

