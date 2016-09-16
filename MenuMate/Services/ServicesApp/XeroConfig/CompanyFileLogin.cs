using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AccountingConfig
{
    public partial class CompanyFileLogin : Form
    {
        public CompanyFileLogin()
        {
            InitializeComponent();
        }

        public string Username
        {
            get { return txtUsername.Text; }
            set { txtUsername.Text = Username; }
        }

        /// <summary>
        /// Public property to set and get Password
        /// </summary>
        /// <value></value>
        /// <returns></returns>
        /// <remarks></remarks>
        public string Password
        {
            get { return txtPassword.Text; }
            set { txtPassword.Text = Password; }
        }
        private void OKButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void OK_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void LoginFormLoad(object sender, System.EventArgs e)
        {
            txtUsername.Text = "Administrator";
        }
    }
}
