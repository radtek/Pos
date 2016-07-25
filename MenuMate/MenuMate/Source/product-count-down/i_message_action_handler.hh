#ifndef __hh_i_message_action_handler__
#define __hh_i_message_action_handler__

#include <functors.hh>

namespace item_management {

template<argument_type>
class i_message_action_handler
    : public utilities::unary_functor<void, argument_type> {
public:
        virtual ~i_message_action_handler() { };

protected:
        i_message_action_handler() { };
};

} /* item_management */

#endif /* ! __hh_i_message_action_handler__ */

