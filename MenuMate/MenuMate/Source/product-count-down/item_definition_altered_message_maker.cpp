#include <i_item_definition_altered_message.hh>
#include <i_item_definition_altered_message_factory.hh>
#include <item_definition_altered_message_maker.hh>
#include <memory>

namespace item_management
{
namespace network_messaging
{

item_definition_altered_message_maker::item_definition_altered_message_maker()
    : manager_(0x0), factory_(0x0)
{
}

item_definition_altered_message_maker::item_definition_altered_message_maker(i_signalling_item_manager *item_manager, const i_item_definition_altered_message_factory *message_factory)
    : manager_(item_manager),
      factory_(message_factory)
{
}

item_definition_altered_message_maker::item_definition_altered_message_maker(const i_item_definition_altered_message_maker &initializer)
    : manager_(&initializer.get_item_manager()),
      factory_(&initializer.get_factory())
{
}

item_definition_altered_message_maker::item_definition_altered_message_maker(const item_definition_altered_message_maker &initializer)
    : i_item_definition_altered_message_maker(),
      manager_(initializer.manager_),
      factory_(initializer.factory_)
{
}

item_definition_altered_message_maker::~item_definition_altered_message_maker()
{
}

item_definition_altered_message_maker &item_definition_altered_message_maker::operator=(const i_item_definition_altered_message_maker &rhs)
{
    factory_ = &rhs.get_factory();
    manager_ = &rhs.get_item_manager();

    return *this;
}

item_definition_altered_message_maker &item_definition_altered_message_maker::operator=(const item_definition_altered_message_maker &rhs)
{
    factory_ = rhs.factory_;
    manager_ = rhs.manager_;

    return *this;
}

const i_item_definition_altered_message_factory &item_definition_altered_message_maker::get_factory() const
{
    return *factory_;
}

i_signalling_item_manager &item_definition_altered_message_maker::get_item_manager() const
{
    return *manager_;
}

TNetMessage * item_definition_altered_message_maker::MakeMessage(TMemoryStream *istream) const
{
    std::auto_ptr<i_item_definition_altered_message> message(factory_->create(manager_));

    message->Decode(istream);

    return message.release();
}

} /* network_messaging */
} /* item_management */

