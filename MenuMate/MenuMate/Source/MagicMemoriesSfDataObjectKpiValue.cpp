#ifndef __c_magic_memories_sf_object_kpi_value__
#define __c_magic_memories_sf_object_kpi_value__

class _Sf_kpi_value : public _Sf_data_object {
public:
   _Sf_kpi_value(_Sf_bridge &bridge, UnicodeString day_id,
                 UnicodeString type_id, double value)
     : _Sf_data_object(bridge, Sf_data_object_type::KPI_VALUE),
        day_id(day_id),
        type_id(type_id),
        value(value)
   { };

   ~_Sf_kpi_value() { };

   operator UnicodeString();

private:
   sObject             *create_object();
   bool                does_object_already_exist(UnicodeString &destination);
   const UnicodeString get_or_create_object(const UnicodeString oid_query);

   UnicodeString day_id;
   UnicodeString type_id;
   bool          updating;
   double        value;
   UnicodeString value_id;
};

sObject *
_Sf_kpi_value::create_object()
{
   KPI_Value__c_ *o = new KPI_Value__c_();

   o->Value__c = value;
   if (value_id.Length())
      o->Id = value_id;
   else {
      o->Day__c = day_id;
      o->KPI__c = type_id;
   }

   return o;
}

_Sf_kpi_value::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object("select Id from KPI_Value__c "
                            "       where Kpi__c = '"
                            + type_id
                            + "' and Day__c = '"
                            + day_id
                            + "'");
   bridge.send_uploaded_notification(type());
   return s;
}

const UnicodeString
_Sf_kpi_value::get_or_create_object(const UnicodeString oid_query)
{
   auto_ptr<sObject> o;

   value_id = bridge.get_oid(oid_query);
   o.reset(create_object());

   return value_id.Length() ? (bridge.update_object(o.get()))->id
                            : (bridge.create_object(o.get()))->id;
}

#endif
