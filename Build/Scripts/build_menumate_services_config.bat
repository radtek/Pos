set sv_menumate_services_utilities_path=%BASEDIR%\MenuMate\Services\ServicesApp\MenumateServices Utililties
set sv_menumate_services_config_path=%BASEDIR%\MenuMate\Services\ServicesApp\MenumateServices Config

devenv "%sv_menumate_services_config_path%\Menumate Services Utilities.sln" /build release
devenv "%sv_menumate_services_config_path%\Menumate Services Config.sln" /build release
