#ifndef __c_magic_memories_sf_bridge__
#define __c_magic_memories_sf_bridge__

class _Sf_bridge {
public:
   _Sf_bridge(Sf_notification_receiver *notification_receiver,
              UnicodeString passwd,
              UnicodeString user);
   ~_Sf_bridge();

   Sf_result login();
   void      logout();

   auto_ptr<SaveResult> create_object (sObject *object);
   Array_Of_SaveResult  create_objects(Array_Of_sObject &objects);
   bool                 delete_objects(Array_Of_ID &ids);
   auto_ptr<SaveResult> update_object (sObject *object);
   Array_Of_SaveResult  update_objects(Array_Of_sObject &objects);

   UnicodeString escape_string(UnicodeString string);
   UnicodeString get_oid(const UnicodeString query);

   void send_authenticated_notification();
   void send_authenticating_notification();
   void send_authentication_failed_notification();

   void send_upload_failed_notification(Sf_data_object_type object_type);
   void send_uploading_notification    (Sf_data_object_type object_type);
   void send_uploaded_notification     (Sf_data_object_type object_type);

private:
   _Sf_bridge(const _Sf_bridge &initializer);
   _Sf_bridge &operator=(const _Sf_bridge &prototype);

   void send_headers();

   THTTPRIO                 *req_io;
   _di_Soap                 binding;
   Sf_notification_receiver *notification_receiver;
   UnicodeString            passwd;
   list<TSOAPHeader *>      prefix_headers;
   UnicodeString            user;
};

_Sf_bridge::_Sf_bridge(
  Sf_notification_receiver *notification_receiver,
  UnicodeString passwd,
  UnicodeString user)
  : req_io(new THTTPRIO(0x0)),
    binding(GetSoap(false, Nil_string, req_io)),
    notification_receiver(notification_receiver),
    passwd(passwd),
    user(user)
{
}

_Sf_bridge::~_Sf_bridge()
{
   list<TSOAPHeader *>::iterator       i = prefix_headers.begin();
   list<TSOAPHeader *>::const_iterator j = prefix_headers.end();

   while (i != j)
      delete *i++;

   prefix_headers.clear();
}

auto_ptr<SaveResult>
_Sf_bridge::create_object(sObject *object)
{
   Array_Of_sObject aso;

   aso.set_length(1);
   aso[0] = object;

   return auto_ptr<SaveResult>((create_objects(aso))[0]);
}

Array_Of_SaveResult
_Sf_bridge::create_objects(Array_Of_sObject &objects)
{
   send_headers();
   return binding->create(objects);
}

bool
_Sf_bridge::delete_objects(Array_Of_ID &ids)
{
   Array_Of_DeleteResult adr;
   int  i = 0;
   bool success = 1;

   send_headers();
   adr = binding->delete_(ids);

   for ( ; i < adr.Length; i++) {
      success &= adr[i]->success;
      delete adr[i], adr[i] = 0x0;
   }

   adr.Length = 0;
   return success;
}

UnicodeString
_Sf_bridge::escape_string(UnicodeString string)
{
   wchar_t       *strr_pos = string.w_str();
   wchar_t       *strr_end = strr_pos + string.Length();
   wchar_t       *strw_buf = new wchar_t[string.Length() << 1];
   wchar_t       *strw_pos = strw_buf;
   UnicodeString escaped_str;

   for ( ; strr_pos < strr_end; *strw_pos++ = *strr_pos++)
      if (*strr_pos == '\\' || *strr_pos == '\'')
         *strw_pos++ = '\\';

   escaped_str = UnicodeString(strw_buf, strw_pos - strw_buf);
   return delete[] strw_buf, escaped_str;
}

UnicodeString
_Sf_bridge::get_oid(const UnicodeString query)
{
   auto_ptr<QueryResult> qr;

   if (!query.Length())
      return Nil_string;

   send_headers();
   qr.reset(binding->query(query));

   return qr->size ? qr->records[0]->Id : Nil_string;
}

Sf_result
_Sf_bridge::login()
{
   SessionHeader         *shdr;
   auto_ptr<LoginResult> lr;

   send_authenticating_notification();

   lr.reset(binding->login(user, passwd));
   req_io->URL = lr->serverUrl;

   shdr = new SessionHeader();
   shdr->sessionId = lr->sessionId;

   prefix_headers.push_back(shdr);

   send_authenticated_notification();

   return Sf_result::SUCCESS;
}

void
_Sf_bridge::logout()
{
   send_headers();

   binding->logout();
}

void
_Sf_bridge::send_authenticated_notification()
{
   if (notification_receiver)
      notification_receiver->push_notification(
         Sf_notification::AUTHENTICATION_SUCCESS);
}

void
_Sf_bridge::send_authentication_failed_notification()
{
   if (notification_receiver)
       notification_receiver->push_notification(
         Sf_notification::AUTHENTICATION_FAILURE);
}

void
_Sf_bridge::send_authenticating_notification()
{
   if (notification_receiver)
       notification_receiver->push_notification(
         Sf_notification::AUTHENTICATING);
}

void
_Sf_bridge::send_upload_failed_notification(Sf_data_object_type object_type)
{
   if (notification_receiver)
      notification_receiver->push_notification(
        Sf_notification::UPLOAD_FAILURE, object_type);
}

void
_Sf_bridge::send_uploaded_notification(Sf_data_object_type object_type)
{
   if (notification_receiver)
      notification_receiver->push_notification(
        Sf_notification::UPLOAD_SUCCESS, object_type);
}

void
_Sf_bridge::send_uploading_notification(Sf_data_object_type object_type)
{
   if (notification_receiver)
      notification_receiver->push_notification(Sf_notification::UPLOADING,
                                               object_type);
}

void
_Sf_bridge::send_headers()
{
   list<TSOAPHeader *>::const_iterator i = prefix_headers.begin();
   list<TSOAPHeader *>::const_iterator j = prefix_headers.end();

   while (i != j)
      req_io->SOAPHeaders->Send(*i++);
}

auto_ptr<SaveResult>
_Sf_bridge::update_object(sObject *object)
{
   Array_Of_sObject aso;

   aso.Length = 1;
   aso[0] = object;

   return auto_ptr<SaveResult>((update_objects(aso))[0]);
}

Array_Of_SaveResult
_Sf_bridge::update_objects(Array_Of_sObject &objects)
{
   send_headers();
   return binding->update(objects);
}


#endif

