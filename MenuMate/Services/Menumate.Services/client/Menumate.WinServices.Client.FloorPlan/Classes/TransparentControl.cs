using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Menumate.WinServices.Client.FloorPlan
{
    public class TransparentPanel : Panel
    {
        /// <summary>
        /// Panel that allows its controls to be transparent
        /// </summary>
        /// 
        public TransparentPanel()
        {
            //
            // TODO: Add constructor logic here
            //

            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
        }
    }

    /// <summary>
    /// Control with a transparent background
    /// </summary>
    /// 
    public class TransparentControl : Control
    {
        private readonly Timer refresher;
        private Image _image;

        public TransparentControl()
        {
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            BackColor = Color.Transparent;
            refresher = new Timer();
            refresher.Tick += TimerOnTick;
            refresher.Interval = 50;
            refresher.Enabled = true;
            refresher.Start();
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x20;
                return cp;
            }
        }

        protected override void OnMove(EventArgs e)
        {
            RecreateHandle();
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            if (_image != null)
            {
                e.Graphics.DrawImage(_image, (Width / 2) - (_image.Width / 2), (Height / 2) - (_image.Height / 2));
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            //Do not paint background
        }

        //Hack
        public void Redraw()
        {
            RecreateHandle();
        }

        private void TimerOnTick(object source, EventArgs e)
        {
            RecreateHandle();
            refresher.Stop();
        }

        public Image Image
        {
            get
            {
                return _image;
            }
            set
            {
                _image = value;
                RecreateHandle();
            }
        }
    }
}
