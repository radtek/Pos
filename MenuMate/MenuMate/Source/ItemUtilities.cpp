#include "Item.h"
#include "ItemSize.H"
#include "ItemUtilities.h"

bool
item_has_sizes_with_set_menu_mask(
    TItem &item,
    const unsigned long set_menu_mask)
{
        TListSizeContainer &sizes = *item.Sizes;
        bool matches = false;

        for (int i = 0; !matches && i < sizes.Count; ++i) {
                TItemSize &size = *sizes.SizeGet(i);

                matches = size.SetMenuMask & set_menu_mask
                          && !size.SetMenuMaster;
        }

        return matches;
}

