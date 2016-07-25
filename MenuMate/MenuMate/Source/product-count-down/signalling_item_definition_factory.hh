#ifndef __hh_signalling_item_definition_factory__
#define __hh_signalling_item_definition_factory__

#include <i_signalling_item_definition_factory.hh>

namespace item_management {

class signalling_item_definition_factory
    : public i_signalling_item_definition_factory {
public:
        signalling_item_definition_factory(
            const i_item_definition_factory &item_definition_factory);
        ~signalling_item_definition_factory();

        i_item_definition *create(
            const i_item_definition &initializer) const;
        i_managed_item_definition *create(
            const i_managed_item_definition &initializer) const;
        i_item_definition *create(
            const string_type &name,
            std::vector<i_size_definition *> &size_definitions,
            item_key_type key) const;
        i_signalling_item_definition *create(
            i_item_definition *definition_to_wrap) const;
        i_signalling_item_definition *create(
            i_item_definition *definition_to_wrap,
            const i_item_altered_signal_handler *item_altered_handler,
            const i_size_altered_signal_handler *size_altered_handler,
            const i_size_warning_reached_signal_handler *
                size_warning_reached_handler) const;

private:
        signalling_item_definition_factory(
            const signalling_item_definition_factory &initializer);
        signalling_item_definition_factory &operator=(
            const signalling_item_definition_factory &rhs);

        const i_item_definition_factory &item_definition_factory_;
};

} /* item_management */

#endif /* ! __hh_signalling_item_definition_factory__ */

