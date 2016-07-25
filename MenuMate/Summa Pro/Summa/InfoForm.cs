using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Summa
{
    public partial class InfoForm : Form
    {
        public InfoForm()
        {
            InitializeComponent();
        }

        public int SelectedTabIndex
        {
            get
            {
                return tabControl1.SelectedIndex;
            }
            set
            {
                tabControl1.SelectedIndex = value;
            }
        }

        //.................................

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
