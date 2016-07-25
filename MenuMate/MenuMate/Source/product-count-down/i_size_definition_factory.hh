#ifndef __hh_i_size_definition_factory__
#define __hh_i_size_definition_factory__

#include <string_type.hh>
#include <key_types.hh>

namespace item_management {

class i_size_definition;

class i_size_definition_factory {
public:
        virtual ~i_size_definition_factory();

        virtual i_size_definition *create() const = 0; 
        virtual i_size_definition *create(
            size_key_type key) const = 0; 
        virtual i_size_definition *create(
            const i_size_definition &initializer) const = 0; 
        virtual i_size_definition *create(
            double available_quantity,
            double default_quantity,
            bool enabled,
            size_key_type key,
            const string_type &name,
            double price,
            double maxRetailPrice,
            double special_price,
            double warning_quantity) const;

protected:
        i_size_definition_factory();
};

} /* item_management */

#endif /* ! __hh_i_size_definition_factory__ */


