using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.Registration;
using System.Net;
using MenumateServices.DTO.MenumateRegistration;
using RegistrationIntegration.Sevices;
using RegistrationIntegration.Utility;
using RegistrationIntegration.Models;

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


        public RegistrationResponse UpdateTerminalRegistrationInfo(string inSyndicateCode, TerminalModel terminalRegistrationInfo)
        {
            try
            {
                IRegistrationIntegrationService registrationService = new RegistrationIntagrationService();
                var response = registrationService.PostTerminalRegistrationInfo(inSyndicateCode, CreateTerminalViewModel(terminalRegistrationInfo));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError("@Failed to registration info to server.", "", RegistrationResponseCode.RegistrationUpdateFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        public CompanySiteModelResponse ValidateCompanyInfo(string inSyndicateCode, int siteCode)
        {
            try
            {
                IRegistrationIntegrationService registrationService = new RegistrationIntagrationService();
                var response = registrationService.ValidateCompanyInfo(inSyndicateCode, siteCode);
                return CreateRegistrationResponseNoError(response);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        #endregion

        #region private

       
        private ApiTerminalViewModel CreateTerminalViewModel(TerminalModel terminalRegistrationInfo)
        {
            var terminalViewModel = new ApiTerminalViewModel();
            try
            {
                terminalViewModel.ComputerName = terminalRegistrationInfo.ComputerName;
                terminalViewModel.MacAdress = terminalRegistrationInfo.MacAdress;
                terminalViewModel.MenumateVersion = terminalRegistrationInfo.MenumateVersion;
                terminalViewModel.OperatingSystemName = terminalRegistrationInfo.OperatingSystemName;
                terminalViewModel.SiteCode = terminalRegistrationInfo.SiteCode;
                //  terminalViewModel.SyndicateCode = terminalRegistrationInfo.Site.sy
                terminalViewModel.StaffName = terminalRegistrationInfo.StaffName;
                terminalViewModel.TerminalDescription = terminalRegistrationInfo.TerminalDescription;
                terminalViewModel.TerminalName = terminalRegistrationInfo.TerminalName;

                if (terminalRegistrationInfo.LicenceSettingsModel != null)
                {
                    terminalViewModel.ApiLicenceSettings = new List<ApiLicenceSetting>();
                    foreach (var terminalSetting in terminalRegistrationInfo.LicenceSettingsModel)
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

        private ApiLicenceSetting CreateTerminalSettingViewModel(LicenceSettingModel licenseSettingInfo)
        {
            var licenseSettingViewModel = new ApiLicenceSetting();
            try
            {
                licenseSettingViewModel.IsActive = licenseSettingInfo.IsActive;
                licenseSettingViewModel.SettingSubType = licenseSettingInfo.SettingSubType;
                licenseSettingViewModel.SettingType = licenseSettingInfo.SettingType;// LicenceSettingSetting.SettingType;
            }
            catch (Exception ex)
            {
                throw ex;
            }
            return licenseSettingViewModel;
        }

        private CompanySiteModelResponse CreateRegistrationResponseNoError(ApiCompanySiteViewModel companyModelResponse)
        {
            return new CompanySiteModelResponse
            {
                CompanyName = companyModelResponse.CompanyName,
                IsSuccessful = companyModelResponse.IsSuccessful,
                Message = companyModelResponse.Message                
            };
        }

        #endregion
    }
}
