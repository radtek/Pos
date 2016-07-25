#ifndef __hh_i_item_definition_factory__
#define __hh_i_item_definition_factory__

#include <vector>

#include <key_types.hh>
#include <string_type.hh>

namespace item_management {

class i_item_definition;
class i_size_definition;

class i_item_definition_factory {
public:
        virtual ~i_item_definition_factory();

        /* Builders (Getters) */
        virtual i_item_definition *create(
            const i_item_definition &initializer) const = 0; 
        virtual i_item_definition *create(
            const string_type &name,
            std::vector<i_size_definition *> &size_definitions,
            item_key_type key) const = 0;

protected:
        i_item_definition_factory();
};

} /* item_management */

#endif /* ! __hh_i_item_definition_factory__ */

