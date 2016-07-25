#include <string_type.hh>

#ifndef __BCPLUSPLUS__
        #include <iomanip>
        #include <sstream>
#endif /* ! __BCPLUSPLUS__ */

string_type
double_to_str(double value)
{
#ifdef __BCPLUSPLUS__
        return UnicodeString::FormatFloat("0.00", value);
#else /* __BCPLUSPLUS__ */
        std::wostringstream output;

        output << std::setprecision(2) << std::fixed << value;

        return output.str();
#endif /* ! __BCPLUSPLUS__ */
}

string_type
long_to_str(long value)
{
#ifdef __BCPLUSPLUS__
        return UnicodeString(value);
#else /* __BCPLUSPLUS__ */
        std::wostringstream output;

        output << value;

        return output.str();
#endif /* ! __BCPLUSPLUS__ */
}

string_type
ulong_to_str(unsigned long value)
{
#ifdef __BCPLUSPLUS__
        return UnicodeString(value);
#else /* __BCPLUSPLUS__ */
        std::wostringstream output;

        output << value;

        return output.str();
#endif /* ! __BCPLUSPLUS__ */
}

string_type
generate_decorated_name(
    const string_type &base_name,
    const string_type &opening_brace,
    const string_type &closing_brace,
    double available_quantity)
{
        const string_type space(L" ");

        return base_name
               + space
               + opening_brace
               + space
               + double_to_str(available_quantity)
               + space
               + closing_brace;
}

