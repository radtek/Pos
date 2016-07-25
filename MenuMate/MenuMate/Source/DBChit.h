#ifndef h_db_chit
#define h_db_chit

#include "DBChitFwd.h"

class db_chit_exception_t : public std::exception {
public:
    db_chit_exception_t();
    db_chit_exception_t(const db_chit_exception_t &initializer);
    virtual ~db_chit_exception_t() throw();

    db_chit_exception_t &operator=(const db_chit_exception_t &rhs);
    virtual const char *what() const;
}; /* db_chit_exception_t */

class chit_not_found_exception_t : public db_chit_exception_t {
public:
    chit_not_found_exception_t(const chit_name_t &name);
    chit_not_found_exception_t(const chit_key_t key);
    chit_not_found_exception_t(const chit_not_found_exception_t &initializer);
    ~chit_not_found_exception_t() throw();

    chit_not_found_exception_t &operator=(
      const chit_not_found_exception_t &rhs);

    const char *what() const;

    const chit_name_t &name() const;
    const chit_key_t key() const;

private:
    chit_name_t name_;
    chit_key_t key_;
}; /* chit_not_found_exception_t */

class chit_option_not_found_exception_t : public db_chit_exception_t {
public:
    chit_option_not_found_exception_t(const chit_option_name_t &name);
    chit_option_not_found_exception_t(const chit_option_key_t option_key);
    chit_option_not_found_exception_t(
      const chit_option_not_found_exception_t &initializer);
    ~chit_option_not_found_exception_t() throw();

    chit_option_not_found_exception_t &operator=(
      const chit_option_not_found_exception_t &rhs);

    const char *what() const;

    const chit_option_name_t &name() const;
    const chit_key_t option_key() const;

private:
    chit_option_name_t name_;
    chit_option_key_t option_key_;
}; /* chit_option_not_found_exception_t */

class db_chit_t {
public:
    /* Get a chit's key by name. */
    static const chit_key_t key(const chit_name_t &name);
    /* Get a chit's name by key. */
    static const chit_name_t name(const chit_key_t key);

    /* Exception-free variants. */
    static bool key(const chit_name_t &name, chit_key_t &key);
    static bool name(const chit_key_t key, chit_name_t &name);

    /* Get a list of all of the chits keys. */
    static std::auto_ptr<chit_key_vector_t> keys();
    /* Finds the keys for the associated names. */
    static std::auto_ptr<chit_key_vector_t> keys(
      const chit_name_vector_t &names);

    /* Get a list of all chit names. */
    static std::auto_ptr<chit_name_vector_t> names();
    /* Finds the names for the associated keys. */
    static std::auto_ptr<chit_name_vector_t> names(
      const chit_key_vector_t &keys);

    /* Creates a list of chit names with their associated keys. */
    static std::auto_ptr<chit_name_key_vector_t> names_and_keys();

    /* Returns a list of a chit's options in name-key form. */
    static std::auto_ptr<chit_option_key_vector_t> option_keys(
      const chit_key_t key);
    static std::auto_ptr<chit_option_name_vector_t> option_names(
      const chit_key_t key);
    static std::auto_ptr<chit_option_name_key_vector_t> option_names_and_keys(
      const chit_key_t key);

    /* Translates an option's key to a name, */
    static const chit_option_name_t option_name(const chit_option_key_t key);
    /* Translates an option's name to a key. */
    static const chit_option_key_t option_key(const chit_key_t key,
                                              const chit_option_name_t &name);

    /* Creates a new chit in the database and returns it's key. */
    static const chit_key_t add_chit(
     const std::wstring &default_menu,
     const std::wstring &mask,
     const chit_name_t &name,
     const EChitFormatType format_type,
     const EChitPromptType prompt_type,
     const bool is_default,
     const bool isHoldAndSendDisabled,
     const bool isNonChargableChit,
     const bool isDisplayTablesDisabled,
     const bool IsCaptureCustomerNameAndPhone,
     const bool IsAutoSaveOnTab,
     const bool IsCaptureCustomerDetails,
     const bool promptForPickUpDeliveryTime,
     const int inAddMinutes,
     const bool savecustomername,
     const bool savemembername,
     const bool savememberaddress,
     const bool onlinepickuporder,
     const bool onlinedeliveryorder);

    /* The same as above except that it'll also create and link options. */
    static const chit_key_t add_chit(
      const std::wstring &default_menu,
      const std::wstring &mask,
      const chit_name_t &name,
      const chit_option_name_set_t &option_names,
      const EChitFormatType format_type,
      const EChitPromptType prompt_type,
      const bool is_default,
      const bool isHoldAndSendDisabled,
      const bool isNonChargableChit,
      const bool isDisplayTablesDisabled,
      const bool IsCaptureCustomerNameAndPhone,
      const bool IsAutoSaveOnTab,
      const bool IsCaptureCustomerDetails,
      const bool promptForPickUpDeliveryTime,
      const int inAddMinutes,
      const bool savecustomername,
      const bool savemembername,
      const bool savememberaddress,
      const bool onlinepickuporder,
      const bool onlinedeliveryorder);

    static void update_chit(
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
      const bool isDisplayTablesDisabled,
      const bool IsCaptureCustomerNameAndPhone,
      const bool IsAutoSaveOnTab,
      const bool IsCaptureCustomerDetails,
      const bool promptForPickUpDeliveryTime,
      const int inAddMinutes,
      const bool savecustomername,
      const bool savemembername,
      const bool savememberaddress,
      const bool onlinepickuporder,
      const bool onlinedeliveryorder);

    static void remove_chit(const chit_key_t key);

    /* Adds an option to a chit. */
    static const chit_option_key_t add_option(const chit_key_t chit_key,
                                              const chit_option_name_t &name);

    /* The same but in bulk. */
    static const chit_option_key_vector_t add_options(
      const chit_key_t chit_key,
      const chit_option_name_vector_t &names);

    /* Renames an existing option. */
    static void rename_option(const chit_option_key_t option_key,
                              const chit_option_name_t &new_option_name);

    /* Removes an option from a chit. */
    static void remove_option(const chit_option_key_t option_key);

private:
    static void ensure_chit_present(const chit_key_t key);
    static void ensure_option_present(const chit_option_key_t key);
}; /* db_chit_t */


#endif /* h_db_chit */
