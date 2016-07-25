#ifndef _h_web_order_integration_
#define _h_web_order_integration_

#include <string>
#include <list>
#include <vector>

#include <tinyxml.h>

using std::list;
using std::string;
using std::vector;

class web_order {
public:
    web_order(string date_required,
              const vector<string> delivery_address,
              const vector<string> instructions,
              string from,
              string purchase_order_no,
              bool use_office_product_price);
    ~web_order();

    void add_item(string name, string size,
                  string stock_code, float price,
                  float gst_percent,
                  unsigned int quantity);
    bool  check_order_response_valid(string response_raw,
                                     string &errstr);
    const string serialize() const;
private:
    web_order(const web_order &);
    web_order &operator=(const web_order &);

    TiXmlElement *create_account(TiXmlElement *payments_element);
    TiXmlElement *create_delivery_details();
    TiXmlElement *create_from();
    list<string> create_info();
    TiXmlElement *create_order();
    TiXmlElement *create_payment(string amount, string method);
    TiXmlElement *create_payments(list<string> &info,
                                  list<TiXmlElement *> payments = sentinel);
    TiXmlElement *create_product(string name, string code, string price,
                                 string size, string quantity);
    string gen_guid();
    string get_timestamp_string();

    string translate(float value);
    string translate(unsigned int value);
    string translate(unsigned long value);

    string         date_required;
    vector<string> delivery_address;
    string         from;
    float          gst_total;
    vector<string> instructions;
    string         order_guid;
    float          order_total;
    string         purchase_order_no;
    bool           use_office_product_price;

    list<string> codes;
    list<string> names;
    list<string> prices;
    list<string> sizes;
    list<string> quantities;

    static list<TiXmlElement *> sentinel;
};

#endif /* _h_web_order_integration_ */

