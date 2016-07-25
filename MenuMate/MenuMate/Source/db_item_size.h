#ifndef h_db_item_size
#define h_db_item_size

#include "db_item_key_types.h"

struct db_item_size_t {
        /*
         * This'll return zero if it finds no such item size in the database.
         */
        static int
        default_patron_count(
            const db_item_key_types::item_key_t item_key,
            const db_item_key_types::item_size_name_t &size_name);
}; /* db_item_size_t */

#endif /* h_db_item_size */
