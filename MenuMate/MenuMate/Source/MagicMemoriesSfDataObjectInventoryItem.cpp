#ifndef __c_magic_memories_sf_object_inventory_item__
#define __c_magic_memories_sf_object_inventory_item__

class _Sf_inventory_item : _Sf_data_object {
public:
   _Sf_inventory_item(
     _Sf_bridge &bridge,
     const Currency &average_cost,
     const Currency &latest_cost,
     double quantity_available,
     const UnicodeString day_id,
     const UnicodeString name,
     const UnicodeString stock_code)
     : _Sf_data_object(bridge, Sf_data_object_type::INVENTORY_ITEM),
       average_cost(average_cost),
       latest_cost(latest_cost),
       quantity_available(quantity_available),
       day_id(day_id),
       name(name),
       stock_code(stock_code)
   {
   };

   ~_Sf_inventory_item() { };

   operator UnicodeString()
   {
      UnicodeString s;

      bridge.send_uploading_notification(type());
      s = get_or_create_object(Nil_string);
      bridge.send_uploaded_notification(type());
      return s;
   };

private:
   sObject *create_object()
   {
		PLU_Stock__c *s = new PLU_Stock__c();

      s->Average_Cost__c    = average_cost;
      s->Day__c             = day_id;
      s->Last_Cost_Price__c = latest_cost;
      s->Name               = name;
      s->Quantity__c        = quantity_available;
      s->Stock_Code__c      = stock_code;

      return s;
   };

   const UnicodeString get_or_create_object(const UnicodeString oid_query)
   {
      UnicodeString        id;
      auto_ptr<sObject>    so(create_object());
      auto_ptr<SaveResult> sr;

      if ((id = bridge.get_oid(oid_query)) == Nil_string)
         sr = bridge.create_object(so.get());
      else {
         so->Id = id;
         bridge.update_object(so.get());
      }

      assert(sr->success);

      return sr->success ? id : Nil_string;
   };

   /* This'll never be used but we have to define it. */
   bool pull_id_from_cache(UnicodeString &destination)
   {
      const bool not_required_and_unimplemented = 1;

      assert(not_required_and_unimplemented == 0);

      return false;
   };

   const UnicodeString push_id_to_cache(const UnicodeString &id)
   {
      return id;
   };

   const Currency average_cost;
   const Currency latest_cost;
   const double   quantity_available;

   const UnicodeString day_id;
   const UnicodeString name;
   const UnicodeString stock_code;
};

#endif

