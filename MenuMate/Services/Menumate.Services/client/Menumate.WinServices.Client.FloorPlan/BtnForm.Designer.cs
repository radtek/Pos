namespace Menumate.WinServices.Client.FloorPlan
{
    partial class BtnForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BtnForm));
            this.toolStrip4 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabelTableName = new System.Windows.Forms.ToolStripLabel();
            this.toolStripTextBoxTableName = new System.Windows.Forms.ToolStripTextBox();
            this.toolStrip5 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabelTableNumber = new System.Windows.Forms.ToolStripLabel();
            this.toolStripTextBoxTableNumber = new System.Windows.Forms.ToolStripTextBox();
            this.toolStrip2 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripTextBoxHeight = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripButtonIncHeight = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonDecHeight = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabelWidth = new System.Windows.Forms.ToolStripLabel();
            this.toolStripTextBoxWidth = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripButtonIncWidth = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonDecWidth = new System.Windows.Forms.ToolStripButton();
            this.toolStrip3 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripButtonMoveLeft = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonMoveRight = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStrip4.SuspendLayout();
            this.toolStrip5.SuspendLayout();
            this.toolStrip2.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.toolStrip3.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip4
            // 
            this.toolStrip4.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip4.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabelTableName,
            this.toolStripTextBoxTableName});
            this.toolStrip4.Location = new System.Drawing.Point(0, 0);
            this.toolStrip4.Name = "toolStrip4";
            this.toolStrip4.Size = new System.Drawing.Size(299, 25);
            this.toolStrip4.TabIndex = 1;
            // 
            // toolStripLabelTableName
            // 
            this.toolStripLabelTableName.AutoSize = false;
            this.toolStripLabelTableName.ForeColor = System.Drawing.Color.SteelBlue;
            this.toolStripLabelTableName.Name = "toolStripLabelTableName";
            this.toolStripLabelTableName.Size = new System.Drawing.Size(83, 22);
            this.toolStripLabelTableName.Text = "Table Name";
            // 
            // toolStripTextBoxTableName
            // 
            this.toolStripTextBoxTableName.Name = "toolStripTextBoxTableName";
            this.toolStripTextBoxTableName.Size = new System.Drawing.Size(100, 25);
            this.toolStripTextBoxTableName.TextChanged += new System.EventHandler(this.toolStripTextBoxTableName_TextChanged);
            // 
            // toolStrip5
            // 
            this.toolStrip5.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip5.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabelTableNumber,
            this.toolStripTextBoxTableNumber});
            this.toolStrip5.Location = new System.Drawing.Point(0, 25);
            this.toolStrip5.Name = "toolStrip5";
            this.toolStrip5.Size = new System.Drawing.Size(299, 25);
            this.toolStrip5.TabIndex = 2;
            // 
            // toolStripLabelTableNumber
            // 
            this.toolStripLabelTableNumber.ForeColor = System.Drawing.Color.SteelBlue;
            this.toolStripLabelTableNumber.Name = "toolStripLabelTableNumber";
            this.toolStripLabelTableNumber.Size = new System.Drawing.Size(83, 22);
            this.toolStripLabelTableNumber.Text = "Table Number";
            // 
            // toolStripTextBoxTableNumber
            // 
            this.toolStripTextBoxTableNumber.MaxLength = 3;
            this.toolStripTextBoxTableNumber.Name = "toolStripTextBoxTableNumber";
            this.toolStripTextBoxTableNumber.Size = new System.Drawing.Size(100, 25);
            this.toolStripTextBoxTableNumber.KeyDown += new System.Windows.Forms.KeyEventHandler(this.toolStripTextBoxTableNumber_KeyDown);
            this.toolStripTextBoxTableNumber.TextChanged += new System.EventHandler(this.toolStripTextBoxTableNumber_TextChanged);
            // 
            // toolStrip2
            // 
            this.toolStrip2.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.toolStripTextBoxHeight,
            this.toolStripButtonIncHeight,
            this.toolStripButtonDecHeight});
            this.toolStrip2.Location = new System.Drawing.Point(0, 50);
            this.toolStrip2.Name = "toolStrip2";
            this.toolStrip2.Size = new System.Drawing.Size(299, 55);
            this.toolStrip2.TabIndex = 15;
            this.toolStrip2.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStrip2_ItemClicked);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.AutoSize = false;
            this.toolStripLabel1.ForeColor = System.Drawing.Color.SteelBlue;
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(83, 22);
            this.toolStripLabel1.Text = "Height";
            // 
            // toolStripTextBoxHeight
            // 
            this.toolStripTextBoxHeight.Name = "toolStripTextBoxHeight";
            this.toolStripTextBoxHeight.Size = new System.Drawing.Size(100, 55);
            this.toolStripTextBoxHeight.KeyDown += new System.Windows.Forms.KeyEventHandler(this.toolStripTextBoxTableNumber_KeyDown);
            this.toolStripTextBoxHeight.TextChanged += new System.EventHandler(this.toolStripTextBoxHeight_TextChanged);
            // 
            // toolStripButtonIncHeight
            // 
            this.toolStripButtonIncHeight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonIncHeight.DoubleClickEnabled = true;
            this.toolStripButtonIncHeight.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonIncHeight.Image")));
            this.toolStripButtonIncHeight.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonIncHeight.Name = "toolStripButtonIncHeight";
            this.toolStripButtonIncHeight.Size = new System.Drawing.Size(52, 52);
            this.toolStripButtonIncHeight.Text = "toolStripButton1";
            this.toolStripButtonIncHeight.ToolTipText = "Increase Height";
            this.toolStripButtonIncHeight.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonIncHeight_MouseDown);
            this.toolStripButtonIncHeight.MouseLeave += new System.EventHandler(this.toolStripButtonIncHeight_MouseLeave);
            this.toolStripButtonIncHeight.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonIncHeight_MouseUp);
            // 
            // toolStripButtonDecHeight
            // 
            this.toolStripButtonDecHeight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonDecHeight.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonDecHeight.Image")));
            this.toolStripButtonDecHeight.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonDecHeight.Name = "toolStripButtonDecHeight";
            this.toolStripButtonDecHeight.Size = new System.Drawing.Size(52, 52);
            this.toolStripButtonDecHeight.Text = "toolStripButton2";
            this.toolStripButtonDecHeight.ToolTipText = "Decrease Height";
            this.toolStripButtonDecHeight.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonDecHeight_MouseDown);
            this.toolStripButtonDecHeight.MouseLeave += new System.EventHandler(this.toolStripButtonDecHeight_MouseLeave);
            this.toolStripButtonDecHeight.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonDecHeight_MouseUp);
            // 
            // toolStrip1
            // 
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabelWidth,
            this.toolStripTextBoxWidth,
            this.toolStripButtonIncWidth,
            this.toolStripButtonDecWidth});
            this.toolStrip1.Location = new System.Drawing.Point(0, 105);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(299, 55);
            this.toolStrip1.TabIndex = 16;
            // 
            // toolStripLabelWidth
            // 
            this.toolStripLabelWidth.AutoSize = false;
            this.toolStripLabelWidth.ForeColor = System.Drawing.Color.SteelBlue;
            this.toolStripLabelWidth.Name = "toolStripLabelWidth";
            this.toolStripLabelWidth.Size = new System.Drawing.Size(83, 22);
            this.toolStripLabelWidth.Text = "Width";
            // 
            // toolStripTextBoxWidth
            // 
            this.toolStripTextBoxWidth.Name = "toolStripTextBoxWidth";
            this.toolStripTextBoxWidth.Size = new System.Drawing.Size(100, 55);
            this.toolStripTextBoxWidth.KeyDown += new System.Windows.Forms.KeyEventHandler(this.toolStripTextBoxTableNumber_KeyDown);
            this.toolStripTextBoxWidth.TextChanged += new System.EventHandler(this.toolStripTextBoxWidth_TextChanged);
            // 
            // toolStripButtonIncWidth
            // 
            this.toolStripButtonIncWidth.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonIncWidth.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonIncWidth.Image")));
            this.toolStripButtonIncWidth.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonIncWidth.Name = "toolStripButtonIncWidth";
            this.toolStripButtonIncWidth.Size = new System.Drawing.Size(52, 52);
            this.toolStripButtonIncWidth.Text = "toolStripButton1";
            this.toolStripButtonIncWidth.ToolTipText = "Increase Width";
            this.toolStripButtonIncWidth.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonIncWidth_MouseDown);
            this.toolStripButtonIncWidth.MouseLeave += new System.EventHandler(this.toolStripButtonIncHeight_MouseLeave);
            this.toolStripButtonIncWidth.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonIncWidth_MouseUp);
            // 
            // toolStripButtonDecWidth
            // 
            this.toolStripButtonDecWidth.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonDecWidth.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonDecWidth.Image")));
            this.toolStripButtonDecWidth.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonDecWidth.Name = "toolStripButtonDecWidth";
            this.toolStripButtonDecWidth.Size = new System.Drawing.Size(52, 52);
            this.toolStripButtonDecWidth.Text = "toolStripButton2";
            this.toolStripButtonDecWidth.ToolTipText = "Decrease Width";
            this.toolStripButtonDecWidth.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonDecWidth_MouseDown);
            this.toolStripButtonDecWidth.MouseLeave += new System.EventHandler(this.toolStripButtonDecWidth_MouseLeave);
            this.toolStripButtonDecWidth.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonDecWidth_MouseUp);
            // 
            // toolStrip3
            // 
            this.toolStrip3.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip3.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel4,
            this.toolStripButtonMoveLeft,
            this.toolStripButtonMoveRight,
            this.toolStripButton1,
            this.toolStripButton2});
            this.toolStrip3.Location = new System.Drawing.Point(0, 160);
            this.toolStrip3.Name = "toolStrip3";
            this.toolStrip3.Size = new System.Drawing.Size(299, 55);
            this.toolStrip3.TabIndex = 17;
            // 
            // toolStripLabel4
            // 
            this.toolStripLabel4.ForeColor = System.Drawing.Color.SteelBlue;
            this.toolStripLabel4.Name = "toolStripLabel4";
            this.toolStripLabel4.Size = new System.Drawing.Size(69, 52);
            this.toolStripLabel4.Text = "Move Table";
            // 
            // toolStripButtonMoveLeft
            // 
            this.toolStripButtonMoveLeft.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMoveLeft.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonMoveLeft.Image")));
            this.toolStripButtonMoveLeft.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMoveLeft.Name = "toolStripButtonMoveLeft";
            this.toolStripButtonMoveLeft.Size = new System.Drawing.Size(52, 52);
            this.toolStripButtonMoveLeft.Text = "toolStripButton1";
            this.toolStripButtonMoveLeft.ToolTipText = "Move to the Left";
            this.toolStripButtonMoveLeft.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveLeft_MouseDown);
            this.toolStripButtonMoveLeft.MouseLeave += new System.EventHandler(this.toolStripButtonMoveLeft_MouseLeave);
            this.toolStripButtonMoveLeft.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveLeft_MouseUp);
            // 
            // toolStripButtonMoveRight
            // 
            this.toolStripButtonMoveRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMoveRight.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonMoveRight.Image")));
            this.toolStripButtonMoveRight.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMoveRight.Name = "toolStripButtonMoveRight";
            this.toolStripButtonMoveRight.Size = new System.Drawing.Size(52, 52);
            this.toolStripButtonMoveRight.Text = "toolStripButton2";
            this.toolStripButtonMoveRight.ToolTipText = "Move to the Right";
            this.toolStripButtonMoveRight.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveRight_MouseDown);
            this.toolStripButtonMoveRight.MouseLeave += new System.EventHandler(this.toolStripButtonMoveRight_MouseLeave);
            this.toolStripButtonMoveRight.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveRight_MouseUp);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(52, 52);
            this.toolStripButton1.Text = "toolStripButton1";
            this.toolStripButton1.ToolTipText = "Move Up";
            this.toolStripButton1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveUp_MouseDown);
            this.toolStripButton1.MouseLeave += new System.EventHandler(this.toolStripButtonMoveUp_MouseLeave);
            this.toolStripButton1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveUp_MouseUp);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(52, 52);
            this.toolStripButton2.Text = "Move Down";
            this.toolStripButton2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveDown_MouseDown);
            this.toolStripButton2.MouseLeave += new System.EventHandler(this.toolStripButtonMoveDown_MouseLeave);
            this.toolStripButton2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolStripButtonMoveDown_MouseUp);
            // 
            // BtnForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(299, 211);
            this.Controls.Add(this.toolStrip3);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.toolStrip2);
            this.Controls.Add(this.toolStrip5);
            this.Controls.Add(this.toolStrip4);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.KeyPreview = true;
            this.Name = "BtnForm";
            this.Text = "Selected Table";
            this.TopMost = true;
            this.toolStrip4.ResumeLayout(false);
            this.toolStrip4.PerformLayout();
            this.toolStrip5.ResumeLayout(false);
            this.toolStrip5.PerformLayout();
            this.toolStrip2.ResumeLayout(false);
            this.toolStrip2.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.toolStrip3.ResumeLayout(false);
            this.toolStrip3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip4;
        private System.Windows.Forms.ToolStripLabel toolStripLabelTableName;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxTableName;
        private System.Windows.Forms.ToolStrip toolStrip5;
        private System.Windows.Forms.ToolStripLabel toolStripLabelTableNumber;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxTableNumber;
        private System.Windows.Forms.ToolStrip toolStrip2;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxHeight;
        private System.Windows.Forms.ToolStripButton toolStripButtonIncHeight;
        private System.Windows.Forms.ToolStripButton toolStripButtonDecHeight;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabelWidth;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxWidth;
        private System.Windows.Forms.ToolStripButton toolStripButtonIncWidth;
        private System.Windows.Forms.ToolStripButton toolStripButtonDecWidth;
        private System.Windows.Forms.ToolStrip toolStrip3;
        private System.Windows.Forms.ToolStripLabel toolStripLabel4;
        private System.Windows.Forms.ToolStripButton toolStripButtonMoveLeft;
        private System.Windows.Forms.ToolStripButton toolStripButtonMoveRight;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripButton toolStripButton2;

    }
}