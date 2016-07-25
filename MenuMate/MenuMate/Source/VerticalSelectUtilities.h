#ifndef h_vertical_select_utilities
#define h_vertical_select_utilities

#include <vector>
#include "VerticalSelect.h"

namespace vertical_selection_utilities {

class item_t {
public:
    item_t(const UnicodeString &title, const bool closes_selection = true);

    operator TVerticalSelection() const;

    item_t &closes_selection(const bool closes_selection = true);
    item_t &latched_bg(const TColor color);
    item_t &latched_fg(const TColor color);
    item_t &normal_bg(const TColor color);
    item_t &normal_fg(const TColor color);
    item_t &property(const UnicodeString &name, const Variant value);
    item_t &title(const UnicodeString &title);

private:
    TVerticalSelection item_;
}; /* item_t */

class item_arglist_t {
public:
    item_arglist_t();

    operator std::vector<TVerticalSelection>() const;

    item_arglist_t &operator<<(const item_t new_item);

private:
    std::vector<TVerticalSelection> items_;
}; /* item_arglist_t */

std::auto_ptr<TfrmVerticalSelect> create_vertical_selection(
  TForm *owner, const item_arglist_t items);

} /* vertical_selection_utilities */

#endif /* ! h_vertical_select_utilities */
