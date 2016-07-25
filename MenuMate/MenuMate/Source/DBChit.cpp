#include "DBChit.h"
#include "DeviceRealTerminal.h"

using Database::TDBTransaction;

db_chit_exception_t::db_chit_exception_t()
{
}

db_chit_exception_t::db_chit_exception_t(
  const db_chit_exception_t &initializer)
{
}

db_chit_exception_t::~db_chit_exception_t()
throw()
{
}

db_chit_exception_t &
db_chit_exception_t::operator=(const db_chit_exception_t &rhs)
{
    return *this;
}

const char *
db_chit_exception_t::what()
const
{
    return "";
}

chit_not_found_exception_t::chit_not_found_exception_t(const chit_name_t &name)
  : name_(name),
    key_(-1)
{
}

chit_not_found_exception_t::chit_not_found_exception_t(const chit_key_t key)
  : name_(L"no name specified."),
    key_(key)
{
}

chit_not_found_exception_t::chit_not_found_exception_t(
  const chit_not_found_exception_t &initializer)
  : db_chit_exception_t(initializer),
    name_(),
    key_()
{
    operator=(initializer);
}

chit_not_found_exception_t &
chit_not_found_exception_t::operator=(const chit_not_found_exception_t &rhs)
{
    if (this == &rhs)
        return *this;

    db_chit_exception_t::operator=(rhs);

    name_ = rhs.name_;
    key_ = rhs.key_;
    return *this;
}

const char *
chit_not_found_exception_t::what()
const
{
    return "The chit specified wasn't present in the database.";
}

const chit_name_t &
chit_not_found_exception_t::name()
const
{
    return name_;
}

const chit_key_t
chit_not_found_exception_t::key()
const
{
    return key_;
}

chit_not_found_exception_t::~chit_not_found_exception_t()
throw()
{
}

chit_option_not_found_exception_t::chit_option_not_found_exception_t(
  const chit_option_name_t &name)
  : name_(name),
    option_key_(-1)
{
}

chit_option_not_found_exception_t::chit_option_not_found_exception_t(
  const chit_option_key_t option_key)
  : name_(L"no name specified."),
    option_key_(option_key)
{
}

chit_option_not_found_exception_t::chit_option_not_found_exception_t(
  const chit_option_not_found_exception_t &initializer)
  : db_chit_exception_t(initializer)
{
    operator=(initializer);
}

chit_option_not_found_exception_t::~chit_option_not_found_exception_t()
throw()
{
}

chit_option_not_found_exception_t &
chit_option_not_found_exception_t::operator=(
  const chit_option_not_found_exception_t &rhs)
{
    if (this == &rhs)
        return *this;

    db_chit_exception_t::operator=(rhs);

    name_ = rhs.name_;
    option_key_ = rhs.option_key_;
    return *this;
}

const char *
chit_option_not_found_exception_t::what()
const
{
    return "The specified chit option couldn't be found.";
}

const chit_option_name_t &
chit_option_not_found_exception_t::name()
const
{
    return name_;
}

const chit_key_t
chit_option_not_found_exception_t::option_key()
const
{
    return option_key_;
}

void
update_chit_option_list(
const chit_key_t chit_key,
  const chit_option_name_set_t &to_be_added,
  const chit_option_name_set_t &to_be_removed,
  const chit_option_rename_map_t &to_be_renamed,
  TDBTransaction &tr)
{
    TIBSQL *q = tr.Query(tr.AddQuery());

    try {
        chit_option_name_set_t::const_iterator i = to_be_removed.begin();
        chit_option_name_set_t::const_iterator j = to_be_removed.end();

        q->SQL->Text = "delete from chitoptions "
                       "       where chitnumber_key = :chitnumber_key "
                       "             and name in (";
        if (to_be_removed.size()) {
            for(--j; i != j; ++i)
                q->SQL->Text = q->SQL->Text + "'" + i->c_str() + "',";
            q->SQL->Text = q->SQL->Text + "'" + i->c_str() + "');";

            q->ParamByName("chitnumber_key")->AsInteger = chit_key;
            q->ExecQuery();
        }

        q->SQL->Text = "update chitoptions "
                       "       set name = :new_name "
                       "where chitnumber_key = :chitnumber_key "
                       "      and name = :old_name;";
        for (chit_option_rename_map_t::const_iterator k = to_be_renamed.begin();
               k != to_be_renamed.end(); q->ExecQuery(), ++k) {
            q->ParamByName("chitnumber_key")->AsInteger = chit_key;
            q->ParamByName("new_name")->AsString = k->second.c_str();
            q->ParamByName("old_name")->AsString = k->first.c_str();
        }
            ;
        q->SQL->Text = "insert into chitoptions values ("
                       "    gen_id(gen_chitoptions, 1), "
                       "    :chitnumber_key, "
                       "    :name "
                       ");";
        for (i = to_be_added.begin(); i != to_be_added.end(); ++i) {
            q->ParamByName("chitnumber_key")->AsInteger = chit_key;
            q->ParamByName("name")->AsString = i->c_str();
            q->ExecQuery();
        }
    } catch (...) {
        q->Close();
    }
}

const chit_key_t
db_chit_t::key(const chit_name_t &name)
{
    chit_key_t key;

    if (!db_chit_t::key(name, key))
        throw chit_not_found_exception_t(name);

    return key;
}

const chit_name_t
db_chit_t::name(const chit_key_t key)
{
    chit_name_t name;

    if (!db_chit_t::name(key, name))
        throw chit_not_found_exception_t(key);

    return name;
}

bool
db_chit_t::key(const chit_name_t &name, chit_key_t &key)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());
    bool succeeded;

    q->SQL->Text = "select chitnumber_key "
                   "       from chitnumber "
                   "       where name = :name;";
    tr.StartTransaction();
        q->ParamByName("name")->AsString = name.c_str();
        if (q->ExecQuery(), (succeeded = q->RecordCount > 0))
            key = q->FieldByName("chitnumber_key")->AsInteger;
    tr.Commit();

    return succeeded;
}

bool
db_chit_t::name(const chit_key_t key, chit_name_t &name)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());
    bool succeeded;

    q->SQL->Text = "select name "
                   "       from chitnumber "
                   "       where chitnumber_key = :key;";
    tr.StartTransaction();
        q->ParamByName("key")->AsString = key;
        if (q->ExecQuery(), (succeeded = q->RecordCount > 0))
            name = q->FieldByName("name")->AsString.c_str();
    tr.Commit();

    return succeeded;
}

std::auto_ptr<chit_key_vector_t>
db_chit_t::keys()
{
    std::auto_ptr<chit_key_vector_t> keys(new chit_key_vector_t);
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    q->SQL->Text = "select chitnumber_key from chitnumber;";
    tr.StartTransaction();
        for (q->ExecQuery(); !q->Eof; q->Next())
            keys->push_back(q->FieldByName("chitnumber_key")->AsInteger);
    tr.Commit();

    return keys;
}

std::auto_ptr<chit_key_vector_t>
db_chit_t::keys(const chit_name_vector_t &names)
{
    std::auto_ptr<chit_key_vector_t> keys;
    chit_key_t key;

    if (!names.size())
        return db_chit_t::keys();

    keys.reset(new chit_key_vector_t);
    for (chit_name_vector_t::const_iterator i = names.begin();
           i != names.end(); ++i)
        keys->push_back(db_chit_t::key(*i, key) ? key : -1);

    return keys;
}

std::auto_ptr<chit_name_vector_t>
db_chit_t::names()
{
    std::auto_ptr<chit_name_vector_t> names(new chit_name_vector_t);
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    q->SQL->Text = "select name from chitnumber;";
    tr.StartTransaction();
        for (q->ExecQuery(); !q->Eof; q->Next())
            names->push_back(q->FieldByName("name")->AsString.c_str());
    tr.Commit();

    return names;
}

std::auto_ptr<chit_name_vector_t>
db_chit_t::names(const chit_key_vector_t &keys)
{
    std::auto_ptr<chit_name_vector_t> names;
    chit_name_t name;

    if (!keys.size())
        return db_chit_t::names();

    names.reset(new chit_name_vector_t);
    for (chit_key_vector_t::const_iterator i = keys.begin();
           i != keys.end(); ++i)
        names->push_back(db_chit_t::name(*i, name) ? name : chit_name_t(L""));

    return names;
}

std::auto_ptr<chit_name_key_vector_t>
db_chit_t::names_and_keys()
{
    std::auto_ptr<chit_name_key_vector_t> name_keys(new chit_name_key_vector_t);
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    q->SQL->Text = "select chitnumber_key key, "
                   "       name "
                   "       from chitnumber;";
    tr.StartTransaction();
        for (q->ExecQuery(); !q->Eof; q->Next())
            name_keys->push_back(
              chit_name_key_pair_t(q->FieldByName("name")->AsString.c_str(),
                                   q->FieldByName("key")->AsInteger));
    tr.Commit();

    return name_keys;
}

std::auto_ptr<chit_option_name_vector_t>
db_chit_t::option_names(const chit_key_t key)
{
    ensure_chit_present(key);

    {
        std::auto_ptr<chit_option_name_vector_t> names(
          new chit_option_name_vector_t);
        TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL *q = tr.Query(tr.AddQuery());

        q->SQL->Text = "select name "
                       "       from chitoptions "
                       "       where chitnumber_key = :key;";
        tr.StartTransaction();
            q->ParamByName("key")->AsInteger = key;
            for (q->ExecQuery(); !q->Eof; q->Next())
                names->push_back(q->FieldByName("name")->AsString.c_str());
        tr.Commit();

        return names;
    }
}

std::auto_ptr<chit_option_name_key_vector_t>
db_chit_t::option_names_and_keys(const chit_key_t key)
{
    ensure_chit_present(key);

    {
        std::auto_ptr<chit_option_name_key_vector_t> name_keys(
          new chit_option_name_key_vector_t);
        TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL *q = tr.Query(tr.AddQuery());

        q->SQL->Text = "select chitoptions_key key, "
                       "       name "
                       "       from chitoptions "
                       "       where chitnumber_key = :key;";
        tr.StartTransaction();
            q->ParamByName("key")->AsInteger = key;
            for (q->ExecQuery(); !q->Eof; q->Next())
                name_keys->push_back(
                  chit_option_name_key_pair_t(
                    q->FieldByName("name")->AsString.c_str(),
                    q->FieldByName("key")->AsInteger));
        tr.Commit();

        return name_keys;
    }
}

const chit_option_name_t
db_chit_t::option_name(const chit_option_key_t key)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());
    chit_option_name_t name;

    q->SQL->Text = "select name "
                   "       from chitoptions "
                   "       where chitoptions_key = :key;";
    tr.StartTransaction();
        q->ParamByName("chitoptions_key")->AsInteger = key;
        if (q->ExecQuery(), !q->RecordCount) {
            tr.Commit();
            throw chit_option_not_found_exception_t(key);
        }
        name = q->FieldByName("name")->AsString.c_str();
    tr.Commit();

    return name;
}

const chit_option_key_t
db_chit_t::option_key(const chit_key_t chit_key,
                      const chit_option_name_t &name)
{
    ensure_chit_present(chit_key);

    {
        TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL *q = tr.Query(tr.AddQuery());
        chit_option_key_t key;

        q->SQL->Text = "select chitoptions_key "
                       "       from chitoptions "
                       "       where chitnumber_key = :key and name = :name;";
        tr.StartTransaction();
            q->ParamByName("key")->AsInteger = chit_key;
            q->ParamByName("name")->AsString = name.c_str();
            if (q->ExecQuery(), !q->RecordCount) {
                tr.Commit();
                throw chit_option_not_found_exception_t(name);
            }
            key = q->FieldByName("chitoptions_key")->AsInteger;
        tr.Commit();

        return key;
    }
}

const chit_key_t
add_chit(const std::wstring default_menu,
         const std::wstring &mask,
         const chit_name_t &name,
         TDBTransaction &tr,
         const EChitFormatType format_type,
         const EChitPromptType prompt_type,
         const bool is_default,
	 const bool isHoldAndSendDisabled,
         const bool isNonChargableChit,
         const bool isDisplayTablesEnabled,
         const bool IsCaptureCustomerNameAndPhone,
         const bool IsAutoSaveOnTab,
         const bool IsCaptureCustomerDetails,
         const bool promptForPickUpDeliveryTime,
         const int inAddMinutes,
         const bool savecustomername,
         const bool savemembername,
         const bool savememberaddress,
         const bool onlinepickuporder,
         const bool onlinedeliveryorder)
{
    TIBSQL *q = tr.Query(tr.AddQuery());
    try {
        chit_key_t generated_key;

		if (is_default) {
            q->SQL->Text = "update chitnumber set is_default = 'F';";
            q->ExecQuery();
            q->Close();
        }

        q->SQL->Text = "select gen_id(gen_chitnumber_key, 1) key "
                       "       from rdb$database;";
        q->ExecQuery();
        generated_key = q->FieldByName("key")->AsInteger;
        q->Close();
        q->SQL->Text = "insert into chitnumber ( "
                       "    chittype, "
                       "    chitnumber_key, "
                       "    default_menu, "
                       "    format, "
                       "    is_default, "
                       "    mask, "
		       "    name, "
		       "    DISABLE_HOLD_AND_SEND, "
                       "    IS_NONCHARGABLECHIT, "
                       "    ENABLE_DISPLAY_TABLES, "
                       "    ENABLE_NAMEANDPHONENUMBER, "
                       "    ENABLE_AUTOSAVETAB, "
                       "    ENABLE_CUSTOMERDETAILS,  "
                       "    PROMPT_FOR_PICKUP_DELIVERY_TIME,  "
                       "    ADDMINUTES, "
                       "    SAVE_CUSTOMER_NAME, "
                       "    SAVE_MEMBER_NAME, "
                       "    SAVE_MEMBER_ADDRESS, "
                       "    ONLINE_PICKUP_ORDER, "
                       "    ONLINE_DELIVERY_ORDER) "
		       "    values ( "
		       "    :chit_type, "
		       "    :chit_number, "
		       "    :default_menu, "
		       "    :format, "
		       "    :is_default, "
		       "    :mask, "
		       "    :name, "
		       "    :disableHoldAndSend, "
                       "    :isnonchargablechit, "
                       "    :isDisplayTablesEnabled, "
                       "    :IsCaptureCustomerNameAndPhone,"
                       "    :IsAutoSaveOnTab,"
                       "    :IsCaptureCustomerDetails, "
                       "    :PROMPT_FOR_PICKUP_DELIVERY_TIME, "
                       "    :ADDMINUTES, "
                       "    :SAVE_CUSTOMER_NAME, "
                       "    :SAVE_MEMBER_NAME, "
                       "    :SAVE_MEMBER_ADDRESS, "
                       "    :ONLINE_PICKUP_ORDER, "
                       "    :ONLINE_DELIVERY_ORDER);";

        q->ParamByName("chit_type")->AsInteger = prompt_type;
        q->ParamByName("chit_number")->AsInteger = generated_key;
        q->ParamByName("format")->AsInteger = format_type;
        q->ParamByName("default_menu")->AsString = default_menu.c_str();
        if (!default_menu.size())
            q->ParamByName("default_menu")->IsNull = true;
        q->ParamByName("is_default")->AsString = is_default ? "T" : "F";
        q->ParamByName("mask")->AsString = mask.c_str();
	q->ParamByName("name")->AsString = name.c_str();
	q->ParamByName("disableHoldAndSend")->AsString = isHoldAndSendDisabled ? 'T' : 'F';
        q->ParamByName("isnonchargablechit")->AsString = isNonChargableChit ? 'T' : 'F';
        q->ParamByName("isDisplayTablesEnabled")->AsString = isDisplayTablesEnabled ? 'T' : 'F';
        q->ParamByName("IsCaptureCustomerNameAndPhone")->AsString = IsCaptureCustomerNameAndPhone ? 'T' : 'F';
        q->ParamByName("IsAutoSaveOnTab")->AsString = IsAutoSaveOnTab ? 'T' : 'F';
        q->ParamByName("IsCaptureCustomerDetails")->AsString = IsCaptureCustomerDetails ? 'T' : 'F';
        q->ParamByName("PROMPT_FOR_PICKUP_DELIVERY_TIME")->AsString = promptForPickUpDeliveryTime ? 'T' : 'F';
        q->ParamByName("ADDMINUTES")->AsInteger = inAddMinutes;
        q->ParamByName("SAVE_CUSTOMER_NAME")->AsString = savecustomername ? 'T' : 'F';
        q->ParamByName("SAVE_MEMBER_NAME")->AsString = savemembername ? 'T' : 'F';
        q->ParamByName("SAVE_MEMBER_ADDRESS")->AsString = savememberaddress ? 'T' : 'F';
        q->ParamByName("ONLINE_PICKUP_ORDER")->AsString = onlinepickuporder ? 'T' : 'F';
        q->ParamByName("ONLINE_DELIVERY_ORDER")->AsString = onlinedeliveryorder ? 'T' : 'F';
        q->ExecQuery();
        q->Close();

        return generated_key;
    }
    catch (...)
    {
        q->Close();
        throw;
    }
}

const chit_option_key_t
add_option(const chit_key_t key,
           const chit_option_name_t &name,
           TDBTransaction &tr)
{
    TIBSQL *q = tr.Query(tr.AddQuery());

    try {
        chit_option_key_t generated_key;

        q->SQL->Text = "select gen_id(gen_chitoptions, 1) key "
                       "       from rdb$database;";
        q->ExecQuery();
        generated_key = q->FieldByName("key")->AsInteger;
        q->Close();

        q->SQL->Text =
          "insert into chitoptions "
          "       values (:key, :chit_key, :name);";
        q->ParamByName("key")->AsInteger = generated_key;
        q->ParamByName("chit_key")->AsInteger = key;
        q->ParamByName("name")->AsString = name.c_str();

        q->ExecQuery();

        return generated_key;
    } catch (...) {
        q->Close();
        throw;
    }
}

const chit_key_t
db_chit_t::add_chit(const std::wstring &default_menu,
                    const std::wstring &mask,
                    const chit_name_t &name,
                    const chit_option_name_set_t &option_names,
                    const EChitFormatType format_type,
                    const EChitPromptType prompt_type,
                    const bool is_default,
		    const bool isHoldAndSendDisabled,
                    const bool isNonChargableChit,
                    const bool isDisplayTablesEnabled,
                    const bool IsCaptureCustomerNameAndPhone,
                    const bool IsAutoSaveOnTab,
                    const bool IsCaptureCustomerDetails,
                    const bool promptForPickUpDeliveryTime,
                    const int inAddMinutes,
                    const bool savecustomername,
                    const bool savemembername,
                    const bool savememberaddress,
                    const bool onlinepickuporder,
                    const bool onlinedeliveryorder)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    chit_key_t key;

    try {
        tr.StartTransaction();
            key = ::add_chit(default_menu,
                             mask,
                             name,
                             tr,
                             format_type,
                             prompt_type,
                             is_default,
			     isHoldAndSendDisabled,
                             isNonChargableChit,
                             isDisplayTablesEnabled,
                             IsCaptureCustomerNameAndPhone,
                             IsAutoSaveOnTab,
                             IsCaptureCustomerDetails,
                             promptForPickUpDeliveryTime,
                             inAddMinutes,
                             savecustomername,
                             savemembername,
                             savememberaddress,
                             onlinepickuporder,
                             onlinedeliveryorder);

            for (chit_option_name_set_t::const_iterator i = option_names.begin();
                   i != option_names.end(); ++i)
                ::add_option(key, *i, tr);

        tr.Commit();

        return key;
    } catch (...) {
        tr.Rollback();
        throw;
    }
}

const chit_option_key_t
db_chit_t::add_option(const chit_key_t key, const chit_option_name_t &name)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    chit_key_t generated_key;

    try {
        tr.StartTransaction();
            generated_key = ::add_option(key, name, tr);
        tr.Commit();

        return generated_key;
    } catch (...) {
        tr.Rollback();
        throw;
    }
}

const chit_option_key_vector_t
db_chit_t::add_options(const chit_key_t chit_key,
                       const chit_option_name_vector_t &names)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    try {
        chit_option_key_vector_t keys;

        tr.StartTransaction();
            for (chit_option_name_vector_t::const_iterator i = names.begin();
                   i != names.end(); ++i)
                keys.push_back(::add_option(chit_key, *i, tr));
        tr.Commit();

        return keys;
    } catch (...) {
        tr.Rollback();
        throw;
    }
}

void
db_chit_t::rename_option(const chit_option_key_t option_key,
                         const chit_option_name_t &new_option_name)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    ensure_option_present(option_key);

    q->SQL->Text = "update chitoptions "
                   "       set name = :name "
                   "where chitoptions_key = :key;";
    tr.StartTransaction();
    try {
        q->ParamByName("key")->AsInteger = option_key;
        q->ExecQuery();
        tr.Commit();
    } catch (...) {
        tr.Rollback();
        throw;
    }
}

void
db_chit_t::remove_option(const chit_option_key_t option_key)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    ensure_option_present(option_key);

    q->SQL->Text = "delete from chitoptions "
                   "       where chitoptions_key = :key;";
    tr.StartTransaction();
    try {
        q->ParamByName("key")->AsInteger = option_key;
        q->ExecQuery();
        tr.Commit();
    } catch (...) {
        tr.Rollback();
        throw;
    }
}

void
db_chit_t::remove_chit(const chit_key_t key)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    try {
        q->SQL->Text = "delete from chitnumber where chitnumber_key = :key;";
        tr.StartTransaction();
            q->ParamByName("key")->AsInteger = key;
            q->ExecQuery();
            q->Close();
        tr.Commit();
    } catch (...) {
        q->Close();
        tr.Rollback();
    }
}

void
db_chit_t::ensure_chit_present(const chit_key_t key)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    q->SQL->Text = "select chitnumber_key "
                   "       from chitnumber "
                   "       where chitnumber_key = :key;";
    tr.StartTransaction();
        q->ParamByName("key")->AsInteger = key;
        if (q->ExecQuery(), !q->RecordCount)
        {
            tr.Commit();
            throw chit_not_found_exception_t(key);
        }
    tr.Commit();
}

void
db_chit_t::ensure_option_present(const chit_option_key_t key)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

    q->SQL->Text = "select chitoptions_key "
                   "       from chitoptions "
                   "       where chitoptions_key = :key;";
    tr.StartTransaction();
    q->ParamByName("key")->AsInteger = key;
    if (q->ExecQuery(), !q->RecordCount)
        {
            tr.Commit();
            throw chit_option_not_found_exception_t(key);
        }
    tr.Commit();
}

void
db_chit_t::update_chit(
  const chit_key_t key,
  const std::wstring &default_menu,
  const std::wstring &mask,
  const chit_name_t &name,
  const chit_option_name_set_t &options_to_be_added,
  const chit_option_name_set_t &options_to_be_deleted,
  const chit_option_rename_map_t &options_to_be_renamed,
  const EChitFormatType format_type,
  const EChitPromptType prompt_type,
  const bool is_default,
  const bool isHoldAndSendDisabled,
  const bool isNonChargableChit,
  const bool isDisplayTablesEnabled,
  const bool IsCaptureCustomerNameAndPhone,
  const bool IsAutoSaveOnTab,
  const bool IsCaptureCustomerDetails,
  const bool promptForPickUpDeliveryTime,
  const int inAddMinutes,
  const bool savecustomername,
  const bool savemembername,
  const bool savememberaddress,
  const bool onlinepickuporder,
  const bool onlinedeliveryorder)
{
    TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL *q = tr.Query(tr.AddQuery());

	try {
        q->SQL->Text = "update chitnumber "
                       "       set name = :name, "
                       "       mask = :mask, "
                       "       chittype = :prompt_type, "
                       "       format = :format_type, "
                       "       is_default = :is_default, "
		       "       default_menu = :default_menu, "
		       "       DISABLE_HOLD_AND_SEND = :isHoldAndSendDisabled, "
                       "       IS_NONCHARGABLECHIT = :IS_NONCHARGABLECHIT,   "
                       "       ENABLE_DISPLAY_TABLES = :isDisplayTablesEnabled, "
                       "       ENABLE_NAMEANDPHONENUMBER = :IsCaptureCustomerNameAndPhone, "
                       "       ENABLE_AUTOSAVETAB = :IsAutoSaveOnTab, "
                       "       ENABLE_CUSTOMERDETAILS = :IsCaptureCustomerDetails, "
                       "       PROMPT_FOR_PICKUP_DELIVERY_TIME = :PROMPT_FOR_PICKUP_DELIVERY_TIME, "
                       "       ADDMINUTES = :ADDMINUTES, "
                       "       SAVE_CUSTOMER_NAME = :SAVE_CUSTOMER_NAME, "
                       "       SAVE_MEMBER_NAME = :SAVE_MEMBER_NAME, "
                       "       SAVE_MEMBER_ADDRESS = :SAVE_MEMBER_ADDRESS, "
                       "       ONLINE_PICKUP_ORDER = :ONLINE_PICKUP_ORDER, "
                       "       ONLINE_DELIVERY_ORDER = :ONLINE_DELIVERY_ORDER "
		       "       where chitnumber_key = :key;";
        tr.StartTransaction();

        q->ParamByName("name")->AsString = name.c_str();
        q->ParamByName("mask")->AsString = mask.c_str();
        q->ParamByName("prompt_type")->AsInteger = prompt_type;
        q->ParamByName("format_type")->AsInteger = format_type;
        q->ParamByName("is_default")->AsString = is_default ? "T" : "F";
	q->ParamByName("default_menu")->AsString = default_menu.c_str();
    	q->ParamByName("isHoldAndSendDisabled")->AsString = isHoldAndSendDisabled ? "T":"F";
        q->ParamByName("IS_NONCHARGABLECHIT")->AsString = isNonChargableChit ? "T":"F";
        q->ParamByName("isDisplayTablesEnabled")->AsString = isDisplayTablesEnabled ? "T":"F";
        q->ParamByName("IsCaptureCustomerNameAndPhone")->AsString = IsCaptureCustomerNameAndPhone ? "T":"F";
        q->ParamByName("IsAutoSaveOnTab")->AsString = IsAutoSaveOnTab ? "T":"F";
        q->ParamByName("IsCaptureCustomerDetails")->AsString = IsCaptureCustomerDetails ? "T":"F";
        q->ParamByName("PROMPT_FOR_PICKUP_DELIVERY_TIME")->AsString = promptForPickUpDeliveryTime ? "T" : "F";
        q->ParamByName("ADDMINUTES")->AsInteger = inAddMinutes;
        q->ParamByName("SAVE_CUSTOMER_NAME")->AsString = savecustomername ? "T" : "F";
        q->ParamByName("SAVE_MEMBER_NAME")->AsString = savemembername ? "T" : "F";
        q->ParamByName("SAVE_MEMBER_ADDRESS")->AsString = savememberaddress ? "T" : "F";
        q->ParamByName("ONLINE_PICKUP_ORDER")->AsString = onlinepickuporder ? 'T' : 'F';
        q->ParamByName("ONLINE_DELIVERY_ORDER")->AsString = onlinedeliveryorder ? 'T' : 'F';
        if (!default_menu.size())
                q->ParamByName("default_menu")->IsNull = true;
        q->ParamByName("key")->AsInteger = key;
        q->ExecQuery();
        update_chit_option_list(key,
                                    options_to_be_added,
                                    options_to_be_deleted,
                                    options_to_be_renamed,
                                    tr);
        tr.Commit();
    }
    catch (...)
    {
        q->Close();
        tr.Rollback();
        throw;
    }
}

