using System.Windows.Forms;

namespace MenumateServices.Config
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.tlpMain = new System.Windows.Forms.TableLayoutPanel();
            this.gbTop = new System.Windows.Forms.GroupBox();
            this.lbTitle = new System.Windows.Forms.Label();
            this.lbVersion = new System.Windows.Forms.Label();
            this.pbLogo = new System.Windows.Forms.PictureBox();
            this.pMenu = new System.Windows.Forms.Panel();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.pnBottom = new System.Windows.Forms.Panel();
            this.lbWarnning = new System.Windows.Forms.Label();
            this.tbSetting = new System.Windows.Forms.TabControl();
            this.tbpDatabase = new System.Windows.Forms.TabPage();
            this.gbDatabaseSettings = new System.Windows.Forms.GroupBox();
            this.lbResetingMMServices = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lbUniqueDBServerDescription = new System.Windows.Forms.Label();
            this.lbDBSettingsDescription = new System.Windows.Forms.Label();
            this.btnWebmateDBFolderBrowse = new System.Windows.Forms.Button();
            this.btnUniqueDBFolderBrowse = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.lbWebmate = new System.Windows.Forms.Label();
            this.rbDatabaseServers = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.tbDBServerWebmateFolder = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbDBServerWebmateName = new System.Windows.Forms.TextBox();
            this.rbUniqueDBserver = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.tbUniqueDBserverFolder = new System.Windows.Forms.TextBox();
            this.lbUniqueDBServer = new System.Windows.Forms.Label();
            this.tbUniqueDBserverName = new System.Windows.Forms.TextBox();
            this.tbpIPAddresses = new System.Windows.Forms.TabPage();
            this.gbIpAddressesSettings = new System.Windows.Forms.GroupBox();
            this.lbIpAddress = new System.Windows.Forms.Label();
            this.fbdDatabase = new System.Windows.Forms.FolderBrowserDialog();
            this.tlpMain.SuspendLayout();
            this.gbTop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).BeginInit();
            this.pMenu.SuspendLayout();
            this.pnBottom.SuspendLayout();
            this.tbSetting.SuspendLayout();
            this.tbpDatabase.SuspendLayout();
            this.gbDatabaseSettings.SuspendLayout();
            this.tbpIPAddresses.SuspendLayout();
            this.gbIpAddressesSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // tlpMain
            // 
            this.tlpMain.ColumnCount = 1;
            this.tlpMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpMain.Controls.Add(this.gbTop, 0, 0);
            this.tlpMain.Controls.Add(this.pMenu, 0, 1);
            this.tlpMain.Controls.Add(this.pnBottom, 0, 3);
            this.tlpMain.Controls.Add(this.tbSetting, 0, 2);
            this.tlpMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tlpMain.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tlpMain.ForeColor = System.Drawing.Color.DarkOrange;
            this.tlpMain.Location = new System.Drawing.Point(0, 0);
            this.tlpMain.Name = "tlpMain";
            this.tlpMain.RowCount = 4;
            this.tlpMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 78F));
            this.tlpMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 33F));
            this.tlpMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 500F));
            this.tlpMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tlpMain.Size = new System.Drawing.Size(811, 667);
            this.tlpMain.TabIndex = 0;
            // 
            // gbTop
            // 
            this.gbTop.Controls.Add(this.lbTitle);
            this.gbTop.Controls.Add(this.lbVersion);
            this.gbTop.Controls.Add(this.pbLogo);
            this.gbTop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbTop.Location = new System.Drawing.Point(3, 3);
            this.gbTop.Name = "gbTop";
            this.gbTop.Size = new System.Drawing.Size(805, 72);
            this.gbTop.TabIndex = 0;
            this.gbTop.TabStop = false;
            // 
            // lbTitle
            // 
            this.lbTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbTitle.ForeColor = System.Drawing.Color.DarkOrange;
            this.lbTitle.Location = new System.Drawing.Point(4, 10);
            this.lbTitle.Name = "lbTitle";
            this.lbTitle.Size = new System.Drawing.Size(461, 36);
            this.lbTitle.TabIndex = 14;
            this.lbTitle.Text = "Menumate Services Configuration";
            // 
            // lbVersion
            // 
            this.lbVersion.AutoSize = true;
            this.lbVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbVersion.ForeColor = System.Drawing.Color.DarkGray;
            this.lbVersion.Location = new System.Drawing.Point(7, 49);
            this.lbVersion.Name = "lbVersion";
            this.lbVersion.Size = new System.Drawing.Size(61, 16);
            this.lbVersion.TabIndex = 13;
            this.lbVersion.Text = "Version";
            // 
            // pbLogo
            // 
            this.pbLogo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pbLogo.Dock = System.Windows.Forms.DockStyle.Right;
            this.pbLogo.Image = ((System.Drawing.Image)(resources.GetObject("pbLogo.Image")));
            this.pbLogo.Location = new System.Drawing.Point(586, 18);
            this.pbLogo.Name = "pbLogo";
            this.pbLogo.Size = new System.Drawing.Size(216, 51);
            this.pbLogo.TabIndex = 2;
            this.pbLogo.TabStop = false;
            // 
            // pMenu
            // 
            this.pMenu.Controls.Add(this.btnSave);
            this.pMenu.Controls.Add(this.btnClose);
            this.pMenu.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pMenu.Location = new System.Drawing.Point(3, 81);
            this.pMenu.Name = "pMenu";
            this.pMenu.Size = new System.Drawing.Size(805, 27);
            this.pMenu.TabIndex = 2;
            // 
            // btnSave
            // 
            this.btnSave.BackColor = System.Drawing.Color.ForestGreen;
            this.btnSave.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSave.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSave.ForeColor = System.Drawing.Color.White;
            this.btnSave.Location = new System.Drawing.Point(614, 0);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(86, 26);
            this.btnSave.TabIndex = 18;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = false;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnClose
            // 
            this.btnClose.BackColor = System.Drawing.Color.Maroon;
            this.btnClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnClose.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClose.ForeColor = System.Drawing.Color.White;
            this.btnClose.Location = new System.Drawing.Point(715, 0);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(86, 26);
            this.btnClose.TabIndex = 17;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = false;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // pnBottom
            // 
            this.pnBottom.Controls.Add(this.lbWarnning);
            this.pnBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnBottom.Location = new System.Drawing.Point(3, 614);
            this.pnBottom.Name = "pnBottom";
            this.pnBottom.Size = new System.Drawing.Size(805, 50);
            this.pnBottom.TabIndex = 3;
            // 
            // lbWarnning
            // 
            this.lbWarnning.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbWarnning.ForeColor = System.Drawing.Color.Black;
            this.lbWarnning.Location = new System.Drawing.Point(7, 7);
            this.lbWarnning.Name = "lbWarnning";
            this.lbWarnning.Size = new System.Drawing.Size(789, 34);
            this.lbWarnning.TabIndex = 0;
            this.lbWarnning.Text = "Settings must be saved before closing the application \r\nMenumate Services will be" +
                " reset after saving the settings";
            this.lbWarnning.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbSetting
            // 
            this.tbSetting.Controls.Add(this.tbpDatabase);
            this.tbSetting.Controls.Add(this.tbpIPAddresses);
            this.tbSetting.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbSetting.Location = new System.Drawing.Point(0, 111);
            this.tbSetting.Margin = new System.Windows.Forms.Padding(0);
            this.tbSetting.Name = "tbSetting";
            this.tbSetting.Padding = new System.Drawing.Point(0, 0);
            this.tbSetting.SelectedIndex = 0;
            this.tbSetting.Size = new System.Drawing.Size(811, 500);
            this.tbSetting.TabIndex = 4;
            // 
            // tbpDatabase
            // 
            this.tbpDatabase.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.tbpDatabase.Controls.Add(this.gbDatabaseSettings);
            this.tbpDatabase.Location = new System.Drawing.Point(4, 25);
            this.tbpDatabase.Margin = new System.Windows.Forms.Padding(30);
            this.tbpDatabase.Name = "tbpDatabase";
            this.tbpDatabase.Size = new System.Drawing.Size(803, 471);
            this.tbpDatabase.TabIndex = 0;
            this.tbpDatabase.Text = "Databases";
            // 
            // gbDatabaseSettings
            // 
            this.gbDatabaseSettings.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.gbDatabaseSettings.Controls.Add(this.lbResetingMMServices);
            this.gbDatabaseSettings.Controls.Add(this.label4);
            this.gbDatabaseSettings.Controls.Add(this.lbUniqueDBServerDescription);
            this.gbDatabaseSettings.Controls.Add(this.lbDBSettingsDescription);
            this.gbDatabaseSettings.Controls.Add(this.btnWebmateDBFolderBrowse);
            this.gbDatabaseSettings.Controls.Add(this.btnUniqueDBFolderBrowse);
            this.gbDatabaseSettings.Controls.Add(this.label5);
            this.gbDatabaseSettings.Controls.Add(this.label6);
            this.gbDatabaseSettings.Controls.Add(this.lbWebmate);
            this.gbDatabaseSettings.Controls.Add(this.rbDatabaseServers);
            this.gbDatabaseSettings.Controls.Add(this.label2);
            this.gbDatabaseSettings.Controls.Add(this.tbDBServerWebmateFolder);
            this.gbDatabaseSettings.Controls.Add(this.label3);
            this.gbDatabaseSettings.Controls.Add(this.tbDBServerWebmateName);
            this.gbDatabaseSettings.Controls.Add(this.rbUniqueDBserver);
            this.gbDatabaseSettings.Controls.Add(this.label1);
            this.gbDatabaseSettings.Controls.Add(this.tbUniqueDBserverFolder);
            this.gbDatabaseSettings.Controls.Add(this.lbUniqueDBServer);
            this.gbDatabaseSettings.Controls.Add(this.tbUniqueDBserverName);
            this.gbDatabaseSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbDatabaseSettings.ForeColor = System.Drawing.Color.Black;
            this.gbDatabaseSettings.Location = new System.Drawing.Point(0, 0);
            this.gbDatabaseSettings.Name = "gbDatabaseSettings";
            this.gbDatabaseSettings.Size = new System.Drawing.Size(803, 471);
            this.gbDatabaseSettings.TabIndex = 2;
            this.gbDatabaseSettings.TabStop = false;
            // 
            // lbResetingMMServices
            // 
            this.lbResetingMMServices.BackColor = System.Drawing.Color.Cornsilk;
            this.lbResetingMMServices.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lbResetingMMServices.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbResetingMMServices.Location = new System.Drawing.Point(196, 192);
            this.lbResetingMMServices.Name = "lbResetingMMServices";
            this.lbResetingMMServices.Size = new System.Drawing.Size(499, 47);
            this.lbResetingMMServices.TabIndex = 20;
            this.lbResetingMMServices.Text = "Reseting Menumate Services\r\nThis will take a few seconds...";
            this.lbResetingMMServices.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lbResetingMMServices.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.ForeColor = System.Drawing.Color.DimGray;
            this.label4.Location = new System.Drawing.Point(28, 224);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(287, 16);
            this.label4.TabIndex = 24;
            this.label4.Text = "All databases are located in different places";
            // 
            // lbUniqueDBServerDescription
            // 
            this.lbUniqueDBServerDescription.AutoSize = true;
            this.lbUniqueDBServerDescription.ForeColor = System.Drawing.Color.DimGray;
            this.lbUniqueDBServerDescription.Location = new System.Drawing.Point(28, 94);
            this.lbUniqueDBServerDescription.Name = "lbUniqueDBServerDescription";
            this.lbUniqueDBServerDescription.Size = new System.Drawing.Size(286, 16);
            this.lbUniqueDBServerDescription.TabIndex = 23;
            this.lbUniqueDBServerDescription.Text = "All databases are located in the same place";
            // 
            // lbDBSettingsDescription
            // 
            this.lbDBSettingsDescription.BackColor = System.Drawing.Color.Gainsboro;
            this.lbDBSettingsDescription.Dock = System.Windows.Forms.DockStyle.Top;
            this.lbDBSettingsDescription.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbDBSettingsDescription.ForeColor = System.Drawing.Color.Gray;
            this.lbDBSettingsDescription.Location = new System.Drawing.Point(3, 18);
            this.lbDBSettingsDescription.Name = "lbDBSettingsDescription";
            this.lbDBSettingsDescription.Size = new System.Drawing.Size(797, 49);
            this.lbDBSettingsDescription.TabIndex = 22;
            this.lbDBSettingsDescription.Text = "Webmate services database location\r\nSet both the server name or IP a" +
                "ddress and the folder where the databases are located";
            this.lbDBSettingsDescription.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnWebmateDBFolderBrowse
            // 
            this.btnWebmateDBFolderBrowse.BackColor = System.Drawing.Color.DimGray;
            this.btnWebmateDBFolderBrowse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnWebmateDBFolderBrowse.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnWebmateDBFolderBrowse.ForeColor = System.Drawing.Color.White;
            this.btnWebmateDBFolderBrowse.Location = new System.Drawing.Point(706, 310);
            this.btnWebmateDBFolderBrowse.Name = "btnWebmateDBFolderBrowse";
            this.btnWebmateDBFolderBrowse.Size = new System.Drawing.Size(83, 23);
            this.btnWebmateDBFolderBrowse.TabIndex = 18;
            this.btnWebmateDBFolderBrowse.Text = "Browse ...";
            this.btnWebmateDBFolderBrowse.UseVisualStyleBackColor = false;
            // 
            // btnUniqueDBFolderBrowse
            // 
            this.btnUniqueDBFolderBrowse.BackColor = System.Drawing.Color.DimGray;
            this.btnUniqueDBFolderBrowse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnUniqueDBFolderBrowse.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnUniqueDBFolderBrowse.ForeColor = System.Drawing.Color.White;
            this.btnUniqueDBFolderBrowse.Location = new System.Drawing.Point(706, 152);
            this.btnUniqueDBFolderBrowse.Name = "btnUniqueDBFolderBrowse";
            this.btnUniqueDBFolderBrowse.Size = new System.Drawing.Size(83, 23);
            this.btnUniqueDBFolderBrowse.TabIndex = 17;
            this.btnUniqueDBFolderBrowse.Text = "Browse ...";
            this.btnUniqueDBFolderBrowse.UseVisualStyleBackColor = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.ForeColor = System.Drawing.Color.DarkGray;
            this.label5.Location = new System.Drawing.Point(42, 402);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(108, 16);
            this.label5.TabIndex = 15;
            this.label5.Text = "Database folder";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.ForeColor = System.Drawing.Color.DarkGray;
            this.label6.Location = new System.Drawing.Point(42, 372);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(153, 16);
            this.label6.TabIndex = 13;
            this.label6.Text = "Host name / IP address";
            // 
            // lbWebmate
            // 
            this.lbWebmate.AutoSize = true;
            this.lbWebmate.ForeColor = System.Drawing.Color.DarkOrange;
            this.lbWebmate.Location = new System.Drawing.Point(42, 255);
            this.lbWebmate.Name = "lbWebmate";
            this.lbWebmate.Size = new System.Drawing.Size(69, 16);
            this.lbWebmate.TabIndex = 11;
            this.lbWebmate.Text = "Webmate";
            // 
            // rbDatabaseServers
            // 
            this.rbDatabaseServers.AutoSize = true;
            this.rbDatabaseServers.Location = new System.Drawing.Point(12, 205);
            this.rbDatabaseServers.Name = "rbDatabaseServers";
            this.rbDatabaseServers.Size = new System.Drawing.Size(134, 20);
            this.rbDatabaseServers.TabIndex = 10;
            this.rbDatabaseServers.TabStop = true;
            this.rbDatabaseServers.Text = "Database servers";
            this.rbDatabaseServers.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.DarkGray;
            this.label2.Location = new System.Drawing.Point(42, 312);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(108, 16);
            this.label2.TabIndex = 9;
            this.label2.Text = "Database folder";
            // 
            // tbDBServerWebmateFolder
            // 
            this.tbDBServerWebmateFolder.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbDBServerWebmateFolder.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbDBServerWebmateFolder.Location = new System.Drawing.Point(198, 310);
            this.tbDBServerWebmateFolder.Name = "tbDBServerWebmateFolder";
            this.tbDBServerWebmateFolder.Size = new System.Drawing.Size(496, 22);
            this.tbDBServerWebmateFolder.TabIndex = 8;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.DarkGray;
            this.label3.Location = new System.Drawing.Point(42, 282);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(153, 16);
            this.label3.TabIndex = 7;
            this.label3.Text = "Host name / IP address";
            // 
            // tbDBServerWebmateName
            // 
            this.tbDBServerWebmateName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbDBServerWebmateName.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbDBServerWebmateName.Location = new System.Drawing.Point(198, 279);
            this.tbDBServerWebmateName.Name = "tbDBServerWebmateName";
            this.tbDBServerWebmateName.Size = new System.Drawing.Size(496, 22);
            this.tbDBServerWebmateName.TabIndex = 6;
            // 
            // rbUniqueDBserver
            // 
            this.rbUniqueDBserver.AutoSize = true;
            this.rbUniqueDBserver.Location = new System.Drawing.Point(12, 74);
            this.rbUniqueDBserver.Name = "rbUniqueDBserver";
            this.rbUniqueDBserver.Size = new System.Drawing.Size(176, 20);
            this.rbUniqueDBserver.TabIndex = 5;
            this.rbUniqueDBserver.TabStop = true;
            this.rbUniqueDBserver.Text = "Unique database server";
            this.rbUniqueDBserver.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.DarkGray;
            this.label1.Location = new System.Drawing.Point(42, 155);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(108, 16);
            this.label1.TabIndex = 4;
            this.label1.Text = "Database folder";
            // 
            // tbUniqueDBserverFolder
            // 
            this.tbUniqueDBserverFolder.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbUniqueDBserverFolder.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbUniqueDBserverFolder.Location = new System.Drawing.Point(198, 153);
            this.tbUniqueDBserverFolder.Name = "tbUniqueDBserverFolder";
            this.tbUniqueDBserverFolder.Size = new System.Drawing.Size(496, 22);
            this.tbUniqueDBserverFolder.TabIndex = 3;
            // 
            // lbUniqueDBServer
            // 
            this.lbUniqueDBServer.AutoSize = true;
            this.lbUniqueDBServer.ForeColor = System.Drawing.Color.DarkGray;
            this.lbUniqueDBServer.Location = new System.Drawing.Point(42, 125);
            this.lbUniqueDBServer.Name = "lbUniqueDBServer";
            this.lbUniqueDBServer.Size = new System.Drawing.Size(153, 16);
            this.lbUniqueDBServer.TabIndex = 2;
            this.lbUniqueDBServer.Text = "Host name / IP address";
            // 
            // tbUniqueDBserverName
            // 
            this.tbUniqueDBserverName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbUniqueDBserverName.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbUniqueDBserverName.Location = new System.Drawing.Point(198, 123);
            this.tbUniqueDBserverName.Name = "tbUniqueDBserverName";
            this.tbUniqueDBserverName.Size = new System.Drawing.Size(496, 22);
            this.tbUniqueDBserverName.TabIndex = 1;
            // 
            // tbpIPAddresses
            // 
            this.tbpIPAddresses.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.tbpIPAddresses.Controls.Add(this.gbIpAddressesSettings);
            this.tbpIPAddresses.Location = new System.Drawing.Point(4, 25);
            this.tbpIPAddresses.Margin = new System.Windows.Forms.Padding(0);
            this.tbpIPAddresses.Name = "tbpIPAddresses";
            this.tbpIPAddresses.Size = new System.Drawing.Size(803, 471);
            this.tbpIPAddresses.TabIndex = 1;
            this.tbpIPAddresses.Text = "IP Addresses";
            // 
            // gbIpAddressesSettings
            // 
            this.gbIpAddressesSettings.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.gbIpAddressesSettings.Controls.Add(this.lbIpAddress);
            this.gbIpAddressesSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbIpAddressesSettings.ForeColor = System.Drawing.Color.Black;
            this.gbIpAddressesSettings.Location = new System.Drawing.Point(0, 0);
            this.gbIpAddressesSettings.Name = "gbIpAddressesSettings";
            this.gbIpAddressesSettings.Size = new System.Drawing.Size(803, 471);
            this.gbIpAddressesSettings.TabIndex = 2;
            this.gbIpAddressesSettings.TabStop = false;
            // 
            // lbIpAddress
            // 
            this.lbIpAddress.AutoSize = true;
            this.lbIpAddress.ForeColor = System.Drawing.Color.DarkGray;
            this.lbIpAddress.Location = new System.Drawing.Point(42, 40);
            this.lbIpAddress.Name = "lbIpAddress";
            this.lbIpAddress.Size = new System.Drawing.Size(75, 16);
            this.lbIpAddress.TabIndex = 2;
            this.lbIpAddress.Text = "IP Address";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(811, 667);
            this.Controls.Add(this.tlpMain);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Menumate Services Configuration";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.tlpMain.ResumeLayout(false);
            this.gbTop.ResumeLayout(false);
            this.gbTop.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).EndInit();
            this.pMenu.ResumeLayout(false);
            this.pnBottom.ResumeLayout(false);
            this.tbSetting.ResumeLayout(false);
            this.tbpDatabase.ResumeLayout(false);
            this.gbDatabaseSettings.ResumeLayout(false);
            this.gbDatabaseSettings.PerformLayout();
            this.tbpIPAddresses.ResumeLayout(false);
            this.gbIpAddressesSettings.ResumeLayout(false);
            this.gbIpAddressesSettings.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tlpMain;
        private System.Windows.Forms.GroupBox gbTop;
        private System.Windows.Forms.PictureBox pbLogo;
        private System.Windows.Forms.Label lbTitle;
        private System.Windows.Forms.Label lbVersion;
        private System.Windows.Forms.Panel pMenu;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Panel pnBottom;
        private System.Windows.Forms.Label lbWarnning;
        private System.Windows.Forms.FolderBrowserDialog fbdDatabase;
        private System.Windows.Forms.TabControl tbSetting;
        private System.Windows.Forms.TabPage tbpDatabase;
        private System.Windows.Forms.TabPage tbpIPAddresses;
        private System.Windows.Forms.GroupBox gbDatabaseSettings;
        private System.Windows.Forms.Label lbResetingMMServices;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lbUniqueDBServerDescription;
        private System.Windows.Forms.Label lbDBSettingsDescription;
        private System.Windows.Forms.Button btnWebmateDBFolderBrowse;
        private System.Windows.Forms.Button btnUniqueDBFolderBrowse;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label lbWebmate;
        private System.Windows.Forms.RadioButton rbDatabaseServers;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbDBServerWebmateFolder;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbDBServerWebmateName;
        private System.Windows.Forms.RadioButton rbUniqueDBserver;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbUniqueDBserverFolder;
        private System.Windows.Forms.Label lbUniqueDBServer;
        private System.Windows.Forms.TextBox tbUniqueDBserverName;
        private System.Windows.Forms.GroupBox gbIpAddressesSettings;
        private System.Windows.Forms.Label lbIpAddress;
    }
}

