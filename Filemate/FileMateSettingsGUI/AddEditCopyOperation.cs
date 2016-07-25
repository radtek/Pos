using System;
using System.Windows.Forms;
using FileMate.Core.Xml;

namespace FileMateSettingsGUI {
    public partial class AddEditCopyOperation : Form {

        CopyOperationDTO _copyOperation = null;

        public CopyOperationDTO CopyOperation {
            get { return _copyOperation; }
        }

        public AddEditCopyOperation(CopyOperationDTO op) {
            InitializeComponent();

            _copyOperation = op;

            ShowOperationSettings();
        }

        public AddEditCopyOperation() {
            InitializeComponent();
                _copyOperation = new CopyOperationDTO();
        }

        private void ShowOperationSettings() {
            tbName.Text = _copyOperation.Name;
            tbSource.Text = _copyOperation.SourceUrl;
            tbSrcUname.Text = _copyOperation.SourceUname;
            tbSrcPwd.Text = _copyOperation.SourcePwd;
            cbCalledFromExternal.Checked = _copyOperation.CalledFromExternal;
            cbDeleteSource.Checked = _copyOperation.DeleteSource;
            tbPattern.Text = _copyOperation.Pattern;
            tbSchedule.Text = _copyOperation.Schedule;
            cbEnabled.Checked = _copyOperation.Enabled;

            lvCopyOperationTargets.Clear();
            foreach (FileMateTargetDTO target in _copyOperation.Targets) {
                ListViewItem i = new ListViewItem(target.TargetUrl, 0);
                i.Tag = target;
                lvCopyOperationTargets.Items.Add(i);
            }
        }

        private void btnOk_Click(object sender, EventArgs e) {
            CaptureFormData();
        }

        private void CaptureFormData() {
            _copyOperation.Name = tbName.Text;
            _copyOperation.SourceUrl = tbSource.Text;
            _copyOperation.SourceUname = tbSrcUname.Text;
            _copyOperation.SourcePwd = tbSrcPwd.Text;
            _copyOperation.Pattern = tbPattern.Text;
            _copyOperation.Schedule = tbSchedule.Text;
            _copyOperation.DeleteSource = cbDeleteSource.Checked;
            _copyOperation.CalledFromExternal = cbCalledFromExternal.Checked;
            _copyOperation.Enabled = cbEnabled.Checked;
        }

        private void btnAddTarget_Click(object sender, EventArgs e) {
            CaptureFormData();
            NewFileMateTarget targetFrm = new NewFileMateTarget();
            DialogResult result = targetFrm.ShowDialog();
            if (result == DialogResult.OK) {
                _copyOperation.Targets.Add(targetFrm.Target);
                ShowOperationSettings();
            }
        }

        private void btnDeleteTarget_Click(object sender, EventArgs e) {
            CaptureFormData();
            if (lvCopyOperationTargets.SelectedItems.Count == 1) {
                FileMateTargetDTO targetDto = (FileMateTargetDTO) lvCopyOperationTargets.SelectedItems[0].Tag;
                _copyOperation.Targets.Remove(targetDto);
                ShowOperationSettings();
            }
        }

        private void lvCopyOperationTargets_ItemActivate(object sender, EventArgs e) {
            CaptureFormData();
            FileMateTargetDTO t = (FileMateTargetDTO) lvCopyOperationTargets.SelectedItems[0].Tag;
            NewFileMateTarget targetFrm = new NewFileMateTarget(t);
            targetFrm.ShowDialog();
        }

        private void btnBrowseExportPath_Click(object sender, EventArgs e) {
            FolderBrowserDialog fb = new FolderBrowserDialog();
            DialogResult result = fb.ShowDialog();
            if (result == DialogResult.OK) {
                tbSource.Text = fb.SelectedPath;
            }
        }
    }
}
