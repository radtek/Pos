#include <memory>

#include <i_signalling_item_definition_factory.hh>
#include <signalling_item_manager.hh>
#include <spinlock.hh>

namespace item_management {

using utilities::locking::spinlock;
using utilities::locking::spinlock_reference;

signalling_item_manager::signalling_item_manager(
    const i_item_definition_factory *id_factory,
    const i_signalling_item_definition_factory *sid_factory,
    const i_size_definition_factory *sd_factory,
    const i_item_altered_signal_handler *ia_handler,
    const i_size_altered_signal_handler *sa_handler,
    const i_size_warning_reached_signal_handler *swr_handler)
    : item_manager(id_factory, sd_factory),
      item_definitions_(),
      sid_factory_(sid_factory),
      ia_handler_(ia_handler),
      sa_handler_(sa_handler),
      srw_handler_(swr_handler),
      manager_lock_(new spinlock())
{
}

signalling_item_manager::~signalling_item_manager()
{
        delete manager_lock_;
}

void
signalling_item_manager::add_item_definition(i_item_definition *definition)
{
        std::auto_ptr<i_item_definition> id(definition);
        std::auto_ptr<i_signalling_item_definition> sid;
        item_key_type key = id->get_key();

        sid.reset(
            sid_factory_->create(
                definition, ia_handler_, sa_handler_, srw_handler_));
        id.release();

        spinlock_reference hold_lock(*manager_lock_);

        // changed the way of inserting the definitions to the collection to include the already beign used item definitions from the orders table
        item_definitions_[key] = sid.get();

//        if (!item_definitions_.insert(sid_map::value_type(
//                                         key, sid.get())).second)
//                throw item_already_present_exception(key);

        sid.release();
}

void
signalling_item_manager::remove_item_definition(item_key_type key)
{
        sid_map::iterator i = item_definitions_.find(key);
        i_signalling_item_definition *sid;

        spinlock_reference hold_lock(*manager_lock_);

        if (i == item_definitions_.end())
                throw item_not_present_exception(key);

        sid = i->second;
        item_definitions_.erase(i);

        delete sid;
}

i_item_definition &
signalling_item_manager::get_item_definition(item_key_type key)
{
   return const_cast<i_item_definition &>(
     static_cast<
       const signalling_item_manager &>(*this).get_item_definition(key));
}

const i_item_definition &
signalling_item_manager::get_item_definition(item_key_type key)
const
{
        sid_map::const_iterator i;

        spinlock_reference hold_lock(*manager_lock_);

        if ((i = item_definitions_.find(key)) == item_definitions_.end())
                throw item_not_present_exception(key);

        return *i->second;
}

i_size_definition &
signalling_item_manager::get_size_definition(
    item_key_type item_key,
    size_key_type size_key)
{
   return const_cast<i_size_definition &>(
              static_cast<
                  const signalling_item_manager &>(
                    *this).get_size_definition(item_key, size_key));
}

const i_size_definition &
signalling_item_manager::get_size_definition(
    item_key_type item_key,
    size_key_type size_key)
const
{
        sid_map::const_iterator i;
        spinlock_reference hold_lock(*manager_lock_);

        if ((i = item_definitions_.find(item_key))
            == item_definitions_.end())
                throw item_not_present_exception(item_key);

        return i->second->get_size_definition(size_key);
}

void
signalling_item_manager::quietly_set_item_enabled(
    item_key_type key,
    bool enabled)
{
        sid_map::iterator i;

        spinlock_reference manager_lock(*manager_lock_);
        if ((i = item_definitions_.find(key)) == item_definitions_.end())
                throw item_not_present_exception(key);

        i->second->quietly_set_enabled(enabled);
}

void
signalling_item_manager::quietly_update_size_definition(
    item_key_type item_key,
    size_key_type size_key,
    double available_quantity,
    double default_quantity,
    bool enabled,
    double price,
    double special_price,
    double warning_quantity)
{
        sid_map::iterator i;
        spinlock_reference hold_lock(*manager_lock_);

        if ((i = item_definitions_.find(item_key))
            == item_definitions_.end())
                throw item_not_present_exception(item_key);

        i->second->quietly_update_size_definition(
            size_key,
            available_quantity,
            default_quantity,
            enabled,
            price,
            special_price,
            warning_quantity);
}

} /* item_management */

