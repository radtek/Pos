#ifndef h_chit_options_prompt_t
#define h_chit_options_prompt_t

#include <map>
#include <set>

#include "DBChit.h"
#include "System.hpp"

class TfrmListManager;
class TfrmTouchKeyboard;

class chit_options_prompt_t {
public:
    chit_options_prompt_t(const chit_key_t chit_key);

    const chit_option_name_set_t &to_be_added() const;
    const chit_option_name_set_t &to_be_deleted() const;
    const chit_option_rename_map_t &to_be_renamed() const;

    void prompt();

private:
    void add_clicked(int, int);
    void add_option(const chit_name_t &name);
    std::auto_ptr<TfrmTouchKeyboard> create_and_initialize_keyboard() const;
    std::auto_ptr<TfrmListManager> create_and_initialize_list_dialog() const;
    void delete_clicked(int row, int);
    void delete_option(const chit_name_t &name);
    const bool in_database(const chit_name_t &name) const;
    void ok_clicked(int, int);
    const bool pending_addition(const chit_name_t &name) const;
    const bool pending_deletion(const chit_name_t &name) const;
    const bool pending_rename(const chit_name_t &name) const;
    std::map<chit_name_t, chit_name_t>::iterator pending_rename_entry(
      const chit_name_t &name);
    void populate_options();
    void redraw_list();
    void rename_clicked(int row, int);
    void rename_option(const chit_name_t &old_name,
                       const chit_name_t &new_name);
    void selection_made(int row, int column);
    void set_edition_button_state(const bool state);
    const bool show_keyboard(const UnicodeString text = L"");
    const bool visible(const chit_name_t &name) const;

    std::auto_ptr<TfrmTouchKeyboard> keyboard_;
    std::auto_ptr<TfrmListManager> list_dialog_;

    chit_option_name_set_t in_database_;
    chit_option_name_set_t to_be_added_;
    chit_option_name_set_t to_be_deleted_;
    chit_option_rename_map_t to_be_renamed_;
    chit_option_name_set_t visible_;

    chit_key_t chit_key_;
}; /* chit_options_prompt_t */

#endif /* ! h_chit_options_prompt_t */
