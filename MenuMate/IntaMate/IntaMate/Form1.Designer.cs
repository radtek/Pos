namespace IntaMate
{
    partial class frmMain
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.LogBox = new System.Windows.Forms.RichTextBox();
            this.QueryOnline = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripOnlineLogging = new System.Windows.Forms.ToolStripMenuItem();
            this.noneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.traceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.infoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.warningsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.errorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fatalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.button1 = new System.Windows.Forms.Button();
            this.lbeCommport = new System.Windows.Forms.Label();
            this.lbEnabled = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.lbListenPort = new System.Windows.Forms.Label();
            this.lbPort = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cbMsgType = new System.Windows.Forms.ComboBox();
            this.cbTransport = new System.Windows.Forms.ComboBox();
            this.tbTimeOut = new System.Windows.Forms.TextBox();
            this.tbPort = new System.Windows.Forms.TextBox();
            this.tbListenPort = new System.Windows.Forms.TextBox();
            this.tbIPAddress = new System.Windows.Forms.TextBox();
            this.cbAPI = new System.Windows.Forms.ComboBox();
            this.cbComPort = new System.Windows.Forms.ComboBox();
            this.cbPMSRef = new System.Windows.Forms.CheckBox();
            this.RoomNumberLength = new System.Windows.Forms.NumericUpDown();
            this.contextMenuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RoomNumberLength)).BeginInit();
            this.SuspendLayout();
            // 
            // LogBox
            // 
            this.LogBox.Location = new System.Drawing.Point(16, 336);
            this.LogBox.Margin = new System.Windows.Forms.Padding(4);
            this.LogBox.Name = "LogBox";
            this.LogBox.Size = new System.Drawing.Size(762, 196);
            this.LogBox.TabIndex = 2;
            this.LogBox.Text = "";
            // 
            // QueryOnline
            // 
            this.QueryOnline.Location = new System.Drawing.Point(619, 13);
            this.QueryOnline.Margin = new System.Windows.Forms.Padding(4);
            this.QueryOnline.Name = "QueryOnline";
            this.QueryOnline.Size = new System.Drawing.Size(159, 27);
            this.QueryOnline.TabIndex = 3;
            this.QueryOnline.Text = "Test Connection";
            this.QueryOnline.UseVisualStyleBackColor = true;
            this.QueryOnline.Click += new System.EventHandler(this.button2_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 134);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(147, 17);
            this.label1.TabIndex = 6;
            this.label1.Text = "Room Number Length";
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip1;
            this.notifyIcon1.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon1.Icon")));
            this.notifyIcon1.Text = "IntaMate";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon1_MouseDoubleClick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem2,
            this.toolStripOnlineLogging,
            this.toolStripMenuItem1});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(148, 70);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(147, 22);
            this.toolStripMenuItem2.Text = "Configure";
            this.toolStripMenuItem2.Click += new System.EventHandler(this.toolStripMenuItem2_Click);
            // 
            // toolStripOnlineLogging
            // 
            this.toolStripOnlineLogging.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.noneToolStripMenuItem,
            this.traceToolStripMenuItem,
            this.debugToolStripMenuItem,
            this.infoToolStripMenuItem,
            this.warningsToolStripMenuItem,
            this.errorsToolStripMenuItem,
            this.fatalToolStripMenuItem,
            this.allToolStripMenuItem});
            this.toolStripOnlineLogging.Name = "toolStripOnlineLogging";
            this.toolStripOnlineLogging.Size = new System.Drawing.Size(147, 22);
            this.toolStripOnlineLogging.Text = "Logging";
            // 
            // noneToolStripMenuItem
            // 
            this.noneToolStripMenuItem.CheckOnClick = true;
            this.noneToolStripMenuItem.Name = "noneToolStripMenuItem";
            this.noneToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.noneToolStripMenuItem.Text = "None";
            this.noneToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // traceToolStripMenuItem
            // 
            this.traceToolStripMenuItem.CheckOnClick = true;
            this.traceToolStripMenuItem.Name = "traceToolStripMenuItem";
            this.traceToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.traceToolStripMenuItem.Text = "Trace";
            this.traceToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // debugToolStripMenuItem
            // 
            this.debugToolStripMenuItem.CheckOnClick = true;
            this.debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            this.debugToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.debugToolStripMenuItem.Text = "Debug";
            this.debugToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // infoToolStripMenuItem
            // 
            this.infoToolStripMenuItem.CheckOnClick = true;
            this.infoToolStripMenuItem.Name = "infoToolStripMenuItem";
            this.infoToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.infoToolStripMenuItem.Text = "Info";
            this.infoToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // warningsToolStripMenuItem
            // 
            this.warningsToolStripMenuItem.CheckOnClick = true;
            this.warningsToolStripMenuItem.Name = "warningsToolStripMenuItem";
            this.warningsToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.warningsToolStripMenuItem.Text = "Warnings";
            this.warningsToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // errorsToolStripMenuItem
            // 
            this.errorsToolStripMenuItem.CheckOnClick = true;
            this.errorsToolStripMenuItem.Name = "errorsToolStripMenuItem";
            this.errorsToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.errorsToolStripMenuItem.Text = "Errors";
            this.errorsToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // fatalToolStripMenuItem
            // 
            this.fatalToolStripMenuItem.CheckOnClick = true;
            this.fatalToolStripMenuItem.Name = "fatalToolStripMenuItem";
            this.fatalToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.fatalToolStripMenuItem.Text = "Fatal";
            this.fatalToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.enabledToolStripMenuItem_CheckStateChanged);
            // 
            // allToolStripMenuItem
            // 
            this.allToolStripMenuItem.Name = "allToolStripMenuItem";
            this.allToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.allToolStripMenuItem.Text = "All";
            this.allToolStripMenuItem.Click += new System.EventHandler(this.allToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(147, 22);
            this.toolStripMenuItem1.Text = "Close IntaMate";
            this.toolStripMenuItem1.Click += new System.EventHandler(this.toolStripMenuItem1_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(678, 301);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 28);
            this.button1.TabIndex = 7;
            this.button1.Text = "Clear Log";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lbeCommport
            // 
            this.lbeCommport.AutoSize = true;
            this.lbeCommport.Location = new System.Drawing.Point(13, 80);
            this.lbeCommport.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbeCommport.Name = "lbeCommport";
            this.lbeCommport.Size = new System.Drawing.Size(77, 17);
            this.lbeCommport.TabIndex = 10;
            this.lbeCommport.Text = "Comm Port";
            // 
            // lbEnabled
            // 
            this.lbEnabled.AutoSize = true;
            this.lbEnabled.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbEnabled.ForeColor = System.Drawing.Color.Red;
            this.lbEnabled.Location = new System.Drawing.Point(80, 16);
            this.lbEnabled.Margin = new System.Windows.Forms.Padding(0);
            this.lbEnabled.Name = "lbEnabled";
            this.lbEnabled.Size = new System.Drawing.Size(76, 17);
            this.lbEnabled.TabIndex = 11;
            this.lbEnabled.Text = "Disabled.";
            this.lbEnabled.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 16);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 17);
            this.label3.TabIndex = 13;
            this.label3.Text = "Interface";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(16, 301);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(156, 28);
            this.btnSave.TabIndex = 14;
            this.btnSave.Text = "Save Settings";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // lbListenPort
            // 
            this.lbListenPort.AutoSize = true;
            this.lbListenPort.Location = new System.Drawing.Point(13, 50);
            this.lbListenPort.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbListenPort.Name = "lbListenPort";
            this.lbListenPort.Size = new System.Drawing.Size(76, 17);
            this.lbListenPort.TabIndex = 17;
            this.lbListenPort.Text = "Listen Port";
            // 
            // lbPort
            // 
            this.lbPort.AutoSize = true;
            this.lbPort.Location = new System.Drawing.Point(13, 109);
            this.lbPort.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbPort.Name = "lbPort";
            this.lbPort.Size = new System.Drawing.Size(34, 17);
            this.lbPort.TabIndex = 19;
            this.lbPort.Text = "Port";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 168);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 17);
            this.label2.TabIndex = 21;
            this.label2.Text = "Time Out (ms)";
            // 
            // cbMsgType
            // 
            this.cbMsgType.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "InterfaceMsgType", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbMsgType.FormattingEnabled = true;
            this.cbMsgType.Location = new System.Drawing.Point(344, 12);
            this.cbMsgType.Name = "cbMsgType";
            this.cbMsgType.Size = new System.Drawing.Size(121, 24);
            this.cbMsgType.TabIndex = 23;
            this.cbMsgType.Text = global::IntaMate.Properties.Settings.Default.InterfaceMsgType;
            this.cbMsgType.SelectedIndexChanged += new System.EventHandler(this.cbMsgType_SelectedIndexChanged);
            // 
            // cbTransport
            // 
            this.cbTransport.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "IOInterfaceType", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbTransport.FormattingEnabled = true;
            this.cbTransport.Location = new System.Drawing.Point(482, 12);
            this.cbTransport.Name = "cbTransport";
            this.cbTransport.Size = new System.Drawing.Size(121, 24);
            this.cbTransport.TabIndex = 22;
            this.cbTransport.Text = global::IntaMate.Properties.Settings.Default.IOInterfaceType;
            this.cbTransport.SelectedIndexChanged += new System.EventHandler(this.cbTransport_SelectedIndexChanged);
            // 
            // tbTimeOut
            // 
            this.tbTimeOut.Location = new System.Drawing.Point(168, 165);
            this.tbTimeOut.Name = "tbTimeOut";
            this.tbTimeOut.Size = new System.Drawing.Size(160, 23);
            this.tbTimeOut.TabIndex = 20;
            this.tbTimeOut.Text = global::IntaMate.Properties.Settings.Default.ReadTimeOut;
            this.tbTimeOut.Visible = false;
            // 
            // tbPort
            // 
            this.tbPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "Port", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbPort.Location = new System.Drawing.Point(168, 106);
            this.tbPort.Name = "tbPort";
            this.tbPort.Size = new System.Drawing.Size(160, 23);
            this.tbPort.TabIndex = 18;
            this.tbPort.Text = global::IntaMate.Properties.Settings.Default.Port;
            this.tbPort.Visible = false;
            // 
            // tbListenPort
            // 
            this.tbListenPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "ListenPort", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbListenPort.Location = new System.Drawing.Point(168, 47);
            this.tbListenPort.Name = "tbListenPort";
            this.tbListenPort.Size = new System.Drawing.Size(160, 23);
            this.tbListenPort.TabIndex = 16;
            this.tbListenPort.Text = global::IntaMate.Properties.Settings.Default.ListenPort;
            this.tbListenPort.Visible = false;
            // 
            // tbIPAddress
            // 
            this.tbIPAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "IPAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbIPAddress.Location = new System.Drawing.Point(168, 77);
            this.tbIPAddress.Name = "tbIPAddress";
            this.tbIPAddress.Size = new System.Drawing.Size(160, 23);
            this.tbIPAddress.TabIndex = 15;
            this.tbIPAddress.Text = global::IntaMate.Properties.Settings.Default.IPAddress;
            this.tbIPAddress.Visible = false;
            this.tbIPAddress.TextChanged += new System.EventHandler(this.tbIPAddress_TextChanged);
            // 
            // cbAPI
            // 
            this.cbAPI.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "InterfaceAPI", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbAPI.FormattingEnabled = true;
            this.cbAPI.Items.AddRange(new object[] {
            "roomMaster 2000",
            "Fadelio"});
            this.cbAPI.Location = new System.Drawing.Point(168, 13);
            this.cbAPI.Margin = new System.Windows.Forms.Padding(4);
            this.cbAPI.Name = "cbAPI";
            this.cbAPI.Size = new System.Drawing.Size(160, 24);
            this.cbAPI.TabIndex = 12;
            this.cbAPI.Text = global::IntaMate.Properties.Settings.Default.InterfaceAPI;
            this.cbAPI.SelectedIndexChanged += new System.EventHandler(this.cbAPI_SelectedIndexChanged);
            // 
            // cbComPort
            // 
            this.cbComPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::IntaMate.Properties.Settings.Default, "ComPort", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbComPort.FormattingEnabled = true;
            this.cbComPort.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "COM10",
            "COM11",
            "COM12",
            "COM13",
            "COM14",
            "COM15",
            "COM16"});
            this.cbComPort.Location = new System.Drawing.Point(168, 77);
            this.cbComPort.Margin = new System.Windows.Forms.Padding(4);
            this.cbComPort.Name = "cbComPort";
            this.cbComPort.Size = new System.Drawing.Size(160, 24);
            this.cbComPort.TabIndex = 9;
            this.cbComPort.Text = global::IntaMate.Properties.Settings.Default.ComPort;
            // 
            // cbPMSRef
            // 
            this.cbPMSRef.AutoSize = true;
            this.cbPMSRef.Checked = global::IntaMate.Properties.Settings.Default.PMSPassThough;
            this.cbPMSRef.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbPMSRef.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::IntaMate.Properties.Settings.Default, "PMSPassThough", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbPMSRef.Location = new System.Drawing.Point(191, 301);
            this.cbPMSRef.Margin = new System.Windows.Forms.Padding(4);
            this.cbPMSRef.Name = "cbPMSRef";
            this.cbPMSRef.Size = new System.Drawing.Size(274, 21);
            this.cbPMSRef.TabIndex = 8;
            this.cbPMSRef.Text = "Pass though MenuMate PMS reference";
            this.cbPMSRef.UseVisualStyleBackColor = true;
            // 
            // RoomNumberLength
            // 
            this.RoomNumberLength.AutoSize = true;
            this.RoomNumberLength.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::IntaMate.Properties.Settings.Default, "RoomNumberLength", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.RoomNumberLength.Location = new System.Drawing.Point(168, 134);
            this.RoomNumberLength.Margin = new System.Windows.Forms.Padding(4);
            this.RoomNumberLength.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.RoomNumberLength.Name = "RoomNumberLength";
            this.RoomNumberLength.Size = new System.Drawing.Size(160, 23);
            this.RoomNumberLength.TabIndex = 5;
            this.RoomNumberLength.Value = global::IntaMate.Properties.Settings.Default.RoomNumberLength;
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(791, 536);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cbMsgType);
            this.Controls.Add(this.tbTimeOut);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lbPort);
            this.Controls.Add(this.cbTransport);
            this.Controls.Add(this.tbPort);
            this.Controls.Add(this.lbListenPort);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.tbListenPort);
            this.Controls.Add(this.tbIPAddress);
            this.Controls.Add(this.lbeCommport);
            this.Controls.Add(this.cbAPI);
            this.Controls.Add(this.cbComPort);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cbPMSRef);
            this.Controls.Add(this.QueryOnline);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.LogBox);
            this.Controls.Add(this.RoomNumberLength);
            this.Controls.Add(this.lbEnabled);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "frmMain";
            this.Text = "MenuMate    IntaMate";
            this.WindowState = System.Windows.Forms.FormWindowState.Minimized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmMain_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.contextMenuStrip1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.RoomNumberLength)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox LogBox;
        private System.Windows.Forms.Button QueryOnline;
        private System.Windows.Forms.NumericUpDown RoomNumberLength;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.CheckBox cbPMSRef;
        private System.Windows.Forms.ComboBox cbComPort;
        private System.Windows.Forms.Label lbeCommport;
        private System.Windows.Forms.Label lbEnabled;
        private System.Windows.Forms.ComboBox cbAPI;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.TextBox tbIPAddress;
        private System.Windows.Forms.TextBox tbListenPort;
        private System.Windows.Forms.Label lbListenPort;
        private System.Windows.Forms.Label lbPort;
        private System.Windows.Forms.TextBox tbPort;
        private System.Windows.Forms.ToolStripMenuItem toolStripOnlineLogging;
        private System.Windows.Forms.ToolStripMenuItem noneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem traceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem infoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem warningsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem errorsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fatalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allToolStripMenuItem;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbTimeOut;
        private System.Windows.Forms.ComboBox cbTransport;
        private System.Windows.Forms.ComboBox cbMsgType;
    }
}

