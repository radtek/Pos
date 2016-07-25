#include <cassert>
#include <cmath>

#include <i_size_definition.hh>
#include <i_size_definition_factory.hh>
#include <item_management_exceptions.hh>
#include <spinlock.hh>
#include <utilities.hh>

#include <item_definition.hh>

namespace item_management {

using utilities::set_and_return_original_value;
using utilities::locking::spinlock;
using utilities::locking::spinlock_reference;

item_definition::item_definition()
    : item_definition_factory_(0x0),
      key_(0),
      enabled_(false),
      decorated_name_(),
      name_(),
      name_map_(),
      sizes_(),
      decorated_name_lock_(0x0),
      enable_lock_(0x0)
{
}

item_definition::item_definition(const i_item_definition &initializer)
    : i_item_definition(),
      item_definition_factory_(&initializer.get_item_definition_factory()),
      key_(initializer.get_key()),
      enabled_(initializer.get_enabled()),
      decorated_name_(initializer.get_decorated_name()),
      name_(initializer.get_name()),
      name_map_(),
      sizes_(),
      decorated_name_lock_(new spinlock()),
      enable_lock_(new spinlock())
{
        initialize_from(initializer);
}

item_definition::item_definition(const item_definition &initializer)
    : i_item_definition(),
      item_definition_factory_(initializer.item_definition_factory_),
      key_(initializer.key_),
      enabled_(initializer.enabled_),
      decorated_name_(initializer.decorated_name_),
      name_(initializer.name_),
      name_map_(),
      sizes_(),
      decorated_name_lock_(new spinlock()),
      enable_lock_(new spinlock())
{
        initialize_from(initializer);
}

item_definition::item_definition(
    const i_item_definition_factory *item_definition_factory,
    const string_type &name,
    std::vector<i_size_definition *> &size_definitions,
    item_key_type key)
    : item_definition_factory_(item_definition_factory),
      key_(key),
      enabled_(false),
      decorated_name_(),
      name_(name),
      name_map_(),
      sizes_(),
      decorated_name_lock_(new spinlock()),
      enable_lock_(new spinlock())
{
        std::vector<i_size_definition *>::iterator i = size_definitions.begin();

        for ( ; i != size_definitions.end(); ++i)
        {
                sizes_[(*i)->get_key()] = *i;
                name_map_[(*i)->get_name()] = *i;
        }

        size_definitions.clear();

        regenerate_decorated_name();
        regenerate_enabled_state();
}

item_definition::~item_definition()
{
        release_held_sizes();

        delete decorated_name_lock_;
        delete enable_lock_;
}

item_definition &
item_definition::operator=(const i_item_definition &rhs)
{
        if (&rhs == this)
                return *this;
        else {
                spinlock_reference hold_enabled(*enable_lock_);
                spinlock_reference hold_decorated(*decorated_name_lock_);

                release_held_sizes();
                initialize_from(rhs);

                return *this;
        }
}

item_definition &
item_definition::operator=(const item_definition &rhs)
{
        return operator=(static_cast<const i_item_definition &>(rhs));
}

void
item_definition::get_available_sizes(
    std::map<size_key_type, const i_size_definition *> &destination)
const
{
        std::map<size_key_type,
                 i_size_definition *>::const_iterator i = sizes_.begin();

        destination.clear();
        for ( ; i != sizes_.end(); ++i)
                destination[i->first] = i->second;
}

const string_type &
item_definition::get_decorated_name()
const
{
        spinlock_reference hold_decorated(*decorated_name_lock_);

        return decorated_name_;
}

const i_item_definition_factory &
item_definition::get_item_definition_factory()
const
{
        return *item_definition_factory_;
}

bool
item_definition::get_enabled()
const
{
    spinlock_reference hold_enabled(*enable_lock_);

    return enabled_;
}

item_key_type
item_definition::get_key()
const
{
    return key_;
}

const string_type &
item_definition::get_name()
const
{
    return name_;
}

const i_size_definition &
item_definition::get_size_definition(size_key_type key)
const
{
    std::map<size_key_type,
             i_size_definition *>::const_iterator i =
        sizes_.find(key);

    if (i == sizes_.end())
            throw size_not_present_exception(key_, key);

    return *i->second;
}

size_key_type item_definition::translate_size_name_to_id(const string_type &name) const
{
    std::map<string_type, i_size_definition *>::const_iterator i;

    if (!name_map_.size())
            throw no_sizes_exception(key_);
    else if ((i = name_map_.find(name)) == name_map_.end())
            throw size_not_present_exception(key_, name);

    return i->second->get_key();
}

bool item_definition::update_size_definition( const i_size_definition &size_definition)
{
    if (get_size_definition_rw(size_definition.get_key()).set_all(size_definition))
    {
        regenerate_enabled_state();
        return regenerate_decorated_name();
    }

    return false;
}

bool item_definition::claim_units(size_key_type key, double n_units)
{
    bool claim_was_successful = get_size_definition_rw(key).claim_units(n_units);

    regenerate_enabled_state();
    regenerate_decorated_name();

    return claim_was_successful;
}

void item_definition::release_units(size_key_type key, double n_units)
{
    get_size_definition_rw(key).release_units(n_units);

    regenerate_enabled_state();
    regenerate_decorated_name();
}

bool
item_definition::adjust_available_quantity(
    size_key_type key,
    double offset)
{
    if (get_size_definition_rw(key).adjust_available_quantity(offset)) {
            regenerate_enabled_state();
            return regenerate_decorated_name();
    }

    return false;
}

bool
item_definition::set_available_quantity(
    size_key_type key,
    double available_quantity)
{
    if (get_size_definition_rw(key).set_available_quantity(available_quantity))
    {
        regenerate_enabled_state();
        return regenerate_decorated_name();
    }

    return false;
}

bool
item_definition::set_default_quantity(
    size_key_type key,
    double default_quantity)
{
    return get_size_definition_rw(key).set_default_quantity(default_quantity);
}

bool
item_definition::set_enabled(bool enabled)
{
    spinlock_reference hold_enabled(*enable_lock_);

    return set_and_return_original_value<bool>(enabled_, enabled) != enabled;
}

bool
item_definition::set_name(const string_type &name)
{
    if (set_and_return_original_value<string_type>(
            name_, name) != name)
            return regenerate_decorated_name();

    return false;
}

bool
item_definition::set_price(size_key_type key, double price)
{
    return get_size_definition_rw(key).set_price(price);
}

bool
item_definition::set_size_enabled(size_key_type key, bool enabled)
{
    if (get_size_definition_rw(key).set_enabled(enabled))
    {
        return regenerate_enabled_state();
    }

    return false;
}

bool
item_definition::set_special_price(
    size_key_type key,
    double special_price)
{
    return get_size_definition_rw(key).set_special_price(special_price);
}

bool
item_definition::set_warning_quantity(
    size_key_type key,
    double warning_quantity)
{
    return get_size_definition_rw(key).set_warning_quantity(warning_quantity);
}

i_size_definition &
item_definition::get_size_definition_rw()
{
    if (!sizes_.size())
    {
        throw no_sizes_exception(key_);
    }

    return *sizes_.begin()->second;
}

i_size_definition &
item_definition::get_size_definition_rw(size_key_type key)
{
    std::map<size_key_type, i_size_definition *>::iterator i;

    if ((i = sizes_.find(key)) == sizes_.end())
            throw size_not_present_exception(key_, key);

    return *i->second;
}

void
item_definition::initialize_from(
    const i_item_definition &initalizer)
{
    std::map<size_key_type, const i_size_definition *> available_sizes;
    std::map<size_key_type, const i_size_definition *>::const_iterator i;
    const i_size_definition *sda;
    i_size_definition *sdb;

    initalizer.get_available_sizes(available_sizes);

    i = available_sizes.begin();
    for ( ; i != available_sizes.end(); ++i)
    {
        sda = i->second;
        sdb = sda->get_size_definition_factory().create(*sda);

        sizes_[i->first] = sdb;
        name_map_[sda->get_name()] = sdb;
    }

    name_ = initalizer.get_name();

    regenerate_enabled_state();
    regenerate_decorated_name();
}

bool
item_definition::regenerate_decorated_name()
{
    spinlock_reference hold_decorated(*decorated_name_lock_);

    double available_quantity =
        sizes_.begin()->second->get_available_quantity();

    if (sizes_.size() > 1 || available_quantity < 0)
    {
        decorated_name_ = name_;
    }
    else
    {
        decorated_name_ = generate_decorated_name(name_, L"(", L")", available_quantity);
    }

    return true;
}

bool
item_definition::regenerate_enabled_state()
{
    spinlock_reference hold_enabled(*enable_lock_);

    std::map<size_key_type,
             i_size_definition *>::const_iterator i = sizes_.begin();
    bool enabled = false;

    for ( ; i != sizes_.end(); ++i)
    {
        const i_size_definition &sd = *i->second;

        enabled |= sd.get_enabled() & (std::fabs(sd.get_available_quantity()) > 0);
    }

    enabled_ = enabled;
    return true;
}

void
item_definition::release_held_sizes()
{
    for (std::map<size_key_type, i_size_definition *>::iterator i = sizes_.begin(); i != sizes_.end(); ++i)
    {
        delete i->second;
    }

    name_map_.clear();
    sizes_.clear();
}

} /* item_management */

