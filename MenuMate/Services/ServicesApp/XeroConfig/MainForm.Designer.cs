namespace XeroConfig
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
            this.pMainDescription = new System.Windows.Forms.Panel();
            this.gbMainDescription = new System.Windows.Forms.GroupBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.lbTitle = new System.Windows.Forms.Label();
            this.lbVersion = new System.Windows.Forms.Label();
            this.lbMainDescription = new System.Windows.Forms.Label();
            this.btnClose = new System.Windows.Forms.Button();
            this.pLogo = new System.Windows.Forms.Panel();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnAddCompany = new System.Windows.Forms.Button();
            this.tlpMain = new System.Windows.Forms.TableLayoutPanel();
            this.pConfig = new System.Windows.Forms.Panel();
            this.tlpCompanies = new System.Windows.Forms.TableLayoutPanel();
            this.pCompanies = new System.Windows.Forms.Panel();
            this.gbCompanies = new System.Windows.Forms.GroupBox();
            this.pTVCompanies = new System.Windows.Forms.Panel();
            this.tvCompanies = new System.Windows.Forms.TreeView();
            this.pEditCompany = new System.Windows.Forms.Panel();
            this.btnDelCompany = new System.Windows.Forms.Button();
            this.btnEditCompany = new System.Windows.Forms.Button();
            this.pNewCompany = new System.Windows.Forms.Panel();
            this.tlpConfig = new System.Windows.Forms.TableLayoutPanel();
            this.gbCompanyName = new System.Windows.Forms.GroupBox();
            this.tbCompanyName = new System.Windows.Forms.TextBox();
            this.pCompany = new System.Windows.Forms.Panel();
            this.lbCompany = new System.Windows.Forms.Label();
            this.gbConsumerSecret = new System.Windows.Forms.GroupBox();
            this.tbConsumerSecret = new System.Windows.Forms.TextBox();
            this.gbConsumerKey = new System.Windows.Forms.GroupBox();
            this.tbConsumerKey = new System.Windows.Forms.TextBox();
            this.pBottom = new System.Windows.Forms.Panel();
            this.lbRestartService = new System.Windows.Forms.Label();
            this.lbRestartingSvc = new System.Windows.Forms.Label();
            this.btnRestartService = new System.Windows.Forms.Button();
            this.pAppInUsed = new System.Windows.Forms.Panel();
            this.cbAppType = new System.Windows.Forms.CheckBox();
            this.cbAppInUsed = new System.Windows.Forms.CheckBox();
            this.pMainDescription.SuspendLayout();
            this.gbMainDescription.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.pLogo.SuspendLayout();
            this.tlpMain.SuspendLayout();
            this.pConfig.SuspendLayout();
            this.tlpCompanies.SuspendLayout();
            this.pCompanies.SuspendLayout();
            this.gbCompanies.SuspendLayout();
            this.pTVCompanies.SuspendLayout();
            this.pEditCompany.SuspendLayout();
            this.pNewCompany.SuspendLayout();
            this.tlpConfig.SuspendLayout();
            this.gbCompanyName.SuspendLayout();
            this.pCompany.SuspendLayout();
            this.gbConsumerSecret.SuspendLayout();
            this.gbConsumerKey.SuspendLayout();
            this.pBottom.SuspendLayout();
            this.pAppInUsed.SuspendLayout();
            this.SuspendLayout();
            // 
            // pMainDescription
            // 
            this.pMainDescription.Controls.Add(this.gbMainDescription);
            this.pMainDescription.Controls.Add(this.lbMainDescription);
            this.pMainDescription.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pMainDescription.Location = new System.Drawing.Point(3, 3);
            this.pMainDescription.Name = "pMainDescription";
            this.pMainDescription.Size = new System.Drawing.Size(787, 68);
            this.pMainDescription.TabIndex = 5;
            // 
            // gbMainDescription
            // 
            this.gbMainDescription.BackColor = System.Drawing.Color.White;
            this.gbMainDescription.Controls.Add(this.pictureBox1);
            this.gbMainDescription.Controls.Add(this.lbTitle);
            this.gbMainDescription.Controls.Add(this.lbVersion);
            this.gbMainDescription.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbMainDescription.Location = new System.Drawing.Point(0, 0);
            this.gbMainDescription.Name = "gbMainDescription";
            this.gbMainDescription.Size = new System.Drawing.Size(787, 68);
            this.gbMainDescription.TabIndex = 10;
            this.gbMainDescription.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::XeroConfig.Properties.Resources.menumate_logo_shifted;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Right;
            this.pictureBox1.Location = new System.Drawing.Point(568, 16);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(216, 49);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // lbTitle
            // 
            this.lbTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbTitle.ForeColor = System.Drawing.Color.DarkOrange;
            this.lbTitle.Location = new System.Drawing.Point(8, 11);
            this.lbTitle.Name = "lbTitle";
            this.lbTitle.Size = new System.Drawing.Size(358, 36);
            this.lbTitle.TabIndex = 12;
            this.lbTitle.Text = "XeroMate Configuration";
            // 
            // lbVersion
            // 
            this.lbVersion.AutoSize = true;
            this.lbVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbVersion.ForeColor = System.Drawing.Color.DarkGray;
            this.lbVersion.Location = new System.Drawing.Point(11, 47);
            this.lbVersion.Name = "lbVersion";
            this.lbVersion.Size = new System.Drawing.Size(61, 16);
            this.lbVersion.TabIndex = 9;
            this.lbVersion.Text = "Version";
            // 
            // lbMainDescription
            // 
            this.lbMainDescription.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbMainDescription.ForeColor = System.Drawing.Color.Black;
            this.lbMainDescription.Location = new System.Drawing.Point(234, 6);
            this.lbMainDescription.Name = "lbMainDescription";
            this.lbMainDescription.Size = new System.Drawing.Size(401, 71);
            this.lbMainDescription.TabIndex = 0;
            // 
            // btnClose
            // 
            this.btnClose.BackColor = System.Drawing.Color.Maroon;
            this.btnClose.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClose.ForeColor = System.Drawing.Color.White;
            this.btnClose.Location = new System.Drawing.Point(384, 7);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(87, 47);
            this.btnClose.TabIndex = 8;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = false;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // pLogo
            // 
            this.pLogo.BackColor = System.Drawing.Color.White;
            this.pLogo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pLogo.Controls.Add(this.btnSave);
            this.pLogo.Controls.Add(this.btnClose);
            this.pLogo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pLogo.Location = new System.Drawing.Point(3, 424);
            this.pLogo.Name = "pLogo";
            this.pLogo.Size = new System.Drawing.Size(479, 58);
            this.pLogo.TabIndex = 2;
            // 
            // btnSave
            // 
            this.btnSave.BackColor = System.Drawing.Color.DarkOrange;
            this.btnSave.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSave.ForeColor = System.Drawing.Color.White;
            this.btnSave.Location = new System.Drawing.Point(15, 5);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(86, 47);
            this.btnSave.TabIndex = 1;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = false;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnAddCompany
            // 
            this.btnAddCompany.BackColor = System.Drawing.Color.DimGray;
            this.btnAddCompany.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddCompany.ForeColor = System.Drawing.Color.White;
            this.btnAddCompany.Location = new System.Drawing.Point(85, 0);
            this.btnAddCompany.Name = "btnAddCompany";
            this.btnAddCompany.Size = new System.Drawing.Size(118, 47);
            this.btnAddCompany.TabIndex = 1;
            this.btnAddCompany.Text = "New Organisation";
            this.btnAddCompany.UseVisualStyleBackColor = false;
            this.btnAddCompany.Click += new System.EventHandler(this.btnAddCompany_Click);
            // 
            // tlpMain
            // 
            this.tlpMain.BackColor = System.Drawing.Color.White;
            this.tlpMain.ColumnCount = 1;
            this.tlpMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpMain.Controls.Add(this.pMainDescription, 0, 0);
            this.tlpMain.Controls.Add(this.pConfig, 0, 1);
            this.tlpMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tlpMain.Location = new System.Drawing.Point(0, 0);
            this.tlpMain.Name = "tlpMain";
            this.tlpMain.RowCount = 2;
            this.tlpMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 74F));
            this.tlpMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpMain.Size = new System.Drawing.Size(793, 565);
            this.tlpMain.TabIndex = 6;
            // 
            // pConfig
            // 
            this.pConfig.Controls.Add(this.tlpCompanies);
            this.pConfig.Controls.Add(this.tlpConfig);
            this.pConfig.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pConfig.Location = new System.Drawing.Point(3, 77);
            this.pConfig.Name = "pConfig";
            this.pConfig.Size = new System.Drawing.Size(787, 485);
            this.pConfig.TabIndex = 6;
            // 
            // tlpCompanies
            // 
            this.tlpCompanies.ColumnCount = 1;
            this.tlpCompanies.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpCompanies.Controls.Add(this.pCompanies, 0, 0);
            this.tlpCompanies.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tlpCompanies.Location = new System.Drawing.Point(0, 0);
            this.tlpCompanies.Name = "tlpCompanies";
            this.tlpCompanies.RowCount = 1;
            this.tlpCompanies.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpCompanies.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tlpCompanies.Size = new System.Drawing.Size(302, 485);
            this.tlpCompanies.TabIndex = 2;
            // 
            // pCompanies
            // 
            this.pCompanies.BackColor = System.Drawing.Color.White;
            this.pCompanies.Controls.Add(this.gbCompanies);
            this.pCompanies.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pCompanies.Location = new System.Drawing.Point(3, 3);
            this.pCompanies.Name = "pCompanies";
            this.pCompanies.Size = new System.Drawing.Size(296, 479);
            this.pCompanies.TabIndex = 1;
            // 
            // gbCompanies
            // 
            this.gbCompanies.Controls.Add(this.pTVCompanies);
            this.gbCompanies.Controls.Add(this.pEditCompany);
            this.gbCompanies.Controls.Add(this.pNewCompany);
            this.gbCompanies.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbCompanies.Location = new System.Drawing.Point(0, 0);
            this.gbCompanies.Name = "gbCompanies";
            this.gbCompanies.Size = new System.Drawing.Size(296, 479);
            this.gbCompanies.TabIndex = 2;
            this.gbCompanies.TabStop = false;
            // 
            // pTVCompanies
            // 
            this.pTVCompanies.Controls.Add(this.tvCompanies);
            this.pTVCompanies.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pTVCompanies.Location = new System.Drawing.Point(3, 71);
            this.pTVCompanies.Name = "pTVCompanies";
            this.pTVCompanies.Size = new System.Drawing.Size(290, 348);
            this.pTVCompanies.TabIndex = 2;
            // 
            // tvCompanies
            // 
            this.tvCompanies.BackColor = System.Drawing.Color.White;
            this.tvCompanies.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tvCompanies.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvCompanies.HideSelection = false;
            this.tvCompanies.Location = new System.Drawing.Point(0, 0);
            this.tvCompanies.Name = "tvCompanies";
            this.tvCompanies.Size = new System.Drawing.Size(290, 348);
            this.tvCompanies.TabIndex = 0;
            // 
            // pEditCompany
            // 
            this.pEditCompany.BackColor = System.Drawing.Color.White;
            this.pEditCompany.Controls.Add(this.btnDelCompany);
            this.pEditCompany.Controls.Add(this.btnEditCompany);
            this.pEditCompany.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pEditCompany.Location = new System.Drawing.Point(3, 419);
            this.pEditCompany.Name = "pEditCompany";
            this.pEditCompany.Size = new System.Drawing.Size(290, 57);
            this.pEditCompany.TabIndex = 0;
            // 
            // btnDelCompany
            // 
            this.btnDelCompany.BackColor = System.Drawing.Color.DimGray;
            this.btnDelCompany.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDelCompany.ForeColor = System.Drawing.Color.White;
            this.btnDelCompany.Location = new System.Drawing.Point(145, 6);
            this.btnDelCompany.Name = "btnDelCompany";
            this.btnDelCompany.Size = new System.Drawing.Size(115, 47);
            this.btnDelCompany.TabIndex = 4;
            this.btnDelCompany.Text = "Delete Organisation";
            this.btnDelCompany.UseVisualStyleBackColor = false;
            this.btnDelCompany.Click += new System.EventHandler(this.btnDelCompany_Click);
            // 
            // btnEditCompany
            // 
            this.btnEditCompany.BackColor = System.Drawing.Color.DimGray;
            this.btnEditCompany.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnEditCompany.ForeColor = System.Drawing.Color.White;
            this.btnEditCompany.Location = new System.Drawing.Point(26, 6);
            this.btnEditCompany.Name = "btnEditCompany";
            this.btnEditCompany.Size = new System.Drawing.Size(115, 47);
            this.btnEditCompany.TabIndex = 3;
            this.btnEditCompany.Text = "View/Edit Organisation";
            this.btnEditCompany.UseVisualStyleBackColor = false;
            this.btnEditCompany.Click += new System.EventHandler(this.btnEditCompany_Click);
            // 
            // pNewCompany
            // 
            this.pNewCompany.Controls.Add(this.btnAddCompany);
            this.pNewCompany.Dock = System.Windows.Forms.DockStyle.Top;
            this.pNewCompany.Location = new System.Drawing.Point(3, 18);
            this.pNewCompany.Name = "pNewCompany";
            this.pNewCompany.Size = new System.Drawing.Size(290, 53);
            this.pNewCompany.TabIndex = 1;
            // 
            // tlpConfig
            // 
            this.tlpConfig.ColumnCount = 1;
            this.tlpConfig.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpConfig.Controls.Add(this.gbCompanyName, 0, 0);
            this.tlpConfig.Controls.Add(this.pCompany, 0, 4);
            this.tlpConfig.Controls.Add(this.gbConsumerSecret, 0, 2);
            this.tlpConfig.Controls.Add(this.pLogo, 0, 6);
            this.tlpConfig.Controls.Add(this.gbConsumerKey, 0, 1);
            this.tlpConfig.Controls.Add(this.pBottom, 0, 5);
            this.tlpConfig.Controls.Add(this.pAppInUsed, 0, 3);
            this.tlpConfig.Dock = System.Windows.Forms.DockStyle.Right;
            this.tlpConfig.Location = new System.Drawing.Point(302, 0);
            this.tlpConfig.Name = "tlpConfig";
            this.tlpConfig.RowCount = 7;
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 74F));
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 74F));
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 74F));
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 74F));
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tlpConfig.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tlpConfig.Size = new System.Drawing.Size(485, 485);
            this.tlpConfig.TabIndex = 1;
            // 
            // gbCompanyName
            // 
            this.gbCompanyName.BackColor = System.Drawing.SystemColors.Control;
            this.gbCompanyName.Controls.Add(this.tbCompanyName);
            this.gbCompanyName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbCompanyName.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbCompanyName.ForeColor = System.Drawing.Color.DarkGray;
            this.gbCompanyName.Location = new System.Drawing.Point(4, 4);
            this.gbCompanyName.Margin = new System.Windows.Forms.Padding(4);
            this.gbCompanyName.Name = "gbCompanyName";
            this.gbCompanyName.Padding = new System.Windows.Forms.Padding(4);
            this.gbCompanyName.Size = new System.Drawing.Size(477, 66);
            this.gbCompanyName.TabIndex = 9;
            this.gbCompanyName.TabStop = false;
            this.gbCompanyName.Text = " Xero Organisation Name ";
            // 
            // tbCompanyName
            // 
            this.tbCompanyName.Location = new System.Drawing.Point(8, 27);
            this.tbCompanyName.Margin = new System.Windows.Forms.Padding(4);
            this.tbCompanyName.Name = "tbCompanyName";
            this.tbCompanyName.Size = new System.Drawing.Size(462, 26);
            this.tbCompanyName.TabIndex = 0;
            this.tbCompanyName.TextChanged += new System.EventHandler(this.ConfigurationChanged);
            // 
            // pCompany
            // 
            this.pCompany.BackColor = System.Drawing.Color.White;
            this.pCompany.Controls.Add(this.lbCompany);
            this.pCompany.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pCompany.Location = new System.Drawing.Point(3, 260);
            this.pCompany.Name = "pCompany";
            this.pCompany.Size = new System.Drawing.Size(479, 68);
            this.pCompany.TabIndex = 6;
            // 
            // lbCompany
            // 
            this.lbCompany.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbCompany.ForeColor = System.Drawing.Color.Black;
            this.lbCompany.Location = new System.Drawing.Point(3, 6);
            this.lbCompany.Name = "lbCompany";
            this.lbCompany.Size = new System.Drawing.Size(468, 51);
            this.lbCompany.TabIndex = 0;
            this.lbCompany.Text = "The Consumer Key and the Consumer Secret are both alphanumeric and 30 character i" +
                "n length.  To get them just go to appi.xero.com (My Applications tab)";
            this.lbCompany.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // gbConsumerSecret
            // 
            this.gbConsumerSecret.BackColor = System.Drawing.SystemColors.Control;
            this.gbConsumerSecret.Controls.Add(this.tbConsumerSecret);
            this.gbConsumerSecret.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbConsumerSecret.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbConsumerSecret.ForeColor = System.Drawing.Color.DarkGray;
            this.gbConsumerSecret.Location = new System.Drawing.Point(4, 152);
            this.gbConsumerSecret.Margin = new System.Windows.Forms.Padding(4);
            this.gbConsumerSecret.Name = "gbConsumerSecret";
            this.gbConsumerSecret.Padding = new System.Windows.Forms.Padding(4);
            this.gbConsumerSecret.Size = new System.Drawing.Size(477, 66);
            this.gbConsumerSecret.TabIndex = 2;
            this.gbConsumerSecret.TabStop = false;
            this.gbConsumerSecret.Text = "Xero  Consumer Secret";
            // 
            // tbConsumerSecret
            // 
            this.tbConsumerSecret.Location = new System.Drawing.Point(8, 31);
            this.tbConsumerSecret.Margin = new System.Windows.Forms.Padding(4);
            this.tbConsumerSecret.Name = "tbConsumerSecret";
            this.tbConsumerSecret.Size = new System.Drawing.Size(462, 26);
            this.tbConsumerSecret.TabIndex = 0;
            this.tbConsumerSecret.TextChanged += new System.EventHandler(this.ConfigurationChanged);
            // 
            // gbConsumerKey
            // 
            this.gbConsumerKey.BackColor = System.Drawing.SystemColors.Control;
            this.gbConsumerKey.Controls.Add(this.tbConsumerKey);
            this.gbConsumerKey.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbConsumerKey.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbConsumerKey.ForeColor = System.Drawing.Color.DarkGray;
            this.gbConsumerKey.Location = new System.Drawing.Point(4, 78);
            this.gbConsumerKey.Margin = new System.Windows.Forms.Padding(4);
            this.gbConsumerKey.Name = "gbConsumerKey";
            this.gbConsumerKey.Padding = new System.Windows.Forms.Padding(4);
            this.gbConsumerKey.Size = new System.Drawing.Size(477, 66);
            this.gbConsumerKey.TabIndex = 1;
            this.gbConsumerKey.TabStop = false;
            this.gbConsumerKey.Text = " Xero Consumer Key ";
            // 
            // tbConsumerKey
            // 
            this.tbConsumerKey.Location = new System.Drawing.Point(8, 27);
            this.tbConsumerKey.Margin = new System.Windows.Forms.Padding(4);
            this.tbConsumerKey.Name = "tbConsumerKey";
            this.tbConsumerKey.Size = new System.Drawing.Size(462, 26);
            this.tbConsumerKey.TabIndex = 0;
            this.tbConsumerKey.TextChanged += new System.EventHandler(this.ConfigurationChanged);
            // 
            // pBottom
            // 
            this.pBottom.BackColor = System.Drawing.Color.White;
            this.pBottom.Controls.Add(this.lbRestartService);
            this.pBottom.Controls.Add(this.lbRestartingSvc);
            this.pBottom.Controls.Add(this.btnRestartService);
            this.pBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pBottom.Location = new System.Drawing.Point(3, 334);
            this.pBottom.Name = "pBottom";
            this.pBottom.Size = new System.Drawing.Size(479, 84);
            this.pBottom.TabIndex = 7;
            // 
            // lbRestartService
            // 
            this.lbRestartService.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbRestartService.ForeColor = System.Drawing.Color.Black;
            this.lbRestartService.Location = new System.Drawing.Point(6, 38);
            this.lbRestartService.Name = "lbRestartService";
            this.lbRestartService.Size = new System.Drawing.Size(295, 37);
            this.lbRestartService.TabIndex = 4;
            this.lbRestartService.Text = "Xero Service must be restarted after any modifications.";
            this.lbRestartService.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.lbRestartService.Visible = false;
            // 
            // lbRestartingSvc
            // 
            this.lbRestartingSvc.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbRestartingSvc.Location = new System.Drawing.Point(12, 12);
            this.lbRestartingSvc.Name = "lbRestartingSvc";
            this.lbRestartingSvc.Size = new System.Drawing.Size(459, 18);
            this.lbRestartingSvc.TabIndex = 3;
            this.lbRestartingSvc.Text = "Restarting Xero Service. This will take a few seconds ...";
            this.lbRestartingSvc.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lbRestartingSvc.Visible = false;
            // 
            // btnRestartService
            // 
            this.btnRestartService.BackColor = System.Drawing.Color.DarkOrange;
            this.btnRestartService.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnRestartService.ForeColor = System.Drawing.Color.White;
            this.btnRestartService.Location = new System.Drawing.Point(304, 33);
            this.btnRestartService.Name = "btnRestartService";
            this.btnRestartService.Size = new System.Drawing.Size(171, 47);
            this.btnRestartService.TabIndex = 2;
            this.btnRestartService.Text = "Restart Xero Service";
            this.btnRestartService.UseVisualStyleBackColor = false;
            this.btnRestartService.Visible = false;
            // 
            // pAppInUsed
            // 
            this.pAppInUsed.BackColor = System.Drawing.Color.White;
            this.pAppInUsed.Controls.Add(this.cbAppType);
            this.pAppInUsed.Controls.Add(this.cbAppInUsed);
            this.pAppInUsed.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pAppInUsed.Location = new System.Drawing.Point(3, 225);
            this.pAppInUsed.Name = "pAppInUsed";
            this.pAppInUsed.Size = new System.Drawing.Size(479, 29);
            this.pAppInUsed.TabIndex = 8;
            // 
            // cbAppType
            // 
            this.cbAppType.AutoSize = true;
            this.cbAppType.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbAppType.Location = new System.Drawing.Point(205, 3);
            this.cbAppType.Name = "cbAppType";
            this.cbAppType.Size = new System.Drawing.Size(76, 20);
            this.cbAppType.TabIndex = 1;
            this.cbAppType.Text = "Private";
            this.cbAppType.UseVisualStyleBackColor = true;
            this.cbAppType.Click += new System.EventHandler(this.ConfigurationChanged);
            // 
            // cbAppInUsed
            // 
            this.cbAppInUsed.AutoSize = true;
            this.cbAppInUsed.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbAppInUsed.Location = new System.Drawing.Point(9, 3);
            this.cbAppInUsed.Name = "cbAppInUsed";
            this.cbAppInUsed.Size = new System.Drawing.Size(173, 20);
            this.cbAppInUsed.TabIndex = 0;
            this.cbAppInUsed.Text = "Use this organisation";
            this.cbAppInUsed.UseVisualStyleBackColor = true;
            this.cbAppInUsed.Click += new System.EventHandler(this.ConfigurationChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(793, 565);
            this.Controls.Add(this.tlpMain);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Xero Integration Configuration";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.pMainDescription.ResumeLayout(false);
            this.gbMainDescription.ResumeLayout(false);
            this.gbMainDescription.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.pLogo.ResumeLayout(false);
            this.tlpMain.ResumeLayout(false);
            this.pConfig.ResumeLayout(false);
            this.tlpCompanies.ResumeLayout(false);
            this.pCompanies.ResumeLayout(false);
            this.gbCompanies.ResumeLayout(false);
            this.pTVCompanies.ResumeLayout(false);
            this.pEditCompany.ResumeLayout(false);
            this.pNewCompany.ResumeLayout(false);
            this.tlpConfig.ResumeLayout(false);
            this.gbCompanyName.ResumeLayout(false);
            this.gbCompanyName.PerformLayout();
            this.pCompany.ResumeLayout(false);
            this.gbConsumerSecret.ResumeLayout(false);
            this.gbConsumerSecret.PerformLayout();
            this.gbConsumerKey.ResumeLayout(false);
            this.gbConsumerKey.PerformLayout();
            this.pBottom.ResumeLayout(false);
            this.pAppInUsed.ResumeLayout(false);
            this.pAppInUsed.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pMainDescription;
        private System.Windows.Forms.Button btnAddCompany;
        private System.Windows.Forms.Label lbMainDescription;
        private System.Windows.Forms.TableLayoutPanel tlpMain;
        private System.Windows.Forms.Panel pConfig;
        private System.Windows.Forms.TableLayoutPanel tlpConfig;
        private System.Windows.Forms.Panel pCompany;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label lbCompany;
        private System.Windows.Forms.GroupBox gbConsumerSecret;
        private System.Windows.Forms.TextBox tbConsumerSecret;
        private System.Windows.Forms.GroupBox gbConsumerKey;
        private System.Windows.Forms.TextBox tbConsumerKey;
        private System.Windows.Forms.Panel pBottom;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.TableLayoutPanel tlpCompanies;
        private System.Windows.Forms.Panel pEditCompany;
        private System.Windows.Forms.Button btnEditCompany;
        private System.Windows.Forms.Panel pCompanies;
        private System.Windows.Forms.Panel pAppInUsed;
        private System.Windows.Forms.CheckBox cbAppInUsed;
        private System.Windows.Forms.GroupBox gbCompanyName;
        private System.Windows.Forms.TextBox tbCompanyName;
        private System.Windows.Forms.CheckBox cbAppType;
        private System.Windows.Forms.Button btnDelCompany;
        private System.Windows.Forms.Panel pLogo;
        private System.Windows.Forms.Button btnRestartService;
        private System.Windows.Forms.Label lbRestartingSvc;
        private System.Windows.Forms.Label lbRestartService;
        private System.Windows.Forms.Label lbVersion;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.GroupBox gbMainDescription;
        private System.Windows.Forms.Label lbTitle;
        private System.Windows.Forms.GroupBox gbCompanies;
        private System.Windows.Forms.Panel pTVCompanies;
        private System.Windows.Forms.TreeView tvCompanies;
        private System.Windows.Forms.Panel pNewCompany;

    }
}

