#include <i_size_definition.hh>
#include <i_size_definition_factory.hh>

namespace item_management {

i_size_definition_factory::i_size_definition_factory()
{
}

i_size_definition_factory::~i_size_definition_factory()
{
}

i_size_definition *
i_size_definition_factory::create(
    double available_quantity,
    double default_quantity,
    bool enabled,
    size_key_type key,
    const string_type &name,
    double price,
    double maxRetailPrice,
    double special_price,
    double warning_quantity)
const
{
        i_size_definition *sd = create(key);

        if (sd) {
                sd->set_available_quantity(available_quantity);
                sd->set_default_quantity(default_quantity);
                sd->set_enabled(enabled);
                sd->set_name(name);
                sd->set_price(price);
                sd->set_maxRetailPrice(maxRetailPrice);
                sd->set_special_price(special_price);
                sd->set_warning_quantity(warning_quantity);
        }

        return sd;
}

} /* item_management */

