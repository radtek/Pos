#include <i_item_definition.hh>
#include <i_item_manager.hh>
#include <spinlock.hh>

#include <menu_manager.hh>

namespace item_management {

using utilities::locking::spinlock;
using utilities::locking::spinlock_reference;

std::set<menu_key_type>
menu_manager::get_loaded_menus()
const
{
   std::set<menu_key_type> menus;

   for (menu_to_course_map::const_iterator i = menu_to_course_.begin();
        i != menu_to_course_.end(); ++i)
      menus.insert(i->first);

   return menus;
}

menu_manager::menu_manager(i_item_manager *item_manager)
    : i_menu_manager(item_manager),
      item_manager_(item_manager),
      menu_to_course_(),
      course_to_items_(),
      manager_lock_(new spinlock())
{
}

menu_manager::~menu_manager()
{
        delete manager_lock_;
}

void
menu_manager::add_menu(menu_key_type key)
{
        spinlock_reference hold_lock(*manager_lock_);

        std::pair<menu_to_course_map::iterator, bool> insertion_result =
            menu_to_course_.insert(
                menu_to_course_map::value_type(key, course_key_set()));

        if (!insertion_result.second)
                throw menu_already_present_exception(key);
}

void
menu_manager::add_course(
    course_key_type course_key,
    menu_key_type menu_key)
{
        spinlock_reference hold_lock(*manager_lock_);

        std::pair<course_to_item_map::iterator, bool> insertion_result;

        ensure_menu_present(menu_key);

        insertion_result =
            course_to_items_.insert(
                course_to_item_map::value_type(course_key, item_key_set()));

        if (!insertion_result.second)
                throw course_already_present_exception(menu_key, course_key);

        menu_to_course_[menu_key].insert(course_key);
}

void
menu_manager::add_item_definition(
    menu_key_type menu_key,
    course_key_type course_key,
    i_item_definition *definition)
{
        spinlock_reference hold_lock(*manager_lock_);

        ensure_menu_present(menu_key);
        ensure_course_present(menu_key, course_key);

        item_manager_->add_item_definition(definition);

        course_to_items_[course_key].insert(definition->get_key());
}

void
menu_manager::add_item_definitions(
    menu_key_type menu_key,
    course_key_type course_key,
    std::vector<i_item_definition *> &item_definitions)
{
        std::vector<i_item_definition *>::iterator i =
            item_definitions.begin();

        for ( ; i != item_definitions.end(); ++i)
                add_item_definition(menu_key, course_key, *i);
}

void
menu_manager::remove_menu(menu_key_type key)
{
   spinlock_reference hold_lock(*manager_lock_);

   menu_to_course_map::iterator i = menu_to_course_.find(key);
   const course_key_set *courses_in_menu;

   if (i == menu_to_course_.end())
      throw menu_not_present_exception(key);

   courses_in_menu = &i->second;
   for (course_key_set::const_iterator j = courses_in_menu->begin();
        j != courses_in_menu->end(); ++j)
      remove_course(*j);

   menu_to_course_.erase(i);
}

void
menu_manager::remove_course(course_key_type key)
{
   course_to_item_map::iterator i = course_to_items_.find(key);
   item_key_set &items_in_course = i->second;
   std::vector<item_key_type> items_to_remove;

   items_to_remove.reserve(items_in_course.size());

   for (item_key_set::iterator j = items_in_course.begin();
        j != items_in_course.end(); ++j)
      items_to_remove.push_back(*j);

   item_manager_->remove_item_definitions(items_to_remove);

   course_to_items_.erase(i);
}

bool
menu_manager::is_menu_loaded(menu_key_type key)
const
{
   return menu_to_course_.find(key) != menu_to_course_.end();
}

const i_item_definition_factory &
menu_manager::get_item_definition_factory()
const
{
        return item_manager_->get_item_definition_factory();
}

const i_size_definition_factory &
menu_manager::get_size_definition_factory()
const
{
        return item_manager_->get_size_definition_factory();
}

/*
 * These functions are not meant to do any locking.
 * They assume that the manager is locked on entry. When they throw an
 * exception, the lock will be released as part of spinlock's destructor.
 */
void
menu_manager::ensure_course_present(
    menu_key_type menu_key,
    course_key_type course_key)
const
{
        if (course_to_items_.find(course_key) == course_to_items_.end())
                throw course_not_present_exception(menu_key, course_key);
}

void
menu_manager::ensure_menu_present(menu_key_type key)
const
{
   if (!is_menu_loaded(key))
                throw menu_not_present_exception(key);
}

} /* item_management */

