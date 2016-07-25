#include "process_weborder_action.hh"
#include "XMLOrdermm.h"

process_weborder_action_t::process_weborder_action_t(
  const webmate_path_configuration_t &configuration)
  : configuration_(configuration)
{
}

bool
process_weborder_action_t::action(
  const std::wstring filename)
{
    CoInitialize(NULL);
        _di_IXMLORDERType weborder = LoadORDER(filename.c_str());
    CoUninitialize();
}

