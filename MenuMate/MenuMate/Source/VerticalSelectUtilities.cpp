#include "VerticalSelectUtilities.h"

namespace vertical_selection_utilities {

item_t::item_t(const UnicodeString &name, const bool closes_selection)
  : item_(TVerticalSelection(name, closes_selection))
{
}

item_t::operator TVerticalSelection()
const
{
    return item_;
}

item_t &
item_t::closes_selection(const bool closes_selection)
{
    item_.CloseSelection = true;
    return *this;
}

item_t &
item_t::latched_bg(const TColor color)
{
    return property("LatchedColor", IntToStr(color));
}

item_t &
item_t::latched_fg(const TColor color)
{
    return property("LatchedFontColor", IntToStr(color));
}

item_t &
item_t::normal_bg(const TColor color)
{
    return property("Color", IntToStr(color));
}

item_t &
item_t::normal_fg(const TColor color)
{
    return property("FontColor", IntToStr(color));
}

item_t &
item_t::property(const UnicodeString &name, const Variant value)
{
    item_.Properties[name] = value;
    return *this;
}

item_t &
item_t::title(const UnicodeString &title)
{
    item_.Title = title;
    return *this;
}

item_arglist_t::item_arglist_t()
  : items_()
{
}

item_arglist_t::operator std::vector<TVerticalSelection>()
const
{
    return items_;
}

item_arglist_t &
item_arglist_t::operator<<(const item_t new_item)
{
    items_.push_back(new_item);
    return *this;
}

std::auto_ptr<TfrmVerticalSelect>
create_vertical_selection(TForm *owner, const item_arglist_t items)
{
    std::auto_ptr<TfrmVerticalSelect> menu(
      TZForm::Create<TfrmVerticalSelect>(owner));

    menu->Items = items;
    return menu;
}

} /* vertical_selection_utilities */

