using System;
using System.Windows.Forms;

namespace FileMateSettingsGUI {
    public partial class ErrorHandler : Form {
        public ErrorHandler(string message) {
            InitializeComponent();

            tbMessage.Text = message;
        }

        private void btnClose_Click(object sender, EventArgs e) {
            this.Close();
        }
    }
}
