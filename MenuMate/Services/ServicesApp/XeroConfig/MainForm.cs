using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ServiceProcess;
using WinOSInfo;
using System.IO;
using AccountingIntegration;
using AccountingIntegration.Model;
using AccountingIntegration.MYOB;
using MYOB.AccountRight.SDK;
using MYOB.AccountRight.SDK.Services;
using System.Reflection;
using Newtonsoft.Json;

namespace AccountingConfig
{
    public partial class MainForm : Form
    {
        const string HeadNodeText = "Accounting Organisations";
        const string UserAgentStringXero = "Xero Integration";
        const string UserAgentStringMYOB = "MYOB Integration";
        bool _configSaved = false;
        WinInfo _winInfo = WinInfo.Instance;
        private AccountingAuthConfiguration _accountingAuthConfiguration;
        int _selectedIndex;
        private Credentials _selectedCredential;
        private string _appInUseXero;
        private string _appInUseMYOB;
        private IApiConfiguration _configurationCloud;
        private IOAuthKeyService _oAuthKeyService;

        #region Public

        public MainForm()
        {
            InitializeComponent();
        }

        #endregion

        #region Properties

        #endregion

        #region Events


        private void MainForm_Load(object sender, EventArgs e)
        {
            ShowRestartingAccountingSvc(false);
            InitializeForm();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            CloseAccountingConfig();
        }

        private void btnAddCompany_Click(object sender, EventArgs e)
        {
            AddNewOrganisation();
            PopulateCompanyTreeView();
            AccountingIntegrationConfigManager.Instance.SaveAccountingConfiguration(_accountingAuthConfiguration);
        }

        private void btnEditCompany_Click(object sender, EventArgs e)
        {
            int indexAccountingSystem = 0;
            if (isMYOBAccounting.Checked)
                indexAccountingSystem = 1;

                if (!_configSaved && ConfirmAction(string.Format(@"Save {0}", _selectedCredential.Organisation)))
                {
                    SaveCompanyInfo();
                }

                if (tvCompanies.SelectedNode.Text == HeadNodeText)
                {
                    ShowCompanyInfoError();
                }
                else
                {
                    _selectedIndex = tvCompanies.SelectedNode.Index;
                    if (isXeroAccounting.Checked)
                        _selectedCredential = _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials[_selectedIndex];
                    else
                        _selectedCredential = _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials[_selectedIndex];
                    ShowCompanyInfo(_selectedCredential);
                }
        }

        private void btnDelCompany_Click(object sender, EventArgs e)
        {
            DelCompanyInfo();
            AccountingIntegrationConfigManager.Instance.SaveAccountingConfiguration(_accountingAuthConfiguration);
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            bool checkedstatus = cbAppInUsed.Checked ? true : false;
            SaveCompanyInfo();
            AccountingIntegrationConfigManager.Instance.SaveAccountingConfiguration(_accountingAuthConfiguration);
            PopulateCompanyTreeView();
            cbAppInUsed.Checked = checkedstatus;
            if (_selectedCredential.UserAgentString == UserAgentStringMYOB)
                GetTokenForMYOB(_selectedCredential);
            RestartAccountingService();
        }

        private void GetTokenForMYOB(Credentials crendetials)
        {
            try
            {
                _configurationCloud = new ApiConfiguration(crendetials.ConsumerKey, crendetials.ConsumerSecret, "http://desktop");
                _oAuthKeyService = new MYOBOAuthKeyService();
                _oAuthKeyService.OAuthResponse = null;
                if (_oAuthKeyService.OAuthResponse == null)
                {
                    var oauthService = new OAuthService(_configurationCloud);
                    string _accessCode = OAuthLogin.GetAuthorizationCode(_configurationCloud);
                    _oAuthKeyService.OAuthResponse =
                        oauthService.GetTokens(_accessCode);
                    var frmLogin = new CompanyFileLogin();
                    frmLogin.ShowDialog(this);
                    if (frmLogin.Username.Length > 0)
                    {
                        AccountingIntegrationConfigManager.Instance.SaveCompanyFileCredentials(frmLogin.Username, frmLogin.Password);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        private void ConfigurationChanged(object sender, EventArgs e)
        {
            SetConfigSaved(false);
        }

        #endregion


        #region Private

        private void InitializeForm()
        {
            lbVersion.Text = @"Version 5.7.0.21";
            _accountingAuthConfiguration = AccountingIntegrationConfigManager.Instance.GetConfiguaration();
            if (_accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.Credentials.Count == 0 && _accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.Credentials.Count == 0)
            {
                AddXeroOrganisation();
                AddMYOBOrganisation();
            }
            _appInUseXero = _accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse;
            _appInUseMYOB = _accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.AppInUse;

            if (_appInUseXero != null)
            {
                isXeroAccounting.Checked = _appInUseXero.Length > 0 ? true : false;
            }
            if (_appInUseMYOB != null)
            {
                isMYOBAccounting.Checked = _appInUseMYOB.Length > 0 ? true : false;
            }
            if(!isXeroAccounting.Checked && !isMYOBAccounting.Checked)
                isXeroAccounting.Checked = true;
            
            PopulateCompanyTreeView();
        }

        private void PopulateCompanyTreeView()
        {
            _selectedIndex = 0;
            tvCompanies.Nodes.Clear();
            tvCompanies.Nodes.Add("0", HeadNodeText);
            int indexAccountingSystem = 0;
            
            if (isMYOBAccounting.Checked)
                indexAccountingSystem = 1;

            foreach (var credential in _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials)
                {
                    tvCompanies.Nodes[0].Nodes.Add(credential.AppID, credential.Organisation);
                }
                tvCompanies.TopNode.Expand();
                _selectedCredential = _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials.SingleOrDefault(s => s.AppID == _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.AppInUse);
                if (_selectedCredential != null)
                {
                    _selectedIndex = _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials.IndexOf(_selectedCredential);
                }
                else
                {
                    _selectedCredential = _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials[_selectedIndex];
                }
                ShowCompanyInfo(_selectedCredential);
                tvCompanies.SelectedNode = tvCompanies.Nodes[0].Nodes[_selectedIndex];
        }

        private void ShowCompanyInfo(Credentials credential)
        {
            tbCompanyName.Text = credential.Organisation;
            tbConsumerKey.Text = credential.ConsumerKey;
            tbConsumerSecret.Text = credential.ConsumerSecret;
            cbAppInUsed.Checked = _appInUseXero == credential.AppID ? _appInUseXero == credential.AppID : _appInUseMYOB == credential.AppID;
            cbAppType.Checked = credential.AppType.ToUpper() == @"PRIVATE";
        }
        private void AddNewOrganisation()
        {
            if (isXeroAccounting.Checked)
                AddXeroOrganisation();
            else
                AddMYOBOrganisation();
        }

        private void AddXeroOrganisation()
        {
            var credential = new Credentials();
            credential.AppID = GetNextAppId();
            credential.APIEndpointURL = @"https://api.xero.com/api.xro/2.0/";
            credential.AppType = "private";
            credential.ConsumerKey = "";
            credential.ConsumerSecret = "";
            credential.UserAgentString = UserAgentStringXero;
            credential.Organisation = "Xero Organisation";
            credential.PfxFileName = "public_privatekey.pfx";
            credential.PfxPassword = "2012menumatexeroapril";
            _accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.Credentials.Add(credential);
        }

        private void AddMYOBOrganisation()
        {
            var credential = new Credentials();
            credential.AppID = GetNextAppId();
            credential.APIEndpointURL = "";
            credential.AppType = "private";
            credential.ConsumerKey = "";
            credential.ConsumerSecret = "";
            credential.UserAgentString = UserAgentStringMYOB;
            credential.Organisation = "MYOB Organisation";
            credential.PfxFileName = "";
            credential.PfxPassword = "";
            _accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.Credentials.Add(credential);
        }


        private string GetNextAppId()
        {
            string appId = "1";
            Int32 appIdIntXero = 0;
            Int32 appIdIntMYOB = 0;
            var credentialXero = _accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.Credentials.LastOrDefault();
            if (credentialXero != null)
            {
                appIdIntXero = Convert.ToInt32(credentialXero.AppID);
            }
            var credentialMYOB = _accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.Credentials.LastOrDefault();
            if (credentialMYOB != null)
            {
                appIdIntMYOB += Convert.ToInt32(credentialMYOB.AppID);
            }
            return appId = appIdIntXero > appIdIntMYOB ? Convert.ToString(appIdIntXero + 1) : Convert.ToString(appIdIntMYOB + 1);
        }

        private void CloseAccountingConfig()
        {
            if (!_configSaved && ConfirmAction(string.Format(@"Save {0}", _selectedCredential.Organisation)))
            {
                SaveCompanyInfo();
            }
            Close();
        }

        private void SaveCompanyInfo()
        {
            _selectedCredential.Organisation = tbCompanyName.Text;
            _selectedCredential.ConsumerKey = tbConsumerKey.Text;
            _selectedCredential.ConsumerSecret = tbConsumerSecret.Text;
            if (cbAppInUsed.Checked)
            {
                if (isXeroAccounting.Checked)
                {
                    _accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse = _selectedCredential.AppID;
                    if (_accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.AppInUse != null)
                        _accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.AppInUse = "";
                    //}
                }
                else
                {
                    _accountingAuthConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.AppInUse = _selectedCredential.AppID;
                    if (_accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse != null)
                        _accountingAuthConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse = "";
                }
            }
            _selectedCredential.AppType = cbAppType.Checked ? @"private" : @"public";
            SetConfigSaved(true);
        }

        private void SetConfigSaved(bool inSaved)
        {
            _configSaved = inSaved;
        }

        private bool ConfirmAction(string inText)
        {
            string message = @"";
            if(isXeroAccounting.Checked)
                message = @"Xero Integration Configuration";
            else
                message = @"MYOB Integration Configuration";
            return MessageBox.Show(inText, message, MessageBoxButtons.YesNo) == DialogResult.OK;
        }

        private void DelCompanyInfo()
        {
            int indexAccountingSystem = 0;

            if (isMYOBAccounting.Checked)
                indexAccountingSystem = 1;

            if (_accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials.Count > 1)
            {
                if (tvCompanies.SelectedNode.Text == HeadNodeText)
                {
                    ShowCompanyInfoError();
                }
                else
                {
                    _selectedIndex = tvCompanies.SelectedNode.Index;
                    _selectedCredential = _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials[_selectedIndex];
                    _accountingAuthConfiguration.AccountingSystemList.ElementAt(indexAccountingSystem).CredentialsList.Credentials.Remove(_selectedCredential);
                    PopulateCompanyTreeView();
                }
            }
        }

        private void ShowErrorMsg(string inErrorMsg)
        {
            MessageBox.Show(inErrorMsg, @"Accounting Integration Configuration",MessageBoxButtons.OK,MessageBoxIcon.Error);
        }

        private bool NeedToBeAdmin()
        {
            WinOSTypes wType = _winInfo.WinOSType();
            return (wType == WinOSTypes.WinVista || wType == WinOSTypes.Win7);
        }

        private void ShowCompanyInfoError()
        {
            ShowErrorMsg(@"No organisation selected");
        }

        private void RestartAccountingService()
        {
            if (NeedToBeAdmin())
            {
                if (!_winInfo.IsAdministrator())
                {
                    ShowErrorMsg(@"Failed to restart Accounting Service. Close Accounting Config and run it as administrator.");
                    return;
                }
            }

            //:::::::::::::::::::::::::::::::::::::::::

            ShowRestartingAccountingSvc(true);
            RestartService(@"MenumateServiceAccounting", 10000);
            ShowRestartingAccountingSvc(false);
        }

        private void ShowRestartingAccountingSvc(bool inShow)
        {
            tlpMain.Enabled = !inShow;
            //lbRestartingSvc.Visible = inShow;
        }

        private void RestartService(string serviceName, int timeoutMilliseconds)
        {
            ServiceController service = new ServiceController(serviceName);

            try
            {
                int millisec1 = Environment.TickCount;
                TimeSpan timeout = TimeSpan.FromMilliseconds(timeoutMilliseconds);

                if (service.Status.ToString() != @"Stopped")
                {
                    service.Stop();
                    service.WaitForStatus(ServiceControllerStatus.Stopped, timeout);
                }

                // count the rest of the timeout
                int millisec2 = Environment.TickCount;
                timeout = TimeSpan.FromMilliseconds(timeoutMilliseconds - (millisec2 - millisec1));

                service.Start();
                service.WaitForStatus(ServiceControllerStatus.Running, timeout);
            }
            catch (Exception exc)
            {
                ShowErrorMsg(exc.Message);
            }
        }

        #endregion

       private void AccountingSystem_CheckedChanged(object sender, EventArgs e)
        {
            PopulateCompanyTreeView();
        }
    }
}
