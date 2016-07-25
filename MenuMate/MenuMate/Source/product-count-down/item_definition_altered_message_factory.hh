#ifndef __hh_item_definition_altered_message_factory__
#define __hh_item_definition_altered_message_factory__

#include <i_item_definition_altered_message_factory.hh>

namespace item_management {
namespace network_messaging {

class item_definition_altered_message_factory
    : public i_item_definition_altered_message_factory {
public:
        item_definition_altered_message_factory();
        ~item_definition_altered_message_factory();

        i_item_definition_altered_message *create(
            i_signalling_item_manager *item_manager) const;

        i_item_definition_altered_message *create(
            const i_item_definition_altered_message &initializer) const;

        i_item_definition_altered_message *create(
            i_signalling_item_manager *item_manager,
            bool enabled,
            item_key_type key) const;

private:
        item_definition_altered_message_factory(
            const item_definition_altered_message_factory &initializer);
        item_definition_altered_message_factory &
            operator=(const item_definition_altered_message_factory &rhs);
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_item_definition_altered_message_factory__ */

