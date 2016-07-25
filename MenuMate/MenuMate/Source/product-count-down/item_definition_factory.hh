#ifndef __hh_item_definition_factory__
#define __hh_item_definition_factory__

#include <i_item_definition_factory.hh>

namespace item_management {

class item_definition;

class item_definition_factory : public i_item_definition_factory {
public:
        item_definition_factory();
        ~item_definition_factory();

        /* Builders (Getters) */
        i_item_definition *create(
            const i_item_definition &initializer) const;
        item_definition *create(
            const item_definition &initializer) const;
        i_item_definition *create(
            const string_type &name,
            std::vector<i_size_definition *> &size_definitions,
            item_key_type key) const;

private:
        item_definition_factory(
            const item_definition_factory &initializer);

        item_definition_factory &operator=(
            const item_definition_factory &rhs);
};

} /* item_management */

#endif /* ! __hh_item_definition_factory__ */

