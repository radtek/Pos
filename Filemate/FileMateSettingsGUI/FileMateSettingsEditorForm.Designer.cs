namespace FileMateSettingsGUI {
    partial class FileMateSettingsEditorForm {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FileMateSettingsEditorForm));
            this.tcSettings = new System.Windows.Forms.TabControl();
            this.tpSumma = new System.Windows.Forms.TabPage();
            this.cbSummaEnabled = new System.Windows.Forms.CheckBox();
            this.tbPassword = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tbUsername = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbSchedule = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.gbIdMap = new System.Windows.Forms.GroupBox();
            this.btnAddIdMapEntry = new System.Windows.Forms.Button();
            this.btnDeleteIdMapEntry = new System.Windows.Forms.Button();
            this.lvIdMap = new System.Windows.Forms.ListView();
            this.chSiteId = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.chSummaId = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tbPort = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnBrowseExportPath = new System.Windows.Forms.Button();
            this.lblExportPath = new System.Windows.Forms.Label();
            this.tbExportPath = new System.Windows.Forms.TextBox();
            this.tpFileMate = new System.Windows.Forms.TabPage();
            this.btnAddCopyOp = new System.Windows.Forms.Button();
            this.btnDeleteCopyOp = new System.Windows.Forms.Button();
            this.lvFileMateCopyOps = new System.Windows.Forms.ListView();
            this.ilCopyGroupImageList = new System.Windows.Forms.ImageList(this.components);
            this.btnSaveAll = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.ttHelpTip = new System.Windows.Forms.ToolTip(this.components);
            this.tcSettings.SuspendLayout();
            this.tpSumma.SuspendLayout();
            this.gbIdMap.SuspendLayout();
            this.tpFileMate.SuspendLayout();
            this.SuspendLayout();
            // 
            // tcSettings
            // 
            this.tcSettings.Controls.Add(this.tpSumma);
            this.tcSettings.Controls.Add(this.tpFileMate);
            this.tcSettings.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tcSettings.Location = new System.Drawing.Point(5, 7);
            this.tcSettings.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tcSettings.Name = "tcSettings";
            this.tcSettings.SelectedIndex = 0;
            this.tcSettings.Size = new System.Drawing.Size(928, 693);
            this.tcSettings.TabIndex = 0;
            // 
            // tpSumma
            // 
            this.tpSumma.Controls.Add(this.cbSummaEnabled);
            this.tpSumma.Controls.Add(this.tbPassword);
            this.tpSumma.Controls.Add(this.label4);
            this.tpSumma.Controls.Add(this.tbUsername);
            this.tpSumma.Controls.Add(this.label1);
            this.tpSumma.Controls.Add(this.tbSchedule);
            this.tpSumma.Controls.Add(this.label2);
            this.tpSumma.Controls.Add(this.gbIdMap);
            this.tpSumma.Controls.Add(this.tbPort);
            this.tpSumma.Controls.Add(this.label3);
            this.tpSumma.Controls.Add(this.btnBrowseExportPath);
            this.tpSumma.Controls.Add(this.lblExportPath);
            this.tpSumma.Controls.Add(this.tbExportPath);
            this.tpSumma.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tpSumma.Location = new System.Drawing.Point(4, 39);
            this.tpSumma.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tpSumma.Name = "tpSumma";
            this.tpSumma.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tpSumma.Size = new System.Drawing.Size(920, 650);
            this.tpSumma.TabIndex = 0;
            this.tpSumma.Text = "Summa Settings";
            this.tpSumma.UseVisualStyleBackColor = true;
            // 
            // cbSummaEnabled
            // 
            this.cbSummaEnabled.AutoSize = true;
            this.cbSummaEnabled.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F);
            this.cbSummaEnabled.Location = new System.Drawing.Point(520, 569);
            this.cbSummaEnabled.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cbSummaEnabled.Name = "cbSummaEnabled";
            this.cbSummaEnabled.Size = new System.Drawing.Size(147, 40);
            this.cbSummaEnabled.TabIndex = 23;
            this.cbSummaEnabled.Text = "Enabled";
            this.ttHelpTip.SetToolTip(this.cbSummaEnabled, "Enables the summa operations. If unchecked, FileMate will ignore this operation.");
            this.cbSummaEnabled.UseVisualStyleBackColor = true;
            // 
            // tbPassword
            // 
            this.tbPassword.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbPassword.Location = new System.Drawing.Point(200, 503);
            this.tbPassword.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbPassword.Name = "tbPassword";
            this.tbPassword.Size = new System.Drawing.Size(411, 41);
            this.tbPassword.TabIndex = 21;
            this.ttHelpTip.SetToolTip(this.tbPassword, "Enter the schedule as an interval e.g., 5m for 5 minutes or\r\nas a set time using " +
        "a 24-hour format e.g., 04:00 for 4am.");
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(25, 507);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(147, 36);
            this.label4.TabIndex = 22;
            this.label4.Text = "Password";
            // 
            // tbUsername
            // 
            this.tbUsername.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbUsername.Location = new System.Drawing.Point(200, 446);
            this.tbUsername.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbUsername.Name = "tbUsername";
            this.tbUsername.Size = new System.Drawing.Size(411, 41);
            this.tbUsername.TabIndex = 19;
            this.ttHelpTip.SetToolTip(this.tbUsername, "Enter the schedule as an interval e.g., 5m for 5 minutes or\r\nas a set time using " +
        "a 24-hour format e.g., 04:00 for 4am.");
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(25, 449);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(150, 36);
            this.label1.TabIndex = 20;
            this.label1.Text = "Username";
            // 
            // tbSchedule
            // 
            this.tbSchedule.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSchedule.Location = new System.Drawing.Point(200, 566);
            this.tbSchedule.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbSchedule.Name = "tbSchedule";
            this.tbSchedule.Size = new System.Drawing.Size(311, 41);
            this.tbSchedule.TabIndex = 6;
            this.ttHelpTip.SetToolTip(this.tbSchedule, "Enter the schedule as an interval e.g., 5m for 5 minutes or\r\nas a set time using " +
        "a 24-hour format e.g., 04:00 for 4am.");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(25, 570);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(140, 36);
            this.label2.TabIndex = 18;
            this.label2.Text = "Schedule";
            // 
            // gbIdMap
            // 
            this.gbIdMap.Controls.Add(this.btnAddIdMapEntry);
            this.gbIdMap.Controls.Add(this.btnDeleteIdMapEntry);
            this.gbIdMap.Controls.Add(this.lvIdMap);
            this.gbIdMap.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbIdMap.Location = new System.Drawing.Point(17, 127);
            this.gbIdMap.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.gbIdMap.Name = "gbIdMap";
            this.gbIdMap.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.gbIdMap.Size = new System.Drawing.Size(881, 302);
            this.gbIdMap.TabIndex = 13;
            this.gbIdMap.TabStop = false;
            this.gbIdMap.Text = "Summa Id Map";
            // 
            // btnAddIdMapEntry
            // 
            this.btnAddIdMapEntry.Location = new System.Drawing.Point(8, 229);
            this.btnAddIdMapEntry.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnAddIdMapEntry.Name = "btnAddIdMapEntry";
            this.btnAddIdMapEntry.Size = new System.Drawing.Size(100, 60);
            this.btnAddIdMapEntry.TabIndex = 4;
            this.btnAddIdMapEntry.Text = "Add";
            this.btnAddIdMapEntry.UseVisualStyleBackColor = true;
            this.btnAddIdMapEntry.Click += new System.EventHandler(this.btnAddIdMapEntry_Click);
            // 
            // btnDeleteIdMapEntry
            // 
            this.btnDeleteIdMapEntry.Location = new System.Drawing.Point(731, 229);
            this.btnDeleteIdMapEntry.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnDeleteIdMapEntry.Name = "btnDeleteIdMapEntry";
            this.btnDeleteIdMapEntry.Size = new System.Drawing.Size(143, 60);
            this.btnDeleteIdMapEntry.TabIndex = 5;
            this.btnDeleteIdMapEntry.Text = "Delete";
            this.btnDeleteIdMapEntry.UseVisualStyleBackColor = true;
            this.btnDeleteIdMapEntry.Click += new System.EventHandler(this.btnDeleteIdMapEntry_Click);
            // 
            // lvIdMap
            // 
            this.lvIdMap.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.chSiteId,
            this.chSummaId});
            this.lvIdMap.FullRowSelect = true;
            this.lvIdMap.GridLines = true;
            this.lvIdMap.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lvIdMap.HideSelection = false;
            this.lvIdMap.Location = new System.Drawing.Point(8, 31);
            this.lvIdMap.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lvIdMap.MultiSelect = false;
            this.lvIdMap.Name = "lvIdMap";
            this.lvIdMap.ShowGroups = false;
            this.lvIdMap.Size = new System.Drawing.Size(864, 190);
            this.lvIdMap.TabIndex = 3;
            this.lvIdMap.UseCompatibleStateImageBehavior = false;
            this.lvIdMap.View = System.Windows.Forms.View.Details;
            this.lvIdMap.ItemActivate += new System.EventHandler(this.lvIdMap_ItemActivate);
            // 
            // chSiteId
            // 
            this.chSiteId.Text = "Site Id";
            this.chSiteId.Width = 277;
            // 
            // chSummaId
            // 
            this.chSummaId.Text = "Summa Id";
            this.chSummaId.Width = 281;
            // 
            // tbPort
            // 
            this.tbPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbPort.Location = new System.Drawing.Point(200, 76);
            this.tbPort.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbPort.Name = "tbPort";
            this.tbPort.Size = new System.Drawing.Size(195, 41);
            this.tbPort.TabIndex = 2;
            this.ttHelpTip.SetToolTip(this.tbPort, "The port number to listen on.");
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(11, 80);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 36);
            this.label3.TabIndex = 5;
            this.label3.Text = "Port";
            // 
            // btnBrowseExportPath
            // 
            this.btnBrowseExportPath.Location = new System.Drawing.Point(817, 26);
            this.btnBrowseExportPath.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnBrowseExportPath.Name = "btnBrowseExportPath";
            this.btnBrowseExportPath.Size = new System.Drawing.Size(81, 43);
            this.btnBrowseExportPath.TabIndex = 1;
            this.btnBrowseExportPath.Text = "...";
            this.btnBrowseExportPath.UseVisualStyleBackColor = true;
            this.btnBrowseExportPath.Click += new System.EventHandler(this.btnBrowseExportPath_Click);
            // 
            // lblExportPath
            // 
            this.lblExportPath.AutoSize = true;
            this.lblExportPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblExportPath.Location = new System.Drawing.Point(11, 30);
            this.lblExportPath.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblExportPath.Name = "lblExportPath";
            this.lblExportPath.Size = new System.Drawing.Size(171, 36);
            this.lblExportPath.TabIndex = 1;
            this.lblExportPath.Text = "Export Path";
            // 
            // tbExportPath
            // 
            this.tbExportPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbExportPath.Location = new System.Drawing.Point(200, 26);
            this.tbExportPath.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbExportPath.Name = "tbExportPath";
            this.tbExportPath.Size = new System.Drawing.Size(608, 41);
            this.tbExportPath.TabIndex = 0;
            // 
            // tpFileMate
            // 
            this.tpFileMate.Controls.Add(this.btnAddCopyOp);
            this.tpFileMate.Controls.Add(this.btnDeleteCopyOp);
            this.tpFileMate.Controls.Add(this.lvFileMateCopyOps);
            this.tpFileMate.Location = new System.Drawing.Point(4, 39);
            this.tpFileMate.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tpFileMate.Name = "tpFileMate";
            this.tpFileMate.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tpFileMate.Size = new System.Drawing.Size(920, 650);
            this.tpFileMate.TabIndex = 1;
            this.tpFileMate.Text = "FileMate Settings";
            this.tpFileMate.UseVisualStyleBackColor = true;
            // 
            // btnAddCopyOp
            // 
            this.btnAddCopyOp.Location = new System.Drawing.Point(8, 578);
            this.btnAddCopyOp.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnAddCopyOp.Name = "btnAddCopyOp";
            this.btnAddCopyOp.Size = new System.Drawing.Size(181, 60);
            this.btnAddCopyOp.TabIndex = 14;
            this.btnAddCopyOp.Text = "Add";
            this.btnAddCopyOp.UseVisualStyleBackColor = true;
            this.btnAddCopyOp.Click += new System.EventHandler(this.btnAddCopyOp_Click);
            // 
            // btnDeleteCopyOp
            // 
            this.btnDeleteCopyOp.Location = new System.Drawing.Point(764, 578);
            this.btnDeleteCopyOp.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnDeleteCopyOp.Name = "btnDeleteCopyOp";
            this.btnDeleteCopyOp.Size = new System.Drawing.Size(143, 60);
            this.btnDeleteCopyOp.TabIndex = 13;
            this.btnDeleteCopyOp.Text = "Delete";
            this.btnDeleteCopyOp.UseVisualStyleBackColor = true;
            this.btnDeleteCopyOp.Click += new System.EventHandler(this.btnDeleteCopyOp_Click);
            // 
            // lvFileMateCopyOps
            // 
            this.lvFileMateCopyOps.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvFileMateCopyOps.HideSelection = false;
            this.lvFileMateCopyOps.LargeImageList = this.ilCopyGroupImageList;
            this.lvFileMateCopyOps.Location = new System.Drawing.Point(4, 4);
            this.lvFileMateCopyOps.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lvFileMateCopyOps.MultiSelect = false;
            this.lvFileMateCopyOps.Name = "lvFileMateCopyOps";
            this.lvFileMateCopyOps.Size = new System.Drawing.Size(897, 566);
            this.lvFileMateCopyOps.TabIndex = 0;
            this.lvFileMateCopyOps.UseCompatibleStateImageBehavior = false;
            this.lvFileMateCopyOps.ItemActivate += new System.EventHandler(this.lvFileMateCopyOps_ItemActivate);
            // 
            // ilCopyGroupImageList
            // 
            this.ilCopyGroupImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ilCopyGroupImageList.ImageStream")));
            this.ilCopyGroupImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.ilCopyGroupImageList.Images.SetKeyName(0, "Group.png");
            // 
            // btnSaveAll
            // 
            this.btnSaveAll.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnSaveAll.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSaveAll.Location = new System.Drawing.Point(5, 708);
            this.btnSaveAll.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnSaveAll.Name = "btnSaveAll";
            this.btnSaveAll.Size = new System.Drawing.Size(159, 68);
            this.btnSaveAll.TabIndex = 7;
            this.btnSaveAll.Text = "Save All";
            this.btnSaveAll.UseVisualStyleBackColor = true;
            this.btnSaveAll.Click += new System.EventHandler(this.btnSaveAll_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.Location = new System.Drawing.Point(775, 708);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(159, 68);
            this.btnCancel.TabIndex = 8;
            this.btnCancel.Text = "Close";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // ttHelpTip
            // 
            this.ttHelpTip.IsBalloon = true;
            this.ttHelpTip.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.ttHelpTip.ToolTipTitle = "Description";
            // 
            // FileMateSettingsEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(941, 791);
            this.ControlBox = false;
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSaveAll);
            this.Controls.Add(this.tcSettings);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FileMateSettingsEditorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Settings Editor";
            this.tcSettings.ResumeLayout(false);
            this.tpSumma.ResumeLayout(false);
            this.tpSumma.PerformLayout();
            this.gbIdMap.ResumeLayout(false);
            this.tpFileMate.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tcSettings;
        private System.Windows.Forms.TabPage tpSumma;
        private System.Windows.Forms.TabPage tpFileMate;
        private System.Windows.Forms.Label lblExportPath;
        private System.Windows.Forms.TextBox tbExportPath;
        private System.Windows.Forms.Button btnBrowseExportPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbPort;
        private System.Windows.Forms.Button btnSaveAll;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ListView lvIdMap;
        private System.Windows.Forms.ColumnHeader chSiteId;
        private System.Windows.Forms.ColumnHeader chSummaId;
        private System.Windows.Forms.Button btnAddIdMapEntry;
        private System.Windows.Forms.Button btnDeleteIdMapEntry;
        private System.Windows.Forms.GroupBox gbIdMap;
        private System.Windows.Forms.TextBox tbSchedule;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolTip ttHelpTip;
        private System.Windows.Forms.ListView lvFileMateCopyOps;
        private System.Windows.Forms.ImageList ilCopyGroupImageList;
        private System.Windows.Forms.Button btnAddCopyOp;
        private System.Windows.Forms.Button btnDeleteCopyOp;
        private System.Windows.Forms.TextBox tbPassword;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbUsername;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox cbSummaEnabled;
    }
}

