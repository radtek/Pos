#ifndef __hh_i_signalling_item_definition_factory__
#define __hh_i_signalling_item_definition_factory__

#include <i_managed_item_definition_factory.hh>
#include <i_signalling_item_definition.hh>

namespace item_management {

class i_signalling_item_definition_factory
    : public i_managed_item_definition_factory {
public:
        i_signalling_item_definition_factory();
        virtual ~i_signalling_item_definition_factory();

        virtual i_signalling_item_definition *create(
            i_item_definition *definition_to_manage) const = 0;
        virtual i_signalling_item_definition *create(
            i_item_definition *definition_to_manage,
            const i_item_altered_signal_handler *item_altered_handler,
            const i_size_altered_signal_handler *size_altered_handler,
            const i_size_warning_reached_signal_handler *
                size_warning_reached_handler) const = 0;
};

} /* item_management */

#endif /* ! __hh_i_signalling_item_definition_factory__ */

