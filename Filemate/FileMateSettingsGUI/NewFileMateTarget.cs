using System;
using System.Windows.Forms;
using FileMate.Core.Xml;

namespace FileMateSettingsGUI {
    public partial class NewFileMateTarget : Form {

        private FileMateTargetDTO _target = null;

        public FileMateTargetDTO Target {
            get { return _target; }
        }


        public NewFileMateTarget() {
            InitializeComponent();
        }

        public NewFileMateTarget(FileMateTargetDTO target) {
            InitializeComponent();
            _target = target;
            ShowTargetSettings();
        }

        private void ShowTargetSettings() {
            tbTargetUrl.Text = _target.TargetUrl;
            tbUsername.Text = _target.Username;
            tbpassword.Text = _target.Password;
            cbTargetAllowOverwrite.Checked = _target.AllowOverwrite;
        }

        private void btnCancel_Click(object sender, EventArgs e) {

        }

        private void btnOk_Click(object sender, EventArgs e) {
            if (_target == null) { _target = new FileMateTargetDTO(); }
            _target.TargetUrl = tbTargetUrl.Text;
            _target.Username = tbUsername.Text;
            _target.Password = tbpassword.Text;
            _target.AllowOverwrite = cbTargetAllowOverwrite.Checked;
        }

        private void btnBrowseExportPath_Click(object sender, EventArgs e) {
            FolderBrowserDialog fb = new FolderBrowserDialog();
            DialogResult result = fb.ShowDialog();
            if (result == DialogResult.OK) {
                tbTargetUrl.Text = fb.SelectedPath;
            }
        }
    }
}
