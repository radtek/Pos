#include "menu_key_generator.h"

menu_key_generator_t* menu_key_generator_t::instance = 0;

menu_key_generator_t::menu_key_generator_t(const __int32 initializer,
                                           const __int16 key_step_value)
  : current_key_value_(initializer),
    key_step_value_(key_step_value)
{
}

menu_key_generator_t::operator __int32()
const
{
   return current_key_value_;
}

__int32
menu_key_generator_t::operator()()
{
   const __int32 key_value = current_key_value_;

   return current_key_value_ += key_step_value_, key_value;
}

void
menu_key_generator_t::Reset( const __int32 initializer, const __int16 step_value )
{
   current_key_value_ = initializer;
   key_step_value_ = step_value;
}

