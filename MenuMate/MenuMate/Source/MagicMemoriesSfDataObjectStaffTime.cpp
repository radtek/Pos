#ifndef __c_magic_memories_sf_object_staff_time__
#define __c_magic_memories_sf_object_staff_time__

class _Sf_staff_time : _Sf_data_object {
public:
   _Sf_staff_time(_Sf_bridge &bridge,
                  double break_time,
                  double hourly_rate,
                  double hours_worked,
                  TDateTime logged_in,
                  TDateTime logged_out,
                  UnicodeString day_id,
                  UnicodeString staff_id);
   ~_Sf_staff_time() { };

   operator UnicodeString();

private:
   double        break_time;
   TDateTime     end_time;
   double        hourly_rate;
   double        hours_worked;
   UnicodeString day_id;
   UnicodeString staff_id;
   TDateTime     start_time;

   sObject             *create_object();
   bool                pull_id_from_cache(UnicodeString &destination);
   const UnicodeString push_id_to_cache(const UnicodeString &id);
};

_Sf_staff_time::_Sf_staff_time(
  _Sf_bridge &bridge,
  double break_time,
  double hourly_rate,
  double hours_worked,
  TDateTime logged_in,
  TDateTime logged_out,
  UnicodeString day_id,
  UnicodeString staff_id)
  : _Sf_data_object(bridge, Sf_data_object_type::STAFF_TIME),
    break_time(break_time),
    end_time(logged_out),
    hourly_rate(hourly_rate),
    hours_worked(hours_worked),
    day_id(day_id),
    staff_id(staff_id),
    start_time(logged_in)
{
}

_Sf_staff_time::operator UnicodeString()
{
   UnicodeString s;

   bridge.send_uploading_notification(type());
   s = get_or_create_object(Nil_string);
   bridge.send_uploaded_notification(type());

   return s;
}

#pragma warn -par

sObject *
_Sf_staff_time::create_object()
{
   Time__c_ *o = new Time__c_();

   o->Day__c         = day_id;
   o->Breaktime__c   = break_time;
   o->End_Time__c    = new TXSDateTime();
   o->Hourly_Rate__c = hourly_rate;
   o->Hours__c       = hours_worked;
   o->Staff__c       = staff_id;
   o->Start_Date__c  = new TXSDateTime();

   o->End_Time__c->AsDateTime   = end_time;
   o->Start_Date__c->AsDateTime = start_time;

   return o;
}

bool
_Sf_staff_time::pull_id_from_cache(UnicodeString &destination)
{
   return false;
}

#pragma warn .par

const UnicodeString
_Sf_staff_time::push_id_to_cache(const UnicodeString &id)
{
   return id;
}

#endif
