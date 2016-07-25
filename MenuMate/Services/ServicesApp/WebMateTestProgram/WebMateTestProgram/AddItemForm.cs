using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WebMateTestProgram
{
    public partial class AddItemForm : Form
    {
        public bool Canceled { get; private set; }

        public AddItemForm()
        {
            InitializeComponent();
            Canceled = true;
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Canceled = true;
            Close();
        }

        private void DoneButton_Click(object sender, EventArgs e)
        {
            Canceled = false;
            Close();
        }
    }
}
