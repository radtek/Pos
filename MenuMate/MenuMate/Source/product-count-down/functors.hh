#ifndef __hh_functors__
#define __hh_functors__

namespace utilities
{

template<typename return_type, typename argument_a_type, typename argument_b_type>
class binary_functor
{
public:
    virtual ~binary_functor() { };

    virtual return_type operator()(argument_a_type argument_a, argument_b_type argument_b) const = 0;

protected:
    binary_functor() { };
};

template<typename return_type>
class nullary_functor
{
public:
    virtual ~nullary_functor() { };

    virtual return_type operator()() const = 0;

protected:
    nullary_functor() { };
};

template<typename return_type, typename argument_type>
class unary_functor {
public:
    virtual ~unary_functor() { };

    virtual return_type operator()(argument_type argument) const = 0;

protected:
    unary_functor() { };
};

} /* utilities */

#endif /* ! __hh_functors__ */

