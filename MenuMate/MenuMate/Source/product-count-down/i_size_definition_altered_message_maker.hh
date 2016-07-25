#ifndef __i_size_definition_altered_message_maker__
#define __i_size_definition_altered_message_maker__

#include <i_message_maker.hh>

namespace item_management {

class i_signalling_item_manager;

namespace network_messaging {

class i_size_definition_altered_message;
class i_size_definition_altered_message_factory;

class i_size_definition_altered_message_maker
    : public i_message_maker<i_size_definition_altered_message_factory> {
public:
        i_size_definition_altered_message_maker();
        i_size_definition_altered_message_maker(
            i_signalling_item_manager *item_manager,
            const i_size_definition_altered_message_factory *
                message_factory);
        virtual ~i_size_definition_altered_message_maker();

        virtual i_size_definition_altered_message_maker &operator=(
            const i_size_definition_altered_message_maker &rhs) = 0;

        virtual const i_size_definition_altered_message_factory &
            get_factory() const = 0;
        virtual i_signalling_item_manager &get_item_manager() const = 0;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __i_size_definition_altered_message_maker__ */

