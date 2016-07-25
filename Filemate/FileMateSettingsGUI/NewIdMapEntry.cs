using System;
using System.Windows.Forms;

namespace FileMateSettingsGUI {
    public partial class NewIdMapEntry : Form {

        private string _siteId;

        public string SiteId {
            get { return _siteId; }
            set { _siteId = value; }
        }
        private string _summaId;

        public string SummaId {
            get { return _summaId; }
            set { _summaId = value; }
        }

        public NewIdMapEntry() {
            InitializeComponent();

            tbSiteId.Focus();
        }

        public NewIdMapEntry(string siteid, string summaid) {
            InitializeComponent();

            _siteId = siteid;
            _summaId = summaid;

            ShowIdMapValue();
        }

        private void ShowIdMapValue() {
            tbSiteId.Text = _siteId;
            tbSummaId.Text = _summaId;
        }

        private void button2_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e) {
            _siteId = tbSiteId.Text;
            _summaId = tbSummaId.Text;
            this.Close();
        }
    }
}
