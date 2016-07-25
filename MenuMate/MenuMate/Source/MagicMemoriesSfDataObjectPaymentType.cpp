#ifndef __c_magic_memories_sf_object_payment_type__
#define __c_magic_memories_sf_object_payment_type__

class _Sf_payment_type : _Sf_data_object {
public:
   _Sf_payment_type(_Sf_bridge &bridge, UnicodeString payment_type);
   ~_Sf_payment_type() { };

   operator UnicodeString();

   static string_map *set_cache(string_map *_cache);

private:
   static string_map *cache;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   UnicodeString payment_type;
};

string_map *_Sf_payment_type::cache = NULL;

_Sf_payment_type::_Sf_payment_type(
  _Sf_bridge &bridge,
  UnicodeString payment_type)
  : _Sf_data_object(bridge, Sf_data_object_type::DEPOSIT),
    payment_type(payment_type)
{
   assert(cache);
}

sObject *
_Sf_payment_type::create_object()
{
   Payment_Type__c_ *pt = new Payment_Type__c_();

   pt->Name = payment_type;

   return pt;
}

_Sf_payment_type::operator const UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from Payment_Type__c where Name = '"
                            + payment_type 
                            + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_payment_type::pull_id_from_cache(UnicodeString &destination)
{
   string_map::const_iterator i = cache->find(payment_type);

   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_payment_type::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[payment_type] = id;
}

string_map *
_Sf_payment_type::set_cache(string_map *_cache)
{
   string_map *old_cache = cache;
   assert(_cache);
   return cache = _cache, old_cache;
}

#endif

