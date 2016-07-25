#ifndef h_menu_key_generator
#define h_menu_key_generator

#include "i_generator.h"

class menu_key_generator_t : public i_generator<__int32>
{
   public:

      operator __int32() const;

      __int32 operator()();

      static menu_key_generator_t* Instance()
      {
         if(!instance)
            instance = new menu_key_generator_t(-2, -1);

         return instance;
      }

      void Reset( const __int32 initializer, const __int16 step_value );

   private:
      __int32 current_key_value_;
      __int32 key_step_value_;

      static menu_key_generator_t* instance;

      menu_key_generator_t(const __int32 initializer,
                           const __int16 key_step_value = 1);

}; /* menu_key_generator_t */

#endif /* ! h_menu_key_generator */
 