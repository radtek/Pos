#include <cmath>
#include <i_size_definition.hh>

namespace item_management {

i_size_definition::i_size_definition()
{
}

i_size_definition::~i_size_definition()
{
}

bool
i_size_definition::operator==(const i_size_definition &rhs)
const
{
        return get_available_quantity() == rhs.get_available_quantity()
               && get_decorated_name() == rhs.get_decorated_name()
               && get_default_quantity() == rhs.get_default_quantity()
               && get_disable_when_count_reaches_zero()
                  == rhs.get_disable_when_count_reaches_zero()
               && get_enabled() == rhs.get_enabled()
               && get_key() == rhs.get_key()
               && get_name() == rhs.get_name()
               && get_price() == rhs.get_price()
               && get_maxRetailPrice() == rhs.get_maxRetailPrice()
               && get_reset_to_default_value_on_zed()
                  == rhs.get_reset_to_default_value_on_zed()
               && &get_size_definition_factory()
                  == &rhs.get_size_definition_factory()
               && get_special_price() == rhs.get_special_price()
               && get_warning_quantity() == rhs.get_warning_quantity();
}

bool
i_size_definition::operator!=(const i_size_definition &rhs)
const
{
        return !operator==(rhs);
}

bool
i_size_definition::adjust_available_quantity(double offset)
{
        double temp = get_available_quantity();

        if (temp < 0 || !offset)
                return false;

        temp += offset;
        set_enabled(temp > 0);
        return set_available_quantity(temp);
}

void i_size_definition::extract_properties(
    double &available_quantity,
    double &default_quantity,
    bool &disable_when_count_reaches_zero,
    bool &enabled,
    size_key_type &key,
    string_type &name,
    double &price,
    double &maxRetailPrice,
    bool &reset_to_default_value_on_zed,
    double &special_price,
    double &warning_quantity) const
{
        available_quantity = get_available_quantity();
        default_quantity = get_default_quantity();
        enabled = get_enabled();
        disable_when_count_reaches_zero = get_disable_when_count_reaches_zero();
        key = get_key();
        name = get_name();
        price = get_price();
        maxRetailPrice = get_maxRetailPrice();
        reset_to_default_value_on_zed = get_reset_to_default_value_on_zed();
        special_price = get_special_price();
        warning_quantity = get_warning_quantity();
}

void i_size_definition::release_units(double n_units)
{
        adjust_available_quantity(std::fabs(n_units));
}

bool i_size_definition::claim_units(double n_units)
{
        /*
         * We have to ensure that the user isn't trying to claim
         * ``negative units.'' This would betray the nature of
         * the function as claiming ``negative'' units would
         * really release.
         */
        
        double clamped_n_units = std::fabs(n_units);
        double n_available = get_available_quantity();

        if (n_available < 0)
                return true;
        else if (n_available >= clamped_n_units) {
                adjust_available_quantity(-clamped_n_units);
                return true;
        }

        return false;
}

bool i_size_definition::set_all(
    double available_quantity,
    double default_quantity,
    bool disable_when_count_reaches_zero,
    bool enabled,
    size_key_type key,
    const string_type &name,
    double price,
    double maxRetailPrice,
    bool reset_to_default_value_on_zed,
    double special_price,
    double warning_quantity)
{
        return set_available_quantity(available_quantity)
               | set_default_quantity(default_quantity)
               | set_disable_when_count_reaches_zero(disable_when_count_reaches_zero)
               | set_enabled(enabled)
               | set_key(key)
               | set_name(name)
               | set_price(price)
               | set_maxRetailPrice(maxRetailPrice)
               | set_reset_to_default_value_on_zed(reset_to_default_value_on_zed)
               | set_special_price(special_price)
               | set_warning_quantity(warning_quantity);
}

bool i_size_definition::set_all(const i_size_definition &size_definition)
{
        return set_available_quantity(size_definition.get_available_quantity())
               | set_default_quantity(size_definition.get_default_quantity())
               | set_disable_when_count_reaches_zero(size_definition.get_disable_when_count_reaches_zero())
               | set_enabled(size_definition.get_enabled())
               | set_key(size_definition.get_key())
               | set_name(size_definition.get_name())
               | set_price(size_definition.get_price())
               | set_maxRetailPrice(size_definition.get_maxRetailPrice())
               | set_reset_to_default_value_on_zed(size_definition.get_reset_to_default_value_on_zed())
               | set_special_price(size_definition.get_special_price())
               | set_warning_quantity(size_definition.get_warning_quantity());
}

} /* item_management */

