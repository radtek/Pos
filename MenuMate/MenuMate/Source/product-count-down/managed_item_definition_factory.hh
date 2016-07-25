#ifndef __hh_managed_item_definition_factory__
#define __hh_managed_item_definition_factory__

#include <i_managed_item_definition_factory.hh>

namespace item_management {

class managed_item_definition_factory
    : public virtual i_managed_item_definition_factory {
public:
        managed_item_definition_factory(
            const i_item_definition_factory &item_definition_factory);
        virtual ~managed_item_definition_factory();

        i_item_definition *create(
            const i_item_definition &initializer) const;
        i_managed_item_definition *create(
            const i_managed_item_definition &initializer) const;
        i_managed_item_definition *create(
            i_item_definition *definition_to_manage) const;
        i_item_definition *create(
            const string_type &name,
            std::vector<i_size_definition *> &size_definitions,
            item_key_type key) const;

private:
        const i_item_definition_factory &item_definition_factory_;
};

} /* item_management */

#endif /* ! __hh_managed_item_definition_factory__ */

