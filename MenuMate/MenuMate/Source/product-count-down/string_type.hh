#ifndef __hh_string_type__
#define __hh_string_type__

#ifdef __BCPLUSPLUS__
        #include <System.hpp>

        typedef UnicodeString string_type;
#else /* __BCPLUSPLUS__ */
        #include <string>

        typedef std::wstring string_type;
#endif /* ! __BCPLUSPLUS__ */

string_type double_to_str(double value);
string_type long_to_str(long value);
string_type ulong_to_str(unsigned long value);

string_type generate_decorated_name(const string_type &base_name,
                                    const string_type &opening_brace,
                                    const string_type &closing_brace,
                                    double available_quantity);

#endif /* ! __hh_string_type__ */

