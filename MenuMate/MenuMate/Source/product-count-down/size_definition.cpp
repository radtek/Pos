#include <cmath>

#include <size_definition.hh>
#include <spinlock.hh>
#include <utilities.hh>

namespace item_management {

using utilities::set_and_return_original_value;
using utilities::locking::spinlock;
using utilities::locking::spinlock_reference;

size_definition::size_definition()
    : available_quantity_(0.0),
      default_quantity_(0.0),
      disable_when_count_reaches_zero_(0),
      price_(0.0),
      maxRetailPrice_(0.0),
      reset_to_default_value_on_zed_(0),
      special_price_(0.0),
      warning_quantity_(0.0),
      base_name_(),
      decorated_name_(),
      key_(0),
      enabled_(false),
      size_definition_factory_(0x0),
      mutator_lock_(0x0)
{
}


size_definition::size_definition(
    const i_size_definition_factory *size_definition_factory)
    : available_quantity_(0.0),
      default_quantity_(0.0),
      disable_when_count_reaches_zero_(0),
      price_(0.0),
      maxRetailPrice_(0.0),
      reset_to_default_value_on_zed_(0),
      special_price_(0.0),
      warning_quantity_(0.0),
      base_name_(),
      decorated_name_(),
      key_(0),
      enabled_(false),
      size_definition_factory_(size_definition_factory),
      mutator_lock_(new spinlock())
{
}

size_definition::size_definition(
    const i_size_definition_factory *size_definition_factory,
    size_key_type key)
    : available_quantity_(0.0),
      default_quantity_(0.0),
      disable_when_count_reaches_zero_(0),
      price_(0.0),
      maxRetailPrice_(0.0),
      reset_to_default_value_on_zed_(0),
      special_price_(0.0),
      warning_quantity_(0.0),
      base_name_(),
      decorated_name_(),
      key_(key),
      enabled_(false),
      size_definition_factory_(size_definition_factory),
      mutator_lock_(new spinlock())
{
}

size_definition::size_definition(
    const i_size_definition &initializer)
    : available_quantity_(initializer.get_available_quantity()),
      default_quantity_(initializer.get_default_quantity()),
      disable_when_count_reaches_zero_(
        initializer.get_disable_when_count_reaches_zero()),
      price_(initializer.get_price()),
      maxRetailPrice_(initializer.get_maxRetailPrice()),
      reset_to_default_value_on_zed_(
        initializer.get_reset_to_default_value_on_zed()),
      special_price_(initializer.get_special_price()),
      warning_quantity_(initializer.get_warning_quantity()),
      base_name_(initializer.get_name()),
      decorated_name_(initializer.get_decorated_name()),
      key_(initializer.get_key()),
      enabled_(initializer.get_enabled()),
      size_definition_factory_(
          &initializer.get_size_definition_factory()),
      mutator_lock_(new spinlock())
{
}

size_definition::size_definition(
    const size_definition &initializer)
    : available_quantity_(initializer.available_quantity_),
      default_quantity_(initializer.default_quantity_),
      disable_when_count_reaches_zero_(
        initializer.disable_when_count_reaches_zero_),
      price_(initializer.price_),
      maxRetailPrice_(initializer.get_maxRetailPrice()),
      reset_to_default_value_on_zed_(
        initializer.reset_to_default_value_on_zed_),
      special_price_(initializer.special_price_),
      warning_quantity_(initializer.warning_quantity_),
      base_name_(initializer.base_name_),
      decorated_name_(initializer.decorated_name_),
      key_(initializer.key_),
      enabled_(initializer.enabled_),
      size_definition_factory_(initializer.size_definition_factory_),
      mutator_lock_(new spinlock())
{
}

size_definition::~size_definition()
{
}

size_definition &
size_definition::operator=(const i_size_definition &rhs)
{
        if (&rhs == this)
                return *this;
        else {
                spinlock_reference hold_lock(*mutator_lock_);

                available_quantity_ = rhs.get_available_quantity();
                base_name_ = rhs.get_name();
                decorated_name_ = rhs.get_decorated_name();
                default_quantity_ = rhs.get_default_quantity();
                disable_when_count_reaches_zero_ =
                  rhs.get_disable_when_count_reaches_zero();
                enabled_ = rhs.get_enabled();
                key_ = rhs.get_key();
                price_ = rhs.get_price();
                maxRetailPrice_ = rhs.get_maxRetailPrice();
                reset_to_default_value_on_zed_ =
                  rhs.get_reset_to_default_value_on_zed();
                size_definition_factory_ =
                    &rhs.get_size_definition_factory();
                special_price_ = rhs.get_special_price();
                warning_quantity_ = rhs.get_warning_quantity();

                return *this;
        }
}

size_definition &
size_definition::operator=(const size_definition &rhs)
{
        return operator=(
                   static_cast<const i_size_definition &>(rhs));
}

const string_type &
size_definition::get_decorated_name()
const
{
        spinlock_reference hold_lock(*mutator_lock_);

        return available_quantity_ < 0 ? base_name_ : decorated_name_;
}

const string_type &
size_definition::get_name()
const
{
        return base_name_;
}

double
size_definition::get_available_quantity()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return available_quantity_;
}

double
size_definition::get_default_quantity()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return default_quantity_;
}

bool
size_definition::get_disable_when_count_reaches_zero()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return disable_when_count_reaches_zero_;
}

bool
size_definition::get_enabled()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return enabled_;
}

double
size_definition::get_price()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return price_;
}

double
size_definition::get_maxRetailPrice()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return maxRetailPrice_;
}

bool
size_definition::get_reset_to_default_value_on_zed()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return reset_to_default_value_on_zed_;
}

double
size_definition::get_special_price()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return special_price_;
}

double
size_definition::get_warning_quantity()
const
{
        spinlock_reference hold_lock(*mutator_lock_);
        return warning_quantity_;
}

size_key_type
size_definition::get_key()
const
{
        return key_;
}

const i_size_definition_factory &
size_definition::get_size_definition_factory()
const
{
        return *size_definition_factory_;
}


bool
size_definition::set_all(
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
        set_name(name);
        set_available_quantity(available_quantity);

        default_quantity_ = default_quantity;
        disable_when_count_reaches_zero_ = disable_when_count_reaches_zero;
        enabled_ = enabled;
        price_ = price;
        maxRetailPrice_ = maxRetailPrice;
        reset_to_default_value_on_zed_ = reset_to_default_value_on_zed;
        special_price_ = special_price;
        warning_quantity_ = warning_quantity;

        return true;
}

bool
size_definition::set_all(const i_size_definition &size_definition)
{
        set_name(size_definition.get_name());
        set_available_quantity(
            size_definition.get_available_quantity());

        available_quantity_ = size_definition.get_available_quantity();
        default_quantity_ = size_definition.get_default_quantity();
        disable_when_count_reaches_zero_ =
            size_definition.get_disable_when_count_reaches_zero();
        enabled_ = size_definition.get_enabled();
        key_ = size_definition.get_key();
        price_ = size_definition.get_price();
        maxRetailPrice_ = size_definition.get_maxRetailPrice();
        special_price_ = size_definition.get_special_price();
        warning_quantity_ = size_definition.get_warning_quantity();

        return true;
}

bool
size_definition::set_available_quantity(double available_quantity)
{
        spinlock_reference hold_lock(*mutator_lock_);

        if (set_and_return_original_value<double>(
                available_quantity_, available_quantity)
            != available_quantity) {
                enabled_ = std::fabs(available_quantity) != 0;
                return regenerate_decorated_name();
        }

        return false;
}

bool
size_definition::set_default_quantity(double default_quantity)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return set_and_return_original_value<double>(
                   default_quantity_, default_quantity)
               != default_quantity;
}

bool
size_definition::set_enabled(bool enabled)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return set_and_return_original_value<bool>(
                   enabled_,
                   enabled & (std::fabs(available_quantity_) != 0))
               != enabled;
}

bool
size_definition::set_key(size_key_type key)
{
        return set_and_return_original_value<size_key_type>(
                   key_, key) != key;
}

bool
size_definition::set_name(const string_type &name)
{
        spinlock_reference hold_lock(*mutator_lock_);
        
        if (set_and_return_original_value<string_type>(
                base_name_, name) != name)
                return regenerate_decorated_name();

        return false;
}

bool
size_definition::set_price(double price)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return
            set_and_return_original_value<double>(price_, price) != price;
}

bool
size_definition::set_maxRetailPrice(double maxRetailPrice)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return
            set_and_return_original_value<double>(maxRetailPrice_, maxRetailPrice) != maxRetailPrice;
}

bool
size_definition::set_special_price(double special_price)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return set_and_return_original_value<double>(
                   special_price_, special_price) != special_price;
}

bool
size_definition::set_warning_quantity(double warning_quantity)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return set_and_return_original_value<double>(
            warning_quantity_, warning_quantity) != warning_quantity;
}

bool
size_definition::regenerate_decorated_name()
{
        decorated_name_ =
            generate_decorated_name(base_name_,
                                    L"(",
                                    L")",
                                    available_quantity_);
        return true;
}

bool
size_definition::set_reset_to_default_value_on_zed(
  bool reset_to_default_value_on_zed)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return set_and_return_original_value<bool>(
                   reset_to_default_value_on_zed_,
                   reset_to_default_value_on_zed)
               != reset_to_default_value_on_zed;
}

bool
size_definition::set_disable_when_count_reaches_zero(
  bool disable_when_count_reaches_zero)
{
        spinlock_reference hold_lock(*mutator_lock_);

        return set_and_return_original_value<bool>(
                   disable_when_count_reaches_zero_,
                   disable_when_count_reaches_zero)
               != disable_when_count_reaches_zero;
}

} /* item_management */

