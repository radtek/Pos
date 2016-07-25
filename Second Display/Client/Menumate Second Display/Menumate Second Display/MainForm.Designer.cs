namespace Menumate_Second_Display
{
    partial class MainForm
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
            this.pnLeft = new System.Windows.Forms.Panel();
            this.pnTop = new System.Windows.Forms.Panel();
            this.pnRight = new System.Windows.Forms.Panel();
            this.pnBottom = new System.Windows.Forms.Panel();
            this.pnMiddle = new System.Windows.Forms.Panel();
            this.pnDisplay = new System.Windows.Forms.Panel();
            this.spDisplay = new System.Windows.Forms.SplitContainer();
            this.gbAnimateImage = new System.Windows.Forms.GroupBox();
            this.pnAnimateImage = new System.Windows.Forms.Panel();
            this.gbOrderItemTotals = new System.Windows.Forms.GroupBox();
            this.pnOrderTotals = new System.Windows.Forms.Panel();
            this.pnOrderTotalsContainer = new System.Windows.Forms.Panel();
            this.gbOrderItems = new System.Windows.Forms.GroupBox();
            this.pnOrderItems = new System.Windows.Forms.Panel();
            this.pnOrderItemsContainer = new System.Windows.Forms.Panel();
            this.gpBottom = new System.Windows.Forms.GroupBox();
            this.pnlLastItem = new System.Windows.Forms.Panel();
            this.pnlDisplayItemName = new System.Windows.Forms.Panel();
            this.lblItemPrice = new System.Windows.Forms.Label();
            this.lblItemName = new System.Windows.Forms.Label();
            this.pnlLogoParent = new System.Windows.Forms.Panel();
            this.pnLogo = new System.Windows.Forms.Panel();
            this.pnMiddle.SuspendLayout();
            this.pnDisplay.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spDisplay)).BeginInit();
            this.spDisplay.Panel1.SuspendLayout();
            this.spDisplay.Panel2.SuspendLayout();
            this.spDisplay.SuspendLayout();
            this.gbAnimateImage.SuspendLayout();
            this.gbOrderItemTotals.SuspendLayout();
            this.pnOrderTotals.SuspendLayout();
            this.gbOrderItems.SuspendLayout();
            this.pnOrderItems.SuspendLayout();
            this.gpBottom.SuspendLayout();
            this.pnlLastItem.SuspendLayout();
            this.pnlDisplayItemName.SuspendLayout();
            this.pnlLogoParent.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnLeft
            // 
            this.pnLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.pnLeft.Location = new System.Drawing.Point(0, 0);
            this.pnLeft.Margin = new System.Windows.Forms.Padding(4);
            this.pnLeft.Name = "pnLeft";
            this.pnLeft.Size = new System.Drawing.Size(5, 768);
            this.pnLeft.TabIndex = 0;
            // 
            // pnTop
            // 
            this.pnTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnTop.Location = new System.Drawing.Point(5, 0);
            this.pnTop.Name = "pnTop";
            this.pnTop.Size = new System.Drawing.Size(1019, 5);
            this.pnTop.TabIndex = 1;
            // 
            // pnRight
            // 
            this.pnRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.pnRight.Location = new System.Drawing.Point(1019, 5);
            this.pnRight.Name = "pnRight";
            this.pnRight.Size = new System.Drawing.Size(5, 763);
            this.pnRight.TabIndex = 2;
            // 
            // pnBottom
            // 
            this.pnBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnBottom.Location = new System.Drawing.Point(5, 763);
            this.pnBottom.Name = "pnBottom";
            this.pnBottom.Size = new System.Drawing.Size(1014, 5);
            this.pnBottom.TabIndex = 3;
            // 
            // pnMiddle
            // 
            this.pnMiddle.Controls.Add(this.pnDisplay);
            this.pnMiddle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnMiddle.Location = new System.Drawing.Point(5, 5);
            this.pnMiddle.Name = "pnMiddle";
            this.pnMiddle.Size = new System.Drawing.Size(1014, 758);
            this.pnMiddle.TabIndex = 4;
            // 
            // pnDisplay
            // 
            this.pnDisplay.Controls.Add(this.spDisplay);
            this.pnDisplay.Controls.Add(this.gpBottom);
            this.pnDisplay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnDisplay.Location = new System.Drawing.Point(0, 0);
            this.pnDisplay.Name = "pnDisplay";
            this.pnDisplay.Size = new System.Drawing.Size(1014, 758);
            this.pnDisplay.TabIndex = 0;
            // 
            // spDisplay
            // 
            this.spDisplay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.spDisplay.IsSplitterFixed = true;
            this.spDisplay.Location = new System.Drawing.Point(0, 0);
            this.spDisplay.Name = "spDisplay";
            // 
            // spDisplay.Panel1
            // 
            this.spDisplay.Panel1.Controls.Add(this.gbAnimateImage);
            // 
            // spDisplay.Panel2
            // 
            this.spDisplay.Panel2.Controls.Add(this.gbOrderItemTotals);
            this.spDisplay.Panel2.Controls.Add(this.gbOrderItems);
            this.spDisplay.Size = new System.Drawing.Size(1014, 658);
            this.spDisplay.SplitterDistance = 338;
            this.spDisplay.TabIndex = 6;
            // 
            // gbAnimateImage
            // 
            this.gbAnimateImage.Controls.Add(this.pnAnimateImage);
            this.gbAnimateImage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbAnimateImage.Location = new System.Drawing.Point(0, 0);
            this.gbAnimateImage.Name = "gbAnimateImage";
            this.gbAnimateImage.Size = new System.Drawing.Size(338, 658);
            this.gbAnimateImage.TabIndex = 3;
            this.gbAnimateImage.TabStop = false;
            // 
            // pnAnimateImage
            // 
            this.pnAnimateImage.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pnAnimateImage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnAnimateImage.Location = new System.Drawing.Point(3, 23);
            this.pnAnimateImage.Name = "pnAnimateImage";
            this.pnAnimateImage.Size = new System.Drawing.Size(332, 632);
            this.pnAnimateImage.TabIndex = 0;
            // 
            // gbOrderItemTotals
            // 
            this.gbOrderItemTotals.Controls.Add(this.pnOrderTotals);
            this.gbOrderItemTotals.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbOrderItemTotals.Location = new System.Drawing.Point(0, 533);
            this.gbOrderItemTotals.Name = "gbOrderItemTotals";
            this.gbOrderItemTotals.Size = new System.Drawing.Size(672, 125);
            this.gbOrderItemTotals.TabIndex = 8;
            this.gbOrderItemTotals.TabStop = false;
            // 
            // pnOrderTotals
            // 
            this.pnOrderTotals.Controls.Add(this.pnOrderTotalsContainer);
            this.pnOrderTotals.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnOrderTotals.Location = new System.Drawing.Point(3, 23);
            this.pnOrderTotals.Name = "pnOrderTotals";
            this.pnOrderTotals.Size = new System.Drawing.Size(666, 99);
            this.pnOrderTotals.TabIndex = 2;
            // 
            // pnOrderTotalsContainer
            // 
            this.pnOrderTotalsContainer.AutoScroll = true;
            this.pnOrderTotalsContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnOrderTotalsContainer.Location = new System.Drawing.Point(0, 0);
            this.pnOrderTotalsContainer.Name = "pnOrderTotalsContainer";
            this.pnOrderTotalsContainer.Size = new System.Drawing.Size(666, 99);
            this.pnOrderTotalsContainer.TabIndex = 0;
            // 
            // gbOrderItems
            // 
            this.gbOrderItems.Controls.Add(this.pnOrderItems);
            this.gbOrderItems.Dock = System.Windows.Forms.DockStyle.Top;
            this.gbOrderItems.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbOrderItems.Location = new System.Drawing.Point(0, 0);
            this.gbOrderItems.Name = "gbOrderItems";
            this.gbOrderItems.Size = new System.Drawing.Size(672, 533);
            this.gbOrderItems.TabIndex = 3;
            this.gbOrderItems.TabStop = false;
            // 
            // pnOrderItems
            // 
            this.pnOrderItems.Controls.Add(this.pnOrderItemsContainer);
            this.pnOrderItems.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnOrderItems.Location = new System.Drawing.Point(3, 23);
            this.pnOrderItems.Name = "pnOrderItems";
            this.pnOrderItems.Size = new System.Drawing.Size(666, 507);
            this.pnOrderItems.TabIndex = 2;
            // 
            // pnOrderItemsContainer
            // 
            this.pnOrderItemsContainer.AutoScroll = true;
            this.pnOrderItemsContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnOrderItemsContainer.Location = new System.Drawing.Point(0, 0);
            this.pnOrderItemsContainer.Name = "pnOrderItemsContainer";
            this.pnOrderItemsContainer.Size = new System.Drawing.Size(666, 507);
            this.pnOrderItemsContainer.TabIndex = 0;
            // 
            // gpBottom
            // 
            this.gpBottom.Controls.Add(this.pnlLastItem);
            this.gpBottom.Controls.Add(this.pnlLogoParent);
            this.gpBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.gpBottom.Location = new System.Drawing.Point(0, 658);
            this.gpBottom.Name = "gpBottom";
            this.gpBottom.Size = new System.Drawing.Size(1014, 100);
            this.gpBottom.TabIndex = 0;
            this.gpBottom.TabStop = false;
            // 
            // pnlLastItem
            // 
            this.pnlLastItem.Controls.Add(this.pnlDisplayItemName);
            this.pnlLastItem.Dock = System.Windows.Forms.DockStyle.Left;
            this.pnlLastItem.Location = new System.Drawing.Point(3, 23);
            this.pnlLastItem.Name = "pnlLastItem";
            this.pnlLastItem.Size = new System.Drawing.Size(788, 74);
            this.pnlLastItem.TabIndex = 5;
            // 
            // pnlDisplayItemName
            // 
            this.pnlDisplayItemName.Controls.Add(this.lblItemPrice);
            this.pnlDisplayItemName.Controls.Add(this.lblItemName);
            this.pnlDisplayItemName.Dock = System.Windows.Forms.DockStyle.Left;
            this.pnlDisplayItemName.Location = new System.Drawing.Point(0, 0);
            this.pnlDisplayItemName.Name = "pnlDisplayItemName";
            this.pnlDisplayItemName.Size = new System.Drawing.Size(788, 74);
            this.pnlDisplayItemName.TabIndex = 3;
            // 
            // lblItemPrice
            // 
            this.lblItemPrice.AutoSize = true;
            this.lblItemPrice.Dock = System.Windows.Forms.DockStyle.Right;
            this.lblItemPrice.Location = new System.Drawing.Point(788, 0);
            this.lblItemPrice.Name = "lblItemPrice";
            this.lblItemPrice.Size = new System.Drawing.Size(0, 19);
            this.lblItemPrice.TabIndex = 2;
            // 
            // lblItemName
            // 
            this.lblItemName.AutoSize = true;
            this.lblItemName.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblItemName.Location = new System.Drawing.Point(0, 0);
            this.lblItemName.Name = "lblItemName";
            this.lblItemName.Size = new System.Drawing.Size(0, 19);
            this.lblItemName.TabIndex = 1;
            // 
            // pnlLogoParent
            // 
            this.pnlLogoParent.Controls.Add(this.pnLogo);
            this.pnlLogoParent.Dock = System.Windows.Forms.DockStyle.Right;
            this.pnlLogoParent.Location = new System.Drawing.Point(797, 23);
            this.pnlLogoParent.Name = "pnlLogoParent";
            this.pnlLogoParent.Size = new System.Drawing.Size(214, 74);
            this.pnlLogoParent.TabIndex = 4;
            // 
            // pnLogo
            // 
            this.pnLogo.BackgroundImage = global::Menumate_Second_Display.Properties.Resources.menumate_logo_shifted;
            this.pnLogo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.pnLogo.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnLogo.Location = new System.Drawing.Point(0, 27);
            this.pnLogo.Name = "pnLogo";
            this.pnLogo.Size = new System.Drawing.Size(214, 47);
            this.pnLogo.TabIndex = 1;
            this.pnLogo.Click += new System.EventHandler(this.pnLogo_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 19F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(1024, 768);
            this.Controls.Add(this.pnMiddle);
            this.Controls.Add(this.pnBottom);
            this.Controls.Add(this.pnRight);
            this.Controls.Add(this.pnTop);
            this.Controls.Add(this.pnLeft);
            this.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Menumate Customer Display";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.pnMiddle.ResumeLayout(false);
            this.pnDisplay.ResumeLayout(false);
            this.spDisplay.Panel1.ResumeLayout(false);
            this.spDisplay.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.spDisplay)).EndInit();
            this.spDisplay.ResumeLayout(false);
            this.gbAnimateImage.ResumeLayout(false);
            this.gbOrderItemTotals.ResumeLayout(false);
            this.pnOrderTotals.ResumeLayout(false);
            this.gbOrderItems.ResumeLayout(false);
            this.pnOrderItems.ResumeLayout(false);
            this.gpBottom.ResumeLayout(false);
            this.pnlLastItem.ResumeLayout(false);
            this.pnlDisplayItemName.ResumeLayout(false);
            this.pnlDisplayItemName.PerformLayout();
            this.pnlLogoParent.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnLeft;
        private System.Windows.Forms.Panel pnTop;
        private System.Windows.Forms.Panel pnRight;
        private System.Windows.Forms.Panel pnBottom;
        private System.Windows.Forms.Panel pnMiddle;
        private System.Windows.Forms.Panel pnDisplay;
        private System.Windows.Forms.SplitContainer spDisplay;
        private System.Windows.Forms.GroupBox gbAnimateImage;
        private System.Windows.Forms.Panel pnAnimateImage;
        private System.Windows.Forms.GroupBox gbOrderItemTotals;
        private System.Windows.Forms.Panel pnOrderTotals;
        private System.Windows.Forms.Panel pnOrderTotalsContainer;
        private System.Windows.Forms.GroupBox gbOrderItems;
        private System.Windows.Forms.Panel pnOrderItems;
        private System.Windows.Forms.Panel pnOrderItemsContainer;
        private System.Windows.Forms.GroupBox gpBottom;
        private System.Windows.Forms.Label lblItemPrice;
        private System.Windows.Forms.Label lblItemName;
        private System.Windows.Forms.Panel pnlDisplayItemName;
        private System.Windows.Forms.Panel pnLogo;
        private System.Windows.Forms.Panel pnlLogoParent;
        private System.Windows.Forms.Panel pnlLastItem;
    }
}

