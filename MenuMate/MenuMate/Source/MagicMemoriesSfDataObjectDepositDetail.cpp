#ifndef __c_magic_memories_sf_object_deposit_detail__
#define __c_magic_memories_sf_object_deposit_detail__

class _Sf_deposit_detail : _Sf_data_object {
public:
   _Sf_deposit_detail(_Sf_bridge &bridge,
                      double calculated_value,
	                   double counted_value,
	                   UnicodeString deposit_id,
	                   UnicodeString payment_type_id);
   ~_Sf_deposit_detail() { };

   operator UnicodeString();

private:
   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   double calculated_value;
   double counted_value;
   UnicodeString deposit_id;
   UnicodeString payment_type_id;
};

_Sf_deposit_detail::_Sf_deposit_detail(
  _Sf_bridge &bridge,
  double calculated_value,
  double counted_value,
  UnicodeString deposit_id,
  UnicodeString payment_type_id)
  : _Sf_data_object(bridge, Sf_data_object_type::DEPOSIT_DETAIL),
    calculated_value(calculated_value),
    counted_value(counted_value),
    deposit_id(deposit_id),
    payment_type_id(payment_type_id)
{
}

sObject *
_Sf_deposit_detail::create_object()
{
   Deposit_Detail__c_ *dd = new Deposit_Detail__c_();

   dd->Deposit__c = deposit_id;
   dd->Deposit_Value__c  = counted_value;
   dd->Menumate_Value__c = calculated_value;
   dd->Payment_Type__c = payment_type_id;

	return dd;
}

_Sf_deposit_detail::operator const UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object(Nil_string);
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_deposit_detail::pull_id_from_cache(UnicodeString &destination)
{
	return false;
}

const UnicodeString
_Sf_deposit_detail::push_id_to_cache(const UnicodeString &id)
{
	return id;
}

#endif

