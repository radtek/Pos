#ifndef h_db_chit_fwd
#define h_db_chit_fwd

#include <map>
#include <memory>
#include <stdexcept>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "EnumChit.h"

typedef int          chit_key_t;
typedef std::wstring chit_name_t;

typedef std::vector<chit_key_t>            chit_key_vector_t;
typedef std::vector<chit_name_t>           chit_name_vector_t;
typedef std::pair<chit_name_t, chit_key_t> chit_name_key_pair_t;
typedef std::vector<chit_name_key_pair_t>  chit_name_key_vector_t;

typedef int          chit_option_key_t;
typedef std::wstring chit_option_name_t;

typedef std::pair<chit_option_name_t,
                  chit_option_key_t> chit_option_name_key_pair_t;
typedef std::map<chit_option_name_t,
                 chit_option_name_t> chit_option_rename_map_t;

typedef std::vector<chit_option_key_t>  chit_option_key_vector_t;
typedef std::vector<chit_option_name_t> chit_option_name_vector_t;
typedef std::set<chit_option_name_t> chit_option_name_set_t;
typedef std::vector<chit_option_name_key_pair_t> chit_option_name_key_vector_t;

#endif /* ! h_db_chit_fwd */

