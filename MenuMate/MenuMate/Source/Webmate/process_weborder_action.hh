#ifndef hh_process_weborder_action
#define hh_process_weborder_action

#include "webmate_process_order_action_base.hh"

class webmate_path_configuration_t;

class process_weborder_action_t
  : public webmate_process_order_action_base_t {
public:
    process_weborder_action_t(
      const webmate_path_configuration_t &configuration);

private:
    bool action(const std::wstring filename);

    const webmate_path_configuration_t &configuration_;
}; /* process_weborder_action_t */

#endif /* ! hh_process_weborder_action */
