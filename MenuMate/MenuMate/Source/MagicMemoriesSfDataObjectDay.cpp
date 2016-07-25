#ifndef __c_magic_memories_sf_object_day__
#define __c_magic_memories_sf_object_day__

class _Sf_day : _Sf_data_object {
public:
   _Sf_day(_Sf_bridge &bridge, TDate date, UnicodeString site_id);
   ~_Sf_day() { };

   operator UnicodeString();

   static date_map *set_cache(date_map *_cache);

private:
   static date_map *cache;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);

   TDate         date;
   UnicodeString site_id;
};

date_map   *_Sf_day::cache         = NULL;

_Sf_day::_Sf_day(_Sf_bridge &bridge, TDate date, UnicodeString site_id)
  : _Sf_data_object(bridge, Sf_data_object_type::DAY),
    date(date),
    site_id(site_id)
{
   assert(cache);
}

sObject *
_Sf_day::create_object()
{
   Day__c_ *d = new Day__c_();

   d->Date__c = new TXSDate();
   d->Date__c->AsDate = date;
   d->Name = date.FormatString("mmm d, yyyy");
   d->Site__c = site_id;

   return d;
}

_Sf_day::operator const UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from Day__c where Date__c = "
                            + date.FormatString("yyyy-mm-dd")
                            + " and Site__c = '"
                            + site_id + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

bool
_Sf_day::pull_id_from_cache(UnicodeString &destination)
{
   date_map::const_iterator i = cache->find(date);

   return i != cache->end() ? (destination = i->second, true) : false;
}

const UnicodeString
_Sf_day::push_id_to_cache(const UnicodeString &id)
{
   return (*cache)[date] = id;
}

date_map *
_Sf_day::set_cache(date_map *_cache)
{
   date_map *old_cache = cache;
   assert(_cache);
   return cache = _cache, old_cache;
}

#endif

