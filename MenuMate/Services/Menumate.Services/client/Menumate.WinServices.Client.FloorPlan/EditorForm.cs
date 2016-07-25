using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.WinServices.Client.FloorPlan
{
    public partial class EditorForm : Form
    {
        string FPErrorMsg = @"";

        EditingFloorPlan editingFloorPlan;

        ReservableTableView currentTableView;

        Panel canvas;
        RectTracker rectTracker;

        int _MIN_TABLE_NUMBER = 1;
        int _MAX_TABLE_NUMBER = 99;

        int BCK_LANDSCAPE_IMAGE_WIDTH  = 800;
        int BCK_LANDSCAPE_IMAGE_HEIGHT = 600;
        string BCK_LANDSCAPE_IMAGE_NAME = @"BlankL.jpeg";

        int BCK_PORTRAIT_IMAGE_WIDTH = 600;
        int BCK_PORTRAIT_IMAGE_HEIGHT = 800;
        string BCK_PORTRAIT_IMAGE_NAME = @"BlankP.jpeg";

        Int16 TableWidth = 80;
        Int16 TableHeight = 60;

        Graphics bckImgGraphics;
        Point bckImgOrigin;
        Timer bckImgTimer;
        Int16 bckImgRefreshTime = 250; // 250 mSec after finishing resizing

        Timer tableNameChangeTimer;
        Int16 tableNameChangeRefreshTime = 500; // 500 mSec after stop typing

        bool currentLocationTypingIn = false;

        public EditorForm()
        {
            InitializeComponent();
        }

        //....................................

        public void initFloorPlan()
        {
            try
            {
                this.labeMenulVersion.Text = "Version VERSTR";

                //...........................

                bckImgTimer = createBckImgTimer();
                tableNameChangeTimer = createTableNameChangeTimer();

                //initCanvas(canvas);

                canvas = createCanvas();
                this.panelCanvas.Controls.Add(canvas);

                //...........................

                editingFloorPlan = EditingFloorPlan.Instance; // Singleton instance of the Editing Floor Plan controller
                editingFloorPlan.Init(); // Initiates Singleton instance of the Editing Floor Plan controller
                checkTableNumbers();

                //...........................

                initGUI(editingFloorPlan.locations, 0);
            }
            catch (PosIntegrationException exc)
            {
                initGUI(null, 0);
                this.panelCanvas.Controls.Add(createFPErrorPanel(exc.Message));
            }
        }

        //....................................

        public void restartFloorPlan()
        {
            try
            {
                //Label label = (Label)this.panelCanvas.Controls[0];
                //label = null;

                //GC.Collect();

                //........................

                this.panelCanvas.Controls.Clear();

                //................................

                editingFloorPlan.Init(); // Initiates Singleton instance of the Editing Floor Plan controller

                initGUI(editingFloorPlan.locations, 0);
            }
            catch (PosIntegrationException exc)
            {
                initGUI(null, 0);
                this.panelCanvas.Controls.Add(createFPErrorPanel(exc.Message));
            }
        }

        //....................................

        #region Timers
        private Timer createBckImgTimer()
        {
            Timer result = Helper.createTimer(bckImgRefreshTime);
            result.Tick += new EventHandler(bckImgTimer_Tick);

            return result;
        }

        //....................................

        private Timer createTableNameChangeTimer()
        {
            Timer result = Helper.createTimer(tableNameChangeRefreshTime);
            result.Tick += new EventHandler(tableNameChangeTimer_Tick);

            return result;
        }

        //....................................

        private void resetTimer(Timer inTimer)
        {
            inTimer.Stop();
            inTimer.Start();
        }

        //....................................

        void bckImgTimer_Tick(object sender, EventArgs e)
        {
            Timer timer = (Timer)sender;
            timer.Stop();

            refreshBckImage();
        }

        //....................................

        void tableNameChangeTimer_Tick(object sender, EventArgs e)
        {
            Timer timer = (Timer)sender;
            timer.Stop();
        }
        #endregion

        //....................................

        private void initGUI(DTOLocation[] inLocations, int inLocIndex)
        {
            if (inLocations == null)
            {
                resetLocationView();
            }
            else
            {
                updateLocationNameList();
                clearStatusBar();

                if (inLocations.Length > 0)
                {
                    if (inLocIndex >= inLocations.Length)
                    {
                        inLocIndex = inLocations.Length - 1;
                    }

                    setLocationViewAtIndex(inLocIndex, true);
                }
                else
                {
                    resetLocationView();
                }
            }

            bool posIOK = this.posIntegrationOK();

            this.listBoxMenuLocations.Enabled = posIOK && (editingFloorPlan.locations.Length > 0);
            this.textBoxCanvasHeaderCurrentLocation.Enabled = posIOK && (editingFloorPlan.locations.Length > 0);
            this.panelMenuLocationsAdd.Enabled = posIOK;
            this.panelMenuLocationsDel.Enabled = posIOK && (editingFloorPlan.locations.Length > 0);
            this.panelMenuLocationsBckImg.Enabled = posIOK && (editingFloorPlan.locations.Length > 0);

            this.listBoxMenuTables.Enabled = posIOK && (editingFloorPlan.tablesInLocation.Length > 0);
            this.textBoxCanvasHeaderSelectedTableName.Enabled = false;
            this.panelMenuTablesAdd.Enabled = posIOK && (editingFloorPlan.locations.Length > 0);
            this.panelMenuTablesDel.Enabled = false;

            this.linkLabelMenuSave.Enabled = posIOK;
            this.panelMenuSave.Enabled = posIOK;

            enableCloseLink(true);
        }

        //....................................

        #region Exception Handlers
        private void unsavedLocationExceptionHandler()
        {
            DialogResult dResult = MessageBox.Show(@"Save current location", @"FloorPlan Editor", MessageBoxButtons.YesNo);

            if (dResult == DialogResult.Yes)
            {
                saveCurrentLocation();
            }
            else
            {
                giveUpSaving();
            }
        }

        //....................................

        private void unsavedTableExceptionHandler()
        {
            DialogResult dResult = MessageBox.Show(@"Save tables for current location", @"FloorPlan Editor", MessageBoxButtons.YesNo);

            if (dResult == DialogResult.Yes)
            {
                try
                {
                    editingFloorPlan.saveTables();
                    MessageBox.Show(string.Format("{0} has been saved", editingFloorPlan.currentLocation.Name));
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                }
            }
            else
            {
                giveUpSaving();
            }
        }

        //....................................

        private void indexOutOfRangeExceptionHandler()
        {
            //this.comboBoxMenuLocations.Text = @"";

            listBoxMenuLocations.SelectedIndex = -1;
            this.textBoxCanvasHeaderCurrentLocation.Text = @"";

            this.panelMenuLocationsDel.Enabled = false;
            this.panelMenuLocationsBckImg.Enabled = false;
        }
        #endregion

        //....................................

        #region PRIVATE METHODS: Canvas
        private Panel createFPErrorPanel(string inText)
        {
            FPErrorMsg = inText;

            Panel result = new Panel();

            //::::::::::::::::::::::::::

            result.Name = @"fpErrorPanel";
            result.Dock = DockStyle.Top;
            result.AutoSize = true;
            //result.Height = 50;
            result.Resize += fpErrorPanel_Resize;

            //..........................

            Button restartBtn = new Button();

            restartBtn.Text = "Restart";
            restartBtn.Left = 10;
            restartBtn.Top = result.Height - restartBtn.Height;
            restartBtn.Click += restartBtn_Click;
            restartBtn.Visible = false;

            //..........................


            Button detailsBtn = new Button();

            detailsBtn.Text = "Details";
            detailsBtn.Left = result.Width - detailsBtn.Width - 10;
            detailsBtn.Top = result.Height - detailsBtn.Height;
            detailsBtn.Click += detailsBtn_Click;

            //..........................

            result.Controls.Add(createFPErrorLabel(@"FloorPlan could not start this time.  Service's end point was not available.  Make sure the Windows Service (Menumate.Service) is running."));
            result.Controls.Add(restartBtn);
            result.Controls.Add(detailsBtn);

            //::::::::::::::::::::::::::

            return result;
        }

        private Label createFPErrorLabel(string inText)
        {
            Label result = new Label();

            //::::::::::::::::::::::::::

            result.Name = @"fpErrorLabel";
            result.Dock = DockStyle.Top;
            //result.AutoSize = true;
            result.Height = 40;
            result.Text = inText;
            result.TextAlign = ContentAlignment.TopCenter;

            float alpha = (255 * 80) / 100;
            int intAlpha = Convert.ToInt16(alpha);

            result.Font = new Font(this.Font.FontFamily, 11, this.Font.Style); ;
            result.ForeColor = Color.White;
            result.BackColor = Color.FromArgb(intAlpha, Color.IndianRed);
            //result.BackColor = System.Drawing.Color.Transparent;

            //::::::::::::::::::::::::::

            return result;
        }

        //....................................

        private void initCanvas(Panel inCanvas)
        {
            inCanvas.BackColor  = System.Drawing.Color.Transparent;
            inCanvas.MouseDown  += canvas_MouseDown;
            inCanvas.MouseMove  += canvas_MouseMove;
            inCanvas.MouseLeave += canvas_MouseLeave;
        }

        //....................................

        private Panel createCanvas()
        {
            Panel result = new Panel();

            //::::::::::::::::::::::::::

            result.Name = @"canvas";
            result.Dock = DockStyle.Fill;
            result.BackColor = System.Drawing.Color.Transparent;
            result.MouseDown  += canvas_MouseDown;
            result.MouseMove  += canvas_MouseMove;
            result.MouseLeave += canvas_MouseLeave;

            //::::::::::::::::::::::::::

            return result;
        }

        //....................................

        private void resizeCanvas()
        {
            canvas.Left = 0;
            canvas.Top = 0;

            //canvas.Width = this.panelWorkshop.Width;
            //canvas.Height = this.panelWorkshop.Height;
        }

        //....................................

        private void refreshCanvas(bool inRefreshLocationList)
        {
            this.panelCanvas.BackgroundImage = null;

            editingFloorPlan.refreshTablesInLocation();
            refreshLocationView(editingFloorPlan.tablesInLocation);

            scaleTableViews(canvas, editingFloorPlan.currentLocation);
            resetTimer(bckImgTimer);
            clearSelectedTableView();

            if (inRefreshLocationList)
            {
                 selectObjectInListBox(editingFloorPlan.indexOfLocation(editingFloorPlan.currentLocation), listBoxMenuLocations);
            }
            this.textBoxCanvasHeaderCurrentLocation.Text = editingFloorPlan.currentLocation.Name;

            this.panelMenuLocationsDel.Enabled = true;
            this.panelMenuLocationsBckImg.Enabled = true;

            this.listBoxMenuTables.Enabled = posIntegrationOK() && (editingFloorPlan.tablesInLocation.Length > 0);
            //this.comboBoxMenuTables.Enabled = posIntegrationOK() && (editingFloorPlan.tablesInLocation.Length > 0);
        }

        //....................................

        private Boolean posIntegrationOK()
        {
            try
            {
                bool result = !editingFloorPlan.posIntegrationFailed;
                return result;
            }
            catch(Exception)
            {
                return false;
            }
        }

        //....................................

        private void refreshBckImage()
        {
            try
            {
                checkBckImageSize(editingFloorPlan.currentLocation);
                //this.panelCanvas.BackgroundImage = Helper.byteArrayToImage(editingFloorPlan.currentLocation.BackgroundImg);
            }
            catch
            {
                //this.panelCanvas.BackgroundImage = null;
            }
        }

        //...................................

        private DTOReservable[] getTablesInLocation(DTOLocation inLocation)
        {
            try
            {
                return editingFloorPlan.getTablesInLocation(inLocation);
            }
            catch
            {
                throw; // TO DO: Review this!!!
            }
        }

        //....................................

        private void saveCurrentLocation()
        {
            try
            {
                editingFloorPlan.saveCurrentLocation();
                MessageBox.Show(string.Format("{0} has been saved", editingFloorPlan.currentLocation.Name));
            }
            catch (PosIntegrationException exc)
            {
                MessageBox.Show(exc.Message);
            }
        }

        //....................................

        private void giveUpSaving()
        {
            editingFloorPlan.giveUpSaving();
        }

        //....................................

        private void resetLocationView()
        {
            canvas.Controls.Clear();
            this.listBoxMenuTables.Items.Clear();
        }

        //....................................

        private void refreshLocationView(DTOReservable[] inTablesInLocation)
        {
            resetLocationView();

            foreach (DTOReservable table in inTablesInLocation)
            {
                switch (table.Shape)
                {
                    case @"r":
                        showSquareTableInCanvas(canvas, table);
                        break;

                    case @"e":
                        showRoundTableInCanvas(canvas, table);
                        break;
                }
                addItemToListBox(listBoxMenuTables, table.Name);
            }
        }

        //....................................

        private void clearSelectedTableView()
        {
            editingFloorPlan.clearCurrentTable();

            try
            {
                setTableViewAtIndex(-1, false);
            }
            catch (PosIntegrationException e)
            {
                MessageBox.Show(e.Message);
            }
        }

        //....................................

        private void setTableViewAtIndex(int inIndex, bool inRefreshTableList)
        {
            if (currentTableView != null)
            {
                selectTableView(currentTableView, false);
                resetRectTracker();
            }

            if (inIndex >= 0)
            {
                setCurrentTableAtIndex(inIndex);
                setRectTracker(currentTableView);

                if (this.listBoxMenuTables.SelectedIndex != inIndex)
                {
                    this.listBoxMenuTables.SelectedIndex = inIndex;
                }

                this.textBoxCanvasHeaderSelectedTableName.Text = editingFloorPlan.currentTable.Name;

                this.textBoxCanvasHeaderSelectedTableName.Enabled = true;
                this.panelMenuTablesDel.Enabled = true;

                showFloatingBtnForm();

                //------------------------------------
/*
                if (currentTableView != null)
                {
                    selectTableView(currentTableView, true);
                }

                setCurrentTableAtIndex(inIndex);

                if(this.listBoxMenuTables.SelectedIndex == inIndex)
                {
                    if (!currentTableView.Selected)
                    {
                        setRectTracker(currentTableView);
                    }
                }
                else
                {
                    resetRectTracker();

                    //.........................

                    this.listBoxMenuTables.SelectedIndex = inIndex;
                    setTableViewAtIndex(this.listBoxMenuTables.SelectedIndex, false);

                    //.................................

                    setRectTracker(canvas.Controls[this.listBoxMenuTables.SelectedIndex]);
                }
*/
            }
            else
            {
                //this.comboBoxMenuTables.Text = @"None";
                selectObjectInListBox(-1, listBoxMenuTables);
                this.textBoxCanvasHeaderSelectedTableName.Text = @"No Table Selected";

                this.textBoxCanvasHeaderSelectedTableName.Enabled = false;
                this.panelMenuTablesDel.Enabled = false;

                hideFloatingBtnForm();
            }
        }

        //....................................

        private void setLocationViewAtIndex(int inIndex, bool inRefreshLocationList)
        {
            canvas.Visible = false;

            //...................................

            Boolean locationGotSet = true;

            try
            {
                editingFloorPlan.setCurrentLocationAtIndex(inIndex, true);
            }
            catch (UnsavedLocationException)
            {
                unsavedLocationExceptionHandler();
                editingFloorPlan.setCurrentLocationAtIndex(inIndex, false);
            }
            catch (UnsavedTablesException)
            {
                unsavedTableExceptionHandler();
                editingFloorPlan.setCurrentLocationAtIndex(inIndex, false);
            }
            catch (IndexOutOfRangeException)
            {
                indexOutOfRangeExceptionHandler();
                locationGotSet = false;
            }
            catch (PosIntegrationException exc)
            {
                locationGotSet = false;
                MessageBox.Show(exc.Message);
            }

            if (locationGotSet)
            {
                try
                {
                    refreshCanvas(inRefreshLocationList);
                }
                catch (PosIntegrationException exc)
                {
                    MessageBox.Show(exc.Message);
                }
            }

            //...................................

            canvas.Visible = true;
        }

        //....................................

        private void unselectAllTableViews()
        {
            foreach (Control control in canvas.Controls)
            {
                if (control is ReservableTableView)
                {
                    ((ReservableTableView)control).Selected = false;
                }
            }
        }

        //....................................

        private void selectTableView(ReservableTableView inTableView, Boolean inSelected)
        {
            if(inSelected)
            {
                foreach (Control control in canvas.Controls)
                {
                    if (control is ReservableTableView)
                    {
                        ReservableTableView tableView = (ReservableTableView)control;

                        if(tableView == inTableView)
                        {
                            tableView.Selected = inSelected;
                        }
                        else
                        {
                            tableView.Selected = !inSelected;
                        }
                    }
                }
            }
            else
            {
                inTableView.Selected = inSelected;
            }
        }

        //....................................

        private void selectTableView(ReservableTableView inTableView)
        {
            //resetRectTracker(); This is no longer needed here as it will be done in SelectedIndexChanged event

            //.................................

            if (!inTableView.Selected)
            {
                //setTableViewAtIndex(inTableView.TabIndex, true);

                setTableViewAtIndex(canvas.Controls.IndexOf(inTableView), true);
            }
            else
            {
                showFloatingBtnForm();
            }

            //.................................

            //setRectTracker(inTableView); This is no longer needed here as it will be done in SelectedIndexChanged event
        }

        //....................................

        private void setCurrentTableAtIndex(int inIndex)
        {
            try
            {
                editingFloorPlan.setCurrentTableAtIndex(inIndex);
                currentTableView = (ReservableTableView)canvas.Controls[inIndex];
            }
            catch
            {
                currentTableView = null;
            }
        }

        //....................................

        private void showSquareTableInCanvas(Control inCanvas, DTOReservable inTable)
        {
            addTableToControl(inCanvas, inTable);
        }

        //....................................

        private void showRoundTableInCanvas(Control inCanvas, DTOReservable inTable)
        {
            addRoundTableToControl(inCanvas, inTable);
        }

        //....................................

        private void addTableToControl(Control inControl, DTOReservable inTable)
        {
            ReservableTableView newTableView = new ReservableTableView();

            int tableCountInControl = inControl.Controls.Count;

            newTableView.Name = string.Format(@"table{0}", tableCountInControl);
            newTableView.Size = new System.Drawing.Size(TableWidth, TableHeight);

            int x = Convert.ToInt16(inTable.X);
            int y = Convert.ToInt16(inTable.Y);

            int width = Convert.ToInt16(inTable.Width);
            int height = Convert.ToInt16(inTable.Height);

            newTableView.Location = new System.Drawing.Point(x, y);
            newTableView.Size = new System.Drawing.Size(width, height);
            newTableView.TabIndex = tableCountInControl;
            newTableView.Caption = inTable.Name;
            newTableView.TabIndex = tableCountInControl;

            newTableView.MouseDown += canvasTable_MouseDown;

            scaleTableView(inControl, editingFloorPlan.currentLocation, inTable, newTableView);
            selectTableView(newTableView, false);

            //.................................

            inControl.Controls.Add(newTableView);
        }

        //....................................

        private void addRoundTableToControl(Control inControl, DTOReservable inTable)
        {
            ReservableRoundTableView newTable = new ReservableRoundTableView();

            int tableCountInControl = inControl.Controls.Count;

            newTable.Name = string.Format(@"table{0}", tableCountInControl);
            newTable.Size = new System.Drawing.Size(TableWidth, TableHeight);

            int x = Convert.ToInt16(inTable.X);
            int y = Convert.ToInt16(inTable.Y);

            newTable.Location = new System.Drawing.Point(x, y);
            newTable.TabIndex = tableCountInControl;
            newTable.Caption = inTable.Name;
            newTable.TabIndex = tableCountInControl;

            newTable.MouseDown += canvasTable_MouseDown;

            selectTableView(newTable, false);

            //.................................

            inControl.Controls.Add(newTable);
        }

        //....................................

        private ReservableTableView addTableToCanvas(Control inCanvas, DTOLocation inLocation, DTOReservable inNewTable)
        {
            switch (inNewTable.Shape)
            {
                case @"r":
                    showSquareTableInCanvas(inCanvas, inNewTable);
                    break;

                case @"e":
                    showRoundTableInCanvas(inCanvas, inNewTable);
                    break;
            }

            ReservableTableView newTableView = (ReservableTableView)(inCanvas.Controls[inCanvas.Controls.Count - 1]);
            scaleTableView(inCanvas, inLocation, inNewTable, newTableView);

            //addItemsToComboBox(comboBoxMenuTables, inNewTable.Name);
            addItemToListBox(listBoxMenuTables, inNewTable.Name);

            return newTableView;
        }

        //....................................

        private void refreshCanvasXY(double inMouseX, double inMouseY)
        {
            labelCanvasBottomMouseXPosValue.Text = string.Format(@"{0}", inMouseX);
            labelCanvasBottomMouseYPosValue.Text = string.Format(@"{0}", inMouseY);
        }

        //....................................

        private void refreshStatusBar(ReservableTableView inTableView)
        {
            labelCanvasBottomCurrentTableName.Text = string.Format(@"{0}", inTableView.Caption);

            labelCanvasBottomCurrentTableXPosValue.Text = string.Format(@"{0}", inTableView.Left);
            labelCanvasBottomCurrentTableYPosValue.Text = string.Format(@"{0}", inTableView.Top);

            labelCanvasBottomCurrentTableWidthValue.Text = string.Format(@"{0}", inTableView.Width);
            labelCanvasBottomCurrentTableHeightValue.Text = string.Format(@"{0}", inTableView.Height);
        }

        //....................................

        private void clearStatusBar()
        {
            labelCanvasBottomCurrentTableName.Text = @"";

            labelCanvasBottomCurrentTableXPosValue.Text = @"";
            labelCanvasBottomCurrentTableYPosValue.Text = @"";

            labelCanvasBottomCurrentTableWidthValue.Text = @"";
            labelCanvasBottomCurrentTableHeightValue.Text = @"";
        }

        //....................................

        private void resetRectTracker()
        {
            if (rectTracker != null)
            {
                canvas.Controls.Remove(rectTracker);

                rectTracker = null;
                GC.Collect();
            }
        }

        //....................................

        private void setRectTracker(Control inControl)
        {
            rectTracker = new RectTracker(inControl);
            rectTracker.MouseUp += new System.Windows.Forms.MouseEventHandler(this.RectTracker_MouseUp);
            rectTracker.MouseMove += new System.Windows.Forms.MouseEventHandler(this.RectTracker_MouseMove);

            canvas.Controls.Add(rectTracker);
        }
        #endregion

        //....................................

        #region PRIVATE METHODS: Canvas' Tools
        private double CanvasCenterX()
        {
            return canvas.Width / 2.0;
        }

        //....................................

        private double CanvasCenterY()
        {
            return canvas.Height / 2.0;
        }

        //....................................

        private double LocationCenterX(DTOLocation inLocation)
        {
            return inLocation.Width / 2.0;
        }

        //....................................

        private double LocationCenterY(DTOLocation inLocation)
        {
            return inLocation.Height / 2.0;
        }

        //....................................

        private void showFloatingBtnForm()
        {
            ReservableTableView tableView = currentTableView;

            ((MainForm)this.MdiParent).showFloatingBtnForm(tableView.Caption, editingFloorPlan.currentTable.Number, tableView.Width, tableView.Height);
        }

        //....................................

        private void hideFloatingBtnForm()
        {
            ((MainForm)this.MdiParent).hideFloatingBtnForm();
        }

        //....................................

        private void enableCloseLink(bool inEnable)
        {
            this.linkLabelMenuClose.Enabled = inEnable;
            this.panelMenuClose.Enabled = inEnable;
        }

        //....................................

        private void updateCurrentTableName(string inName)
        {
            try
            {
                DTOReservable table = editingFloorPlan.currentTable;

                if (table.Name != inName)
                {
                    table.Name = inName;
                    editingFloorPlan.signalTablesChanged();

                    currentTableView.Caption = inName;

                    listBoxMenuTables.Items.Clear();

                    foreach (DTOReservable tb in editingFloorPlan.tablesInLocation)
                    {
                        addItemToListBox(listBoxMenuTables, tb.Name);
                    }

                    listBoxMenuTables.SelectedIndex = indexOfString(inName, listBoxMenuTables.Items);
                }
            }
            catch
            {
            }
        }

        //....................................

        private void updateCurrentTableNumber(int inNumber)
        {
            try
            {
                DTOReservable table = editingFloorPlan.currentTable;

                if (table.Number != inNumber)
                {
                    table.Number = inNumber;
                    editingFloorPlan.signalTablesChanged();
                }
            }
            catch
            {
            }
        }

        //....................................

        private void textBoxCanvasHeaderCurrentLocation_KeyPress(object sender, KeyPressEventArgs e)
        {
            currentLocationTypingIn = true;
        }

        //....................................

        private void textBoxCanvasHeaderCurrentLocation_Leave(object sender, EventArgs e)
        {
            currentLocationTypingIn = false;
        }
        #endregion

        //....................................

        #region PRIVATE METHODS: Scaling
        private double resolutionRatio(DTOLocation inLocation, Control inCanvas)
        {
            if(inLocation.Width >= inLocation.Height)
            {
                return inLocation.Width / inLocation.Height;
            }
                
            return inLocation.Height / inLocation.Width;
        }

        //....................................

        private void calculateImgDimensionsInCanvas(DTOLocation inLocation, Control inCanvas, ref double inImgWidth, ref double inImgHeight)
        {
            if (inLocation.Width >= inLocation.Height)
            {
                calculateWideImgDimensionsInCanvas(inLocation, inCanvas, ref inImgWidth, ref inImgHeight);
            }
            else
            {
                calculateTallImgDimensionsInCanvas(inLocation, inCanvas, ref inImgWidth, ref inImgHeight);
            }
        }

        //....................................

        private void calculateWideImgDimensionsInCanvas(DTOLocation inLocation, Control inCanvas, ref double inImgWidth, ref double inImgHeight)
        {
            double locationRatio = resolutionRatio(inLocation, inCanvas);

            if (inCanvas.Width >= inCanvas.Height)
            {
                inImgHeight = inCanvas.Height;
                inImgWidth = inImgHeight * locationRatio;

                if (inImgWidth > inCanvas.Width)
                {
                    inImgWidth = inCanvas.Width;
                    inImgHeight = inImgWidth / locationRatio;
                }
            }
            else
            {
                inImgWidth = inCanvas.Width;
                inImgHeight = inImgWidth / locationRatio;
            }
        }

        //....................................

        private void calculateTallImgDimensionsInCanvas(DTOLocation inLocation, Control inCanvas, ref double inImgWidth, ref double inImgHeight)
        {
            double locationRatio = resolutionRatio(inLocation, inCanvas);

            if (inCanvas.Width >= inCanvas.Height)
            {
                inImgHeight = inCanvas.Height;
                inImgWidth = inImgHeight / locationRatio;
            }
            else
            {
                inImgWidth = inCanvas.Width;
                inImgHeight = inImgWidth * locationRatio;
            }
        }

        //....................................

        private void calculateXYOrigin(Control inControl, double inXScale, double inYScale)
        {
            bckImgOrigin.Y = 0;
            bckImgOrigin.X = 0;

            switch (this.panelCanvas.BackgroundImageLayout)
            {
                case ImageLayout.Center:
                    {
                    }
                    break;

                case ImageLayout.None:
                    {
                    } break;

                case ImageLayout.Stretch:
                    {
                    } break;

                case ImageLayout.Tile:
                    {
                    } break;

                case ImageLayout.Zoom:
                    {
                        double imgWidth = 0;
                        double imgHeight = 0;

                        calculateImgDimensionsInCanvas(editingFloorPlan.currentLocation, inControl, ref imgWidth, ref imgHeight);

                        if (imgWidth == inControl.Width)
                        {
                            bckImgOrigin.Y = Convert.ToInt16((inControl.Height / 2) - (imgHeight / 2));
                            bckImgOrigin.X = 0;
                        }
                        else
                        {
                            bckImgOrigin.Y = 0;
                            bckImgOrigin.X = Convert.ToInt16((inControl.Width / 2) - (imgWidth / 2));                            
                        }
                    } break;
            }
        }

        //....................................

        private void calculateScaleFactors(Control inCanvas, DTOLocation inLocation, ref double outXScale, ref double outYScale)
        {
            int maxXC = inCanvas.Width;
            int maxYC = inCanvas.Height;

            double maxXL = inLocation.Width;
            double maxYL = inLocation.Height;

            double locationRatio = resolutionRatio(inLocation, inCanvas);

            switch (this.panelCanvas.BackgroundImageLayout)
            {
                case ImageLayout.Center:
                    {
                        maxXC = Convert.ToInt16(maxXL);
                        maxYC = Convert.ToInt16(maxYL);
                    }
                    break;

                case ImageLayout.None:
                    {
                        maxXC = Convert.ToInt16(maxXL);
                        maxYC = Convert.ToInt16(maxYL);
                    } break;

                case ImageLayout.Stretch:
                    {
                        maxXC = inCanvas.Width;
                        maxYC = inCanvas.Height;
                    } break;

                case ImageLayout.Tile:
                    {
                        maxXC = Convert.ToInt16(maxXL);
                        maxYC = Convert.ToInt16(maxYL);
                    } break;

                case ImageLayout.Zoom:
                    {
                        double imgWidth = 0;
                        double imgHeight = 0;

                        calculateImgDimensionsInCanvas(editingFloorPlan.currentLocation, inCanvas, ref imgWidth, ref imgHeight);

                        maxXC = Convert.ToInt16(imgWidth);
                        maxYC = Convert.ToInt16(imgHeight);
                    } break;
            }

            outXScale = maxXC / maxXL;
            outYScale = maxYC / maxYL;
        }

        //....................................

        private void scaleTableViews(Control inCanvas, DTOLocation inLocation)
        {
            DTOReservable[] tablesInLocation = getTablesInLocation(inLocation);

            double xScale = 1;
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);
            calculateXYOrigin(inCanvas, xScale, yScale);

            int i = 0;
            foreach (Control tableView in inCanvas.Controls)
            {
                try
                {
                    DTOReservable table = tablesInLocation[i++];
                    //tableView.Left = Convert.ToInt16((table.X + bckImgOrigin.X) * xScale);
                    //tableView.Top = Convert.ToInt16((table.Y + bckImgOrigin.Y) * yScale);

                    tableView.Left = bckImgOrigin.X + Convert.ToInt16(table.X * xScale);
                    tableView.Top = bckImgOrigin.Y + Convert.ToInt16(table.Y * yScale);

                    tableView.Width = Convert.ToInt16(table.Width * xScale);
                    tableView.Height = Convert.ToInt16(table.Height * yScale);
                }
                catch
                {
                }
            }
        }

        //....................................

        private void scaleTableView(Control inCanvas, DTOLocation inLocation, DTOReservable inTable, ReservableTableView inTableView)
        {
            double xScale = 1;
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);
            calculateXYOrigin(inCanvas, xScale, yScale);

            try
            {
                //inTableView.Left = Convert.ToInt16((inTable.X + bckImgOrigin.X) * xScale);
                //inTableView.Top = Convert.ToInt16((inTable.Y + bckImgOrigin.Y) * yScale);

                inTableView.Left = bckImgOrigin.X + Convert.ToInt16(inTable.X * xScale);
                inTableView.Top = bckImgOrigin.Y + Convert.ToInt16(inTable.Y * yScale);

                inTableView.Width = Convert.ToInt16(inTable.Width * xScale);
                inTableView.Height = Convert.ToInt16(inTable.Height * yScale);
            }
            catch
            {
            }
        }

        //....................................

        private void scaleDTOTable(Control inCanvas, DTOLocation inLocation, DTOReservable inTable, ReservableTableView inTableView)
        {
            double xScale = 1;
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);
            calculateXYOrigin(inCanvas, xScale, yScale);

            try
            {
                inTable.X = (inTableView.Left - bckImgOrigin.X) / xScale;
                inTable.Y = (inTableView.Top - bckImgOrigin.Y) / yScale;

                inTable.Width = inTableView.Width / xScale;
                inTable.Height = inTableView.Height / yScale;
            }
            catch
            {
            }
        }
        #endregion

        //.....................................

        #region PRIVATE METHODS: panelMenu
        private void updateLocationNameList()
        {
            //comboBoxMenuLocations.Items.Clear();
            //foreach (DTOLocation loc in editingFloorPlan.locations)
            //{
            //    addItemsToComboBox(comboBoxMenuLocations, loc.Name);
            //}

            listBoxMenuLocations.Items.Clear();
            foreach (DTOLocation loc in editingFloorPlan.locations)
            {
                addItemToListBox(listBoxMenuLocations, loc.Name);
            }
        }

        //......................................

        private void addItemToListBox(ListBox inListBox, string inText)
        {
            inListBox.Items.Add(inText);
        }

        //......................................

        private void selectObjectInListBox(int inIndex, ListBox inListBoxMenuLocations)
        {
            try
            {
                listBoxMenuLocations.SelectedIndex = inIndex;
            }
            catch(Exception)
            {
                listBoxMenuLocations.SelectedIndex = -1;
            }
        }

        //......................................

        private int indexOf(object inObject, object[] inObjectList)
        {
            return Array.IndexOf(inObjectList, inObject);
        }

        //......................................

        private int indexOfString(string inStr, ListBox.ObjectCollection inObjectList)
        {
            int i = 0;
            foreach (string str in inObjectList)
            {
                if(str == inStr)
                {
                    return i;
                }
                i++;
            }

            return -1;
        }

        //......................................

        private void updateCurrentLocationName(string inName)
        {
            if (editingFloorPlan.currentLocation.Name != inName)
            {
                editingFloorPlan.currentLocation.Name = inName;
                editingFloorPlan.signalLocationChanged();
            }

            updateLocationNameList();

            //toolStripDropDownButtonLocations.Text = inName;
        }

        //........................................

        private LocationTypeForm createLocationTypeForm()
        {
            LocationTypeForm result = new LocationTypeForm();

            //........................

            //result.MdiParent = this.MdiParent;

            //........................

            return result;
        }

        //........................................

        private void addNewLocation()
        {
            string locName = @"";

            DialogResult dialogResult = Helper.InputBox(@"New Location", @"Location Name", ref locName);

            switch (dialogResult)
            {
                case DialogResult.OK:
                    if (!locationExists(locName))
                    {
                        if (editingFloorPlan.floorPlanChanged)
                        {
                            askAndSave();
                        }

                        //...............................

                        LocationTypeForm form = createLocationTypeForm();
                        dialogResult = form.ShowDialog();

                        DTOLocation newLoc = new DTOLocation();
                        newLoc.Name = locName;

                        if (form.imageOrientation == LocationTypeForm.ImageOrientation.Portrait)
                        {
                            setLocPortrait(newLoc, "", ImageFormat.Jpeg);
                        }
                        else
                        {
                            setLocLandscape(newLoc, "", ImageFormat.Jpeg);
                        }

                        editingFloorPlan.addNewLocation(newLoc);
                        initGUI(editingFloorPlan.locations, editingFloorPlan.locations.Length - 1);
                        editingFloorPlan.signalLocationChanged();
                    }
                    else
                    {
                        MessageBox.Show(string.Format(@"Location name {0} already exist.  Cannot duplicate location names.", locName), @"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    break;
            }
        }

        //.....................................

        private void deleteLocation()
        {
            if (doThis(string.Format(@"Delete location: {0}", editingFloorPlan.currentLocation.Name)))
            {
                try
                {
                    int index = editingFloorPlan.indexOfLocation(editingFloorPlan.currentLocation);

                    editingFloorPlan.removeCurrentLocation();
                    editingFloorPlan.signalLocationDeleted();

                    if (index < editingFloorPlan.locations.Length)
                    {
                        editingFloorPlan.setCurrentLocationAtIndex(index, false);
                    }
                    else if (editingFloorPlan.locations.Length > 0)
                    {
                        editingFloorPlan.setCurrentLocationAtIndex(editingFloorPlan.locations.Length - 1, false);
                    }


                    initGUI(editingFloorPlan.locations, editingFloorPlan.indexOfLocation(editingFloorPlan.currentLocation));
                }
                catch (NoCurrentLocationException)
                {
                    //TO DO: do something if needed!

                    DTOLocation[] emptyList = { };
                    initGUI(emptyList, 0);
                }
            }
        }

        //.....................................

        private Image formatImage(string inFileName)
        {
            Image img = Image.FromFile(openFileDialogLocImg.FileName);

            editingFloorPlan.currentLocation.Width = img.Width;
            editingFloorPlan.currentLocation.Height = img.Height;

            return img;
        }

        //......................................

        private void openImgFileDialog()
        {
            DialogResult dialogResult = openFileDialogLocImg.ShowDialog();

            switch (dialogResult)
            {
                case DialogResult.OK:
                    if(editingFloorPlan.currentLocation.Height > editingFloorPlan.currentLocation.Width)
                    {
                        setLocPortrait(editingFloorPlan.currentLocation, openFileDialogLocImg.FileName, ImageFormat.Jpeg);
                    }
                    else
                    {
                        setLocLandscape(editingFloorPlan.currentLocation, openFileDialogLocImg.FileName, ImageFormat.Jpeg);
                    }
                    editingFloorPlan.signalLocationChanged();

                    refreshBckImage();
                break;
            }

        }

        //......................................

        private ImageFormat fileFormatFromFilter(int inFilterIndex)
        {
            switch (inFilterIndex)
            {
                case 1: return ImageFormat.Bmp;
                case 2: return ImageFormat.Gif;
                case 3: return ImageFormat.Icon;
                case 4: return ImageFormat.Jpeg;
                case 5: return ImageFormat.Png;
                case 6: return ImageFormat.Tiff;
                default: return ImageFormat.Bmp;
            }
        }

        //.......................................

        private void addNewTable(DTOLocation inLocation, int inTableShape)
        {
            int tableNumber = getNextTableNumber(); 
            string tableName = string.Format(@"Table{0}", tableNumber);
            int tableShape = inTableShape;  // Table Shape: 0 => Square "e",  1 => Round "r"
            string[] tableShapes = { @"r", @"e" };

            DialogResult dialogResult = Helper.TableInputBox(@"New Table", @"Table Name", @"Table Number", ref tableName, ref tableShape, ref tableNumber);

            switch (dialogResult)
            {
                case DialogResult.OK:
                    resetRectTracker();

                    DTOReservable newTable = new DTOReservable();
                    newTable.Name = tableName;
                    newTable.Number = tableNumber;
                    newTable.Shape = tableShapes[tableShape];
                    newTable.X = LocationCenterX(inLocation) - (TableWidth / 2);
                    newTable.Y = LocationCenterY(inLocation) - (TableHeight / 2);
                    newTable.Width = TableWidth;
                    newTable.Height = TableHeight;

                    editingFloorPlan.addNewTable(newTable);
                    editingFloorPlan.signalTablesChanged();

                    selectTableView(addTableToCanvas(canvas, inLocation, newTable));
                    this.listBoxMenuTables.Enabled = true;
                    //this.comboBoxMenuTables.Enabled = true;

                    break;
            }
        }

        //.......................................

        private void checkTableNumbers()
        {
            getNextTableNumber();
        }

        //.......................................

        private int getNextTableNumber()
        {
            int result = Helper.nextTableNumber();

            //::::::::::::::::::::::::::::::

            if ((result < _MIN_TABLE_NUMBER) || (result > _MAX_TABLE_NUMBER))
            {
                Helper.reArrangeTableNumbers();
                result = Helper.nextTableNumber();

                MessageBox.Show(@"An invalid table number was found in the DB. The table numbers have been re-assigned", @"FloorPlan Editor", MessageBoxButtons.OK);
            }

            //::::::::::::::::::::::::::::::

            return result;
        }

        //.......................................

        private void deleteTable(ReservableTableView inTableView, DTOLocation inLocation, Control inCanvas)
        {
            if (inTableView != null)
            {
                DTOReservable tableDTO = editingFloorPlan.tablesInLocation[inTableView.TabIndex];

                if (doThis(string.Format(@"Delete table: {0}", tableDTO.Name)))
                {
                    inCanvas.Controls.Remove(inTableView);
                    editingFloorPlan.removeTable(tableDTO);
                    editingFloorPlan.signalTableDeleted();

                    resetRectTracker();
                    clearSelectedTableView();

                    listBoxMenuTables.Items.Clear();
                    foreach (DTOReservable table in editingFloorPlan.tablesInLocation)
                    {
                        addItemToListBox(listBoxMenuTables, table.Name);
                    }

                    this.listBoxMenuTables.Enabled = posIntegrationOK() && (editingFloorPlan.tablesInLocation.Length > 0);
                }
            }
        }

        //.......................................

        private void showVisualControl(Control inControl, bool inShow)
        {
            inControl.Visible = inShow;
        }
        #endregion

        //.....................................

        #region PRIVATE METHODS: I/O
        private Boolean locationExists(string inLocName)
        {
            Boolean result = false;

            //...........................

            foreach (DTOLocation loc in editingFloorPlan.locations)
            {
                if (loc.Name.ToUpper() == inLocName.ToUpper())
                {
                    result = true;
                    break;
                }
            }

            //...........................

            return result;
        }

        //.....................................

        private Boolean doThis(string inText)
        {
            return MessageBox.Show(inText, @"FloorPlan", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes;
        }

        //.....................................

        private void askAndSave()
        {
            if (editingFloorPlan.floorPlanChanged)
            {
                DialogResult dResult = MessageBox.Show(@"Save current changes", @"FloorPlan Editor", MessageBoxButtons.YesNo);

                switch (dResult)
                {
                    case DialogResult.Yes:
                        {
                            saveFP();
                        } break;
                    case DialogResult.No: editingFloorPlan.giveUpSaving(); break;
                }
            }
        }

        //.....................................

        private void saveFP()
        {
            try
            {
                editingFloorPlan.saveFP();
                MessageBox.Show(string.Format("{0} has been saved", editingFloorPlan.currentLocation.Name));
            }
            catch (NoCurrentLocationException) 
            {
                ;
            }
            catch (PosIntegrationException exc)
            {
                MessageBox.Show(exc.Message);
            }
        }
        #endregion

        //....................................

        #region PUBLIC METHODS: BtnForm

        public void setTableName(string inName)
        {
            updateCurrentTableName(inName);
        }

        //....................................

        public void setTableNumber(int inNumber)
        {
            updateCurrentTableNumber(inNumber);
        }

        //....................................

        public void setTableWidth(int inWidth)
        {
            currentTableView.Width = inWidth;
            refreshStatusBar(currentTableView);
        }

        //....................................

        public void setTableHeight(int inHeight)
        {
            currentTableView.Height = inHeight;
            refreshStatusBar(currentTableView);
        }

        //....................................

        public void startTableViewChanges()
        {
            resetRectTracker();
            //currentTableView.enableAntiFlicker = true;
        }

        //....................................

        public void finishTableViewChanges()
        {
            scaleDTOTable(canvas, editingFloorPlan.currentLocation, editingFloorPlan.tablesInLocation[currentTableView.TabIndex], currentTableView);

            editingFloorPlan.signalTablesChanged();
            refreshStatusBar(currentTableView);

            setRectTracker(currentTableView);
            //currentTableView.enableAntiFlicker = false;
            //currentTableView.Invalidate();

            showFloatingBtnForm();
        }

        //....................................

        public void incTableWidth(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Width + 1 + inOffSet;

                if (tmp >= 0)
                {
                    currentTableView.Width = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void decTableWidth(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Width - 1 - inOffSet;

                if (tmp >= 0)
                {
                    currentTableView.Width = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void incTableHeigth(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Height + 1 + inOffSet;

                if (tmp >= 0)
                {
                    currentTableView.Height = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void decTableHeigth(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Height - 1 - inOffSet;

                if (tmp >= 0)
                {
                    currentTableView.Height = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void moveTableLeft(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Left - 1 - inOffSet;

                if (tmp >= 0)
                {
                    currentTableView.Left = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void moveTableRight(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Left + 1 + inOffSet;

                if (tmp <= canvas.Width)
                {
                    currentTableView.Left = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void moveTableDown(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Top + 1 + inOffSet;

                if (tmp <= canvas.Height)
                {
                    currentTableView.Top = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }

        //....................................

        public void moveTableUp(int inOffSet)
        {
            try
            {
                int tmp = currentTableView.Top - 1 - inOffSet;

                if (tmp >= 0)
                {
                    currentTableView.Top = tmp;
                }

                refreshStatusBar(currentTableView);
            }
            catch
            {
            }
        }
        #endregion

        //.....................................

        #region PRIVATE METHODS: BckImage Size
        private void checkBckImageSize(DTOLocation inLocation)
        {
            try
            {
                Image img = Helper.byteArrayToImage(inLocation.BackgroundImg);

                if ((inLocation.Width != img.Width) || (inLocation.Height != img.Height))
                {
                    Size newSize = new Size(Convert.ToInt16(inLocation.Width), Convert.ToInt16(inLocation.Height));

                    editingFloorPlan.currentLocation.BackgroundImg = Helper.imageToByteArray(new Bitmap(img, newSize), ImageFormat.Jpeg);
                    editingFloorPlan.saveFP();
                }

                this.panelCanvas.BackgroundImage = Helper.byteArrayToImage(editingFloorPlan.currentLocation.BackgroundImg);
            }
            catch
            {
                //this.panelCanvas.BackgroundImage = null;
            }
        }

        //.....................................

        private void setLocLandscape(DTOLocation inLoc, string inImgName, ImageFormat inImgFormat)
        {
            try
            {
                setLocSizeAndBckImg(inLoc, inImgName, inImgFormat, BCK_LANDSCAPE_IMAGE_WIDTH, BCK_LANDSCAPE_IMAGE_HEIGHT);
            }
            catch (Exception)
            {
                setDefaultLandscape(inLoc);
            }
        }

        //........................................

        private void setLocPortrait(DTOLocation inLoc, string inImgName, ImageFormat inImgFormat)
        {
            try
            {
                setLocSizeAndBckImg(inLoc, inImgName, inImgFormat, BCK_PORTRAIT_IMAGE_WIDTH, BCK_PORTRAIT_IMAGE_HEIGHT);
            }
            catch (Exception)
            {
                setDefaultPortrait(inLoc);
            }
        }

        //........................................
 
        private void setLocSizeAndBckImg(DTOLocation inLoc, string inImgName, ImageFormat inImgFormat, int inWidth, int inHeight)
        {
            Image img = Image.FromFile(inImgName);

            Size newSize = new Size(inWidth, inHeight);
            Image newImg = new Bitmap(Helper.byteArrayToImage(Helper.imageToByteArray(img, inImgFormat)), newSize);

            inLoc.Width = newSize.Width;
            inLoc.Height = newSize.Height;
            inLoc.BackgroundImg = Helper.imageToByteArray(newImg, inImgFormat);
        }

        //........................................

        private void setDefaultPortrait(DTOLocation inLoc)
        {
            Image img =  Properties.Resources.BlankLOPLabeledSR;

            inLoc.Width = BCK_PORTRAIT_IMAGE_WIDTH;
            inLoc.Height = BCK_PORTRAIT_IMAGE_HEIGHT;
            inLoc.BackgroundImg = Helper.imageToByteArray(img, ImageFormat.Jpeg);
        }

        //........................................

        private void setDefaultLandscape(DTOLocation inLoc)
        {
            Image img = Properties.Resources.BlankLOLabeledSR;

            inLoc.Width = BCK_LANDSCAPE_IMAGE_WIDTH;
            inLoc.Height = BCK_LANDSCAPE_IMAGE_HEIGHT;
            inLoc.BackgroundImg = Helper.imageToByteArray(img, ImageFormat.Jpeg);
        }

        #endregion

        //:::::::::::::::::::::::::::::::::::::
        // Events
        //:::::::::::::::::::::::::::::::::::::

        #region EVENTS: FP Error Panel
        private void fpErrorPanel_Resize(object sender, EventArgs e)
        {
            Panel panel = (Panel)sender;
            Label label = (Label)panel.Controls[0];
            Button restartButton = (Button)panel.Controls[1];
            Button detailsButton = (Button)panel.Controls[2];

            using (Graphics g = CreateGraphics())
            {
                SizeF size = g.MeasureString(label.Text, label.Font, label.Width);
                label.Height = (int)Math.Ceiling(size.Height) + Convert.ToInt16(label.Font.Size);
                panel.Height = label.Height + 30 + 30;
                restartButton.Top = panel.Height - restartButton.Height - 30;
                detailsButton.Top = panel.Height - detailsButton.Height - 30;
                detailsButton.Left = panel.Width - detailsButton.Width - 10;

                if (detailsButton.Left < ( restartButton.Left + restartButton.Width))
                {
                    detailsButton.Top = restartButton.Top + restartButton.Height + 5;
                    detailsButton.Left = restartButton.Left;
                }
            }
        }

        //....................................

        private void restartBtn_Click(object sender, EventArgs e)
        {
            restartFloorPlan();
        }

        //....................................

        private void detailsBtn_Click(object sender, EventArgs e)
        {
           MessageBox.Show(FPErrorMsg, @"FloorPlan error details");
        }
        #endregion

        //....................................

        #region EVENTS: EditorForm
        private void EditorForm_Shown(object sender, EventArgs e)
        {
            initFloorPlan();
        }

        //....................................

        private void EditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (editingFloorPlan.floorPlanChanged)
                {
                    DialogResult dResult = MessageBox.Show(@"Save current changes", @"FloorPlan Editor", MessageBoxButtons.YesNoCancel);

                    switch (dResult)
                    {
                        case DialogResult.Yes: saveFP(); break;
                        case DialogResult.Cancel: e.Cancel = true; break;
                    }
                }
            }
            catch (PosIntegrationException exc)
            {
                MessageBox.Show(exc.Message);
            }
            catch (Exception)
            {
            }
        }
        #endregion

        //....................................

        #region EVENTS: panelMenu
        private void panelMenuClose_MouseClick(object sender, MouseEventArgs e)
        {
            this.MdiParent.Close();
        }

        //....................................
       
        private void listBoxMenuLocations_Click(object sender, EventArgs e)
        {
            try
            {
                if ((((ListBox)sender).SelectedIndex > -1) && (((ListBox)sender).SelectedIndex != editingFloorPlan.indexOfLocation(editingFloorPlan.currentLocation)))
                {
                    setLocationViewAtIndex(((ListBox)sender).SelectedIndex, false);
                }
            }
            catch(NoCurrentTableException)
            {
            }
        }

        //....................................

        private void listBoxMenuTables_Click(object sender, EventArgs e)
        {
            try
            {
                if (((ListBox)sender).SelectedIndex > -1)
                {
                    ReservableTableView tableView = (ReservableTableView)canvas.Controls[((ListBox)sender).SelectedIndex];

                    selectTableView(tableView);
                    refreshStatusBar(currentTableView);
                }
            }
            catch(Exception)
            {
            }
        }

        //....................................

        private void comboBoxMenuLocations_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        //....................................

        private void panelMenuLocationsDel_Click(object sender, EventArgs e)
        {
            deleteLocation();
        }

        //.....................................

        private void panelMenuLocationsBckImg_Click(object sender, EventArgs e)
        {
            openImgFileDialog();
        }

        //....................................

        private void panelMenuLocationsAdd_Click(object sender, EventArgs e)
        {
            addNewLocation();
        }

        //.....................................

        private void panelMenuTablesAdd_Click(object sender, EventArgs e)
        {
            showVisualControl(listBoxMenuAddTable, true);
        }

        //.....................................

        private void panelMenuTablesDel_Click(object sender, EventArgs e)
        {
            deleteTable(currentTableView, editingFloorPlan.currentLocation, canvas);
        }

        //.....................................

        private void listBoxMenuAddTable_MouseLeave(object sender, EventArgs e)
        {
            showVisualControl((Control)sender, false);
        }

        //.....................................

        private void listBoxMenuAddTable_Click(object sender, EventArgs e)
        {
            showVisualControl((Control)sender, false);

            switch (((ListBox)sender).SelectedIndex)
            {
                case 0: addNewTable(editingFloorPlan.currentLocation, 0); break;
                case 1: addNewTable(editingFloorPlan.currentLocation, 1); break;
            }
        }

        //....................................

        private void linkLabelMenuSave_Click(object sender, EventArgs e)
        {
            saveFP();
        }

        //.....................................

        private void listBoxMenuAddTable_Resize(object sender, EventArgs e)
        {
            ListBox lb = (ListBox)sender;

            lb.Left = 16;
            lb.Top = 255;
        }

        //....................................

        private void comboBoxMenuTables_SelectedIndexChanged(object sender, EventArgs e)
        {
        }
        #endregion

        //....................................

        #region EVENTS: panelCanvas
        private void panelCanvas_Resize(object sender, EventArgs e)
        {
            try
            {
                bckImgGraphics = ((Control)sender).CreateGraphics();

                resetRectTracker();

                //.........................

                this.panelCanvas.BackgroundImage = null;

                resetTimer(bckImgTimer);

                resizeCanvas();
                scaleTableViews(canvas, editingFloorPlan.currentLocation);

                //.......................................

                if(currentTableView != null)
                {
                    //setTableViewAtIndex(currentTableView.TabIndex, true);

                    setTableViewAtIndex(canvas.Controls.IndexOf(currentTableView), true);
                    setRectTracker(canvas.Controls[currentTableView.TabIndex]);
                }
            }
            catch
            {
            }
        }
        #endregion

        //....................................

        #region EVENTS: Canvas
        private void canvas_MouseMove(object sender, MouseEventArgs e)
        {
            refreshCanvasXY(e.X, e.Y);
        }

        //....................................

        private void canvas_MouseDown(object sender, MouseEventArgs e)
        {
            clearStatusBar();
            resetRectTracker();
            clearSelectedTableView();
        }

        //.....................................

        private void canvas_MouseLeave(object sender, EventArgs e)
        {
            refreshCanvasXY(0, 0);
        }

        //.....................................

        private void textBoxCanvasHeaderCurrentLocation_TextChanged(object sender, EventArgs e)
        {
            TextBox textBox = (TextBox)sender;

            if (currentLocationTypingIn)
            {
                updateCurrentLocationName(textBox.Text);
            }
            clearStatusBar();
        }

        //....................................

        private void textBoxCanvasHeaderSelectedTableName_TextChanged(object sender, EventArgs e)
        {
            TextBox textBox = (TextBox)sender;

            updateCurrentTableName(textBox.Text);
        }
        #endregion

        //....................................

        #region EVENTS: Canvas Table
        private void canvasTable_MouseDown(object sender, MouseEventArgs e)
        {
            selectTableView((ReservableTableView)sender);
            refreshStatusBar(currentTableView);
        }
        #endregion

        //....................................

        #region EVENTS: RecTracker
        private void RectTracker_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            scaleDTOTable(canvas, editingFloorPlan.currentLocation, editingFloorPlan.tablesInLocation[currentTableView.TabIndex], currentTableView);

            editingFloorPlan.signalTablesChanged();
            refreshStatusBar(currentTableView);

            currentTableView.Width = currentTableView.Width;
        }

        //....................................

        private void RectTracker_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            refreshStatusBar(currentTableView);
        }

        #endregion

        private void panelMenuLocationsDel_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
