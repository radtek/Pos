#include <i_item_definition_factory.hh>
#include <managed_item_definition.hh>

namespace item_management {

managed_item_definition::managed_item_definition()
    : item_definition_(),
      item_definition_factory_(0x0)
{
}

managed_item_definition::managed_item_definition(
    const managed_item_definition &initializer)
    : item_definition_(),
      item_definition_factory_(initializer.item_definition_factory_)
{
        clone_definition(*initializer.item_definition_);
}

managed_item_definition::managed_item_definition(
    const i_managed_item_definition &initializer)
    : item_definition_(),
      item_definition_factory_(&initializer.get_item_definition_factory())
{
        clone_definition(initializer.get_item_definition());
}

managed_item_definition::managed_item_definition(
    const i_item_definition_factory *item_definition_factory,
    i_item_definition *definition_to_wrap)
    : item_definition_(definition_to_wrap),
      item_definition_factory_(item_definition_factory)
{
}

managed_item_definition::~managed_item_definition()
{
}

managed_item_definition &
managed_item_definition::operator=(const i_item_definition &rhs)
{
        if (&rhs == this)
                return *this;

        clone_definition(rhs);
        return *this;
}

managed_item_definition &
managed_item_definition::operator=(const i_managed_item_definition &rhs)
{
        if (&rhs == this)
                return *this;

        clone_definition(rhs);

        return *this;
}

managed_item_definition &
managed_item_definition::operator=(const managed_item_definition &rhs)
{
        if (&rhs == this)
                return *this;

        clone_definition(rhs);

        return *this;
}

void
managed_item_definition::get_available_sizes(
    std::map<size_key_type, const i_size_definition *> &destination)
const
{
        return item_definition_->get_available_sizes(destination);
}

double
managed_item_definition::get_available_quantity(
    size_key_type key)
const
{
        return item_definition_->get_available_quantity(key);
}

const string_type &
managed_item_definition::get_decorated_name()
const
{
        return item_definition_->get_decorated_name();
}

double
managed_item_definition::get_default_quantity(size_key_type key)
const
{
        return item_definition_->get_default_quantity(key);
}

bool
managed_item_definition::get_enabled()
const
{
        return item_definition_->get_enabled();
}

i_item_definition &
managed_item_definition::get_item_definition()
const
{
        return *item_definition_;
}

const i_item_definition_factory &
managed_item_definition::get_item_definition_factory()
const
{
        return *item_definition_factory_;
}

item_key_type
managed_item_definition::get_key()
const
{
        return item_definition_->get_key();
}

const string_type &
managed_item_definition::get_name()
const
{
        return item_definition_->get_name();
}

double
managed_item_definition::get_price(size_key_type key)
const
{
        return item_definition_->get_price(key);
}

const string_type &
managed_item_definition::get_size_decorated_name(size_key_type key)
const
{
        return item_definition_->get_size_decorated_name(key);
}

const i_size_definition &
managed_item_definition::get_size_definition(size_key_type key)
const
{
        return item_definition_->get_size_definition(key);
}

bool
managed_item_definition::get_size_enabled(size_key_type key)
const
{
        return item_definition_->get_size_enabled(key);
}

const string_type &
managed_item_definition::get_size_name(size_key_type key)
const
{
        return item_definition_->get_size_name(key);
}

double
managed_item_definition::get_special_price(size_key_type key)
const
{
        return item_definition_->get_special_price(key);
}

double
managed_item_definition::get_warning_quantity(size_key_type key)
const
{
        return item_definition_->get_warning_quantity(key);
}

size_key_type
managed_item_definition::translate_size_name_to_id(
    const string_type &name)
const
{
        return item_definition_->translate_size_name_to_id(name);
}

bool
managed_item_definition::claim_units(size_key_type key, double n_units)
{
        return item_definition_->claim_units(key, n_units);
}

void
managed_item_definition::release_units(size_key_type key, double n_units)
{
        return item_definition_->release_units(key, n_units);
}

bool
managed_item_definition::update_size_definition(
    const i_size_definition &definition)
{
        return item_definition_->update_size_definition(definition);
}

bool
managed_item_definition::adjust_available_quantity(
    size_key_type key,
    double offset)
{
        return item_definition_->adjust_available_quantity(key, offset);
}

bool
managed_item_definition::set_available_quantity(
    size_key_type key,
    double available_quantity)
{
        return item_definition_->set_available_quantity(
                   key, available_quantity);
}

bool
managed_item_definition::set_default_quantity(
    size_key_type key,
    double default_quantity)
{
        return item_definition_->set_default_quantity(
                   key, default_quantity);
}

bool
managed_item_definition::set_enabled(bool enabled)
{
        return item_definition_->set_enabled(enabled);
}

bool
managed_item_definition::set_name(const string_type &name)
{
        return item_definition_->set_name(name);
}

bool
managed_item_definition::set_price(
    size_key_type key,
    double price)
{
        return item_definition_->set_price(key, price);
}

bool
managed_item_definition::set_size_enabled(
    size_key_type key,
    bool enabled)
{
        return item_definition_->set_size_enabled(key, enabled);
}

bool
managed_item_definition::set_special_price(
    size_key_type key,
    double special_price)
{
        return item_definition_->set_special_price(key, special_price);
}

bool
managed_item_definition::set_warning_quantity(
    size_key_type key,
    double warning_quantity)
{
        return item_definition_->set_warning_quantity(
                   key, warning_quantity);
}

void
managed_item_definition::clone_definition(
    const i_item_definition &initializer)
{
        item_definition_.reset(
            initializer.get_item_definition_factory().create(initializer));
}

} /* item_management */

