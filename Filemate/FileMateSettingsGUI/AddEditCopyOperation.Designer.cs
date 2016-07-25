namespace FileMateSettingsGUI {
    partial class AddEditCopyOperation {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddEditCopyOperation));
            this.lvCopyOperationTargets = new System.Windows.Forms.ListView();
            this.ilCopyGroupTargetImageList = new System.Windows.Forms.ImageList(this.components);
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOk = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tbSource = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnAddTarget = new System.Windows.Forms.Button();
            this.btnDeleteTarget = new System.Windows.Forms.Button();
            this.tbSchedule = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbPattern = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tbName = new System.Windows.Forms.TextBox();
            this.tbSrcUname = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbSrcPwd = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cbCalledFromExternal = new System.Windows.Forms.CheckBox();
            this.cbDeleteSource = new System.Windows.Forms.CheckBox();
            this.btnBrowseExportPath = new System.Windows.Forms.Button();
            this.ttHelpTip = new System.Windows.Forms.ToolTip(this.components);
            this.cbEnabled = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lvCopyOperationTargets
            // 
            this.lvCopyOperationTargets.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lvCopyOperationTargets.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvCopyOperationTargets.Location = new System.Drawing.Point(8, 37);
            this.lvCopyOperationTargets.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lvCopyOperationTargets.MultiSelect = false;
            this.lvCopyOperationTargets.Name = "lvCopyOperationTargets";
            this.lvCopyOperationTargets.Size = new System.Drawing.Size(771, 144);
            this.lvCopyOperationTargets.SmallImageList = this.ilCopyGroupTargetImageList;
            this.lvCopyOperationTargets.TabIndex = 8;
            this.lvCopyOperationTargets.UseCompatibleStateImageBehavior = false;
            this.lvCopyOperationTargets.View = System.Windows.Forms.View.SmallIcon;
            this.lvCopyOperationTargets.ItemActivate += new System.EventHandler(this.lvCopyOperationTargets_ItemActivate);
            // 
            // ilCopyGroupTargetImageList
            // 
            this.ilCopyGroupTargetImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ilCopyGroupTargetImageList.ImageStream")));
            this.ilCopyGroupTargetImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.ilCopyGroupTargetImageList.Images.SetKeyName(0, "target.gif");
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.Location = new System.Drawing.Point(645, 649);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(159, 68);
            this.btnCancel.TabIndex = 14;
            this.btnCancel.Text = "Close";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOk
            // 
            this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOk.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOk.Location = new System.Drawing.Point(23, 649);
            this.btnOk.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(159, 68);
            this.btnOk.TabIndex = 13;
            this.btnOk.Text = "OK";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(16, 57);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 31);
            this.label1.TabIndex = 13;
            this.label1.Text = "Source";
            // 
            // tbSource
            // 
            this.tbSource.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSource.Location = new System.Drawing.Point(177, 53);
            this.tbSource.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbSource.Name = "tbSource";
            this.tbSource.Size = new System.Drawing.Size(536, 37);
            this.tbSource.TabIndex = 2;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnAddTarget);
            this.groupBox1.Controls.Add(this.btnDeleteTarget);
            this.groupBox1.Controls.Add(this.lvCopyOperationTargets);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(16, 252);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Size = new System.Drawing.Size(788, 257);
            this.groupBox1.TabIndex = 15;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Targets";
            // 
            // btnAddTarget
            // 
            this.btnAddTarget.Location = new System.Drawing.Point(7, 190);
            this.btnAddTarget.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnAddTarget.Name = "btnAddTarget";
            this.btnAddTarget.Size = new System.Drawing.Size(100, 60);
            this.btnAddTarget.TabIndex = 9;
            this.btnAddTarget.Text = "Add";
            this.btnAddTarget.UseVisualStyleBackColor = true;
            this.btnAddTarget.Click += new System.EventHandler(this.btnAddTarget_Click);
            // 
            // btnDeleteTarget
            // 
            this.btnDeleteTarget.Location = new System.Drawing.Point(637, 190);
            this.btnDeleteTarget.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnDeleteTarget.Name = "btnDeleteTarget";
            this.btnDeleteTarget.Size = new System.Drawing.Size(143, 60);
            this.btnDeleteTarget.TabIndex = 10;
            this.btnDeleteTarget.Text = "Delete";
            this.btnDeleteTarget.UseVisualStyleBackColor = true;
            this.btnDeleteTarget.Click += new System.EventHandler(this.btnDeleteTarget_Click);
            // 
            // tbSchedule
            // 
            this.tbSchedule.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSchedule.Location = new System.Drawing.Point(177, 530);
            this.tbSchedule.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbSchedule.Name = "tbSchedule";
            this.tbSchedule.Size = new System.Drawing.Size(311, 41);
            this.tbSchedule.TabIndex = 11;
            this.ttHelpTip.SetToolTip(this.tbSchedule, "Enter the schedule as an interval e.g., 5m for 5 minutes or\r\nas a set time using " +
        "a 24-hour format e.g., 04:00 for 4am.");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(9, 530);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(140, 36);
            this.label2.TabIndex = 21;
            this.label2.Text = "Schedule";
            // 
            // tbPattern
            // 
            this.tbPattern.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbPattern.Location = new System.Drawing.Point(177, 581);
            this.tbPattern.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbPattern.Name = "tbPattern";
            this.tbPattern.Size = new System.Drawing.Size(311, 41);
            this.tbPattern.TabIndex = 12;
            this.ttHelpTip.SetToolTip(this.tbPattern, "The pattern to match. Can be a single file e.g., file.txt or a goup of files e.g." +
        ", *.txt.");
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(16, 588);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(110, 36);
            this.label3.TabIndex = 23;
            this.label3.Text = "Pattern";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(16, 11);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(86, 31);
            this.label4.TabIndex = 25;
            this.label4.Text = "Name";
            // 
            // tbName
            // 
            this.tbName.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbName.Location = new System.Drawing.Point(177, 7);
            this.tbName.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(311, 37);
            this.tbName.TabIndex = 1;
            // 
            // tbSrcUname
            // 
            this.tbSrcUname.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSrcUname.Location = new System.Drawing.Point(177, 98);
            this.tbSrcUname.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbSrcUname.Name = "tbSrcUname";
            this.tbSrcUname.Size = new System.Drawing.Size(357, 37);
            this.tbSrcUname.TabIndex = 4;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(16, 102);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(139, 31);
            this.label5.TabIndex = 27;
            this.label5.Text = "Username";
            // 
            // tbSrcPwd
            // 
            this.tbSrcPwd.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSrcPwd.Location = new System.Drawing.Point(177, 144);
            this.tbSrcPwd.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbSrcPwd.Name = "tbSrcPwd";
            this.tbSrcPwd.Size = new System.Drawing.Size(357, 37);
            this.tbSrcPwd.TabIndex = 5;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(16, 148);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(134, 31);
            this.label6.TabIndex = 29;
            this.label6.Text = "Password";
            // 
            // cbCalledFromExternal
            // 
            this.cbCalledFromExternal.AutoSize = true;
            this.cbCalledFromExternal.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbCalledFromExternal.Location = new System.Drawing.Point(487, 209);
            this.cbCalledFromExternal.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cbCalledFromExternal.Name = "cbCalledFromExternal";
            this.cbCalledFromExternal.Size = new System.Drawing.Size(289, 35);
            this.cbCalledFromExternal.TabIndex = 7;
            this.cbCalledFromExternal.Text = "Called From External";
            this.ttHelpTip.SetToolTip(this.cbCalledFromExternal, "Generally only used with Summa. Will not fire an internal\r\nevent to perform the o" +
        "peration, but will notify a parent\r\nprocess and expect that process to perform t" +
        "he operation.");
            this.cbCalledFromExternal.UseVisualStyleBackColor = true;
            // 
            // cbDeleteSource
            // 
            this.cbDeleteSource.AutoSize = true;
            this.cbDeleteSource.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbDeleteSource.Location = new System.Drawing.Point(23, 209);
            this.cbDeleteSource.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cbDeleteSource.Name = "cbDeleteSource";
            this.cbDeleteSource.Size = new System.Drawing.Size(208, 35);
            this.cbDeleteSource.TabIndex = 6;
            this.cbDeleteSource.Text = "Delete Source";
            this.ttHelpTip.SetToolTip(this.cbDeleteSource, "Delete the files from the location specified above after\r\nthe operation has compl" +
        "eted successfully.");
            this.cbDeleteSource.UseVisualStyleBackColor = true;
            // 
            // btnBrowseExportPath
            // 
            this.btnBrowseExportPath.Location = new System.Drawing.Point(723, 53);
            this.btnBrowseExportPath.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnBrowseExportPath.Name = "btnBrowseExportPath";
            this.btnBrowseExportPath.Size = new System.Drawing.Size(81, 43);
            this.btnBrowseExportPath.TabIndex = 3;
            this.btnBrowseExportPath.Text = "...";
            this.btnBrowseExportPath.UseVisualStyleBackColor = true;
            this.btnBrowseExportPath.Click += new System.EventHandler(this.btnBrowseExportPath_Click);
            // 
            // ttHelpTip
            // 
            this.ttHelpTip.IsBalloon = true;
            this.ttHelpTip.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.ttHelpTip.ToolTipTitle = "Description";
            // 
            // cbEnabled
            // 
            this.cbEnabled.AutoSize = true;
            this.cbEnabled.Checked = true;
            this.cbEnabled.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbEnabled.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbEnabled.Location = new System.Drawing.Point(295, 209);
            this.cbEnabled.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cbEnabled.Name = "cbEnabled";
            this.cbEnabled.Size = new System.Drawing.Size(135, 35);
            this.cbEnabled.TabIndex = 30;
            this.cbEnabled.Text = "Enabled";
            this.ttHelpTip.SetToolTip(this.cbEnabled, "Enables the operation. If unchecked FileMate will ignore this operation.");
            this.cbEnabled.UseVisualStyleBackColor = true;
            // 
            // AddEditCopyOperation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(820, 725);
            this.ControlBox = false;
            this.Controls.Add(this.cbEnabled);
            this.Controls.Add(this.btnBrowseExportPath);
            this.Controls.Add(this.cbDeleteSource);
            this.Controls.Add(this.cbCalledFromExternal);
            this.Controls.Add(this.tbSrcPwd);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tbSrcUname);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tbName);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tbPattern);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbSchedule);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbSource);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOk);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "AddEditCopyOperation";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Add or edit a copy operation";
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView lvCopyOperationTargets;
        private System.Windows.Forms.ImageList ilCopyGroupTargetImageList;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbSource;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnAddTarget;
        private System.Windows.Forms.Button btnDeleteTarget;
        private System.Windows.Forms.TextBox tbSchedule;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbPattern;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.TextBox tbSrcUname;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbSrcPwd;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox cbCalledFromExternal;
        private System.Windows.Forms.CheckBox cbDeleteSource;
        private System.Windows.Forms.Button btnBrowseExportPath;
        private System.Windows.Forms.ToolTip ttHelpTip;
        private System.Windows.Forms.CheckBox cbEnabled;
    }
}