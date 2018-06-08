using System;
using System.Windows.Forms;
using FiscalLibraries;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                FiscalLibraries.FiscalLibraryClass d = new FiscalLibraryClass();
                d.PrintReceipt();
            }
            catch(Exception Ex)
            {
                MessageBox.Show("in Exception " + Ex.Message);
            }
            //d.PrintZReport();
        }
    }
}
