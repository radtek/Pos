#ifndef __c_magic_memories_sf_object_staff__
#define __c_magic_memories_sf_object_staff__

class _Sf_staff : _Sf_data_object {
public:
   _Sf_staff(_Sf_bridge &bridge, UnicodeString name, UnicodeString site_id,
             UnicodeString title_id);
   ~_Sf_staff() { };

   operator UnicodeString();

   static string_map *set_cache(string_map *cache);

private:
   static string_map *cache;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   UnicodeString key;
   UnicodeString name;
   UnicodeString site_id;
   UnicodeString title_id;
};

string_map *_Sf_staff::cache = NULL;

_Sf_staff::_Sf_staff(
  _Sf_bridge &bridge,
  UnicodeString name,
  UnicodeString site_id,
  UnicodeString title_id)
  : _Sf_data_object(bridge, Sf_data_object_type::STAFF),
    key(name + site_id + title_id),
    name(name),
    site_id(site_id),
    title_id(title_id)
{
   assert(cache);
}

sObject *
_Sf_staff::create_object()
{
   Staff__c_ *s = new Staff__c_();

   s->Name = name;
   s->Site__c = site_id;
   s->Staff_Title__c = title_id;

   return s;
}

_Sf_staff::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from Staff__c where Name = '"
                            + name
                            + "' and Site__c = '"
                            + site_id
                            + "' and Staff_Title__c = '"
                            + title_id
                            + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_staff::pull_id_from_cache(UnicodeString &destination)
{
   string_map::const_iterator i = cache->find(key);
   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_staff::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[key] = id;
}

string_map *
_Sf_staff::set_cache(string_map *_cache)
{
   string_map *orig_cache = cache;
   assert(_cache);
   return cache = _cache, orig_cache;
}

#endif
