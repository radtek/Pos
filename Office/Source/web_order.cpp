#include "web_order.h"

#include <ctime>
#include <rpc.h>
#include <sstream>
#include <memory>

using std::stringstream;
using std::time_t;
using std::tm;
using std::auto_ptr;

list<TiXmlElement *>web_order::sentinel;

web_order::web_order(
  string date_required,
  const vector<string> delivery_address,
  const vector<string> instructions,
  string from,
  string purchase_order_no,
  bool use_office_product_price)
  :  date_required(date_required),
     delivery_address(delivery_address),
     from(from),
     gst_total(0),
     instructions(instructions),
     order_total(0),
     purchase_order_no(purchase_order_no),
     use_office_product_price(use_office_product_price)

{
}

web_order::~web_order()
{
}

void
web_order::add_item(string name, string size, string stock_code,
                    float price, float gst_percent, unsigned int quantity)
{
    codes.push_back (stock_code);
    names.push_back (name);
    prices.push_back(translate(price));
    sizes.push_back (size);
    quantities.push_back(translate(quantity));

    order_total += quantity * price;
    gst_total += (quantity * price * gst_percent) / 100;
}

bool
web_order::check_order_response_valid(string response_raw,
                                      string &errstr)
{
    string        guid;
    string        message;
    TiXmlElement  *order;
    TiXmlDocument response;
    int           result;

    response.Parse(response_raw.c_str());
    if (response.Error()) {
        errstr =
          "We've encountered an error parsing the response.";
        return false;
    }

    if (!(order = response.FirstChildElement("OrderResult"))) {
        errstr = "The response does not contain a result.";
        return false;
    }

    if ((guid = order->Attribute("OrderID")) != order_guid) {
        errstr = "We've received a response for a different order.";
        return false;
    }

    if (!(message = order->Attribute("Msg")).length()) {
        errstr = "The response contains no informational message.";
        return false;
    }

    if (!order->Attribute("Status", &result)) {
        errstr = "The response contains no status field.";
        return false;
    } else if (result != 255) {
        errstr = message +  "\r\rOrderID = " + guid;
        return false;
    }

    return true;
}

TiXmlElement *
web_order::create_account(TiXmlElement *payments_element)
{
    TiXmlElement *account = new TiXmlElement("ACCOUNT");

    account->SetAttribute("COVERS",        1);
    account->SetAttribute("MEMBER_NUMBER", 0);
    account->SetAttribute("NAME",          from);

    account->LinkEndChild(payments_element);
    account->LinkEndChild(create_delivery_details());

    list<string>::iterator code = codes.begin();
    list<string>::iterator name = names.begin();
    list<string>::iterator price = prices.begin();
    list<string>::iterator size = sizes.begin();
    list<string>::iterator quantity = quantities.begin();

    for ( ; code != codes.end(); ++code) {
        account->LinkEndChild(create_product(*name,
                                             *code,
                                             *price,
                                             *size,
                                             *quantity));
    }

    return account;
}

TiXmlElement *
web_order::create_delivery_details()
{
    TiXmlElement *delivery = new TiXmlElement("DELIVERY");
    TiXmlElement *to;

    for (int i = 0, j = delivery_address.size(); i < j; i++) {
        to = new TiXmlElement("TO");
        delivery->LinkEndChild(
          (to->LinkEndChild(new TiXmlText(delivery_address[i])), to));
    }

    return delivery;
}

TiXmlElement *
web_order::create_from()
{
    TiXmlElement *from = new TiXmlElement("FROM");
    TiXmlElement *name = new TiXmlElement("NAME");

    from->SetAttribute("ID", this->from);
    name->LinkEndChild(new TiXmlText(this->from));
    return from->LinkEndChild(name), from;
}

list<string>
web_order::create_info()
{
    list<string> info;

    info.push_back("Stock Purchase Order No. " + purchase_order_no);
    info.push_back("");
    info.push_back("Delivery Address:");

    for (int i = 0, j = delivery_address.size(); i < j; i++)
        info.push_back(delivery_address[i]);

    info.push_back("");
    info.push_back("Special Instructions:");
    for (int i = 0, j = instructions.size(); i < j; i++)
        info.push_back(instructions[i]);
    info.push_back("");

    return info;
}

TiXmlElement *
web_order::create_order()
{
    TiXmlElement *order = new TiXmlElement("ORDER");

    order_guid = gen_guid();

    order->SetAttribute("GUID",          order_guid);
    order->SetAttribute("EXPECTED_DATE", date_required);
    order->SetAttribute("ORDER_TOTAL",   translate(order_total + gst_total));
    order->SetAttribute("ORDER_DATE",    get_timestamp_string());
    order->SetAttribute("RESPOND",       "true");
    order->SetAttribute("SCHEDULED",     "0");
    order->SetAttribute("STORE_NAME",    from);

    order->LinkEndChild(create_from());

    order->LinkEndChild(
      create_account(create_payments(create_info())));

    return order;
}

TiXmlElement *
web_order::create_payment(string amount, string method)
{
    TiXmlElement *payment = new TiXmlElement("PAYMENT");
    TiXmlElement *name    = new TiXmlElement("NAME");
    TiXmlElement *value   = new TiXmlElement("VALUE");

    name->LinkEndChild(new TiXmlText(method));
    value->LinkEndChild(new TiXmlText(amount));

    payment->LinkEndChild(name);
    return payment->LinkEndChild(value), payment;
}

TiXmlElement *
web_order::create_payments(list<string> &info,
                           list<TiXmlElement *> payments)
{
    int i = 0, j = info.size();

    TiXmlElement *payment_info = new TiXmlElement("PAYMENT_INFO");
    TiXmlElement *_payments    = new TiXmlElement("PAYMENTS");

    for (TiXmlElement *_info; i < j; i++) {
        _info = new TiXmlElement("INFO");
        _info->LinkEndChild(new TiXmlText(info.front()));
        payment_info->LinkEndChild(_info);
        info.pop_front();
    }

    _payments->LinkEndChild(payment_info);
    while (payments.size()) {
        _payments->LinkEndChild(payments.front());
        payments.pop_front();
    }

    return _payments;
}

TiXmlElement *
web_order::create_product(string name, string code, string price,
                          string size, string quantity)
{
    static const string pricelevel[2] = {
        "0", "-1"
    };

    TiXmlElement *product = new TiXmlElement("PRODUCT");
    TiXmlElement *temp;

    product->SetAttribute("GUID", gen_guid());

    temp = new TiXmlElement("PRODUCT_NAME");
    temp->LinkEndChild(new TiXmlText(name));
    product->LinkEndChild(temp);

    temp = new TiXmlElement("SIZE");
    temp->LinkEndChild(new TiXmlText(size));
    product->LinkEndChild(temp);

    temp = new TiXmlElement("PRODUCT_PLU");
    temp->LinkEndChild(new TiXmlText(code));
    product->LinkEndChild(temp);

    temp = new TiXmlElement("PRICELEVEL");
    temp->LinkEndChild(
      new TiXmlText(pricelevel[use_office_product_price]));
    product->LinkEndChild(temp);
    if (use_office_product_price) {
        temp = new TiXmlElement("PRODUCT_CHARGE_PRICE");
        temp->LinkEndChild(new TiXmlText(price));
        product->LinkEndChild(temp);
    }

    temp = new TiXmlElement("QTY");
    temp->LinkEndChild(new TiXmlText(quantity));

    return product->LinkEndChild(temp), product;
}

string
web_order::gen_guid()
{
    union {
        char *c;
        unsigned char *uc;
    } un;

    string gstr;
    UUID   guid;

    UuidCreate(&guid);
    UuidToString(&guid, &un.uc);

    RpcStringFree((gstr = un.c, &un.uc));
    return gstr;
}

string
web_order::get_timestamp_string()
{
    char   buf[20];
    time_t time;
    struct tm *tm;

    std::time(&time);
    tm = std::localtime(&time);
    strftime(buf, 20, "%Y-%m-%d %H:%M:%S", tm);

    return buf;
}

const string
web_order::serialize() const
{
    std::stringstream buf;
	std::auto_ptr<TiXmlDocument> order(new TiXmlDocument());
    
    order->LinkEndChild(new TiXmlDeclaration("1.0", "UTF-8", "yes"));
    order->LinkEndChild(create_order());
    buf << *order;

    return buf.str();
}

string
web_order::translate(float value)
{
    char strbuf[16];

    snprintf(strbuf, 16, "%01.2f", value);
    return strbuf;
}

string
web_order::translate(unsigned int value)
{
    unsigned long i = value;

    return translate(i);
}

string
web_order::translate(unsigned long value)
{
    char strbuf[32];

    snprintf(strbuf, 32, "%d", value);
    return strbuf;
}

