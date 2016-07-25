#ifndef h_i_generator
#define h_i_generator

template<typename return_type_t>
class i_generator {
public:
   ~i_generator() { };

   virtual operator return_type_t() const = 0;

   virtual return_type_t operator()() = 0;

   virtual void Reset(const return_type_t, const __int16) = 0;

protected:
   i_generator() { };
}; /* i_generator */

#endif /* ! h_i_generator */
