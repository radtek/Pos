#include <i_size_definition_altered_message.hh>
#include <i_size_definition_altered_message_factory.hh>
#include <memory>
#include <size_definition_altered_message_maker.hh>

namespace item_management {
namespace network_messaging {

size_definition_altered_message_maker
    ::size_definition_altered_message_maker()
        : manager_(0x0),
          factory_(0x0)
{
}

size_definition_altered_message_maker
    ::size_definition_altered_message_maker(
        i_signalling_item_manager *manager,
        const i_size_definition_altered_message_factory *factory)
        : manager_(manager),
          factory_(factory)
{
}

size_definition_altered_message_maker
    ::size_definition_altered_message_maker(
        const i_size_definition_altered_message_maker &initializer)
        : manager_(&initializer.get_item_manager()),
          factory_(&initializer.get_factory())
{
}

size_definition_altered_message_maker
    ::size_definition_altered_message_maker(
        const size_definition_altered_message_maker &initializer)
        : i_size_definition_altered_message_maker(),
          manager_(initializer.manager_),
          factory_(initializer.factory_)
{
}

size_definition_altered_message_maker
    ::~size_definition_altered_message_maker()
{
}

size_definition_altered_message_maker &
size_definition_altered_message_maker::operator=(
    const i_size_definition_altered_message_maker &rhs)
{
        factory_ = &rhs.get_factory();
        manager_ = &rhs.get_item_manager();

        return *this;
}

size_definition_altered_message_maker &
size_definition_altered_message_maker::operator=(
    const size_definition_altered_message_maker &rhs)
{
        factory_ = rhs.factory_;
        manager_ = rhs.manager_;

        return *this;
}

const i_size_definition_altered_message_factory &
size_definition_altered_message_maker::get_factory()
const
{
        return *factory_;
}

i_signalling_item_manager &
size_definition_altered_message_maker::get_item_manager()
const
{
        return *manager_;
}

TNetMessage *
size_definition_altered_message_maker::MakeMessage(TMemoryStream *istream)
const
{
        std::auto_ptr<i_size_definition_altered_message> message(
            factory_->create(manager_));

        message->Decode(istream);

        return message.release();
}

} /* network_messaging */
} /* item_management */


