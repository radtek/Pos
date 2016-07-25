#ifndef h_db_item_key_types
#define h_db_item_key_types

#include <System.hpp>

struct db_item_key_types {
        typedef int           item_id_t;
        typedef int           item_key_t;
        typedef UnicodeString item_name_t;
        typedef int           item_size_id_t;
        typedef int           item_size_key_t;
        typedef UnicodeString item_size_name_t;
}; /* db_item_key_types */

#endif /* h_db_item_key_types */
