#include "webmate_path_configuration.hh"

#define CONFIGURATION_T webmate_path_configuration_t

const std::wstring CONFIGURATION_T::default_root_directory =
  L"Import\\WebOrders";
const std::wstring CONFIGURATION_T::default_cache_subdirectory =
  L"Cached";
const std::wstring CONFIGURATION_T::default_failed_subdirectory =
  L"Failed";
const std::wstring CONFIGURATION_T::default_processed_subdirectory =
  L"Processed";

CONFIGURATION_T::webmate_path_configuration_t()
  : cache_directory_(
      default_root_directory + L"\\" + default_cache_subdirectory),
    failed_directory_(
      default_root_directory + L"\\" + default_failed_subdirectory),
    processed_directory_(
      default_root_directory + L"\\" + default_processed_subdirectory)
{
}

CONFIGURATION_T::webmate_path_configuration_t(
  const std::wstring cache_directory,
  const std::wstring failed_directory,
  const std::wstring processed_directory)
  : cache_directory_(cache_directory),
    failed_directory_(failed_directory),
    processed_directory_(processed_directory)
{
}

CONFIGURATION_T::webmate_path_configuration_t(
  const std::wstring root_directory,
  const std::wstring cache_subdirectory,
  const std::wstring failed_subdirectory,
  const std::wstring processed_subdirectory)
  : cache_directory_(root_directory + L"\\" + cache_subdirectory),
    failed_directory_(root_directory + L"\\" + failed_subdirectory),
    processed_directory_(root_directory + L"\\" + processed_subdirectory)
{
}

const std::wstring &
CONFIGURATION_T::cache_directory()
const
{
    return cache_directory_;
}

const std::wstring &
CONFIGURATION_T::failed_directory()
const
{
    return failed_directory_;
}

const std::wstring &
CONFIGURATION_T::processed_directory()
const
{
    return processed_directory_;
}

#undef CONFIGURATION_T

