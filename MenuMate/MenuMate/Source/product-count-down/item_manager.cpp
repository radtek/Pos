#include <i_item_definition.hh>
#include <i_item_definition_factory.hh>
#include <i_size_definition.hh>
#include <i_size_definition_factory.hh>
#include <item_manager.hh>

namespace item_management {

item_manager::item_manager(
    const i_item_definition_factory *item_definition_factory,
    const i_size_definition_factory *size_definition_factory)
    : item_definition_factory_(item_definition_factory),
      size_definition_factory_(size_definition_factory)
{
}

item_manager::~item_manager()
{
}

void
item_manager::add_item_definitions(const std::vector<i_item_definition *> &definitions)
{
    std::vector<i_item_definition *>::const_iterator i = definitions.begin();

    for ( ; i != definitions.end(); ++i)
    {
        add_item_definition(*i);
    }
}

void
item_manager::remove_item_definitions(
    const std::vector<item_key_type> &keys)
{
    std::vector<item_key_type>::const_iterator i = keys.begin();

    for ( ; i != keys.end(); ++i)
    {
        remove_item_definition(*i);
    }
}

const i_item_definition_factory &
item_manager::get_item_definition_factory()
const
{
    return *item_definition_factory_;
}

const i_size_definition_factory &
item_manager::get_size_definition_factory()
const
{
    return *size_definition_factory_;
}

size_key_type
item_manager::translate_size_name_to_key(
    item_key_type key,
    const string_type &name)
const
{
    return get_item_definition(key).translate_size_name_to_id(name);
}

void
item_manager::get_item_available_sizes(
    item_key_type key,
    std::map<size_key_type, const i_size_definition *> &destination)
const
{
    get_item_definition(key).get_available_sizes(destination);
}

const string_type &
item_manager::get_item_decorated_name(item_key_type key)
const
{
    return get_item_definition(key).get_decorated_name();
}

const string_type &
item_manager::get_item_name(item_key_type key)
const
{
    return get_item_definition(key).get_name();
}

const string_type &
item_manager::get_size_decorated_name(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_decorated_name();
}

const string_type &
item_manager::get_size_name(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_name();
}

bool
item_manager::get_item_enabled(item_key_type key)
const
{
        return get_item_definition(key).get_enabled();
}

bool
item_manager::get_size_enabled(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_enabled();
}

double
item_manager::get_available_quantity(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_available_quantity();
}

double
item_manager::get_default_quantity(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_default_quantity();
}

double
item_manager::get_price(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_price();
}

double
item_manager::get_special_price(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_special_price();
}

double
item_manager::get_warning_quantity(
    item_key_type item_key,
    size_key_type size_key)
const
{
    return get_size_definition(item_key, size_key).get_warning_quantity();
}

bool
item_manager::claim_units(
    item_key_type item_key,
    size_key_type size_key,
    double n_units)
{
    return get_item_definition(item_key).claim_units(size_key, n_units);
}

void
item_manager::release_units(
    item_key_type item_key,
    size_key_type size_key,
    double n_units)
{
    get_item_definition(item_key).release_units(size_key, n_units);
}

} /* item_management */

