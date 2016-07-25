#ifndef __c_magic_memories_sf_object_product__
#define __c_magic_memories_sf_object_product__

class _Sf_product : _Sf_data_object {
public:
   _Sf_product(
     _Sf_bridge &bridge,
     const UnicodeString name,
     double plu,
     const UnicodeString site_id,
     const UnicodeString size)
     : _Sf_data_object(bridge, Sf_data_object_type::PRODUCT),
       key(name + site_id + size),
       name(name),
       plu(plu),
       site(site_id),
       size(size)
   {
      assert(cache);
   };

   ~_Sf_product() { };

   operator UnicodeString()
   {
      UnicodeString s;

      bridge.send_uploading_notification(type());
      s = get_or_create_object("select Id from Product__c "
                               "          where Name = '"
                               + name
                               + "' and Site__c = '"
                               + site
                               + "' and Size__c = '"
                               + size
                               + "'");
      bridge.send_uploaded_notification(type());
      return s;
   };

   static string_map *set_cache(string_map *_cache)
   {
      string_map *old_cache = cache;
      assert(_cache);
      return cache = _cache, old_cache;
   };

private:
   sObject *create_object()
   {
      Product__c_ *o = new Product__c_();

      o->Name    = name;
      o->PLU__c  = plu;
      o->Site__c = site;
      o->Size__c = size;

      return o;
   };

   bool pull_id_from_cache(UnicodeString &destination)
   {
      string_map::const_iterator i = cache->find(key);
      return i != cache->end() ? (destination = i->second, true) : false;
   };

   const UnicodeString push_id_to_cache(const UnicodeString &id)
   {
      return (*cache)[key] = id;
   };

   const UnicodeString key;
   const UnicodeString name;
   double              plu;
   const UnicodeString site;
   const UnicodeString size;

   static string_map *cache;
};

string_map *_Sf_product::cache = NULL;

#endif

