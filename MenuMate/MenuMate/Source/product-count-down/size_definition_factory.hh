#ifndef __hh_size_definition_factory__
#define __hh_size_definition_factory__

#include <i_size_definition_factory.hh>

namespace item_management {

class size_definition_factory : public i_size_definition_factory {
public:
        size_definition_factory();
        ~size_definition_factory();

        i_size_definition *create() const;
        i_size_definition *create(size_key_type key) const;
        i_size_definition *create(
            const i_size_definition &initializer) const;
        i_size_definition *create(
            double available_quantity,
            double default_quantity,
            bool enabled,
            size_key_type key,
            const string_type &name,
            double price,
            double maxRetailPrice,
            double special_price,
            double warning_quantity) const;
};

} /* item_management */

#endif /* ! __hh_size_definition_factory__ */

