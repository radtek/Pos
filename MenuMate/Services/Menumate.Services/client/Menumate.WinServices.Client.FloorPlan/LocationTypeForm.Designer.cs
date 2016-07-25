namespace Menumate.WinServices.Client.FloorPlan
{
    partial class LocationTypeForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LocationTypeForm));
            this.groupBoxLocationImageType = new System.Windows.Forms.GroupBox();
            this.groupBoxText = new System.Windows.Forms.GroupBox();
            this.labelHelp = new System.Windows.Forms.Label();
            this.buttonClose = new System.Windows.Forms.Button();
            this.radioButtonLandscape = new System.Windows.Forms.RadioButton();
            this.radioButtonPortrait = new System.Windows.Forms.RadioButton();
            this.groupBoxLocationImageType.SuspendLayout();
            this.groupBoxText.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxLocationImageType
            // 
            this.groupBoxLocationImageType.Controls.Add(this.radioButtonLandscape);
            this.groupBoxLocationImageType.Controls.Add(this.radioButtonPortrait);
            this.groupBoxLocationImageType.ForeColor = System.Drawing.Color.SteelBlue;
            this.groupBoxLocationImageType.Location = new System.Drawing.Point(12, 12);
            this.groupBoxLocationImageType.Name = "groupBoxLocationImageType";
            this.groupBoxLocationImageType.Size = new System.Drawing.Size(302, 160);
            this.groupBoxLocationImageType.TabIndex = 3;
            this.groupBoxLocationImageType.TabStop = false;
            this.groupBoxLocationImageType.Text = " Location  Image Orientation";
            // 
            // groupBoxText
            // 
            this.groupBoxText.Controls.Add(this.labelHelp);
            this.groupBoxText.Location = new System.Drawing.Point(12, 178);
            this.groupBoxText.Name = "groupBoxText";
            this.groupBoxText.Size = new System.Drawing.Size(301, 67);
            this.groupBoxText.TabIndex = 4;
            this.groupBoxText.TabStop = false;
            // 
            // labelHelp
            // 
            this.labelHelp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelHelp.ForeColor = System.Drawing.Color.SteelBlue;
            this.labelHelp.Location = new System.Drawing.Point(3, 16);
            this.labelHelp.Name = "labelHelp";
            this.labelHelp.Size = new System.Drawing.Size(295, 48);
            this.labelHelp.TabIndex = 0;
            this.labelHelp.Text = "Select the image orientation. It will not be changed in the future";
            this.labelHelp.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // buttonClose
            // 
            this.buttonClose.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonClose.Location = new System.Drawing.Point(235, 251);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 5;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            // 
            // radioButtonLandscape
            // 
            this.radioButtonLandscape.AutoSize = true;
            this.radioButtonLandscape.Checked = true;
            this.radioButtonLandscape.ForeColor = System.Drawing.Color.SteelBlue;
            this.radioButtonLandscape.Image = ((System.Drawing.Image)(resources.GetObject("radioButtonLandscape.Image")));
            this.radioButtonLandscape.Location = new System.Drawing.Point(33, 42);
            this.radioButtonLandscape.Name = "radioButtonLandscape";
            this.radioButtonLandscape.Size = new System.Drawing.Size(94, 77);
            this.radioButtonLandscape.TabIndex = 4;
            this.radioButtonLandscape.TabStop = true;
            this.radioButtonLandscape.Text = "Landscape";
            this.radioButtonLandscape.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.radioButtonLandscape.TextImageRelation = System.Windows.Forms.TextImageRelation.TextAboveImage;
            this.radioButtonLandscape.UseVisualStyleBackColor = true;
            // 
            // radioButtonPortrait
            // 
            this.radioButtonPortrait.AutoSize = true;
            this.radioButtonPortrait.BackColor = System.Drawing.SystemColors.Control;
            this.radioButtonPortrait.ForeColor = System.Drawing.Color.SteelBlue;
            this.radioButtonPortrait.Image = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.Portrait;
            this.radioButtonPortrait.Location = new System.Drawing.Point(187, 32);
            this.radioButtonPortrait.Name = "radioButtonPortrait";
            this.radioButtonPortrait.Size = new System.Drawing.Size(74, 97);
            this.radioButtonPortrait.TabIndex = 3;
            this.radioButtonPortrait.Text = "Portrait";
            this.radioButtonPortrait.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.radioButtonPortrait.TextImageRelation = System.Windows.Forms.TextImageRelation.TextAboveImage;
            this.radioButtonPortrait.UseVisualStyleBackColor = false;
            // 
            // LocationTypeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(325, 284);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.groupBoxText);
            this.Controls.Add(this.groupBoxLocationImageType);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "LocationTypeForm";
            this.Text = "FloorPlan";
            this.groupBoxLocationImageType.ResumeLayout(false);
            this.groupBoxLocationImageType.PerformLayout();
            this.groupBoxText.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxLocationImageType;
        private System.Windows.Forms.RadioButton radioButtonLandscape;
        private System.Windows.Forms.RadioButton radioButtonPortrait;
        private System.Windows.Forms.GroupBox groupBoxText;
        private System.Windows.Forms.Label labelHelp;
        private System.Windows.Forms.Button buttonClose;

    }
}