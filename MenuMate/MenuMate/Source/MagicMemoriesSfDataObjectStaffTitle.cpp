#ifndef __c_magic_memories_sf_object_staff_title__
#define __c_magic_memories_sf_object_staff_title__

class _Sf_staff_title : public _Sf_data_object {
public:
   _Sf_staff_title(_Sf_bridge &bridge,
                   double commision_rate,
                   double hourly_rate,
                   UnicodeString site_id,
                   UnicodeString title);
   ~_Sf_staff_title() { };

   operator UnicodeString();

   static string_map *set_cache(string_map *_cache);

private:
   static string_map *cache;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   UnicodeString site_id;
   UnicodeString title;

   double commision_rate;
   double hourly_rate;
};

string_map *_Sf_staff_title::cache = NULL;

_Sf_staff_title::_Sf_staff_title(
  _Sf_bridge &bridge,
  double commision_rate,
  double hourly_rate,
  UnicodeString site_id,
  UnicodeString title)
  : _Sf_data_object(bridge, Sf_data_object_type::STAFF_TITLE),
    commision_rate(commision_rate),
    hourly_rate(hourly_rate),
    site_id(site_id),
    title(title)
{
   assert(cache);
}

sObject *
_Sf_staff_title::create_object()
{
   Staff_Title__c_ *t = new Staff_Title__c_();

   t->Name    = title;
   t->Site__c = site_id;
   t->Commision_Rate__c = commision_rate;
   t->Hourly_Rate__c = hourly_rate;

   return t;
}

_Sf_staff_title::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from Staff_Title__c where Name = '"
                            + title
                            + "' and Site__c = '"
                            + site_id + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_staff_title::pull_id_from_cache(UnicodeString &destination)
{
   string_map::const_iterator i = cache->find(title);
   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_staff_title::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[title] = id;
}

string_map *
_Sf_staff_title::set_cache(string_map *_cache)
{
   string_map *old_cache = cache;
   assert(_cache);
   return cache = _cache, old_cache;
}

#endif
