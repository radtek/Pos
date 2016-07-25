#ifndef __hh_utilities__
#define __hh_utilities__

namespace utilities {

template<typename type>
type
set_and_return_original_value(type &target, type new_value)
{
        type original_value = target;

        target = new_value;
        return original_value;
}

} /* utilities */

#endif /* ! __hh_utilities__ */

