#ifndef hh_redeem_points_only_on_select_items
#define hh_redeem_points_only_on_select_items

#include "VersionParser.h"

namespace MenumateVersionParser {
namespace RedeemPointsOnlyOnSelectItems {

PARSER_ERROR apply_update(Database::TDBControl * const);
bool has_already_been_applied(Database::TDBControl * const);

} /* RedeemPointsOnlyOnSelectItems */
} /* MenumateVersionParser */

#endif /* ! hh_redeem_points_only_on_select_items */

