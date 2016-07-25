#include <algorithm>
#include <functional>

#include "ChitOptionsPrompt.h"
#include "ListManager.h"
#include "MMTouchKeyboard.h"

class rename_search_predicate_t :
  public std::unary_function<const std::pair<chit_option_name_t,
                                             chit_option_name_t> &,
                             bool> {
public:
    rename_search_predicate_t(const chit_option_name_t &name)
      : name_(name)
    {
    }

    bool
    operator()(const std::pair<chit_option_name_t, chit_option_name_t> &input)
    {
        return input.first == name_ || input.second == name_;
    };

private:
    chit_option_name_t name_;
};

chit_options_prompt_t::chit_options_prompt_t(const chit_key_t chit_key)
  : keyboard_(create_and_initialize_keyboard()),
    list_dialog_(create_and_initialize_list_dialog()),
    in_database_(),
    to_be_added_(),
    to_be_deleted_(),
    to_be_renamed_(),
    visible_(),
    chit_key_(chit_key)
{
    if (chit_key != 0)
        populate_options();
    redraw_list();
}

const std::set<chit_option_name_t> &
chit_options_prompt_t::to_be_added()
const
{
    return to_be_added_;
}

const std::set<chit_option_name_t> &
chit_options_prompt_t::to_be_deleted()
const
{
    return to_be_deleted_;
}

const chit_option_rename_map_t &
chit_options_prompt_t::to_be_renamed()
const
{
    return to_be_renamed_;
}

void
chit_options_prompt_t::prompt()
{
    list_dialog_->ShowModal();
}

void
chit_options_prompt_t::add_clicked(int, int)
{
    if (!show_keyboard())
        return;

    add_option(keyboard_->KeyboardText.c_str());
    redraw_list();
}

void
chit_options_prompt_t::add_option(const chit_name_t &name)
{
    if (visible(name))
        return;
    else if (pending_deletion(name))
        to_be_deleted_.erase(name);

    to_be_added_.insert(name);
    visible_.insert(name);
    redraw_list();
}

std::auto_ptr<TfrmTouchKeyboard>
chit_options_prompt_t::create_and_initialize_keyboard()
const
{
    std::auto_ptr<TfrmTouchKeyboard> keyboard(
      TZForm::Create<TfrmTouchKeyboard>(0x0));

    keyboard->AllowCarriageReturn = false;
    keyboard->Caption = "Enter an option name";
    keyboard->MustHaveValue = true;

    return keyboard;
}

std::auto_ptr<TfrmListManager>
chit_options_prompt_t::create_and_initialize_list_dialog()
const
{
    std::auto_ptr<TfrmListManager> list_(new TfrmListManager(0x0));

    list_->Add.RegisterForEvent(add_clicked);
    list_->Delete.RegisterForEvent(delete_clicked);
    list_->Close.RegisterForEvent(ok_clicked);
    list_->Edit.RegisterForEvent(rename_clicked);
    list_->Select.RegisterForEvent(selection_made);

    list_->SetCaption("Chit options");

    list_->btnActive = false;
    list_->sgDisplay->ColCount = 1;
    list_->tbtnDelete->Enabled = false;
    list_->tbtnEdit->Caption = "Rename";
    list_->tbtnEdit->Enabled = false;

    return list_;
}

void
chit_options_prompt_t::delete_clicked(int row, int)
{
    delete_option(list_dialog_->sgDisplay->Cells[0][row].c_str());
    redraw_list();
}

void
chit_options_prompt_t::delete_option(const chit_option_name_t &name)
{
    if (in_database(name))
        to_be_deleted_.insert(name);
    else if (pending_addition(name))
        to_be_added_.erase(name);
    else if (pending_rename(name))
        to_be_renamed_.erase(name);

    visible_.erase(name);
    set_edition_button_state(visible_.size());
}

const bool
chit_options_prompt_t::in_database(const chit_name_t &name)
const
{
    return in_database_.find(name) != in_database_.end();
}

void
chit_options_prompt_t::ok_clicked(int, int)
{
    list_dialog_->ModalResult = mrOk;
}

const bool
chit_options_prompt_t::pending_addition(const chit_option_name_t &name)
const
{
    return to_be_added_.find(name) != to_be_added_.end();
}

const bool
chit_options_prompt_t::pending_deletion(const chit_option_name_t &name)
const
{
    return to_be_deleted_.find(name) != to_be_deleted_.end();
}

const bool
chit_options_prompt_t::pending_rename(const chit_option_name_t &name)
const
{
    return const_cast<chit_options_prompt_t *>(this)->pending_rename_entry(name)
           != to_be_renamed_.end();
}

chit_option_rename_map_t::iterator
chit_options_prompt_t::pending_rename_entry(const chit_option_name_t &name)
{
    rename_search_predicate_t predicate(name);

    return std::find_if(to_be_renamed_.begin(),
                        to_be_renamed_.end(),
                        predicate);
}

void
chit_options_prompt_t::populate_options()
{
    std::auto_ptr<chit_option_name_vector_t> options(
      db_chit_t::option_names(chit_key_));

    for (chit_option_name_vector_t::const_iterator i = options->begin();
           i != options->end(); ++i) {
        in_database_.insert(*i);
        visible_.insert(*i);
    }
}

void
chit_options_prompt_t::redraw_list()
{
    TStringGrid &sg = *list_dialog_->sgDisplay;
    int i = 0;

    sg.RowCount = visible_.size();

    sg.Cells[0][0] = "";
    sg.Objects[0][0] = NULL;

    for (std::set<chit_name_t>::const_iterator j = visible_.begin();
           j != visible_.end(); ++i, ++j) {
        sg.Cells[0][i] = j->c_str();
        sg.Objects[0][i] = reinterpret_cast<TObject *>(i);
    }
}

void
chit_options_prompt_t::rename_clicked(int row, int)
{
    if (!show_keyboard())
        return;

    rename_option(list_dialog_->sgDisplay->Cells[0][row].c_str(),
                  keyboard_->KeyboardText.c_str());
    redraw_list();
}

void
chit_options_prompt_t::rename_option(const chit_option_name_t &old_name,
                                     const chit_option_name_t &new_name)
{
    chit_option_rename_map_t::iterator rename_entry;

    if (visible(new_name)) {
        return;
    }

    if (pending_addition(old_name)) {
        to_be_added_.erase(old_name);
        to_be_added_.insert(new_name);
    } else if ((rename_entry = pending_rename_entry(old_name))
               != to_be_renamed_.end())
        rename_entry->second = new_name;
    else if (in_database(old_name))
        to_be_renamed_[old_name] = new_name;

    visible_.erase(old_name);
    visible_.insert(new_name);
}

void
chit_options_prompt_t::selection_made(int row, int column)
{
    set_edition_button_state(
      list_dialog_->sgDisplay->Cells[column][row] != "");
}

void
chit_options_prompt_t::set_edition_button_state(const bool state)
{
    list_dialog_->tbtnEdit->Enabled = state;
    list_dialog_->tbtnDelete->Enabled = state;
}

const bool
chit_options_prompt_t::show_keyboard(const UnicodeString text)
{
    keyboard_->KeyboardText = text;
    return keyboard_->ShowModal() == mrOk;
}

const bool
chit_options_prompt_t::visible(const chit_option_name_t &name)
const
{
    return visible_.find(name) != visible_.end();
}

