#ifndef __c_magic_memories_sf_object_transaction__
#define __c_magic_memories_sf_object_transaction__

class _Sf_transaction : _Sf_data_object {
public:
   _Sf_transaction(_Sf_bridge &bridge,
                   const UnicodeString &day_id,
                   const UnicodeString &invoice_number,
                   const UnicodeString &staff_id,
                   const TDateTime transaction_datetime);
   ~_Sf_transaction() { };

   void add_line_item(const UnicodeString product_id, double price,
                      double quantity);

   operator UnicodeString();

private:
   Array_Of_SaveResult create_line_items();
   sObject             *create_object();
   const UnicodeString get_or_create_object(const UnicodeString oid_query);
   void                reparent_line_items(const UnicodeString &id);

   list<Line_Item__c_ *> line_items;

   const UnicodeString day_id;
   const UnicodeString invoice_number;
   const UnicodeString staff_id;
   const TDateTime     transaction_datetime;
};

_Sf_transaction::_Sf_transaction(
  _Sf_bridge &bridge,
  const UnicodeString &day_id,
  const UnicodeString &invoice_number,
  const UnicodeString &staff_id,
  const TDateTime transaction_datetime)
  : _Sf_data_object(bridge, Sf_data_object_type::TRANSACTION),
    day_id(day_id),
    invoice_number(invoice_number),
    staff_id(staff_id),
    transaction_datetime(transaction_datetime)
{
}

void
_Sf_transaction::add_line_item(
  const UnicodeString product_id,
  double price,
  double quantity)
{
   Line_Item__c_ *o = new Line_Item__c_();

   o->PLU_Product__c  = product_id;
   o->Price__c        = price;
   o->Quantity__c     = quantity;

   line_items.push_back(o);
}

Array_Of_SaveResult
_Sf_transaction::create_line_items()
{
   Array_Of_sObject ao;
   list<Line_Item__c_ *>::iterator       i = line_items.begin();
   list<Line_Item__c_ *>::const_iterator j = line_items.end();
   unsigned int k = 0;

   for (ao.Length = line_items.size(); i != j; )
      ao[k++] = *i++;

   return bridge.create_objects(ao);
}

sObject *
_Sf_transaction::create_object()
{
   Transaction__c_ *o = new Transaction__c_();

   o->Date__c = new TXSDateTime();
   o->Date__c->AsDateTime = transaction_datetime;
   o->Day__c = day_id;
   o->Invoice_Number__c = invoice_number;
   o->Staff__c = staff_id;

   return o;
}

#pragma warn -par

const UnicodeString
_Sf_transaction::get_or_create_object(const UnicodeString oid_query)
{
   auto_ptr<sObject>    o(create_object());
   auto_ptr<SaveResult> sr(bridge.create_object(o.get()));
   Array_Of_SaveResult  asr;
   unsigned int         i = 0;
   unsigned int         j;

   assert(sr->success);
   if (!sr->success)
      return Nil_string;

   /*
    * The order of creation is important. We need to create the
    * transaction first so to get the ID to link the Line Items to.
    */
   reparent_line_items(sr->id);

   j = (asr = create_line_items()).Length;

   /*
    * We have no reason to keep the array around. it's elements are not
    * automatically released as they themselves are just references. We
    * have to release them manually. While doing this, we might as well
    * check that all the Line Items were created successfully.
    */
   for (unsigned int k = 0; k < j; k++) {
      i += asr[k]->success;
      delete asr[k];
   }
   asr.Length = 0;

   assert(i == j);

   return sr->id;
}

#pragma warn .par

void
_Sf_transaction::reparent_line_items(const UnicodeString &id)
{
   list<Line_Item__c_ *>::iterator       i = line_items.begin();
   list<Line_Item__c_ *>::const_iterator j = line_items.end();

   for ( ; i != j; i++)
      (*i)->Transaction__c = id;
}

_Sf_transaction::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());

   s = get_or_create_object(Nil_string);

   bridge.send_uploaded_notification(type());
   return s;
}

#endif
