#ifndef hh_webmate_path_configuration
#define hh_webmate_path_configuration

#include <string>

class webmate_path_configuration_t {
public:
    static const std::wstring default_root_directory;
    static const std::wstring default_cache_subdirectory;
    static const std::wstring default_failed_subdirectory;
    static const std::wstring default_processed_subdirectory;

    webmate_path_configuration_t();
    webmate_path_configuration_t(
      const std::wstring cache_directory,
      const std::wstring failed_directory,
      const std::wstring processed_directory);
    webmate_path_configuration_t(
      const std::wstring root_directory,
      const std::wstring cache_subdirectory,
      const std::wstring failed_subdirectory,
      const std::wstring processed_subdirectory);

    const std::wstring &cache_directory() const;
    const std::wstring &failed_directory() const;
    const std::wstring &processed_directory() const;

private:
    std::wstring cache_directory_;
    std::wstring failed_directory_;
    std::wstring processed_directory_;
}; /* webmate_path_configuration_t */

#endif /* ! hh_webmate_path_configuration */
