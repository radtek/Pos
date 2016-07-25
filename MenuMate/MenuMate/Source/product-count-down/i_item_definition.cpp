#include <iostream>

#include <i_item_definition.hh>
#include <i_size_definition.hh>

namespace item_management {

i_item_definition::i_item_definition()
{
}

i_item_definition::~i_item_definition()
{
}

double
i_item_definition::get_available_quantity(size_key_type key)
const
{
        return get_size_definition(key).get_available_quantity();
}

double
i_item_definition::get_default_quantity(size_key_type key)
const
{
        return get_size_definition(key).get_default_quantity();
}

double
i_item_definition::get_price(size_key_type key)
const
{
        return get_size_definition(key).get_price();
}

const string_type &
i_item_definition::get_size_decorated_name(size_key_type key)
const
{
        return get_size_definition(key).get_decorated_name();
}

bool
i_item_definition::get_size_enabled(size_key_type key)
const
{
        return get_size_definition(key).get_enabled();
}

const string_type &
i_item_definition::get_size_name(size_key_type key)
const
{
        return get_size_definition(key).get_name();
}

double
i_item_definition::get_special_price(size_key_type key)
const
{
        return get_size_definition(key).get_special_price();
}

double
i_item_definition::get_warning_quantity(size_key_type key)
const
{
        return get_size_definition(key).get_warning_quantity();
}

} /* item_management */

