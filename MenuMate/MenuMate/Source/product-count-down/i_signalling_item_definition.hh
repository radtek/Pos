#ifndef __hh_i_signalling_item_definition__
#define __hh_i_signalling_item_definition__

#include <i_managed_item_definition.hh>
#include <functors.hh>

namespace item_management {

class i_item_altered_signal_handler
    : public utilities::unary_functor<void, const i_item_definition &> {
public:
        virtual ~i_item_altered_signal_handler() { };

protected:
        i_item_altered_signal_handler() { };
};

class i_size_altered_signal_handler
    : public utilities::binary_functor<void,
                                       const i_item_definition &,
                                       const i_size_definition &> {
public:
        virtual ~i_size_altered_signal_handler() { };

protected:
        i_size_altered_signal_handler() { };
};

class i_size_warning_reached_signal_handler
    : public utilities::binary_functor<void,
                                       const i_item_definition &,
                                       const i_size_definition &> {
public:
        virtual ~i_size_warning_reached_signal_handler() { };

protected:
        i_size_warning_reached_signal_handler() { };
};


class i_signalling_item_definition : public virtual i_managed_item_definition {
public:
        i_signalling_item_definition();
        virtual ~i_signalling_item_definition();

        virtual i_signalling_item_definition &operator=(
            const i_signalling_item_definition &rhs) = 0;

        virtual const i_size_altered_signal_handler &
            get_size_altered_handler() const = 0;
        virtual const i_item_altered_signal_handler &
            get_item_altered_handler() const = 0;
        virtual const i_size_warning_reached_signal_handler &
            get_size_warning_reached_handler() const = 0;

        virtual bool quietly_set_enabled(bool enabled) = 0;

        virtual bool quietly_update_size_definition(
            size_key_type key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity) = 0;
};

} /* item_management */

#endif /* ! __hh_i_signalling_item_definition__ */
