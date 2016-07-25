#ifndef __c_magic_memories_sf_object_deposit__
#define __c_magic_memories_sf_object_deposit__

class _Sf_deposit : _Sf_data_object {
public:
   _Sf_deposit(_Sf_bridge &bridge, UnicodeString bag_tag,
               UnicodeString day_id);
   ~_Sf_deposit() { };

   operator UnicodeString();

   static string_map *set_cache(string_map *_cache);

private:
   static string_map *cache;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   UnicodeString bag_tag;
   UnicodeString day_id;
};

string_map *_Sf_deposit::cache = NULL;

_Sf_deposit::_Sf_deposit(
  _Sf_bridge &bridge,
  UnicodeString bag_tag,
  UnicodeString day_id)
  : _Sf_data_object(bridge, Sf_data_object_type::DEPOSIT),
    bag_tag(bag_tag),
    day_id(day_id)
{
   assert(cache);
}

sObject *
_Sf_deposit::create_object()
{
   Deposit__c_ *d = new Deposit__c_();

   d->Day__c = day_id;
   d->Name = bag_tag;

   return d;
}

_Sf_deposit::operator const UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from Deposit__c where Day__c = '"
                            + day_id
                            + "' and Name = '"
                            + bag_tag
                            + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_deposit::pull_id_from_cache(UnicodeString &destination)
{
   string_map::const_iterator i = cache->find(bag_tag);

   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_deposit::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[bag_tag] = id;
}

string_map *
_Sf_deposit::set_cache(string_map *_cache)
{
   string_map *old_cache = cache;
   assert(_cache);
   return cache = _cache, old_cache;
}

#endif

