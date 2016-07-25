using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;

namespace Menumate.WinServices.Client.FloorPlan
{
    #region Windows Message IDs

    // WM_USER = 0x8000 + 1
    // FPM_TABLEVIEW_CHANGES = WM_USER + 0x0100     --- NOT USED!!!!!!!!!! ---
    public enum FPWindowsMessages
    {
        FPM_START_TABLEVIEW_CHANGES = 0x8000 + 1 + 0x01,
        FPM_FINISH_TABLEVIEW_CHANGES,
        FPM_INCWIDTH_TABLEVIEW,
        FPM_DECWIDTH_TABLEVIEW,
        FPM_INCHEIGHT_TABLEVIEW,
        FPM_DECHEIGHT_TABLEVIEW,
        FPM_MOVELEFT_TABLEVIEW,
        FPM_MOVERIGHT_TABLEVIEW,
        FPM_MOVEUP_TABLEVIEW,
        FPM_MOVEDOWN_TABLEVIEW
    };

        #endregion

    public partial class BtnForm : Form
    {
        delegate void TableChangeFuncDelegate(int a);

        Timer counterTimer;
        int counter = 0;
        int counterDelay = 1; // 1 mSec
        bool startedCounting = false;
        TableChangeFuncDelegate tableChangeFunc;

        //protected DateTime _startTickCount;
        //protected DateTime _tickCount;


        // NOT USED!!!!!
        #region DLL Export

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr SendMessage(IntPtr hwnd, uint Msg, IntPtr wParam, IntPtr lParam);

        #endregion

        public BtnForm()
        {
            InitializeComponent();

            counterTimer = Helper.createTimer(100); // 1 mSec
            counterTimer.Tick += new EventHandler(counterTimer_Tick);
        }

/*
        private FPEditorForm fpEditorForm
        {
            get
            {
                return ((MainForm)this.MdiParent).fpEditorForm;
            }
        }
*/

        private EditorForm fpEditorForm
        {
            get
            {
                return ((MainForm)this.MdiParent).fpEditorForm;
            }
        }

        // NOT USED!!!!!
        #region Windows Messages

        protected void sendFPWMessage(FPWindowsMessages inWMsg, Int32 inWParam, Int32 inLParam) // NOT USED!!!!!
        {
            SendMessage(fpEditorForm.Handle, (uint)inWMsg, new IntPtr(inWParam), new IntPtr(inLParam));
        }

        protected override void WndProc(ref Message m) // NOT USED!!!!!
        {
            int _iWParam = (int)m.WParam;
            int _iLParam = (int)m.LParam;

            switch (_iWParam)
            {
                // handling code goes here
            }
            base.WndProc(ref m);
        }

        #endregion

        void counterTimer_Tick(object sender, EventArgs e)
        {
            counter++;

            if (startedCounting)
            {
                if (counter >= counterDelay)
                {
                    tableChangeFunc(0);

                    counter = 0;
                }
            }
        }

        private void startCounting(TableChangeFuncDelegate inTableChangeFunc)
        {
            startedCounting = true;
            counter = 0;
            counterTimer.Start();

            fpEditorForm.startTableViewChanges();

            tableChangeFunc = new TableChangeFuncDelegate(inTableChangeFunc);
        }

        private void resetCounter()
        {
            //tableChangeFunc(0);

            startedCounting = false;
            counter = 0;
            counterTimer.Stop();

            fpEditorForm.finishTableViewChanges();
        }

        private void toolStripButtonIncWidth_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.incTableWidth);
            }
        }

        private void toolStripButtonIncWidth_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonIncWidth_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonDecWidth_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.decTableWidth);
            }
        }

        private void toolStripButtonDecWidth_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonDecWidth_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonIncHeight_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.incTableHeigth);
            }
        }

        private void toolStripButtonIncHeight_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonIncHeight_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonDecHeight_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.decTableHeigth);
            }
        }

        private void toolStripButtonDecHeight_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonDecHeight_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveLeft_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.moveTableLeft);
            }
        }

        private void toolStripButtonMoveLeft_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveLeft_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveRight_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.moveTableRight);
            }
        }

        private void toolStripButtonMoveRight_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveRight_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveUp_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.moveTableUp);
            }
        }

        private void toolStripButtonMoveUp_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveUp_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveDown_MouseDown(object sender, MouseEventArgs e)
        {
            if (!startedCounting)
            {
                startCounting(fpEditorForm.moveTableDown);
            }
        }

        private void toolStripButtonMoveDown_MouseUp(object sender, MouseEventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripButtonMoveDown_MouseLeave(object sender, EventArgs e)
        {
            if (startedCounting)
            {
                resetCounter();
            }
        }

        private void toolStripTextBoxTableNumber_KeyDown(object sender, KeyEventArgs e)
        {
            e.SuppressKeyPress = !Helper.keyIsANumber(e.KeyCode) &&
                                 !Helper.keyIsBackSpace(e.KeyCode) &&
                                 !Helper.keyIsDelete(e.KeyCode);
        }

        private void toolStripTextBoxTableName_TextChanged(object sender, EventArgs e)
        {
            fpEditorForm.setTableName(((ToolStripTextBox)sender).Text);
        }

        private void toolStripTextBoxTableNumber_TextChanged(object sender, EventArgs e)
        {
            try
            {
                fpEditorForm.setTableNumber(Convert.ToInt16(((ToolStripTextBox)sender).Text));
            }
            catch
            {
                //fpEditorForm.setTableNumber(0);
            }
        }

        private void toolStripTextBoxHeight_TextChanged(object sender, EventArgs e)
        {
            try
            {
                fpEditorForm.setTableHeight(Convert.ToInt16(((ToolStripTextBox)sender).Text));
            }
            catch
            {
               //fpEditorForm.setTableHeight(0);
            }
        }

        private void toolStripTextBoxWidth_TextChanged(object sender, EventArgs e)
        {
            try
            {
                fpEditorForm.setTableWidth(Convert.ToInt16(((ToolStripTextBox)sender).Text));
            }
            catch
            {
                //fpEditorForm.setTableWidth(0);
            }
        }

        #region Public

        public void initTableName(string inName)
        {
            toolStripTextBoxTableName.Text = inName;
        }

        public void initTableNumber(int inNumber)
        {
            toolStripTextBoxTableNumber.Text = string.Format(@"{0}", inNumber);
        }

        public void initTableWidth(int inWidth)
        {
             toolStripTextBoxWidth.Text = string.Format(@"{0}", inWidth);
        }

        public void initTableHeight(int inHeight)
        {
            toolStripTextBoxHeight.Text = string.Format(@"{0}", inHeight);
        }

        #endregion

        private void toolStrip2_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }
    }
}
