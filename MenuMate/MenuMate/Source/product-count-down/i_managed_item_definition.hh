#ifndef __hh_i_managed_item_definition__
#define __hh_i_managed_item_definition__

#include <i_item_definition.hh>

namespace item_management {

class i_managed_item_definition : public i_item_definition {
public:
        i_managed_item_definition();
        virtual ~i_managed_item_definition();

        virtual i_managed_item_definition &operator=(
            const i_managed_item_definition &rhs) = 0;

        virtual i_item_definition &get_item_definition() const = 0;
};

} /* item_management */

#endif /* ! __hh_i_managed_item_definition__ */

