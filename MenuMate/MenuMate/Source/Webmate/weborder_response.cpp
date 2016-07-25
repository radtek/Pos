#include <sstream>

#include "weborder_response.hh"

weborder_response_t::weborder_response_t()
  : order_id_(L"Unknown"),
    message_(L"No message specified"),
    result_(weborder_response_t::R_ERROR)
{
}

weborder_response_t::weborder_response_t(
  const std::wstring order_id,
  const std::wstring message,
  const result_t result)
  : order_id_(order_id),
    message_(message),
    result_(result)
{
}

weborder_response_t::operator TXMLDocument *()
const
{
    std::auto_ptr<TXMLDocument> xml(new TXMLDocument(NULL));
    Xmlintf::_di_IXMLNode order_result;
    std::wostringstream result_as_string;

    result_as_string << static_cast<int>(result_);

    xml->Active = true;

    order_result = xml->CreateElement(L"ORDERRESULT", L"");
    order_result->SetAttribute(L"message",
                               UnicodeString(message_.c_str()));
    order_result->SetAttribute(L"orderid",
                               UnicodeString(order_id_.c_str()));
    order_result->SetAttribute(L"status",
                               UnicodeString(result_as_string.str().c_str()));

    xml->DocumentElement = order_result;
    return xml.release();
}

const std::wstring &
weborder_response_t::order_id()
const
{
    return order_id_;
}

const std::wstring &
weborder_response_t::message()
const
{
    return message_;
}

weborder_response_t::result_t
weborder_response_t::result()
const
{
    return result_;
}

const std::wstring &
weborder_response_t::order_id(const std::wstring order_id)
{
    return order_id_ = order_id;
}

const std::wstring &
weborder_response_t::message(const std::wstring message)
{
    return message_ = message;
}

weborder_response_t::result_t
weborder_response_t::result(const result_t result)
{
    return result_ = result;
}
