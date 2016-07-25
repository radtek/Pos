using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using System.Runtime.InteropServices;

namespace Menumate.WinServices.Client.FloorPlan
{
    public partial class MainForm : Form
    {
        FPEditorForm _fpEditorForm;
        BtnForm _floatingBtnForm;

        EditorForm _editorForm;
        //PanelForm _panelForm;

        int _lastLeft;
        int _lastTop;


        [DllImport("user32")]
        public static extern int SetParent(int hWndChild, int hWndNewParent);

        public MainForm()
        {
            InitializeComponent();
        }

/*
        public FPEditorForm fpEditorForm
        {
            get
            {
                return _fpEditorForm;
            }
        }
*/

        public EditorForm fpEditorForm
        {
            get
            {
                return _editorForm;
            }
        }

/*
        protected override CreateParams CreateParams
        {
            get
            {
                var parms = base.CreateParams;
                parms.Style &= ~0x02000000;  // Turn off WS_CLIPCHILDREN
                return parms;
            }
        }
*/

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x02000000;  // Turn on WS_EX_COMPOSITED
                return cp;
            }
        } 

        #region FPForm

        private void initFPForm()
        {
            //_fpEditorForm = createFPForm();
            _editorForm = createEditorForm();

            //_panelForm = createPanelForm();

            showFPForm();
        }

        private FPEditorForm createFPForm()
        {
            FPEditorForm result = new FPEditorForm();

            //........................

             result.MdiParent = this;

            //........................

            return result;
        }

        private EditorForm createEditorForm()
        {
            EditorForm result = new EditorForm();

            //........................

             result.MdiParent = this;

            //........................

            return result;
        }

        private PanelForm createPanelForm()
        {
            PanelForm result = new PanelForm();

            //........................

            result.MdiParent = this;

            //........................

            return result;
        }

        private void showFPForm()
        {
            //_fpEditorForm.Show();
            //_fpEditorForm.BringToFront();

            _editorForm.Show();
            _editorForm.BringToFront();

            //_panelForm.Show();
            //_panelForm.BringToFront();
        }

        private void refreshFPForm()
        {
            //_fpEditorForm.Width = this.ClientSize.Width - 5;
            //_fpEditorForm.Height = this.ClientSize.Height - 5;

            if (_editorForm != null)
            {
                _editorForm.Width = this.ClientSize.Width - 5;
                _editorForm.Height = this.ClientSize.Height - 5;
            }

            //if (_panelForm != null)
            //{
            //    _panelForm.Width = this.ClientSize.Width - 5;
            //    _panelForm.Height = this.ClientSize.Height - 5;
            //}   
        }

        #endregion

        #region Floating BtnForm

        private BtnForm createFloatingBtnForm()
        {
            BtnForm result = new BtnForm();

            //........................

            result.MdiParent = this;

            //........................

            return result;
        }

        public void showFloatingBtnForm(string inName, int inNumber, int inWidth, int inHeight)
        {
            if(_floatingBtnForm == null)
            {
                _floatingBtnForm = createFloatingBtnForm();

                SetParent((int)_floatingBtnForm.Handle, (int)this.Handle); // To make sure floatingBtnForm stays always on top of all the MDI child windows

                _floatingBtnForm.Left = _lastLeft;
                _floatingBtnForm.Top  = _lastTop;

                _floatingBtnForm.Show();
            }
            else
            {
                if (!_floatingBtnForm.Visible)
                {
                    try
                    {
                        SetParent((int)_floatingBtnForm.Handle, (int)this.Handle); // To make sure floatingBtnForm stays always on top of all the MDI child windows

                        _floatingBtnForm.Left = _lastLeft;
                        _floatingBtnForm.Top = _lastTop;

                        _floatingBtnForm.Show();
                    }
                    catch(ObjectDisposedException)
                    {
                        _floatingBtnForm = createFloatingBtnForm();

                        SetParent((int)_floatingBtnForm.Handle, (int)this.Handle); // To make sure floatingBtnForm stays always on top of all the MDI child windows

                        _floatingBtnForm.Left = _lastLeft;
                        _floatingBtnForm.Top = _lastTop;

                        _floatingBtnForm.Show();
                    }

                 }
            }

            _floatingBtnForm.initTableName(inName);
            _floatingBtnForm.initTableNumber(inNumber);
            _floatingBtnForm.initTableWidth(inWidth);
            _floatingBtnForm.initTableHeight(inHeight);
        }

        public void showFloatingBtnForm(int inLeft, int inTop)
        {
            //showFloatingBtnForm();

            //_floatingBtnForm.Left = inLeft;
            //_floatingBtnForm.Top = inTop;
        }

        public void hideFloatingBtnForm()
        {
            if (_floatingBtnForm != null)
            {
                _lastLeft = _floatingBtnForm.Left;
                _lastTop  = _floatingBtnForm.Top;

                _floatingBtnForm.Visible = false;
            }
        }

        #endregion

        private void MainForm_Shown(object sender, EventArgs e)
        {        
            initFPForm();

            _lastLeft = 10;
            _lastTop = 380;

            this.IsMdiContainer = true;

            //::::::::::::::::::::::::::::::::::

            refreshFPForm();
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            refreshFPForm();
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public int floatingBtnFormWidth
        {
            get
            {
                return _floatingBtnForm.Width;
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public int floatingBtnFormHeight
        {
            get
            {
                return _floatingBtnForm.Height;
            }
        }

    }
}
