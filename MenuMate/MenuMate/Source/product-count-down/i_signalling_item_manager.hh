#ifndef __hh_i_signalling_item_manager__
#define __hh_i_signalling_item_manager__

#include <i_item_manager.hh>

namespace item_management {

class i_signalling_item_manager : public virtual i_item_manager {
public:
        i_signalling_item_manager();
        virtual ~i_signalling_item_manager();

        virtual void quietly_set_item_enabled(
            item_key_type key, bool enabled) = 0;
        virtual void quietly_update_size_definition(
            item_key_type item_key,
            size_key_type size_key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity) = 0;

private:
        i_signalling_item_manager(
            const i_signalling_item_manager &initializer);
        i_signalling_item_manager &operator=(
            const i_signalling_item_manager &rhs);
};

} /* item_management */

#endif /* ! __hh_i_signalling_item_manager__ */

