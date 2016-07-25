#ifndef hh_weborder_response
#define hh_weborder_response

#include <XMLDoc.hpp>

#include <string>
#include <memory>

class weborder_response_t {
public:
    enum result_t {
        R_ERROR,
        R_SUCCESS,
        R_WARNING
    }; /* result_t */

    weborder_response_t();
    weborder_response_t(const std::wstring order_id,
                        const std::wstring message,
                        const result_t result);

    /*
     * Ensure that CoInitialize(...) has been executed before calling this
     * method. Likewise, ensure that you call CoUninitialize(...) when you're
     * done using the TXMLDocument or Embarcadero's XML library in general.
     */
    operator TXMLDocument *() const;

    const std::wstring &order_id() const;
    const std::wstring &message() const;
    result_t result() const;

    const std::wstring &order_id(const std::wstring order_id);
    const std::wstring &message(const std::wstring message);
    result_t result(const result_t result);

private:
    std::wstring order_id_;
    std::wstring message_;
    result_t result_;
}; /* weborder_response_t */

#endif /* ! hh_weborder_response */
