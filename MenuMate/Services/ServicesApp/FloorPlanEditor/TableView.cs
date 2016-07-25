using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Forms;
using System.Drawing;

namespace FloorPlanIntegration
{
    public class TableView : Control
    {
        private readonly Timer refresher;

        protected Boolean _selected;
        protected string _caption;
        protected Pen _pen;
        protected Brush _brush;
        protected Brush _captionBrush;
        protected Pen _selPen;
        protected Brush _selBrush;
        protected Pen _resizeAimPen;

        protected Rectangle _resizeAimRectangle;

        protected int _innerX;
        protected int _innerY;
        protected int _innerWidth;
        protected int _innerHeight;
        protected int _paddingSpace;

        //public bool enableAntiFlicker;

        public TableView(): base()
        {
            //enableAntiFlicker = false;

            SetStyle(ControlStyles.SupportsTransparentBackColor, true);

            BackColor = Color.Transparent;
            refresher = new Timer();
            refresher.Tick += TimerOnTick;
            refresher.Interval = 50;
            refresher.Enabled = true;
            refresher.Start();

            _selected = false;
            _caption = @"";

            _pen = CreatePen();
            _brush = CreateBrush();
            _captionBrush = CreateCaptionBrush();
            _selPen = CreateSelPen();
            _selBrush = CreateSelBrush();
            _resizeAimPen = CreateResizeAimPen();

            SetPaddingAndInner();

            //BackColor = ColorAlphaPercentage(55, Color.SteelBlue);
        }

        protected override CreateParams CreateParams
        {
            get
            {
                /*
                                CreateParams cp = base.CreateParams;

                                if (enableAntiFlicker)
                                {
                                    cp.ExStyle |= 0x02000000; //WS_EX_COMPOSITED
                                }
                                else
                                {
                                    cp.ExStyle |= 0x20;
                                }

                                return cp;
                */

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
            /*
                        // to avoid a design-time error we need to add the following line
                        if (e.ClipRectangle.Width == 0)
                        {
                            return;
                        }

                        base.OnPaint(e);
                        if (position > this.Width)
                        {
                            // Reset the text to scroll back onto the control.
                            position = -(int)e.Graphics.MeasureString(text, this.Font).Width;
                        }

                        // Create the drawing area in memory.
                        // Double buffering is used to prevent flicker.
                        Bitmap bufl = new Bitmap(e.ClipRectangle.Width, e.ClipRectangle.Height);
                        Graphics g = Graphics.FromImage(bufl);
                        g.FillRectangle(new SolidBrush(this.BackColor), e.ClipRectangle);
                        g.DrawString(text, this.Font, new SolidBrush(this.ForeColor), position, 0);
                        // Render the finished image on the form.  
                        e.Graphics.DrawImageUnscaled(bufl, 0, 0);
                        g.Dispose();
            */

            //:::::::::::::::::::::::::::

            /*
                        // to avoid a design-time error we need to add the following line
                        if (e.ClipRectangle.Width == 0)
                        {
                            return;
                        }

                        base.OnPaint(e);

                        // Create the drawing area in memory.
                        // Double buffering is used to prevent flicker.
                        Bitmap bufl = new Bitmap(e.ClipRectangle.Width, e.ClipRectangle.Height);
                        Graphics g = Graphics.FromImage(bufl);

                        SetPaddingAndInner();

                        DrawTable(g);
                        FillTable(g);

                        DrawTableCaption(e.Graphics);

                        e.Graphics.DrawImageUnscaled(bufl, 0, 0);
                        g.Dispose();

                        //:::::::::::::::::::::::::::
            */

            SetPaddingAndInner();

            DrawTable(e.Graphics);
            FillTable(e.Graphics);

            //if (Selected)
            //{
            //    DrawSelection(e.Graphics);
            //    FillSelection(e.Graphics);
            //    DrawResizeAim(e.Graphics);
            //}

            DrawTableCaption(e.Graphics);
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            //Do not paint background
            //            Rectangle rc = new Rectangle(this.Location, this.Size);
            //            this.Parent.Invalidate(rc, false);
        }

        //Hack
        public void Redraw()
        {
            RecreateHandle();
        }

        private void TimerOnTick(object source, EventArgs e)
        {
            //RecreateHandle();
            refresher.Stop();
        }

        public Boolean Selected
        {
            get
            {
                return _selected;
            }
            set
            {
                _selected = value;
                RecreateHandle();
            }
        }

        public string Caption
        {
            get
            {
                return _caption;
            }
            set
            {
                _caption = value;
                RecreateHandle();
            }
        }

        #region My protected methods

        protected virtual void SetPaddingAndInner()
        {
            _paddingSpace = 2;
            _innerHeight = Height - (2 * _paddingSpace);
            _innerWidth = Width - (2 * _paddingSpace);

            _innerX = (Width / 2) - (Convert.ToInt16(_innerWidth) / 2);
            _innerY = (Height / 2) - (Convert.ToInt16(_innerHeight) / 2); ;

        }

        public static Color ColorAlphaPercentage(float inPercentage, Color inColor)
        {
            float alpha = (255 * inPercentage) / 100;
            int intAlpha = Convert.ToInt16(alpha);

            return Color.FromArgb(intAlpha, inColor);
        }

        protected virtual Pen CreatePen()
        {
            //Color penColor = Color.FromArgb( 200, Color.Aquamarine);
            Color penColor = ColorAlphaPercentage(95, Color.SteelBlue); //Color.LightSteelBlue
            Pen result = new Pen(penColor, 1);

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected virtual Brush CreateBrush()
        {
            Brush result = new SolidBrush(ColorAlphaPercentage(55, Color.SteelBlue));

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected virtual Brush CreateCaptionBrush()
        {
            Brush result = new SolidBrush(ColorAlphaPercentage(100, Color.White));

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected virtual Pen CreateSelPen()
        {
            //Color penColor = Color.FromArgb( 200, Color.Aquamarine);
            Color penColor = ColorAlphaPercentage(95, Color.Silver);
            Pen result = new Pen(penColor, 2);

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected virtual Brush CreateSelBrush()
        {
            Brush result = new SolidBrush(ColorAlphaPercentage(75, Color.Silver));

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected virtual Pen CreateResizeAimPen()
        {
            Color penColor = ColorAlphaPercentage(95, Color.Black);
            Pen result = new Pen(penColor, 1);

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected virtual Rectangle CurrentRectangle()  // NOT USED!!
        {
            return new Rectangle(this.Location, this.Size);
            //return Rectangle.Round(new Rectangle(this.Location, this.Size));
        }

        protected virtual void DrawTable(Graphics inGraphics)
        {
            inGraphics.DrawRectangle(_pen, _innerX, _innerY, _innerWidth, _innerHeight);

            //inGraphics.DrawRectangle(_pen, CurrentRectangle());
        }

        protected virtual void FillTable(Graphics inGraphics)
        {
            inGraphics.FillRectangle(_brush, _innerX, _innerY, _innerWidth, _innerHeight);

            //inGraphics.FillRectangle(_brush, CurrentRectangle());
        }

        protected virtual void DrawTableCaption(Graphics inGraphics)
        {
            Font txtFont = new Font("Arial", 10, FontStyle.Bold);
            SizeF txtSize = inGraphics.MeasureString(_caption, txtFont);

            inGraphics.DrawString(_caption, txtFont, _captionBrush,
                new Point((Width / 2) - (Convert.ToInt16(txtSize.Width) / 2), (Height / 2) - (Convert.ToInt16(txtSize.Height) / 2)));
        }

        protected void DrawSelection(Graphics inGraphics)
        {
            inGraphics.DrawRectangle(_selPen, 0, 0, Width, Height);
        }

        protected virtual void FillSelection(Graphics inGraphics)
        {
            inGraphics.FillRectangle(_selBrush, 0, 0, Width, Height);
        }

        protected virtual void DrawResizeAim(Graphics inGraphics)
        {
            Point point1 = new Point(_innerWidth - (2 * _paddingSpace), _innerHeight);
            Point point2 = new Point(_innerWidth + (2 * _paddingSpace), _innerHeight);

            inGraphics.DrawLine(_resizeAimPen, point1, point2);

            //...................................

            Point point3 = new Point(_innerWidth, _innerHeight - (2 * _paddingSpace));
            Point point4 = new Point(_innerWidth, _innerHeight + (2 * _paddingSpace));

            inGraphics.DrawLine(_resizeAimPen, point3, point4);

            //...................................

            inGraphics.DrawEllipse(_resizeAimPen, _innerWidth - _paddingSpace, _innerHeight - _paddingSpace, (2 * _paddingSpace), (2 * _paddingSpace));

            //......................................

            Point point = new Point(_innerWidth - (2 * _paddingSpace), _innerHeight - (2 * _paddingSpace));
            Size size = new Size(4 * _paddingSpace, 4 * _paddingSpace);

            _resizeAimRectangle = new Rectangle(point, size);
        }

        #endregion

        #region Public

        public virtual Boolean PointInResizeAim(Point inPoint)
        {
            return _resizeAimRectangle.Contains(inPoint);
        }

        #endregion

    }

    public class RoundTableView : TableView
    {
        public RoundTableView(): base()
        {
        }

        #region My protected methods

        protected override Pen CreatePen()
        {
            Color penColor = ColorAlphaPercentage(95, Color.Lime); //Color.PeachPuff
            Pen result = new Pen(penColor, 2);

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected override Brush CreateBrush()
        {
            Brush result = new SolidBrush(ColorAlphaPercentage(55, Color.Lime));

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected override Brush CreateCaptionBrush()
        {
            Brush result = new SolidBrush(ColorAlphaPercentage(100, Color.White));

            //::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::

            return result;
        }

        protected override Rectangle CurrentRectangle() // NOT USED!!
        {
            return new Rectangle(this.Location, this.Size);
        }

        protected override void DrawTable(Graphics inGraphics)
        {
            inGraphics.DrawEllipse(_pen, _innerX, _innerY, _innerWidth, _innerHeight);

            //inGraphics.DrawEllipse(_pen, CurrentRectangle());
        }

        protected override void FillTable(Graphics inGraphics)
        {
            inGraphics.FillEllipse(_brush, _innerX, _innerY, _innerWidth, _innerHeight);

            //inGraphics.FillEllipse(_brush, CurrentRectangle());
        }

        #endregion

    }
}
