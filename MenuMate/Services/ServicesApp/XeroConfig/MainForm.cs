using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.ServiceProcess;

using XeroIntegration;
using WinOSInfo;
using XeroIntegration.Model;

namespace XeroConfig
{
    public partial class MainForm : Form
    {
        const string HeadNodeText = "Xero Organisations";
        bool _configSaved = false;
        WinInfo _winInfo = WinInfo.Instance;
        private XeroAuthConfiguration _xeroAuthConfiguration;
        int _selectedIndex;
        private Credentials _selectedCredential;
        private string _appInUse;



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
            ShowRestartingXeroSvc(false);
            InitializeForm();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            CloseXeroConfig();
        }

        private void btnAddCompany_Click(object sender, EventArgs e)
        {
            AddNewOrganisation();
            PopulateCompanyTreeView();
            XeroIntegrationConfigManager.Instance.SaveXeroConfiguration(_xeroAuthConfiguration);
        }

        private void btnEditCompany_Click(object sender, EventArgs e)
        {
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
                _selectedCredential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials[_selectedIndex];
                ShowCompanyInfo(_selectedCredential);
            }
        }

        private void btnDelCompany_Click(object sender, EventArgs e)
        {
            DelCompanyInfo();
            XeroIntegrationConfigManager.Instance.SaveXeroConfiguration(_xeroAuthConfiguration);
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveCompanyInfo();
            XeroIntegrationConfigManager.Instance.SaveXeroConfiguration(_xeroAuthConfiguration);
            PopulateCompanyTreeView();
            RestartXeroService();
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
            _xeroAuthConfiguration = XeroIntegrationConfigManager.Instance.GetConfiguaration();
            if (_xeroAuthConfiguration.OAuth.CredentialsList.Credentials.Count == 0)
            {
                AddNewOrganisation();
            }
            _appInUse = _xeroAuthConfiguration.OAuth.CredentialsList.AppInUse;
            PopulateCompanyTreeView();
        }

        private void PopulateCompanyTreeView()
        {
            _selectedIndex = 0;
            tvCompanies.Nodes.Clear();
            tvCompanies.Nodes.Add("0", HeadNodeText);
            foreach (var credential in _xeroAuthConfiguration.OAuth.CredentialsList.Credentials)
            {
                tvCompanies.Nodes[0].Nodes.Add(credential.AppID, credential.Organisation);
            }
            tvCompanies.TopNode.Expand();
            _selectedCredential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.SingleOrDefault(s => s.AppID == _xeroAuthConfiguration.OAuth.CredentialsList.AppInUse);

            if (_selectedCredential != null)
            {
                _selectedIndex = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.IndexOf(_selectedCredential);
            }
            else
            {
                _selectedCredential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials[_selectedIndex];
            }
            ShowCompanyInfo(_selectedCredential);
            tvCompanies.SelectedNode = tvCompanies.Nodes[0].Nodes[_selectedIndex];
        }

        private void ShowCompanyInfo(Credentials credential)
        {
            tbCompanyName.Text = credential.Organisation;
            tbConsumerKey.Text = credential.ConsumerKey;
            tbConsumerSecret.Text = credential.ConsumerSecret;
            cbAppInUsed.Checked = _appInUse == credential.AppID;
            cbAppType.Checked = credential.AppType.ToUpper() == @"PRIVATE";
        }

        private void AddNewOrganisation()
        {
            var credential = new Credentials();
            credential.AppID = GetNextAppId();
            credential.APIEndpointURL = @"https://api.xero.com/api.xro/2.0/";
            credential.AppType = "private";
            credential.ConsumerKey = "";
            credential.ConsumerSecret = "";
            credential.UserAgentString = "Xero Integration";
            credential.Organisation = "My Organisation";
            credential.PfxFileName = "public_privatekey.pfx";
            credential.PfxPassword = "2012menumatexeroapril";
            _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.Add(credential);
        }

        private string GetNextAppId()
        {
            string appId = "1";
            var credential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.LastOrDefault();
            if (credential != null)
            {
                var appIdInt = Convert.ToInt32(credential.AppID);
                appId = Convert.ToString(appIdInt + 1);
            }
            return appId;
        }

        private void CloseXeroConfig()
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
                _xeroAuthConfiguration.OAuth.CredentialsList.AppInUse = _selectedCredential.AppID;
            _selectedCredential.AppType = cbAppType.Checked ? @"private" : @"public";
            SetConfigSaved(true);
        }

        private void SetConfigSaved(bool inSaved)
        {
            _configSaved = inSaved;
        }

        private bool ConfirmAction(string inText)
        {
            return MessageBox.Show(inText, @"Xero Integration Configuration", MessageBoxButtons.YesNo) == DialogResult.OK;
        }

        private void DelCompanyInfo()
        {

            if (_xeroAuthConfiguration.OAuth.CredentialsList.Credentials.Count > 1)
            {
                if (tvCompanies.SelectedNode.Text == HeadNodeText)
                {
                    ShowCompanyInfoError();
                }
                else
                {
                    _selectedIndex = tvCompanies.SelectedNode.Index;
                    _selectedCredential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials[_selectedIndex];
                    _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.Remove(_selectedCredential);
                    PopulateCompanyTreeView();
                }
            }
        }

        private void ShowErrorMsg(string inErrorMsg)
        {
            MessageBox.Show(inErrorMsg, @"Xero Integration Configuration",MessageBoxButtons.OK,MessageBoxIcon.Error);
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

        private void RestartXeroService()
        {
            if (NeedToBeAdmin())
            {
                if (!_winInfo.IsAdministrator())
                {
                    ShowErrorMsg(@"Failed to restart Xero Service. Close Xero Config and run it as administrator.");
                    return;
                }
            }

            //:::::::::::::::::::::::::::::::::::::::::

            ShowRestartingXeroSvc(true);
            RestartService(@"MenumateServiceXero", 10000);
            //pRestartService(@"Menumate Xero Integration", 10000); // 10 sec timeout 
            ShowRestartingXeroSvc(false);
        }

        private void ShowRestartingXeroSvc(bool inShow)
        {
            tlpMain.Enabled = !inShow;
            lbRestartingSvc.Visible = inShow;
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




    }
}
