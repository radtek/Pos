#ifndef __hh_size_definition_altered_message_factory__
#define __hh_size_definition_altered_message_factory__

#include <i_size_definition_altered_message_factory.hh>

namespace item_management {
namespace network_messaging {

class size_definition_altered_message_factory
    : public i_size_definition_altered_message_factory {
public:
        size_definition_altered_message_factory();
        ~size_definition_altered_message_factory();

        i_size_definition_altered_message *create(
            i_signalling_item_manager *item_manager) const;

        i_size_definition_altered_message *create(
            const i_size_definition_altered_message &initializer) const;

        i_size_definition_altered_message *create(
            i_signalling_item_manager *item_manager,
            item_key_type item_key,
            size_key_type size_key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity) const;

private:
        size_definition_altered_message_factory(
            const size_definition_altered_message_factory &initializer);
        size_definition_altered_message_factory &
            operator=(const size_definition_altered_message_factory &rhs);
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_size_definition_altered_message_factory__ */

