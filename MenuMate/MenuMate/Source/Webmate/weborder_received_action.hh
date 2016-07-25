#ifndef hh_weborder_received_action
#define hh_weborder_received_action

#include "webmate_order_received_action_base.hh"

class webmate_order_processor_t;
class webmate_path_configuration_t;
class TiXmlDocument;

class weborder_received_action_t
  : public webmate_order_received_action_base_t {
public:
    weborder_received_action_t(
      webmate_order_processor_t &order_processor,
      const webmate_path_configuration_t &configuration);

private:
    bool action(TMemoryStream &input_buffer,
                TMemoryStream &output_buffer);

    webmate_order_processor_t &order_processor_;
    const webmate_path_configuration_t &configuration_;
}; /* weborder_received_action_t */

#endif /* ! hh_weborder_received_action */

