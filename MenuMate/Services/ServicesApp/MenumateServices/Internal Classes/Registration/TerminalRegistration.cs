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
using RegistrationIntegration.Exceptions;
using MenumateServices.Tools;

namespace MenumateServices.Internal_Classes.Registration
{
    public class TerminalRegistration : TerminalRegistrationResponsive
    {
        private static volatile TerminalRegistration _instance;
        private static object _syncRoot = new Object();
        private List<string> stringList = new List<string>();

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
                stringList.Add("Inside UpdateTerminalRegistrationInfo() in TerminalRegistration.cs ");
                stringList.Add("calling registrationService.PostTerminalRegistrationInfo(inSyndicateCode, CreateTerminalViewModel(terminalRegistrationInfo)");
                var response = registrationService.PostTerminalRegistrationInfo(inSyndicateCode, CreateTerminalViewModel(terminalRegistrationInfo));
                stringList.Add("After calling registrationservice file's  PostTerminalRegistrationInfo() response received is: " + response);
                if (response)
                {
                    return CreateResponseNoError();
                }
                else
                {
                    stringList.Add("Failed to update registration info to server.");
                    return CreateResponseError("@Failed to registration info to server.", "", RegistrationResponseCode.RegistrationUpdateFailed);
                }
            }
            catch (AuthenticationFailedException ex)
            {
                stringList.Add("Authentication failed and exception message is: " + ex.Message);
                return CreateResponseError(@"Failed to Authenticate", ex.Message, RegistrationResponseCode.AuthenticationFailed);
            }
            catch (NoSettingChangeException ex)
            {
                stringList.Add("NoSettingChangeException and exception message is: " + ex.Message);
                return CreateResponseError(@"No new Setting found for Update.", ex.Message, RegistrationResponseCode.NoNewSettingChange);
            }
            catch (BadRequestException ex)
            {
                stringList.Add("BadRequestException and exception message is: " + ex.Message);
                return CreateResponseError(@"Bad Request Exception.", ex.Message, RegistrationResponseCode.BadRequestError);
            }
            catch (CodeNotExistException ex)
            {
                stringList.Add("CodeNotExistException and exception message is: " + ex.Message);
                return CreateResponseError(@"Site Code doesn't exist.", ex.Message, RegistrationResponseCode.SiteCodeNotExist);
            }
            catch (NotAccessibleException ex)
            {
                stringList.Add("NotAccessibleException and exception message is: " + ex.Message);
                return CreateResponseError(@"Site Code inactive/not found.", ex.Message, RegistrationResponseCode.SiteCodeInAcive);
            }
            catch (Exception ex)
            {
                stringList.Add("exception caught and exception message is: " + ex.Message);
                return CreateResponseError("@Registration update failed.", ex.Message, RegistrationResponseCode.RegistrationUpdateFailed);
            }
            finally
            {
                stringList.Add("End of UpdateTerminalRegistrationInfo() in TerminalRegistration.cs ");
                FileWriter.WriteToFile(stringList, "Registration Integration Logs", "RegistrationLogs ");
                stringList.Clear();
            }
        }

        public CompanySiteModelResponse ValidateCompanyInfo(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("Inside ValidateCompanyInfo() in TerminalRegistration.cs");
                stringList.Add("calling registrationService.ValidateCompanyInfo(inSyndicateCode, siteCode)");
                
                IRegistrationIntegrationService registrationService = new RegistrationIntagrationService();
                var response = registrationService.ValidateCompanyInfo(inSyndicateCode, siteCode);
                stringList.Add("After calling registrationservice file's  ValidateCompanyInfo() response received is: " + response.ToString());
                return CreateSiteModelResponseNoError(response);
            }
            catch (AuthenticationFailedException ex)
            {
                stringList.Add("Authentication failed and exception message is: " + ex.Message);
                return CreateSiteModelResponseNoError(@"Failed to Authenticate");
            }
            catch (NoSettingChangeException ex)
            {
                stringList.Add("NoSettingChangeException and exception message is: " + ex.Message);
                return CreateSiteModelResponseNoError(@"No new Setting found for Update."); 
            }
            catch (BadRequestException ex)
            {
                stringList.Add("BadRequestException and exception message is: " + ex.Message);
                return CreateSiteModelResponseNoError(@"Bad Request Exception."); 
            }
            catch (CodeNotExistException ex)
            {
                stringList.Add("CodeNotExistException and exception message is: " + ex.Message);
                return CreateSiteModelResponseNoError(@"Site Code doesn't exist."); 
            }
            catch (NotAccessibleException ex)
            {
                stringList.Add("NotAccessibleException and exception message is: " + ex.Message);
                return CreateSiteModelResponseNoError(@"Site Code inactive/not found."); 
            }
            catch (Exception ex)
            {
                stringList.Add("exception caught and exception message is: " + ex.Message);
                return CreateSiteModelResponseNoError(@"Registration update failed.");  
            }
            finally
            {
                stringList.Add("End of ValidateCompanyInfo() in TerminalRegistration.cs ");
                FileWriter.WriteToFile(stringList, "Registration Integration Logs", "RegistrationLogs ");
                stringList.Clear();
            }
        }

        #endregion

        #region private

       
        private ApiTerminalViewModel CreateTerminalViewModel(TerminalModel terminalRegistrationInfo)
        {
            stringList.Add("Inside CreateTerminalViewModel() ");
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
                stringList.Add("Terminal Model Json is: " + requestData.ToString());
            }
            catch (Exception ex)
            {
                stringList.Add("Exception in CreateTerminalViewModel : " + ex.Message);
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

        private CompanySiteModelResponse CreateSiteModelResponseNoError(ApiCompanySiteViewModel companyModelResponse)
        {
            return new CompanySiteModelResponse
            {
                CompanyName = companyModelResponse.CompanyName,
                IsSuccessful = companyModelResponse.IsSuccessful,
                Message = companyModelResponse.Message,
                CompanyId = companyModelResponse.CompanyId,
                IsCompanyActive = companyModelResponse.IsCompanyActive,
                IsSiteActive = companyModelResponse.IsSiteActive,
                SiteCode = companyModelResponse.SiteCode,
                SiteName = companyModelResponse.SiteName,
                SyndicateCode = companyModelResponse.SyndicateCode
            };
        }

        private CompanySiteModelResponse CreateSiteModelResponseNoError(string message)
        {
            return new CompanySiteModelResponse
            {
                CompanyName = "",
                IsSuccessful = false,
                Message = message,
                CompanyId = 0,
                IsCompanyActive = false,
                IsSiteActive = false,
                SiteCode = 0,
                SiteName = "",
                SyndicateCode = ""
            };
        }
       
        #endregion
    }
}
