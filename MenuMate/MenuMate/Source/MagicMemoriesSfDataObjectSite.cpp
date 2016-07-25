#ifndef __c_magic_memories_sf_object_site__
#define __c_magic_memories_sf_object_site__

class _Sf_site : public _Sf_data_object {
public:
   _Sf_site(_Sf_bridge &bridge, UnicodeString name);
   ~_Sf_site() { };

   operator UnicodeString();

   static string_map *set_cache(string_map *_cache);

private:
   static string_map *cache;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   UnicodeString name;
};

string_map *_Sf_site::cache = NULL;

_Sf_site::_Sf_site(_Sf_bridge &bridge, UnicodeString name)
  : _Sf_data_object(bridge, Sf_data_object_type::SITE), name(name)
{
   assert(cache);
}

sObject *
_Sf_site::create_object()
{
   Site__c_ *s = new Site__c_();

   s->Name = name;
   return s;
}

_Sf_site::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from Site__c where Name = '"
                            + bridge.escape_string(name)
                            + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_site::pull_id_from_cache(UnicodeString &destination)
{
   string_map::const_iterator i = cache->find(name);

   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_site::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[name] = id;
}

string_map *
_Sf_site::set_cache(string_map *_cache)
{
   string_map *old_cache = cache;
   assert(_cache);
   return cache = _cache, old_cache;
}

#endif

