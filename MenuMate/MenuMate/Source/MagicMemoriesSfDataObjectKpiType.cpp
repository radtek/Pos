#ifndef __c_magic_memories_sf_object_kpi_type__
#define __c_magic_memories_sf_object_kpi_type__

class _Sf_kpi_type : _Sf_data_object {
public:
   _Sf_kpi_type(_Sf_bridge &bridge, UnicodeString name);
   ~_Sf_kpi_type() { };

   operator UnicodeString();

   static string_map *set_cache(string_map *_cache);

private:
   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   UnicodeString name;
   UnicodeString site_id;

   static string_map *cache;
};

string_map *_Sf_kpi_type::cache    = NULL;

_Sf_kpi_type::_Sf_kpi_type(_Sf_bridge &bridge, UnicodeString name)
  : _Sf_data_object(bridge, Sf_data_object_type::KPI_TYPE), name(name)
{
   assert(cache);
}

sObject *
_Sf_kpi_type::create_object()
{
   KPI__c_ *o = new KPI__c_();

   o->Name = name;
   return o;
}

string_map *
_Sf_kpi_type::set_cache(string_map *_cache)
{
   string_map *old_cache = cache;
   assert(_cache);
   return cache = _cache, old_cache;
}

_Sf_kpi_type::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object(
     "select Id from KPI__c where Name = '" + name + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_kpi_type::pull_id_from_cache(UnicodeString &destination)
{
   string_map::const_iterator i = cache->find(name);

   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_kpi_type::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[name] = id;
}

#endif

