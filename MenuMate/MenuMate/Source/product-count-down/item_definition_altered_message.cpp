#include <i_signalling_item_manager.hh>
#include <item_definition_altered_message.hh>
#include <item_management_network_messaging_fwd.hh>
#include <i_signalling_item_manager.hh>
#include <item_management_exceptions.hh>
#include <Forms.hpp>
#include <Stream.h>
namespace item_management
{
namespace network_messaging
{

using item_management::item_exception;

item_definition_altered_message::item_definition_altered_message()
    : enabled_(false),
      item_key_(0),
      manager_(0x0)
{
}

item_definition_altered_message::item_definition_altered_message(
    i_signalling_item_manager *item_manager)
    : enabled_(false),
      item_key_(0),
      manager_(item_manager)
{
}

item_definition_altered_message::item_definition_altered_message(
    i_signalling_item_manager *signalling_item_manager,
    bool enabled,
    item_key_type item_key)
    : enabled_(enabled),
      item_key_(item_key),
      manager_(signalling_item_manager)
{
}

item_definition_altered_message::item_definition_altered_message(
    const i_item_definition_altered_message &initializer)
    : enabled_(initializer.get_enabled()),
      item_key_(initializer.get_item_key()),
      manager_(&initializer.get_item_manager())
{
}

item_definition_altered_message::item_definition_altered_message(
    const item_definition_altered_message &initializer)
    : i_item_definition_altered_message(),
      enabled_(initializer.enabled_),
      item_key_(initializer.item_key_),
      manager_(initializer.manager_)
{
}

item_definition_altered_message::~item_definition_altered_message()
{
}

item_definition_altered_message &
item_definition_altered_message::operator=(
    const i_item_definition_altered_message &rhs)
{
        enabled_ = rhs.get_enabled();
        item_key_ = rhs.get_item_key();
        manager_ = &rhs.get_item_manager();

        return *this;
}

item_definition_altered_message &
item_definition_altered_message::operator=(
    const item_definition_altered_message &rhs)
{
        enabled_ = rhs.enabled_;
        item_key_ = rhs.item_key_;
        manager_ = rhs.manager_;

        return *this;
}

i_signalling_item_manager &
item_definition_altered_message::get_item_manager()
const
{
        return *manager_;
}

bool
item_definition_altered_message::get_enabled()
const
{
        return enabled_;
}

item_key_type
item_definition_altered_message::get_item_key()
const
{
        return item_key_;
}

void
item_definition_altered_message::Action()
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
        get_item_manager().quietly_set_item_enabled(item_key_, enabled_);

        Screen->ActiveForm->Perform(refresh_item_display, 0, 0);
   } catch (item_exception &ie) {
      /* Lalalalala I can't hear you ... */
   }
}

void
item_definition_altered_message::Decode(TMemoryStream *istream)
{
        DecodeNetMessage(istream);
        StreamRead(istream, enabled_);
        StreamRead(istream, item_key_);
}

void
item_definition_altered_message::Encode(TMemoryStream *ostream)
{
        EncodeNetMessage(ostream);
        StreamWrite(ostream, enabled_);
        StreamWrite(ostream, item_key_);
}

} /* network_messaging */
} /* item_management */

