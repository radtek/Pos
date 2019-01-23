using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.Registration;
using System.Net;
using MenumateServices.DTO.MenumateRegistration;
using RegistrationIntegration.ViewModels;
using RegistrationIntegration.Sevices;
using RegistrationIntegration.Utility;

namespace MenumateServices.Internal_Classes.Registration
{
    public class TerminalRegistration : TerminalRegistrationResponsive
    {
        private static volatile TerminalRegistration _instance;
        private static object _syncRoot = new Object();

        private TerminalRegistration()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static TerminalRegistration Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new TerminalRegistration();
                    }
                }

                return _instance;
            }
        }

        #region Public


        public RegistrationResponse UpdateTerminalRegistrationInfo(string inSyndicateCode, Terminal terminalRegistrationInfo)
        {
            try
            {
                IRegistrationIntegrationService registrationService = new RegistrationIntagrationService();
                var response = registrationService.PostTerminalRegistrationInfo(inSyndicateCode, CreateTerminalViewModel(terminalRegistrationInfo));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to registration info to server.",
                        "",
                        RegistrationResponseCode.RegistrationUpdateFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        #endregion

        #region private

        //Creating Menu View Models
        private ApiTerminalViewModel CreateTerminalViewModel(Terminal terminalRegistrationInfo)
        {
            var terminalViewModel = new ApiTerminalViewModel();
            try
            {
                terminalViewModel.ComputerName = terminalRegistrationInfo.ComputerName;
                terminalViewModel.MacAdress = terminalRegistrationInfo.MacAdress;
                terminalViewModel.MenumateVersion = terminalRegistrationInfo.MenumateVersion;
                terminalViewModel.OperatingSystemName = terminalRegistrationInfo.OperatingSystemName;
                terminalViewModel.SiteCode = terminalRegistrationInfo.Site.SiteCode;
                //  terminalViewModel.SyndicateCode = terminalRegistrationInfo.Site.sy
                terminalViewModel.StaffName = terminalRegistrationInfo.StaffName;
                terminalViewModel.TerminalDescription = terminalRegistrationInfo.Description;
                terminalViewModel.TerminalName = terminalRegistrationInfo.Name;

                if (terminalViewModel.ApiLicenceSettings != null)
                {
                    foreach (var terminalSetting in terminalRegistrationInfo.LicenceSettingMappings)
                    {
                        terminalViewModel.ApiLicenceSettings.Add(CreateTerminalSettingViewModel(terminalSetting));
                    }
                }
                var requestData = JsonUtility.Serialize<ApiTerminalViewModel>(terminalViewModel);//just to test json
            }
            catch (Exception ex)
            {
                throw ex;
            }
            return terminalViewModel;
        }

        private ApiLicenceSetting CreateTerminalSettingViewModel(LicenceSettingMapping licenseSettingInfo)
        {
            var licenseSettingViewModel = new ApiLicenceSetting();
            try
            {
                licenseSettingViewModel.IsActive = licenseSettingInfo.LicenceSettingSetting.IsEnabledByDefault;
                licenseSettingViewModel.SettingSubType = licenseSettingInfo.LicenceSettingSetting.Name;
                licenseSettingViewModel.SettingType = licenseSettingInfo.Text;// LicenceSettingSetting.SettingType;
            }
            catch (Exception ex)
            {
                throw ex;
            }
            return licenseSettingViewModel;
        }
       
        #endregion
    }
}
