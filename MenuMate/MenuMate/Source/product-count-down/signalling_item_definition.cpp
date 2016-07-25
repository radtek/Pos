#include <i_item_definition_factory.hh>
#include <i_size_definition.hh>
#include <signalling_item_definition.hh>
#include <cmath>

namespace item_management {

#ifdef __BCPLUSPLUS__
#       pragma warn -par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#endif /* __GNUC__ */

class null_item_alteration_handler : public i_item_altered_signal_handler {
public:
        null_item_alteration_handler() { };
        ~null_item_alteration_handler() { };

        void operator()(const i_item_definition &i) const { };
};

class null_size_alteration_handler : public i_size_altered_signal_handler {
public:
        null_size_alteration_handler() { };
        ~null_size_alteration_handler() { };

        void operator()(const i_item_definition &i,
                        const i_size_definition &s) const { };
};

class null_size_warning_reached_handler
    : public i_size_warning_reached_signal_handler {
public:
        null_size_warning_reached_handler() { };
        ~null_size_warning_reached_handler() { };

        void operator()(const i_item_definition &i,
                        const i_size_definition &s) const { };
};

#ifdef __BCPLUSPLUS__
#       pragma warn .par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic pop
#endif /* __GNUC__ */

static null_item_alteration_handler default_item_alteration_handler;
static null_size_alteration_handler default_size_alteration_handler;
static null_size_warning_reached_handler default_size_warning_reached_handler;

signalling_item_definition::signalling_item_definition()
    : item_altered_handler_(&default_item_alteration_handler),
      size_altered_handler_(&default_size_alteration_handler),
      size_warning_reached_handler_(&default_size_warning_reached_handler)
{
}

signalling_item_definition::signalling_item_definition(
    const i_managed_item_definition &initializer)
    : managed_item_definition(initializer),
      item_altered_handler_(&default_item_alteration_handler),
      size_altered_handler_(&default_size_alteration_handler),
      size_warning_reached_handler_(&default_size_warning_reached_handler)
{
}

signalling_item_definition::signalling_item_definition(
    const i_signalling_item_definition &initializer)
    : managed_item_definition(initializer),
      item_altered_handler_(&initializer.get_item_altered_handler()),
      size_altered_handler_(&initializer.get_size_altered_handler()),
      size_warning_reached_handler_(
          &initializer.get_size_warning_reached_handler())
{
}

signalling_item_definition::signalling_item_definition(
    const signalling_item_definition &initializer)
    : managed_item_definition(initializer),
      item_altered_handler_(initializer.item_altered_handler_),
      size_altered_handler_(initializer.size_altered_handler_),
      size_warning_reached_handler_(initializer.size_warning_reached_handler_)
{
}

signalling_item_definition::signalling_item_definition(
    const i_item_definition_factory *item_definition_factory,
    i_item_definition *definition_to_wrap,
    const i_item_altered_signal_handler *item_altered_handler,
    const i_size_altered_signal_handler *size_altered_handler,
    const i_size_warning_reached_signal_handler *size_warning_reached_handler)
    : managed_item_definition(item_definition_factory, definition_to_wrap),
      item_altered_handler_(item_altered_handler),
      size_altered_handler_(size_altered_handler),
      size_warning_reached_handler_(size_warning_reached_handler)
{
        if (!item_altered_handler_)
                item_altered_handler_ = &default_item_alteration_handler;
        if (!size_altered_handler_)
                size_altered_handler_ = &default_size_alteration_handler;
        if (!size_warning_reached_handler_)
                size_warning_reached_handler_ =
                    &default_size_warning_reached_handler;
}

signalling_item_definition::~signalling_item_definition()
{
}

signalling_item_definition &
signalling_item_definition::operator=(const i_item_definition &rhs)
{
        managed_item_definition::operator=(rhs);
        return *this;
}

signalling_item_definition &
signalling_item_definition::operator=(
    const i_managed_item_definition &rhs)
{
        managed_item_definition::operator=(rhs);
        return *this;
}

signalling_item_definition &
signalling_item_definition::operator=(
    const i_signalling_item_definition &rhs)
{
        managed_item_definition::operator=(rhs);

        item_altered_handler_ = &rhs.get_item_altered_handler();
        size_altered_handler_ = &rhs.get_size_altered_handler();
        size_warning_reached_handler_ =
            &rhs.get_size_warning_reached_handler();

        return *this;
}

signalling_item_definition &
signalling_item_definition::operator=(
    const signalling_item_definition &rhs)
{
        return operator=(
            static_cast<const i_signalling_item_definition &>(rhs));
}

const i_item_altered_signal_handler &
signalling_item_definition::get_item_altered_handler()
const
{
        return *item_altered_handler_;
}

const i_size_altered_signal_handler &
signalling_item_definition::get_size_altered_handler()
const
{
        return *size_altered_handler_;
}

const i_size_warning_reached_signal_handler &
signalling_item_definition::get_size_warning_reached_handler()
const
{
        return *size_warning_reached_handler_;
}

bool
signalling_item_definition::adjust_available_quantity(
    size_key_type key,
    double offset)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::adjust_available_quantity(
                       key, offset));
}

bool
signalling_item_definition::set_available_quantity(
    size_key_type key,
    double available_quantity)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::set_available_quantity(
                       key, available_quantity));
}

bool
signalling_item_definition::set_default_quantity(
    size_key_type key,
    double default_quantity)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::set_default_quantity(
                       key, default_quantity));
}

bool
signalling_item_definition::set_enabled(bool enabled)
{
    return forward_item_alteration(managed_item_definition::set_enabled(enabled));
}

bool
signalling_item_definition::set_name(const string_type &name)
{
        return forward_item_alteration(
                   managed_item_definition::set_name(name));
}

bool
signalling_item_definition::set_price(
    size_key_type key,
    double price)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::set_price(key, price));
}

bool
signalling_item_definition::set_size_enabled(
    size_key_type key,
    bool enabled)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::set_size_enabled(
                       key, enabled));
}

bool
signalling_item_definition::set_special_price(
    size_key_type key,
    double special_price)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::set_special_price(
                       key, special_price));
}

bool
signalling_item_definition::set_warning_quantity(
    size_key_type key,
    double warning_quantity)
{
        return forward_size_alteration(
                   key,
                   managed_item_definition::set_warning_quantity(
                       key, warning_quantity));
}

bool
signalling_item_definition::claim_units(size_key_type key, double n_units)
{
        bool claimed = managed_item_definition::claim_units(key, n_units);

        double n_available =
          get_available_quantity(key);
        if (claimed != false && n_available != -1) {
                forward_size_alteration(key, claimed);
                if (n_available < get_warning_quantity(key))
                        (*size_warning_reached_handler_)(
                            *this, get_size_definition(key));
        }

        return claimed;
}

void
signalling_item_definition::release_units(size_key_type key, double n_units)
{
        managed_item_definition::release_units(key, n_units);

        if (managed_item_definition::get_available_quantity(key) != -1)
                forward_size_alteration(key, true);
}

bool
signalling_item_definition::quietly_set_enabled(bool enabled)
{
        return managed_item_definition::set_enabled(enabled);
}

bool
signalling_item_definition::quietly_update_size_definition(
            size_key_type key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity)
{
        bool altered =
                 managed_item_definition::set_available_quantity(
                     key, available_quantity)
                 | managed_item_definition::set_default_quantity(
                       key, default_quantity)
                 | managed_item_definition::set_size_enabled(
                       key, enabled)
                 | managed_item_definition::set_price(
                       key, price)
                 | managed_item_definition::set_special_price(
                       key, special_price)
                 | managed_item_definition::set_warning_quantity(
                       key, warning_quantity);
}

bool
signalling_item_definition::update_size_definition(
    const i_size_definition &definition)
{
        return forward_size_alteration(
                   definition.get_key(),
                   managed_item_definition::update_size_definition(
                       definition));
}

bool
signalling_item_definition::forward_item_alteration(bool was_altered)
{
        if (was_altered)
                (*item_altered_handler_)(get_item_definition());

        return was_altered;
}

bool
signalling_item_definition::forward_size_alteration(
    size_key_type key,
    bool was_altered)
{
        if (was_altered) {
                const i_item_definition &item_definition =
                    get_item_definition();
                const i_size_definition &size_definition =
                    get_size_definition(key);
                double available_quantity =
                    size_definition.get_available_quantity();

                (*size_altered_handler_)(item_definition, size_definition);
        }

        return was_altered;
}

} /* item_management */

