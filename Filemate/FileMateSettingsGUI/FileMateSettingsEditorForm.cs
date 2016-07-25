using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Xml;
using System.Xml.XPath;
using FileMate.Core.Xml;
using System.ServiceProcess;

namespace FileMateSettingsGUI {
    public partial class FileMateSettingsEditorForm : Form {

        private const string SUMMA_SETTINGS_FILENAME = @"summaSettings.xml";
        private const string SUMMA_ID_MAP_FILENAME = @"summaIdMap.xml";
        private const string FILEMATE_CONFIG_FILENAME = @"FileMateConfig.xml";

        private const string SUMMA_FTP_URL = @"ftp://ftp.summa.co.nz/";

        private SummaSettingsDTO _summaSettings;
        private SummaIdMapDTO _summaIdMap;
        private CopyGroupDTO _fileMateSettings;

        private string _path = Path.GetDirectoryName(Application.ExecutablePath);

        public FileMateSettingsEditorForm() {
            InitializeComponent();


            LoadSettings();
            ShowAllSettings();
        }

        #region Private Methods

        /// <summary>
        /// Load all settings xml files from the current directory.
        /// </summary>
        /// <returns></returns>
        private void LoadSettings() {

            if (File.Exists(Path.Combine(_path, SUMMA_SETTINGS_FILENAME))) {
                LoadSummaSettings();
            }
            else {
                SetSummaDefaults();
            }

            if (File.Exists(Path.Combine(_path, SUMMA_ID_MAP_FILENAME))) {
                LoadSummaIdMapSettings();
            }
            else {
                SetSummaIdMapDefaults();
            }

            if (File.Exists(Path.Combine(_path, FILEMATE_CONFIG_FILENAME))) {
                LoadFileMateSettings();
            }
            else {
                SetFileMateSettingsDefaults();
            }
        }

        private void LoadFileMateSettings() {
            XmlDocument doc = new XmlDocument();
            doc.Load(Path.Combine(_path, FILEMATE_CONFIG_FILENAME));

            _fileMateSettings = new CopyGroupDTO(doc);
        }

        private void LoadSummaIdMapSettings() {
            XmlDocument doc = new XmlDocument();
            doc.Load(Path.Combine(_path, SUMMA_ID_MAP_FILENAME));
            _summaIdMap = new SummaIdMapDTO();

            XmlNodeList entries = null;
            try {
                entries = doc.DocumentElement.SelectNodes("/SummaSiteIdMap/SummaIdMapEntry");

                foreach (XmlNode entry in entries) {
                    _summaIdMap.AddMapEntry(
                        Int32.Parse(entry.Attributes["siteId"].Value),
                        Int32.Parse(entry.Attributes["summaSiteId"].Value)
                        );
                }
            }
            catch (XPathException xpe) {
                new ErrorHandler(xpe.ToString()).ShowDialog();
            }
            catch (FormatException fe) {
                new ErrorHandler(fe.ToString()).ShowDialog();
            }
        }

        private void LoadSummaSettings() {

            XmlDocument doc = new XmlDocument();
            doc.Load(Path.Combine(_path, SUMMA_SETTINGS_FILENAME));

            _summaSettings = new SummaSettingsDTO();
            _summaSettings.ExportPath = doc.DocumentElement.Attributes["OutputFilePath"].Value;
            _summaSettings.Port = doc.DocumentElement.Attributes["ListenPort"].Value;
            _summaSettings.Enabled = doc.DocumentElement.Attributes["Enabled"] != null && doc.DocumentElement.Attributes["Enabled"].Value == "true" ? true : false;
        }

        private void SetSummaDefaults() {
            _summaSettings = new SummaSettingsDTO();
            _summaSettings.Port = "4444";
            _summaSettings.ExportPath = @"c:\program files\menumate\summa\";
            _summaSettings.Enabled = false;
        }

        private void SetFileMateSettingsDefaults() {
            _fileMateSettings = new CopyGroupDTO();

            CopyOperationDTO copyOp = new CopyOperationDTO();
            copyOp.Name = "summa";
            copyOp.DeleteSource = true;
            copyOp.CalledFromExternal = true;
            copyOp.SourceUrl = @"c:\program files\menumate\summa\";
            copyOp.Schedule = "04:00";
            copyOp.Pattern = "*.txt";
            copyOp.Enabled = false;

            FileMateTargetDTO ftpTarget = new FileMateTargetDTO();
            ftpTarget.TargetUrl = "ftp://ftp.summa.co.nz/";
            ftpTarget.Username = "menumate";
            ftpTarget.Password = "aceking21";
            copyOp.Targets.Add(ftpTarget);

            FileMateTargetDTO archiveTarget = new FileMateTargetDTO();
            archiveTarget.TargetUrl = @"c:\program files\menumate\summa\archive\{date}\";
            copyOp.Targets.Add(archiveTarget);

            _fileMateSettings.Operations.Add(copyOp);
        }

        private void SetSummaIdMapDefaults() {
            _summaIdMap = new SummaIdMapDTO();
            _summaIdMap.AddMapEntry(1, 1);
        }

        private void ShowAllSettings() {

            if (_summaSettings != null) {
                tbExportPath.Text = _summaSettings.ExportPath;
                tbPort.Text = _summaSettings.Port;
                cbSummaEnabled.Checked = _summaSettings.Enabled;
            }

            if (_summaIdMap != null) {
                lvIdMap.Items.Clear();
                foreach (KeyValuePair<int, int> kvp in _summaIdMap.IdMap) {

                    ListViewItem item = new ListViewItem(kvp.Key.ToString());
                    item.SubItems.Add(kvp.Value.ToString());
                    lvIdMap.Items.Add(item);
                }
            }

            if (_fileMateSettings != null) {
                lvFileMateCopyOps.Clear();
                foreach (CopyOperationDTO op in _fileMateSettings.Operations) {
                    if (op.Name.Equals("summa")) {
                        tbSchedule.Text = op.Schedule;
                        foreach (FileMateTargetDTO t in op.Targets) {
                            if (t.TargetUrl.Equals(SUMMA_FTP_URL)) {
                                tbUsername.Text = t.Username;
                                tbPassword.Text = t.Password;
                                break;
                            }
                        }
                    }
                    else {

                        ListViewItem i = new ListViewItem(op.Name, 0);
                        i.Tag = op;
                        lvFileMateCopyOps.Items.Add(i);
                    }
                }
            }
        }

        private void btnCancel_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void btnSaveAll_Click(object sender, EventArgs e) {

            try {
                // write summa settings xml file
                XmlDocument summaSettingsDoc = new XmlDocument();

                summaSettingsDoc.AppendChild(summaSettingsDoc.CreateNode(XmlNodeType.XmlDeclaration, "", ""));
                XmlNode rootNode = summaSettingsDoc.CreateNode(XmlNodeType.Element, "SummaSettings", "");

                XmlAttribute ListenPortAttr = summaSettingsDoc.CreateAttribute("ListenPort");
                ListenPortAttr.InnerText = tbPort.Text;
                rootNode.Attributes.Append(ListenPortAttr);

                XmlAttribute OutputFilePathAttr = summaSettingsDoc.CreateAttribute("OutputFilePath");
                OutputFilePathAttr.InnerText = Helper.FormatUrl(tbExportPath.Text);
                rootNode.Attributes.Append(OutputFilePathAttr);                

                XmlAttribute enabledAttr = summaSettingsDoc.CreateAttribute("Enabled");
                enabledAttr.InnerText = cbSummaEnabled.Checked ? "true" : "false";
                rootNode.Attributes.Append(enabledAttr);

                summaSettingsDoc.AppendChild(rootNode);

                if (File.Exists(Path.Combine(_path, SUMMA_SETTINGS_FILENAME))) {
                    File.Copy(
                        Path.Combine(_path, SUMMA_SETTINGS_FILENAME),
                        Path.Combine(_path, SUMMA_SETTINGS_FILENAME + ".bak"), true);
                }
                summaSettingsDoc.Save(Path.Combine(_path, SUMMA_SETTINGS_FILENAME));


                // write id map
                XmlDocument summaIdMapDoc = new XmlDocument();
                summaIdMapDoc.AppendChild(summaIdMapDoc.CreateNode(XmlNodeType.XmlDeclaration, "", ""));


                rootNode = summaIdMapDoc.CreateNode(XmlNodeType.Element, "SummaSiteIdMap", "");
                foreach (KeyValuePair<int, int> kvp in _summaIdMap.IdMap) {

                    XmlNode n = summaIdMapDoc.CreateNode(XmlNodeType.Element, "SummaIdMapEntry", "");

                    XmlAttribute siteIdAttr = summaIdMapDoc.CreateAttribute("siteId");
                    siteIdAttr.InnerText = kvp.Key.ToString();

                    XmlAttribute summaIdAttr = summaIdMapDoc.CreateAttribute("summaSiteId");
                    summaIdAttr.InnerText = kvp.Value.ToString();

                    n.Attributes.Append(siteIdAttr);
                    n.Attributes.Append(summaIdAttr);

                    rootNode.AppendChild(n);

                }

                summaIdMapDoc.AppendChild(rootNode);
                if (File.Exists(Path.Combine(_path, SUMMA_ID_MAP_FILENAME))) {
                    File.Copy(
                        Path.Combine(_path, SUMMA_ID_MAP_FILENAME),
                        Path.Combine(_path, SUMMA_ID_MAP_FILENAME + ".bak"), true);
                }

                summaIdMapDoc.Save(Path.Combine(_path, SUMMA_ID_MAP_FILENAME));

                CaptureSummaInfo();

                // backup
                if (File.Exists(Path.Combine(_path, FILEMATE_CONFIG_FILENAME))) {
                    File.Copy(
                        Path.Combine(_path, FILEMATE_CONFIG_FILENAME),
                        Path.Combine(_path, FILEMATE_CONFIG_FILENAME + ".bak"), true);
                }

                // write filemate copy operations (including summa copyop)
                _fileMateSettings.AsXml().Save(Path.Combine(_path, FILEMATE_CONFIG_FILENAME));

                // since we have saved/modified settings.. need to restart the windows service
                // restartService("FileMate", 1000);

                // close form
                this.Close();
            }
            catch (IOException ioe) {
                new ErrorHandler(ioe.ToString()).ShowDialog();
            }
            catch (UnauthorizedAccessException uae) {
                new ErrorHandler(uae.ToString()).ShowDialog();
            }
        }

        private void restartService(string serviceName, int timeoutMilliseconds)
        {
            ServiceController service = new ServiceController(serviceName);
            try
            {
                int millisec1 = Environment.TickCount;
                TimeSpan timeout = TimeSpan.FromMilliseconds(timeoutMilliseconds);

                service.Stop();
                service.WaitForStatus(ServiceControllerStatus.Stopped, timeout);

                // count the rest of the timeout
                int millisec2 = Environment.TickCount;
                timeout = TimeSpan.FromMilliseconds(timeoutMilliseconds - (millisec2 - millisec1));

                service.Start();
                service.WaitForStatus(ServiceControllerStatus.Running, timeout);
            }
            catch(Exception ex)
            {
                MessageBox.Show("Failed to restart the service FileMate. Please restart the service manually");
            }
        }

        private void CaptureSummaInfo() {
            // capture summa-specific copy operation information from form
            foreach (CopyOperationDTO co in _fileMateSettings.Operations) {
                if (co.Name.Equals("summa")) {
                    co.Schedule = tbSchedule.Text;
                    foreach (FileMateTargetDTO t in co.Targets) {
                        if (t.TargetUrl.Equals(SUMMA_FTP_URL)) {
                            t.Username = tbUsername.Text;
                            t.Password = tbPassword.Text;
                            break;
                        }
                    }
                }
            }
        }

        private void btnAddIdMapEntry_Click(object sender, EventArgs e) {

            CaptureSummaInfo();

            NewIdMapEntry newEntryForm = new NewIdMapEntry();
            newEntryForm.ShowDialog();

            if (newEntryForm.DialogResult == DialogResult.OK) {
                try {
                    int siteId = Int32.Parse(newEntryForm.SiteId);
                    int summaId = Int32.Parse(newEntryForm.SummaId);

                    _summaIdMap.AddMapEntry(siteId, summaId);
                }
                catch (ArgumentException ae) {
                    new ErrorHandler(ae.ToString()).ShowDialog();
                }
                catch (FormatException fe) {
                    new ErrorHandler(fe.ToString()).ShowDialog();
                }
            }
            ShowAllSettings();
        }

        private void lvIdMap_ItemActivate(object sender, EventArgs e) {

            ListViewItem selectedItem = lvIdMap.SelectedItems[0];
            string siteid = selectedItem.Text;
            string summaid = selectedItem.SubItems[1].Text;
            NewIdMapEntry idMapForm = new NewIdMapEntry(siteid, summaid);
            DialogResult result = idMapForm.ShowDialog();
            if (result == DialogResult.OK) {
                try {
                    _summaIdMap.RemoveMapEntry(Int32.Parse(selectedItem.Text));
                    _summaIdMap.AddMapEntry(Int32.Parse(idMapForm.SiteId),
                        Int32.Parse(idMapForm.SummaId));
                }
                catch (FormatException fe) {
                    new ErrorHandler(fe.ToString()).ShowDialog();
                }
            }
            ShowAllSettings();
        }

        private void btnDeleteIdMapEntry_Click(object sender, EventArgs e) {
            if (lvIdMap.SelectedItems.Count > 0) {

                try {
                    int key = Int32.Parse(lvIdMap.SelectedItems[0].Text);

                    _summaIdMap.IdMap.Remove(key);
                }
                catch (FormatException fe) {
                    new ErrorHandler(fe.ToString()).ShowDialog();
                }
            }
            ShowAllSettings();
        }

        #endregion

        private void btnBrowseExportPath_Click(object sender, EventArgs e) {
            FolderBrowserDialog fb = new FolderBrowserDialog();
            fb.ShowNewFolderButton = true;

            if (tbExportPath.Text != null || tbExportPath.Text != string.Empty) {
                fb.SelectedPath = tbExportPath.Text;
            }

            DialogResult result = fb.ShowDialog();
            if (result == DialogResult.OK) {
                tbExportPath.Text = fb.SelectedPath;
            }
        }

        private void lvFileMateCopyOps_ItemActivate(object sender, EventArgs e) {
            CopyOperationDTO op = (CopyOperationDTO) lvFileMateCopyOps.SelectedItems[0].Tag;
            AddEditCopyOperation editCopyOp = new AddEditCopyOperation(op);
            editCopyOp.ShowDialog();    // clicking ok will update the copy operation dto

        }

        private void btnAddCopyOp_Click(object sender, EventArgs e) {

            // will create a new copy operation dto which will need to be added to the collection
            AddEditCopyOperation copyOpfrm = new AddEditCopyOperation();
            DialogResult result = copyOpfrm.ShowDialog();
            if (result == DialogResult.OK) {
                _fileMateSettings.Operations.Add(copyOpfrm.CopyOperation);
                ShowAllSettings();
            }
        }

        private void btnDeleteCopyOp_Click(object sender, EventArgs e) {
            if (lvFileMateCopyOps.SelectedItems.Count == 1) {
                CopyOperationDTO op = (CopyOperationDTO) lvFileMateCopyOps.SelectedItems[0].Tag;
                _fileMateSettings.Operations.Remove(op);
                ShowAllSettings();
            }
        }
    }
}
