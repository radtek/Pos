#include <i_signalling_item_manager.hh>
#include <size_definition_altered_message.hh>

#include <Stream.h>
#include <item_management_network_messaging_fwd.hh>
#include <Forms.hpp>

#include <item_management_exceptions.hh>

namespace item_management {
namespace network_messaging {

using item_management::item_exception;

size_definition_altered_message::size_definition_altered_message()
    : available_quantity_(0),
      default_quantity_(0),
      enabled_(0),
      item_key_(0),
      price_(0),
      size_key_(0),
      special_price_(0),
      warning_quantity_(0),
      manager_(0x0)
{
}

size_definition_altered_message::size_definition_altered_message(
    i_signalling_item_manager *item_manager)
    : available_quantity_(0),
      default_quantity_(0),
      enabled_(0),
      item_key_(0),
      price_(0),
      size_key_(0),
      special_price_(0),
      warning_quantity_(0),
      manager_(item_manager)
{
}

size_definition_altered_message::size_definition_altered_message(
    const i_size_definition_altered_message &initializer)
    : available_quantity_(initializer.get_available_quantity()),
      default_quantity_(initializer.get_default_quantity()),
      enabled_(initializer.get_enabled()),
      item_key_(initializer.get_item_key()),
      price_(initializer.get_price()),
      size_key_(initializer.get_size_key()),
      special_price_(initializer.get_special_price()),
      warning_quantity_(initializer.get_warning_quantity()),
      manager_(&initializer.get_item_manager())
{
}

size_definition_altered_message::size_definition_altered_message(
    const size_definition_altered_message &initializer)
    : available_quantity_(initializer.available_quantity_),
      default_quantity_(initializer.default_quantity_),
      enabled_(initializer.enabled_),
      item_key_(initializer.item_key_),
      price_(initializer.price_),
      size_key_(initializer.size_key_),
      special_price_(initializer.special_price_),
      warning_quantity_(initializer.warning_quantity_),
      manager_(initializer.manager_)
{
}

size_definition_altered_message::~size_definition_altered_message()
{
}

size_definition_altered_message::size_definition_altered_message(
    i_signalling_item_manager *item_manager,
    item_key_type item_key,
    size_key_type size_key,
    double available_quantity,
    double default_quantity,
    bool enabled,
    double price,
    double special_price,
    double warning_quantity)
    : available_quantity_(available_quantity),
      default_quantity_(default_quantity),
      enabled_(enabled),
      item_key_(item_key),
      price_(price),
      size_key_(size_key),
      special_price_(special_price),
      warning_quantity_(warning_quantity),
      manager_(item_manager)
{
}

size_definition_altered_message &
size_definition_altered_message::operator=(
    const i_size_definition_altered_message &rhs)
{
        if (this == &rhs)
                return *this;

        available_quantity_ = rhs.get_available_quantity();
        default_quantity_ = rhs.get_default_quantity();
        enabled_ = rhs.get_enabled();
        item_key_ = rhs.get_item_key();
        price_ = rhs.get_price();
        size_key_ = rhs.get_size_key();
        special_price_ = rhs.get_special_price();
        warning_quantity_ = rhs.get_warning_quantity();
        manager_ = &rhs.get_item_manager();

        return *this;
}

size_definition_altered_message &
size_definition_altered_message::operator=(
    const size_definition_altered_message &rhs)
{
        if (this == &rhs)
                return *this;

        available_quantity_ = rhs.available_quantity_;
        default_quantity_ = rhs.default_quantity_;
        enabled_ = rhs.enabled_;
        item_key_ = rhs.item_key_;
        price_ = rhs.price_;
        size_key_ = rhs.size_key_;
        special_price_ = rhs.special_price_;
        warning_quantity_ = rhs.warning_quantity_;
        manager_ = rhs.manager_;

        return *this;
}

i_signalling_item_manager &
size_definition_altered_message::get_item_manager()
const
{
        return *manager_;
}


double
size_definition_altered_message::get_available_quantity()
const
{
        return available_quantity_;
}

double
size_definition_altered_message::get_default_quantity()
const
{
        return default_quantity_;
}

bool
size_definition_altered_message::get_enabled()
const
{
        return enabled_;
}

item_key_type
size_definition_altered_message::get_item_key()
const
{
        return item_key_;
}

double
size_definition_altered_message::get_price()
const
{
        return price_;
}

size_key_type
size_definition_altered_message::get_size_key()
const
{
        return size_key_;
}

double
size_definition_altered_message::get_special_price()
const
{
        return special_price_;
}

double
size_definition_altered_message::get_warning_quantity()
const
{
        return warning_quantity_;
}

void
size_definition_altered_message::Action()
{
   /* A quick fix. We're ignoring any exceptions thrown here by the
    * Item manager. I.e. whether a size exists or not. Those exceptions are
    * thrown by the Item manager when it attempts to alter a size or item that
    * it does not know about.
    *
    * This can happen when there are machines on the network using different
    * menus.
    */
   try {
        manager_->quietly_update_size_definition(item_key_,
                                                 size_key_,
                                                 available_quantity_,
                                                 default_quantity_,
                                                 enabled_,
                                                 price_,
                                                 special_price_,
                                                 warning_quantity_);

        Screen->ActiveForm->Perform(refresh_item_display, 0, 0);
   } catch (item_exception &ie) {
      /* Lalalalala I can't hear you ... */
   }
}

void
size_definition_altered_message::Decode(TMemoryStream *istream)
{
        DecodeNetMessage(istream);
        StreamRead(istream, available_quantity_);
        StreamRead(istream, default_quantity_);
        StreamRead(istream, enabled_);
        StreamRead(istream, item_key_);
        StreamRead(istream, price_);
        StreamRead(istream, size_key_);
        StreamRead(istream, special_price_);
        StreamRead(istream, warning_quantity_);
}

void
size_definition_altered_message::Encode(TMemoryStream *ostream)
{
        EncodeNetMessage(ostream);
        StreamWrite(ostream, available_quantity_);
        StreamWrite(ostream, default_quantity_);
        StreamWrite(ostream, enabled_);
        StreamWrite(ostream, item_key_);
        StreamWrite(ostream, price_);
        StreamWrite(ostream, size_key_);
        StreamWrite(ostream, special_price_);
        StreamWrite(ostream, warning_quantity_);
}

} /* network_messaging */
} /* item_management */

