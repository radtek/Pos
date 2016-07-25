#ifndef __hh_i_managed_item_definition_factory__
#define __hh_i_managed_item_definition_factory__

#include <i_item_definition_factory.hh>

namespace item_management {

class i_managed_item_definition;

class i_managed_item_definition_factory : public i_item_definition_factory {
public:
        virtual ~i_managed_item_definition_factory();

        virtual i_managed_item_definition *create(
            const i_managed_item_definition &initializer) const = 0;
        virtual i_managed_item_definition *create(
            i_item_definition *definition_to_manage) const = 0;

protected:
        i_managed_item_definition_factory();

private:
        i_managed_item_definition_factory(
            const i_managed_item_definition_factory &initializer);
        i_managed_item_definition_factory &operator=(
            const i_managed_item_definition_factory &rhs);
};

} /* item_management */

#endif /* ! __hh_i_managed_item_definition_factory__ */

