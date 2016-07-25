namespace Menumate.WinServices.Client.FloorPlan
{
    partial class EditorForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditorForm));
            this.panelWork = new System.Windows.Forms.Panel();
            this.tableLayoutPanelCanvas = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanelCanvasBottom = new System.Windows.Forms.TableLayoutPanel();
            this.panelCanvasHeader = new System.Windows.Forms.Panel();
            this.labelCanvasHeaderCurrentLocation = new System.Windows.Forms.Label();
            this.textBoxCanvasHeaderCurrentLocation = new System.Windows.Forms.TextBox();
            this.labelCanvasHeaderSelectedTableName = new System.Windows.Forms.Label();
            this.textBoxCanvasHeaderSelectedTableName = new System.Windows.Forms.TextBox();
            this.panelLeft = new System.Windows.Forms.Panel();
            this.listBoxMenuAddTable = new System.Windows.Forms.ListBox();
            this.tableLayoutPanelMenu = new System.Windows.Forms.TableLayoutPanel();
            this.toolTipLocationAdd = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipLocationDel = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipLocationBckImg = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipSave = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipClose = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipCanvasHeaderCurrentLocation = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipCanvasHeaderSelectedTableName = new System.Windows.Forms.ToolTip(this.components);
            this.openFileDialogLocImg = new System.Windows.Forms.OpenFileDialog();
            this.panelCanvas = new System.Windows.Forms.Panel();
            this.panelCanvasBottomMousePos = new System.Windows.Forms.Panel();
            this.labelCanvasBottomMouseYPosValue = new System.Windows.Forms.Label();
            this.labelCanvasBottomMouseXPosValue = new System.Windows.Forms.Label();
            this.labelCanvasBottomMouseYPosTitle = new System.Windows.Forms.Label();
            this.labelCanvasBottomMouseXPosTitle = new System.Windows.Forms.Label();
            this.labelCanvasBottomMousePos = new System.Windows.Forms.Label();
            this.panelCanvasBottomCurrentTablePos = new System.Windows.Forms.Panel();
            this.labelCanvasBottomCurrentTableName = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableHeightValue = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableWidthValue = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableHeightTitle = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableWidthTitle = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableYPosValue = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableXPosValue = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableYPosTitle = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTableXPosTitle = new System.Windows.Forms.Label();
            this.labelCanvasBottomCurrentTablePos = new System.Windows.Forms.Label();
            this.panelMenuTables = new System.Windows.Forms.Panel();
            this.listBoxMenuTables = new System.Windows.Forms.ListBox();
            this.panelMenuTablesDel = new System.Windows.Forms.Panel();
            this.panelMenuTablesAdd = new System.Windows.Forms.Panel();
            this.labelMenuTables = new System.Windows.Forms.Label();
            this.panelMenuLogo = new System.Windows.Forms.Panel();
            this.panelMenuLogoImg = new System.Windows.Forms.Panel();
            this.labeMenulVersion = new System.Windows.Forms.Label();
            this.linkLabelMenuLogo = new System.Windows.Forms.LinkLabel();
            this.panelMenuLocations = new System.Windows.Forms.Panel();
            this.listBoxMenuLocations = new System.Windows.Forms.ListBox();
            this.panelMenuLocationsBckImg = new System.Windows.Forms.Panel();
            this.panelMenuLocationsDel = new System.Windows.Forms.Panel();
            this.panelMenuLocationsAdd = new System.Windows.Forms.Panel();
            this.labelMenuLocations = new System.Windows.Forms.Label();
            this.panelMenuEdit = new System.Windows.Forms.Panel();
            this.linkLabelMenuSave = new System.Windows.Forms.LinkLabel();
            this.panelMenuSave = new System.Windows.Forms.Panel();
            this.linkLabelMenuClose = new System.Windows.Forms.LinkLabel();
            this.panelMenuClose = new System.Windows.Forms.Panel();
            this.panelWork.SuspendLayout();
            this.tableLayoutPanelCanvas.SuspendLayout();
            this.tableLayoutPanelCanvasBottom.SuspendLayout();
            this.panelCanvasHeader.SuspendLayout();
            this.panelLeft.SuspendLayout();
            this.tableLayoutPanelMenu.SuspendLayout();
            this.panelCanvasBottomMousePos.SuspendLayout();
            this.panelCanvasBottomCurrentTablePos.SuspendLayout();
            this.panelMenuTables.SuspendLayout();
            this.panelMenuLogo.SuspendLayout();
            this.panelMenuLocations.SuspendLayout();
            this.panelMenuEdit.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelWork
            // 
            this.panelWork.Controls.Add(this.tableLayoutPanelCanvas);
            this.panelWork.Controls.Add(this.panelLeft);
            this.panelWork.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelWork.Location = new System.Drawing.Point(0, 0);
            this.panelWork.Name = "panelWork";
            this.panelWork.Size = new System.Drawing.Size(800, 600);
            this.panelWork.TabIndex = 0;
            // 
            // tableLayoutPanelCanvas
            // 
            this.tableLayoutPanelCanvas.ColumnCount = 1;
            this.tableLayoutPanelCanvas.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelCanvas.Controls.Add(this.panelCanvas, 0, 1);
            this.tableLayoutPanelCanvas.Controls.Add(this.tableLayoutPanelCanvasBottom, 0, 2);
            this.tableLayoutPanelCanvas.Controls.Add(this.panelCanvasHeader, 0, 0);
            this.tableLayoutPanelCanvas.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelCanvas.Location = new System.Drawing.Point(221, 0);
            this.tableLayoutPanelCanvas.Name = "tableLayoutPanelCanvas";
            this.tableLayoutPanelCanvas.RowCount = 3;
            this.tableLayoutPanelCanvas.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanelCanvas.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelCanvas.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 86F));
            this.tableLayoutPanelCanvas.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanelCanvas.Size = new System.Drawing.Size(579, 600);
            this.tableLayoutPanelCanvas.TabIndex = 2;
            // 
            // tableLayoutPanelCanvasBottom
            // 
            this.tableLayoutPanelCanvasBottom.ColumnCount = 3;
            this.tableLayoutPanelCanvasBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tableLayoutPanelCanvasBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 305F));
            this.tableLayoutPanelCanvasBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelCanvasBottom.Controls.Add(this.panelCanvasBottomMousePos, 0, 0);
            this.tableLayoutPanelCanvasBottom.Controls.Add(this.panelCanvasBottomCurrentTablePos, 1, 0);
            this.tableLayoutPanelCanvasBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelCanvasBottom.Location = new System.Drawing.Point(3, 517);
            this.tableLayoutPanelCanvasBottom.Name = "tableLayoutPanelCanvasBottom";
            this.tableLayoutPanelCanvasBottom.RowCount = 1;
            this.tableLayoutPanelCanvasBottom.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelCanvasBottom.Size = new System.Drawing.Size(573, 80);
            this.tableLayoutPanelCanvasBottom.TabIndex = 2;
            // 
            // panelCanvasHeader
            // 
            this.panelCanvasHeader.BackColor = System.Drawing.Color.Gray;
            this.panelCanvasHeader.Controls.Add(this.labelCanvasHeaderCurrentLocation);
            this.panelCanvasHeader.Controls.Add(this.textBoxCanvasHeaderCurrentLocation);
            this.panelCanvasHeader.Controls.Add(this.labelCanvasHeaderSelectedTableName);
            this.panelCanvasHeader.Controls.Add(this.textBoxCanvasHeaderSelectedTableName);
            this.panelCanvasHeader.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelCanvasHeader.Location = new System.Drawing.Point(3, 3);
            this.panelCanvasHeader.Name = "panelCanvasHeader";
            this.panelCanvasHeader.Size = new System.Drawing.Size(573, 34);
            this.panelCanvasHeader.TabIndex = 3;
            // 
            // labelCanvasHeaderCurrentLocation
            // 
            this.labelCanvasHeaderCurrentLocation.AutoSize = true;
            this.labelCanvasHeaderCurrentLocation.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasHeaderCurrentLocation.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasHeaderCurrentLocation.ForeColor = System.Drawing.Color.White;
            this.labelCanvasHeaderCurrentLocation.Location = new System.Drawing.Point(5, 11);
            this.labelCanvasHeaderCurrentLocation.Name = "labelCanvasHeaderCurrentLocation";
            this.labelCanvasHeaderCurrentLocation.Size = new System.Drawing.Size(101, 13);
            this.labelCanvasHeaderCurrentLocation.TabIndex = 7;
            this.labelCanvasHeaderCurrentLocation.Text = "Current Location";
            // 
            // textBoxCanvasHeaderCurrentLocation
            // 
            this.textBoxCanvasHeaderCurrentLocation.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBoxCanvasHeaderCurrentLocation.Location = new System.Drawing.Point(108, 7);
            this.textBoxCanvasHeaderCurrentLocation.Name = "textBoxCanvasHeaderCurrentLocation";
            this.textBoxCanvasHeaderCurrentLocation.Size = new System.Drawing.Size(164, 20);
            this.textBoxCanvasHeaderCurrentLocation.TabIndex = 6;
            this.toolTipCanvasHeaderCurrentLocation.SetToolTip(this.textBoxCanvasHeaderCurrentLocation, "Change Current Location\'s Name");
            this.textBoxCanvasHeaderCurrentLocation.TextChanged += new System.EventHandler(this.textBoxCanvasHeaderCurrentLocation_TextChanged);
            this.textBoxCanvasHeaderCurrentLocation.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxCanvasHeaderCurrentLocation_KeyPress);
            this.textBoxCanvasHeaderCurrentLocation.Leave += new System.EventHandler(this.textBoxCanvasHeaderCurrentLocation_Leave);
            // 
            // labelCanvasHeaderSelectedTableName
            // 
            this.labelCanvasHeaderSelectedTableName.AutoSize = true;
            this.labelCanvasHeaderSelectedTableName.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasHeaderSelectedTableName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasHeaderSelectedTableName.ForeColor = System.Drawing.Color.White;
            this.labelCanvasHeaderSelectedTableName.Location = new System.Drawing.Point(299, 11);
            this.labelCanvasHeaderSelectedTableName.Name = "labelCanvasHeaderSelectedTableName";
            this.labelCanvasHeaderSelectedTableName.Size = new System.Drawing.Size(93, 13);
            this.labelCanvasHeaderSelectedTableName.TabIndex = 5;
            this.labelCanvasHeaderSelectedTableName.Text = "Selected Table";
            // 
            // textBoxCanvasHeaderSelectedTableName
            // 
            this.textBoxCanvasHeaderSelectedTableName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBoxCanvasHeaderSelectedTableName.Location = new System.Drawing.Point(395, 7);
            this.textBoxCanvasHeaderSelectedTableName.Name = "textBoxCanvasHeaderSelectedTableName";
            this.textBoxCanvasHeaderSelectedTableName.Size = new System.Drawing.Size(164, 20);
            this.textBoxCanvasHeaderSelectedTableName.TabIndex = 4;
            this.toolTipCanvasHeaderSelectedTableName.SetToolTip(this.textBoxCanvasHeaderSelectedTableName, "Change Selected Table\'s Name\r\n");
            this.textBoxCanvasHeaderSelectedTableName.TextChanged += new System.EventHandler(this.textBoxCanvasHeaderSelectedTableName_TextChanged);
            // 
            // panelLeft
            // 
            this.panelLeft.Controls.Add(this.listBoxMenuAddTable);
            this.panelLeft.Controls.Add(this.tableLayoutPanelMenu);
            this.panelLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelLeft.Location = new System.Drawing.Point(0, 0);
            this.panelLeft.Name = "panelLeft";
            this.panelLeft.Size = new System.Drawing.Size(221, 600);
            this.panelLeft.TabIndex = 1;
            // 
            // listBoxMenuAddTable
            // 
            this.listBoxMenuAddTable.BackColor = System.Drawing.Color.Gray;
            this.listBoxMenuAddTable.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listBoxMenuAddTable.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listBoxMenuAddTable.ForeColor = System.Drawing.Color.White;
            this.listBoxMenuAddTable.FormattingEnabled = true;
            this.listBoxMenuAddTable.ItemHeight = 16;
            this.listBoxMenuAddTable.Items.AddRange(new object[] {
            "Add Rectangular Table",
            "Add Round Table"});
            this.listBoxMenuAddTable.Location = new System.Drawing.Point(12, 427);
            this.listBoxMenuAddTable.Name = "listBoxMenuAddTable";
            this.listBoxMenuAddTable.Size = new System.Drawing.Size(170, 34);
            this.listBoxMenuAddTable.TabIndex = 2;
            this.listBoxMenuAddTable.Visible = false;
            this.listBoxMenuAddTable.Click += new System.EventHandler(this.listBoxMenuAddTable_Click);
            this.listBoxMenuAddTable.MouseLeave += new System.EventHandler(this.listBoxMenuAddTable_MouseLeave);
            // 
            // tableLayoutPanelMenu
            // 
            this.tableLayoutPanelMenu.ColumnCount = 1;
            this.tableLayoutPanelMenu.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelMenu.Controls.Add(this.panelMenuTables, 0, 2);
            this.tableLayoutPanelMenu.Controls.Add(this.panelMenuLogo, 0, 0);
            this.tableLayoutPanelMenu.Controls.Add(this.panelMenuLocations, 0, 1);
            this.tableLayoutPanelMenu.Controls.Add(this.panelMenuEdit, 0, 3);
            this.tableLayoutPanelMenu.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanelMenu.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanelMenu.Name = "tableLayoutPanelMenu";
            this.tableLayoutPanelMenu.RowCount = 5;
            this.tableLayoutPanelMenu.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanelMenu.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 180F));
            this.tableLayoutPanelMenu.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 180F));
            this.tableLayoutPanelMenu.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanelMenu.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanelMenu.Size = new System.Drawing.Size(221, 542);
            this.tableLayoutPanelMenu.TabIndex = 0;
            // 
            // toolTipLocationAdd
            // 
            this.toolTipLocationAdd.ToolTipTitle = "Add";
            // 
            // toolTipLocationDel
            // 
            this.toolTipLocationDel.ToolTipTitle = "Delete";
            // 
            // toolTipLocationBckImg
            // 
            this.toolTipLocationBckImg.ToolTipTitle = "Background image";
            // 
            // toolTipSave
            // 
            this.toolTipSave.ToolTipTitle = "Save";
            // 
            // toolTipClose
            // 
            this.toolTipClose.ToolTipTitle = "Close";
            // 
            // toolTipCanvasHeaderCurrentLocation
            // 
            this.toolTipCanvasHeaderCurrentLocation.ToolTipTitle = "Location\'s Name";
            // 
            // toolTipCanvasHeaderSelectedTableName
            // 
            this.toolTipCanvasHeaderSelectedTableName.ToolTipTitle = "Table\'s Name";
            // 
            // openFileDialogLocImg
            // 
            this.openFileDialogLocImg.Filter = "BIT Map files|*.bmp|GIF files|*.gif|ICON files|*.icon|JPG files|*.jpg|JPEG files|" +
    "*.jpeg|PNG files|*.png|TIF files|*.tiff";
            this.openFileDialogLocImg.FilterIndex = 4;
            // 
            // panelCanvas
            // 
            this.panelCanvas.BackColor = System.Drawing.Color.Gainsboro;
            this.panelCanvas.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.panelCanvas.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelCanvas.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelCanvas.Location = new System.Drawing.Point(3, 43);
            this.panelCanvas.Name = "panelCanvas";
            this.panelCanvas.Size = new System.Drawing.Size(573, 468);
            this.panelCanvas.TabIndex = 1;
            this.panelCanvas.Resize += new System.EventHandler(this.panelCanvas_Resize);
            // 
            // panelCanvasBottomMousePos
            // 
            this.panelCanvasBottomMousePos.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelCanvasBottomMousePos.BackgroundImage")));
            this.panelCanvasBottomMousePos.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelCanvasBottomMousePos.Controls.Add(this.labelCanvasBottomMouseYPosValue);
            this.panelCanvasBottomMousePos.Controls.Add(this.labelCanvasBottomMouseXPosValue);
            this.panelCanvasBottomMousePos.Controls.Add(this.labelCanvasBottomMouseYPosTitle);
            this.panelCanvasBottomMousePos.Controls.Add(this.labelCanvasBottomMouseXPosTitle);
            this.panelCanvasBottomMousePos.Controls.Add(this.labelCanvasBottomMousePos);
            this.panelCanvasBottomMousePos.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelCanvasBottomMousePos.Location = new System.Drawing.Point(3, 3);
            this.panelCanvasBottomMousePos.Name = "panelCanvasBottomMousePos";
            this.panelCanvasBottomMousePos.Size = new System.Drawing.Size(144, 74);
            this.panelCanvasBottomMousePos.TabIndex = 0;
            // 
            // labelCanvasBottomMouseYPosValue
            // 
            this.labelCanvasBottomMouseYPosValue.AutoSize = true;
            this.labelCanvasBottomMouseYPosValue.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomMouseYPosValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomMouseYPosValue.ForeColor = System.Drawing.Color.White;
            this.labelCanvasBottomMouseYPosValue.Location = new System.Drawing.Point(27, 45);
            this.labelCanvasBottomMouseYPosValue.Name = "labelCanvasBottomMouseYPosValue";
            this.labelCanvasBottomMouseYPosValue.Size = new System.Drawing.Size(19, 20);
            this.labelCanvasBottomMouseYPosValue.TabIndex = 51;
            this.labelCanvasBottomMouseYPosValue.Text = "0";
            // 
            // labelCanvasBottomMouseXPosValue
            // 
            this.labelCanvasBottomMouseXPosValue.AutoSize = true;
            this.labelCanvasBottomMouseXPosValue.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomMouseXPosValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomMouseXPosValue.ForeColor = System.Drawing.Color.White;
            this.labelCanvasBottomMouseXPosValue.Location = new System.Drawing.Point(27, 25);
            this.labelCanvasBottomMouseXPosValue.Name = "labelCanvasBottomMouseXPosValue";
            this.labelCanvasBottomMouseXPosValue.Size = new System.Drawing.Size(19, 20);
            this.labelCanvasBottomMouseXPosValue.TabIndex = 50;
            this.labelCanvasBottomMouseXPosValue.Text = "0";
            // 
            // labelCanvasBottomMouseYPosTitle
            // 
            this.labelCanvasBottomMouseYPosTitle.AutoSize = true;
            this.labelCanvasBottomMouseYPosTitle.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomMouseYPosTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomMouseYPosTitle.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomMouseYPosTitle.Location = new System.Drawing.Point(5, 45);
            this.labelCanvasBottomMouseYPosTitle.Name = "labelCanvasBottomMouseYPosTitle";
            this.labelCanvasBottomMouseYPosTitle.Size = new System.Drawing.Size(21, 20);
            this.labelCanvasBottomMouseYPosTitle.TabIndex = 49;
            this.labelCanvasBottomMouseYPosTitle.Text = "Y";
            // 
            // labelCanvasBottomMouseXPosTitle
            // 
            this.labelCanvasBottomMouseXPosTitle.AutoSize = true;
            this.labelCanvasBottomMouseXPosTitle.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomMouseXPosTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomMouseXPosTitle.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomMouseXPosTitle.Location = new System.Drawing.Point(6, 25);
            this.labelCanvasBottomMouseXPosTitle.Name = "labelCanvasBottomMouseXPosTitle";
            this.labelCanvasBottomMouseXPosTitle.Size = new System.Drawing.Size(21, 20);
            this.labelCanvasBottomMouseXPosTitle.TabIndex = 48;
            this.labelCanvasBottomMouseXPosTitle.Text = "X";
            // 
            // labelCanvasBottomMousePos
            // 
            this.labelCanvasBottomMousePos.AutoSize = true;
            this.labelCanvasBottomMousePos.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomMousePos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomMousePos.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomMousePos.Location = new System.Drawing.Point(6, 9);
            this.labelCanvasBottomMousePos.Name = "labelCanvasBottomMousePos";
            this.labelCanvasBottomMousePos.Size = new System.Drawing.Size(93, 13);
            this.labelCanvasBottomMousePos.TabIndex = 47;
            this.labelCanvasBottomMousePos.Text = "Mouse Position";
            // 
            // panelCanvasBottomCurrentTablePos
            // 
            this.panelCanvasBottomCurrentTablePos.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelCanvasBottomCurrentTablePos.BackgroundImage")));
            this.panelCanvasBottomCurrentTablePos.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableName);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableHeightValue);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableWidthValue);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableHeightTitle);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableWidthTitle);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableYPosValue);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableXPosValue);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableYPosTitle);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTableXPosTitle);
            this.panelCanvasBottomCurrentTablePos.Controls.Add(this.labelCanvasBottomCurrentTablePos);
            this.panelCanvasBottomCurrentTablePos.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelCanvasBottomCurrentTablePos.Location = new System.Drawing.Point(153, 3);
            this.panelCanvasBottomCurrentTablePos.Name = "panelCanvasBottomCurrentTablePos";
            this.panelCanvasBottomCurrentTablePos.Size = new System.Drawing.Size(299, 74);
            this.panelCanvasBottomCurrentTablePos.TabIndex = 1;
            // 
            // labelCanvasBottomCurrentTableName
            // 
            this.labelCanvasBottomCurrentTableName.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableName.ForeColor = System.Drawing.Color.Orange;
            this.labelCanvasBottomCurrentTableName.Location = new System.Drawing.Point(155, 9);
            this.labelCanvasBottomCurrentTableName.Name = "labelCanvasBottomCurrentTableName";
            this.labelCanvasBottomCurrentTableName.Size = new System.Drawing.Size(139, 13);
            this.labelCanvasBottomCurrentTableName.TabIndex = 60;
            this.labelCanvasBottomCurrentTableName.Text = "Table1";
            this.labelCanvasBottomCurrentTableName.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // labelCanvasBottomCurrentTableHeightValue
            // 
            this.labelCanvasBottomCurrentTableHeightValue.AutoSize = true;
            this.labelCanvasBottomCurrentTableHeightValue.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableHeightValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableHeightValue.ForeColor = System.Drawing.Color.White;
            this.labelCanvasBottomCurrentTableHeightValue.Location = new System.Drawing.Point(140, 47);
            this.labelCanvasBottomCurrentTableHeightValue.Name = "labelCanvasBottomCurrentTableHeightValue";
            this.labelCanvasBottomCurrentTableHeightValue.Size = new System.Drawing.Size(19, 20);
            this.labelCanvasBottomCurrentTableHeightValue.TabIndex = 59;
            this.labelCanvasBottomCurrentTableHeightValue.Text = "0";
            // 
            // labelCanvasBottomCurrentTableWidthValue
            // 
            this.labelCanvasBottomCurrentTableWidthValue.AutoSize = true;
            this.labelCanvasBottomCurrentTableWidthValue.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableWidthValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableWidthValue.ForeColor = System.Drawing.Color.White;
            this.labelCanvasBottomCurrentTableWidthValue.Location = new System.Drawing.Point(140, 27);
            this.labelCanvasBottomCurrentTableWidthValue.Name = "labelCanvasBottomCurrentTableWidthValue";
            this.labelCanvasBottomCurrentTableWidthValue.Size = new System.Drawing.Size(19, 20);
            this.labelCanvasBottomCurrentTableWidthValue.TabIndex = 58;
            this.labelCanvasBottomCurrentTableWidthValue.Text = "0";
            // 
            // labelCanvasBottomCurrentTableHeightTitle
            // 
            this.labelCanvasBottomCurrentTableHeightTitle.AutoSize = true;
            this.labelCanvasBottomCurrentTableHeightTitle.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableHeightTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableHeightTitle.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomCurrentTableHeightTitle.Location = new System.Drawing.Point(81, 47);
            this.labelCanvasBottomCurrentTableHeightTitle.Name = "labelCanvasBottomCurrentTableHeightTitle";
            this.labelCanvasBottomCurrentTableHeightTitle.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.labelCanvasBottomCurrentTableHeightTitle.Size = new System.Drawing.Size(62, 20);
            this.labelCanvasBottomCurrentTableHeightTitle.TabIndex = 57;
            this.labelCanvasBottomCurrentTableHeightTitle.Text = "Height";
            // 
            // labelCanvasBottomCurrentTableWidthTitle
            // 
            this.labelCanvasBottomCurrentTableWidthTitle.AutoSize = true;
            this.labelCanvasBottomCurrentTableWidthTitle.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableWidthTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableWidthTitle.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomCurrentTableWidthTitle.Location = new System.Drawing.Point(82, 27);
            this.labelCanvasBottomCurrentTableWidthTitle.Name = "labelCanvasBottomCurrentTableWidthTitle";
            this.labelCanvasBottomCurrentTableWidthTitle.Size = new System.Drawing.Size(55, 20);
            this.labelCanvasBottomCurrentTableWidthTitle.TabIndex = 56;
            this.labelCanvasBottomCurrentTableWidthTitle.Text = "Width";
            // 
            // labelCanvasBottomCurrentTableYPosValue
            // 
            this.labelCanvasBottomCurrentTableYPosValue.AutoSize = true;
            this.labelCanvasBottomCurrentTableYPosValue.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableYPosValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableYPosValue.ForeColor = System.Drawing.Color.White;
            this.labelCanvasBottomCurrentTableYPosValue.Location = new System.Drawing.Point(25, 46);
            this.labelCanvasBottomCurrentTableYPosValue.Name = "labelCanvasBottomCurrentTableYPosValue";
            this.labelCanvasBottomCurrentTableYPosValue.Size = new System.Drawing.Size(39, 20);
            this.labelCanvasBottomCurrentTableYPosValue.TabIndex = 55;
            this.labelCanvasBottomCurrentTableYPosValue.Text = "800";
            // 
            // labelCanvasBottomCurrentTableXPosValue
            // 
            this.labelCanvasBottomCurrentTableXPosValue.AutoSize = true;
            this.labelCanvasBottomCurrentTableXPosValue.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableXPosValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableXPosValue.ForeColor = System.Drawing.Color.White;
            this.labelCanvasBottomCurrentTableXPosValue.Location = new System.Drawing.Point(24, 26);
            this.labelCanvasBottomCurrentTableXPosValue.Name = "labelCanvasBottomCurrentTableXPosValue";
            this.labelCanvasBottomCurrentTableXPosValue.Size = new System.Drawing.Size(49, 20);
            this.labelCanvasBottomCurrentTableXPosValue.TabIndex = 54;
            this.labelCanvasBottomCurrentTableXPosValue.Text = "1600";
            // 
            // labelCanvasBottomCurrentTableYPosTitle
            // 
            this.labelCanvasBottomCurrentTableYPosTitle.AutoSize = true;
            this.labelCanvasBottomCurrentTableYPosTitle.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableYPosTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableYPosTitle.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomCurrentTableYPosTitle.Location = new System.Drawing.Point(4, 46);
            this.labelCanvasBottomCurrentTableYPosTitle.Name = "labelCanvasBottomCurrentTableYPosTitle";
            this.labelCanvasBottomCurrentTableYPosTitle.Size = new System.Drawing.Size(21, 20);
            this.labelCanvasBottomCurrentTableYPosTitle.TabIndex = 53;
            this.labelCanvasBottomCurrentTableYPosTitle.Text = "Y";
            // 
            // labelCanvasBottomCurrentTableXPosTitle
            // 
            this.labelCanvasBottomCurrentTableXPosTitle.AutoSize = true;
            this.labelCanvasBottomCurrentTableXPosTitle.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTableXPosTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTableXPosTitle.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomCurrentTableXPosTitle.Location = new System.Drawing.Point(5, 26);
            this.labelCanvasBottomCurrentTableXPosTitle.Name = "labelCanvasBottomCurrentTableXPosTitle";
            this.labelCanvasBottomCurrentTableXPosTitle.Size = new System.Drawing.Size(21, 20);
            this.labelCanvasBottomCurrentTableXPosTitle.TabIndex = 52;
            this.labelCanvasBottomCurrentTableXPosTitle.Text = "X";
            // 
            // labelCanvasBottomCurrentTablePos
            // 
            this.labelCanvasBottomCurrentTablePos.AutoSize = true;
            this.labelCanvasBottomCurrentTablePos.BackColor = System.Drawing.Color.Transparent;
            this.labelCanvasBottomCurrentTablePos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCanvasBottomCurrentTablePos.ForeColor = System.Drawing.Color.LightGray;
            this.labelCanvasBottomCurrentTablePos.Location = new System.Drawing.Point(7, 9);
            this.labelCanvasBottomCurrentTablePos.Name = "labelCanvasBottomCurrentTablePos";
            this.labelCanvasBottomCurrentTablePos.Size = new System.Drawing.Size(142, 13);
            this.labelCanvasBottomCurrentTablePos.TabIndex = 48;
            this.labelCanvasBottomCurrentTablePos.Text = "Current Table\'s Position";
            // 
            // panelMenuTables
            // 
            this.panelMenuTables.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.cell_snow90_tangerine80_215x174;
            this.panelMenuTables.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelMenuTables.Controls.Add(this.listBoxMenuTables);
            this.panelMenuTables.Controls.Add(this.panelMenuTablesDel);
            this.panelMenuTables.Controls.Add(this.panelMenuTablesAdd);
            this.panelMenuTables.Controls.Add(this.labelMenuTables);
            this.panelMenuTables.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMenuTables.Location = new System.Drawing.Point(3, 263);
            this.panelMenuTables.Name = "panelMenuTables";
            this.panelMenuTables.Size = new System.Drawing.Size(215, 174);
            this.panelMenuTables.TabIndex = 2;
            // 
            // listBoxMenuTables
            // 
            this.listBoxMenuTables.FormattingEnabled = true;
            this.listBoxMenuTables.Location = new System.Drawing.Point(11, 22);
            this.listBoxMenuTables.Name = "listBoxMenuTables";
            this.listBoxMenuTables.Size = new System.Drawing.Size(192, 108);
            this.listBoxMenuTables.TabIndex = 50;
            this.listBoxMenuTables.Click += new System.EventHandler(this.listBoxMenuTables_Click);
            // 
            // panelMenuTablesDel
            // 
            this.panelMenuTablesDel.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuTablesDel.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.remove;
            this.panelMenuTablesDel.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuTablesDel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuTablesDel.Location = new System.Drawing.Point(48, 133);
            this.panelMenuTablesDel.Name = "panelMenuTablesDel";
            this.panelMenuTablesDel.Size = new System.Drawing.Size(32, 32);
            this.panelMenuTablesDel.TabIndex = 49;
            this.toolTipLocationDel.SetToolTip(this.panelMenuTablesDel, "Remove the selected table from the database");
            this.panelMenuTablesDel.Click += new System.EventHandler(this.panelMenuTablesDel_Click);
            // 
            // panelMenuTablesAdd
            // 
            this.panelMenuTablesAdd.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuTablesAdd.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelMenuTablesAdd.BackgroundImage")));
            this.panelMenuTablesAdd.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuTablesAdd.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuTablesAdd.Location = new System.Drawing.Point(10, 133);
            this.panelMenuTablesAdd.Name = "panelMenuTablesAdd";
            this.panelMenuTablesAdd.Size = new System.Drawing.Size(32, 32);
            this.panelMenuTablesAdd.TabIndex = 48;
            this.toolTipLocationAdd.SetToolTip(this.panelMenuTablesAdd, "Add a new table");
            this.panelMenuTablesAdd.Click += new System.EventHandler(this.panelMenuTablesAdd_Click);
            // 
            // labelMenuTables
            // 
            this.labelMenuTables.AutoSize = true;
            this.labelMenuTables.BackColor = System.Drawing.Color.Transparent;
            this.labelMenuTables.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMenuTables.ForeColor = System.Drawing.Color.White;
            this.labelMenuTables.Location = new System.Drawing.Point(8, 6);
            this.labelMenuTables.Name = "labelMenuTables";
            this.labelMenuTables.Size = new System.Drawing.Size(45, 13);
            this.labelMenuTables.TabIndex = 46;
            this.labelMenuTables.Text = "Tables";
            // 
            // panelMenuLogo
            // 
            this.panelMenuLogo.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.celSnow80Lead50_300x70;
            this.panelMenuLogo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelMenuLogo.Controls.Add(this.panelMenuLogoImg);
            this.panelMenuLogo.Controls.Add(this.labeMenulVersion);
            this.panelMenuLogo.Controls.Add(this.linkLabelMenuLogo);
            this.panelMenuLogo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMenuLogo.Location = new System.Drawing.Point(3, 3);
            this.panelMenuLogo.Name = "panelMenuLogo";
            this.panelMenuLogo.Size = new System.Drawing.Size(215, 74);
            this.panelMenuLogo.TabIndex = 0;
            // 
            // panelMenuLogoImg
            // 
            this.panelMenuLogoImg.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuLogoImg.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.circle_menumate_logo;
            this.panelMenuLogoImg.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelMenuLogoImg.Location = new System.Drawing.Point(9, 16);
            this.panelMenuLogoImg.Name = "panelMenuLogoImg";
            this.panelMenuLogoImg.Size = new System.Drawing.Size(46, 45);
            this.panelMenuLogoImg.TabIndex = 43;
            // 
            // labeMenulVersion
            // 
            this.labeMenulVersion.BackColor = System.Drawing.Color.Transparent;
            this.labeMenulVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labeMenulVersion.ForeColor = System.Drawing.Color.Black;
            this.labeMenulVersion.Location = new System.Drawing.Point(60, 47);
            this.labeMenulVersion.Name = "labeMenulVersion";
            this.labeMenulVersion.Size = new System.Drawing.Size(126, 14);
            this.labeMenulVersion.TabIndex = 42;
            this.labeMenulVersion.Text = "Version ";
            // 
            // linkLabelMenuLogo
            // 
            this.linkLabelMenuLogo.AutoEllipsis = true;
            this.linkLabelMenuLogo.AutoSize = true;
            this.linkLabelMenuLogo.BackColor = System.Drawing.Color.Transparent;
            this.linkLabelMenuLogo.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkLabelMenuLogo.LinkBehavior = System.Windows.Forms.LinkBehavior.NeverUnderline;
            this.linkLabelMenuLogo.LinkColor = System.Drawing.Color.Black;
            this.linkLabelMenuLogo.Location = new System.Drawing.Point(53, 11);
            this.linkLabelMenuLogo.Name = "linkLabelMenuLogo";
            this.linkLabelMenuLogo.Size = new System.Drawing.Size(150, 24);
            this.linkLabelMenuLogo.TabIndex = 41;
            this.linkLabelMenuLogo.TabStop = true;
            this.linkLabelMenuLogo.Text = "Floor Plan Editor";
            // 
            // panelMenuLocations
            // 
            this.panelMenuLocations.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.cell_snow90_tangerine80_215x174;
            this.panelMenuLocations.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelMenuLocations.Controls.Add(this.listBoxMenuLocations);
            this.panelMenuLocations.Controls.Add(this.panelMenuLocationsBckImg);
            this.panelMenuLocations.Controls.Add(this.panelMenuLocationsDel);
            this.panelMenuLocations.Controls.Add(this.panelMenuLocationsAdd);
            this.panelMenuLocations.Controls.Add(this.labelMenuLocations);
            this.panelMenuLocations.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMenuLocations.Location = new System.Drawing.Point(3, 83);
            this.panelMenuLocations.Name = "panelMenuLocations";
            this.panelMenuLocations.Size = new System.Drawing.Size(215, 174);
            this.panelMenuLocations.TabIndex = 1;
            // 
            // listBoxMenuLocations
            // 
            this.listBoxMenuLocations.FormattingEnabled = true;
            this.listBoxMenuLocations.Location = new System.Drawing.Point(11, 21);
            this.listBoxMenuLocations.Name = "listBoxMenuLocations";
            this.listBoxMenuLocations.Size = new System.Drawing.Size(192, 108);
            this.listBoxMenuLocations.TabIndex = 47;
            this.listBoxMenuLocations.Click += new System.EventHandler(this.listBoxMenuLocations_Click);
            // 
            // panelMenuLocationsBckImg
            // 
            this.panelMenuLocationsBckImg.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuLocationsBckImg.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.target;
            this.panelMenuLocationsBckImg.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuLocationsBckImg.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuLocationsBckImg.Location = new System.Drawing.Point(87, 134);
            this.panelMenuLocationsBckImg.Name = "panelMenuLocationsBckImg";
            this.panelMenuLocationsBckImg.Size = new System.Drawing.Size(32, 32);
            this.panelMenuLocationsBckImg.TabIndex = 46;
            this.toolTipLocationBckImg.SetToolTip(this.panelMenuLocationsBckImg, "Change the current location\'s background image");
            this.panelMenuLocationsBckImg.Click += new System.EventHandler(this.panelMenuLocationsBckImg_Click);
            // 
            // panelMenuLocationsDel
            // 
            this.panelMenuLocationsDel.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuLocationsDel.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.remove;
            this.panelMenuLocationsDel.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuLocationsDel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuLocationsDel.Location = new System.Drawing.Point(49, 134);
            this.panelMenuLocationsDel.Name = "panelMenuLocationsDel";
            this.panelMenuLocationsDel.Size = new System.Drawing.Size(32, 32);
            this.panelMenuLocationsDel.TabIndex = 45;
            this.toolTipLocationDel.SetToolTip(this.panelMenuLocationsDel, "Remove the current location from the database");
            this.panelMenuLocationsDel.Click += new System.EventHandler(this.panelMenuLocationsDel_Click);
            this.panelMenuLocationsDel.Paint += new System.Windows.Forms.PaintEventHandler(this.panelMenuLocationsDel_Paint);
            // 
            // panelMenuLocationsAdd
            // 
            this.panelMenuLocationsAdd.AutoSize = true;
            this.panelMenuLocationsAdd.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuLocationsAdd.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelMenuLocationsAdd.BackgroundImage")));
            this.panelMenuLocationsAdd.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuLocationsAdd.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuLocationsAdd.Location = new System.Drawing.Point(11, 134);
            this.panelMenuLocationsAdd.Name = "panelMenuLocationsAdd";
            this.panelMenuLocationsAdd.Size = new System.Drawing.Size(32, 32);
            this.panelMenuLocationsAdd.TabIndex = 44;
            this.toolTipLocationAdd.SetToolTip(this.panelMenuLocationsAdd, "Add a new location");
            this.panelMenuLocationsAdd.Click += new System.EventHandler(this.panelMenuLocationsAdd_Click);
            // 
            // labelMenuLocations
            // 
            this.labelMenuLocations.AutoSize = true;
            this.labelMenuLocations.BackColor = System.Drawing.Color.Transparent;
            this.labelMenuLocations.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMenuLocations.ForeColor = System.Drawing.Color.White;
            this.labelMenuLocations.Location = new System.Drawing.Point(7, 4);
            this.labelMenuLocations.Name = "labelMenuLocations";
            this.labelMenuLocations.Size = new System.Drawing.Size(62, 13);
            this.labelMenuLocations.TabIndex = 0;
            this.labelMenuLocations.Text = "Locations";
            // 
            // panelMenuEdit
            // 
            this.panelMenuEdit.BackgroundImage = global::Menumate.WinServices.Client.FloorPlan.Properties.Resources.celSnow80Tangerine90_300x70;
            this.panelMenuEdit.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panelMenuEdit.Controls.Add(this.linkLabelMenuSave);
            this.panelMenuEdit.Controls.Add(this.panelMenuSave);
            this.panelMenuEdit.Controls.Add(this.linkLabelMenuClose);
            this.panelMenuEdit.Controls.Add(this.panelMenuClose);
            this.panelMenuEdit.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMenuEdit.Location = new System.Drawing.Point(3, 443);
            this.panelMenuEdit.Name = "panelMenuEdit";
            this.panelMenuEdit.Size = new System.Drawing.Size(215, 74);
            this.panelMenuEdit.TabIndex = 3;
            // 
            // linkLabelMenuSave
            // 
            this.linkLabelMenuSave.AutoSize = true;
            this.linkLabelMenuSave.BackColor = System.Drawing.Color.Transparent;
            this.linkLabelMenuSave.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkLabelMenuSave.LinkColor = System.Drawing.Color.White;
            this.linkLabelMenuSave.Location = new System.Drawing.Point(47, 24);
            this.linkLabelMenuSave.Name = "linkLabelMenuSave";
            this.linkLabelMenuSave.Size = new System.Drawing.Size(44, 16);
            this.linkLabelMenuSave.TabIndex = 53;
            this.linkLabelMenuSave.TabStop = true;
            this.linkLabelMenuSave.Text = "Save";
            this.linkLabelMenuSave.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTipSave.SetToolTip(this.linkLabelMenuSave, "Change curent changes in the database");
            this.linkLabelMenuSave.VisitedLinkColor = System.Drawing.Color.White;
            this.linkLabelMenuSave.Click += new System.EventHandler(this.linkLabelMenuSave_Click);
            // 
            // panelMenuSave
            // 
            this.panelMenuSave.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuSave.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelMenuSave.BackgroundImage")));
            this.panelMenuSave.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuSave.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuSave.Location = new System.Drawing.Point(11, 20);
            this.panelMenuSave.Name = "panelMenuSave";
            this.panelMenuSave.Size = new System.Drawing.Size(32, 32);
            this.panelMenuSave.TabIndex = 52;
            this.toolTipSave.SetToolTip(this.panelMenuSave, "Change curent changes in the database");
            this.panelMenuSave.Click += new System.EventHandler(this.linkLabelMenuSave_Click);
            // 
            // linkLabelMenuClose
            // 
            this.linkLabelMenuClose.AutoSize = true;
            this.linkLabelMenuClose.BackColor = System.Drawing.Color.Transparent;
            this.linkLabelMenuClose.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkLabelMenuClose.LinkColor = System.Drawing.Color.White;
            this.linkLabelMenuClose.Location = new System.Drawing.Point(122, 24);
            this.linkLabelMenuClose.Name = "linkLabelMenuClose";
            this.linkLabelMenuClose.Size = new System.Drawing.Size(48, 16);
            this.linkLabelMenuClose.TabIndex = 51;
            this.linkLabelMenuClose.TabStop = true;
            this.linkLabelMenuClose.Text = "Close";
            this.linkLabelMenuClose.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTipClose.SetToolTip(this.linkLabelMenuClose, "Close Floor Plan Editor");
            this.linkLabelMenuClose.VisitedLinkColor = System.Drawing.Color.White;
            this.linkLabelMenuClose.MouseClick += new System.Windows.Forms.MouseEventHandler(this.panelMenuClose_MouseClick);
            // 
            // panelMenuClose
            // 
            this.panelMenuClose.BackColor = System.Drawing.Color.Transparent;
            this.panelMenuClose.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelMenuClose.BackgroundImage")));
            this.panelMenuClose.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.panelMenuClose.Cursor = System.Windows.Forms.Cursors.Hand;
            this.panelMenuClose.Location = new System.Drawing.Point(173, 21);
            this.panelMenuClose.Name = "panelMenuClose";
            this.panelMenuClose.Size = new System.Drawing.Size(32, 32);
            this.panelMenuClose.TabIndex = 50;
            this.toolTipClose.SetToolTip(this.panelMenuClose, "Close Floor Plan Editor\r\n");
            this.panelMenuClose.MouseClick += new System.Windows.Forms.MouseEventHandler(this.panelMenuClose_MouseClick);
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Controls.Add(this.panelWork);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "EditorForm";
            this.Text = "EditorForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EditorForm_FormClosing);
            this.Shown += new System.EventHandler(this.EditorForm_Shown);
            this.panelWork.ResumeLayout(false);
            this.tableLayoutPanelCanvas.ResumeLayout(false);
            this.tableLayoutPanelCanvasBottom.ResumeLayout(false);
            this.panelCanvasHeader.ResumeLayout(false);
            this.panelCanvasHeader.PerformLayout();
            this.panelLeft.ResumeLayout(false);
            this.tableLayoutPanelMenu.ResumeLayout(false);
            this.panelCanvasBottomMousePos.ResumeLayout(false);
            this.panelCanvasBottomMousePos.PerformLayout();
            this.panelCanvasBottomCurrentTablePos.ResumeLayout(false);
            this.panelCanvasBottomCurrentTablePos.PerformLayout();
            this.panelMenuTables.ResumeLayout(false);
            this.panelMenuTables.PerformLayout();
            this.panelMenuLogo.ResumeLayout(false);
            this.panelMenuLogo.PerformLayout();
            this.panelMenuLocations.ResumeLayout(false);
            this.panelMenuLocations.PerformLayout();
            this.panelMenuEdit.ResumeLayout(false);
            this.panelMenuEdit.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelWork;
        private System.Windows.Forms.Panel panelLeft;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelMenu;
        private System.Windows.Forms.Panel panelMenuLogo;
        private System.Windows.Forms.Panel panelMenuLocations;
        private System.Windows.Forms.Panel panelMenuLogoImg;
        private System.Windows.Forms.Label labeMenulVersion;
        private System.Windows.Forms.LinkLabel linkLabelMenuLogo;
        private System.Windows.Forms.Label labelMenuLocations;
        private System.Windows.Forms.Panel panelMenuTables;
        private System.Windows.Forms.Panel panelMenuEdit;
        private System.Windows.Forms.Panel panelMenuLocationsAdd;
        private System.Windows.Forms.Panel panelMenuLocationsDel;
        private System.Windows.Forms.Panel panelMenuTablesDel;
        private System.Windows.Forms.Panel panelMenuTablesAdd;
        private System.Windows.Forms.Label labelMenuTables;
        private System.Windows.Forms.Panel panelMenuLocationsBckImg;
        private System.Windows.Forms.Panel panelMenuClose;
        private System.Windows.Forms.LinkLabel linkLabelMenuSave;
        private System.Windows.Forms.Panel panelMenuSave;
        private System.Windows.Forms.LinkLabel linkLabelMenuClose;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelCanvas;
        private System.Windows.Forms.Panel panelCanvas;
        private System.Windows.Forms.ToolTip toolTipLocationAdd;
        private System.Windows.Forms.ToolTip toolTipLocationDel;
        private System.Windows.Forms.ToolTip toolTipLocationBckImg;
        private System.Windows.Forms.ToolTip toolTipSave;
        private System.Windows.Forms.ToolTip toolTipClose;
        private System.Windows.Forms.ToolTip toolTipCanvasHeaderCurrentLocation;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelCanvasBottom;
        private System.Windows.Forms.Panel panelCanvasBottomMousePos;
        private System.Windows.Forms.Panel panelCanvasBottomCurrentTablePos;
        private System.Windows.Forms.Label labelCanvasBottomMouseYPosValue;
        private System.Windows.Forms.Label labelCanvasBottomMouseXPosValue;
        private System.Windows.Forms.Label labelCanvasBottomMouseYPosTitle;
        private System.Windows.Forms.Label labelCanvasBottomMouseXPosTitle;
        private System.Windows.Forms.Label labelCanvasBottomMousePos;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTablePos;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableHeightValue;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableWidthValue;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableHeightTitle;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableWidthTitle;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableYPosValue;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableXPosValue;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableYPosTitle;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableXPosTitle;
        private System.Windows.Forms.Label labelCanvasBottomCurrentTableName;
        private System.Windows.Forms.ToolTip toolTipCanvasHeaderSelectedTableName;
        private System.Windows.Forms.Panel panelCanvasHeader;
        private System.Windows.Forms.Label labelCanvasHeaderCurrentLocation;
        private System.Windows.Forms.TextBox textBoxCanvasHeaderCurrentLocation;
        private System.Windows.Forms.Label labelCanvasHeaderSelectedTableName;
        private System.Windows.Forms.TextBox textBoxCanvasHeaderSelectedTableName;
        private System.Windows.Forms.OpenFileDialog openFileDialogLocImg;
        private System.Windows.Forms.ListBox listBoxMenuLocations;
        private System.Windows.Forms.ListBox listBoxMenuTables;
        private System.Windows.Forms.ListBox listBoxMenuAddTable;


    }
}