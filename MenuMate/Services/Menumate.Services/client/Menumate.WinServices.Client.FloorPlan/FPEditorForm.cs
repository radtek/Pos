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
    public partial class FPEditorForm : Form
    {
        EditingFloorPlan editingFloorPlan;

        ReservableTableView currentTableView;

        Panel canvas;
        RectTracker rectTracker;

        Int16 BtnWidth = 90;
        Int16 BtnHeight = 60;

        Int16 TableWidth = 30;
        Int16 TableHeight = 40;

        Graphics bckImgGraphics;
        Point bckImgOrigin;
        Timer bckImgTimer;
        Int16 bckImgRefreshTime = 250; // 250 mSec after finishing resizing

        Timer tableNameChangeTimer;
        Int16 tableNameChangeRefreshTime = 500; // 500 mSec after stop typing

        public FPEditorForm()
        {
            InitializeComponent();

            //::::::::::::::::::::::::::::::::::

            //initFloorPlan();
        }

        public void initFloorPlan()
        {
            try
            {
                bckImgTimer = Helper.createTimer(bckImgRefreshTime);
                bckImgTimer.Tick += new EventHandler(bckImgTimer_Tick);
                //bckImgTimer.Stop();

                tableNameChangeTimer = Helper.createTimer(tableNameChangeRefreshTime);
                tableNameChangeTimer.Tick += new EventHandler(tableNameChangeTimer_Tick);
                //tableNameChangeTimer.Stop();

                canvas = createCanvas();
                this.panelWorkshop.Controls.Add(canvas);

                //...........................

                editingFloorPlan = EditingFloorPlan.Instance;

                initGUI(editingFloorPlan.locations, 0);
            }
            catch (PosIntegrationException exc)
            {
                initGUI(null, 0);
                MessageBox.Show(exc.Message);
            }
        }

        private Timer createTimer(int inInterval)
        {
            Timer result = new Timer();

            result.Interval = inInterval;

            return result;
        }

        private void resetTimer(Timer inTimer)
        {
            inTimer.Stop();
            inTimer.Start();
        }

        void bckImgTimer_Tick(object sender, EventArgs e)
        {
            Timer timer = (Timer)sender;
            timer.Stop();

            refreshBckImage();

            refreshSelectedTableView();
        }

        void tableNameChangeTimer_Tick(object sender, EventArgs e)
        {
            Timer timer = (Timer)sender;
            timer.Stop();

            updateCurrentTableName(toolStripTextBoxTable.Text);
        }

        private Panel createCanvas()
        {
            Panel result = new Panel();

            //::::::::::::::::::::::::::

            result.Name = @"canvas";
            result.Dock = DockStyle.Fill;
            result.BackColor = System.Drawing.Color.Transparent;
            result.MouseDown += canvas_MouseDown;
            result.MouseMove += canvas_MouseMove;

            //::::::::::::::::::::::::::

            return result;
        }

        private void resizeCanvas()
        {
            canvas.Left = 0;
            canvas.Top = 0;

            canvas.Width = this.panelWorkshop.Width;
            canvas.Height = this.panelWorkshop.Height;
        }

        private Boolean posIntegrationOK()
        {
           return !editingFloorPlan.posIntegrationFailed;
        }

        private void initGUI(DTOLocation[] inLocations, int inLocIndex)
        {
            toolStripContainerWorkshop.Dock = DockStyle.Fill;

            if (inLocations == null)
            {
                resetLocationView();
            }
            else
            {
                toolStripDropDownButtonLocations.DropDownItems.Clear();

                if (inLocations.Length > 0)
                {
                    if (inLocIndex >= inLocations.Length)
                    {
                        inLocIndex = inLocations.Length - 1;
                    }

                    setLocationViewAtIndex(inLocIndex);
                }
                else
                {
                    resetLocationView();
                }
            }

            this.toolStripDropDownButtonLocations.Enabled = posIntegrationOK() && (editingFloorPlan.locations.Length > 0);
            this.toolStripTextBoxLocation.Enabled = posIntegrationOK() && (editingFloorPlan.locations.Length > 0);
            this.toolStripButtonNewLocation.Enabled = posIntegrationOK();
            this.toolStripButtonDelLocation.Enabled = posIntegrationOK() && (editingFloorPlan.locations.Length > 0);
            this.toolStripButtonLocImg.Enabled = posIntegrationOK() && (editingFloorPlan.locations.Length > 0);

            this.toolStripDropDownButtonTables.Enabled = posIntegrationOK() && (editingFloorPlan.tablesInLocation.Length > 0);
            this.toolStripTextBoxTable.Enabled = false;
            this.toolStripButtonNewTable.Enabled = posIntegrationOK() && (editingFloorPlan.locations.Length > 0);
            this.toolStripButtonDelTable.Enabled = false;

            this.toolStripButtonSave.Enabled = true;
        }

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

        private void setLocationViewAtIndex(int inIndex)
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
                DialogResult dResult = MessageBox.Show(@"Save current location", @"FloorPlan Editor", MessageBoxButtons.YesNo);

                if (dResult == DialogResult.Yes)
                {
                    saveCurrentLocation();
                }
                else
                {
                    giveUpSaving();
                }

                editingFloorPlan.setCurrentLocationAtIndex(inIndex, false);
            }
            catch (UnsavedTablesException)
            {
                DialogResult dResult = MessageBox.Show(@"Save tables for current location", @"FloorPlan Editor", MessageBoxButtons.YesNo);

                if (dResult == DialogResult.Yes)
                {
                    try
                    {
                        editingFloorPlan.saveTables();
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

                editingFloorPlan.setCurrentLocationAtIndex(inIndex, false);
            }
            catch (IndexOutOfRangeException)
            {
                this.toolStripDropDownButtonLocations.Text = @"";
                this.toolStripTextBoxLocation.Text = @"";

                this.toolStripButtonDelLocation.Enabled = false;
                this.toolStripButtonLocImg.Enabled = false;

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
                    editingFloorPlan.refreshTablesInLocation();
                    refreshLocationView(editingFloorPlan.tablesInLocation);

                    scaleTableViews(canvas, editingFloorPlan.currentLocation);

                    resetTimer(bckImgTimer);

                    this.toolStripDropDownButtonLocations.Text = editingFloorPlan.currentLocation.Name;
                    this.toolStripTextBoxLocation.Text = editingFloorPlan.currentLocation.Name;

                    this.toolStripButtonDelLocation.Enabled = true;
                    this.toolStripButtonLocImg.Enabled = true;

                    clearSelectedTableView();
                }
                catch (PosIntegrationException exc)
                {
                     MessageBox.Show(exc.Message);
                }
            }

            //...................................

            canvas.Visible = true;
        }

        private void saveCurrentLocation()
        {
            try
            {
                editingFloorPlan.saveCurrentLocation();
            }
            catch (PosIntegrationException exc)
            {
                MessageBox.Show(exc.Message);
            }
        }

        private void giveUpSaving()
        {
            editingFloorPlan.giveUpSaving();
        }

        private void clearSelectedTableView()
        {
            editingFloorPlan.clearCurrentTable();

            try
            {
                setTableViewAtIndex(-1);
            }
            catch (PosIntegrationException e)
            {
                MessageBox.Show(e.Message);
            }
        }

        private void setTableViewAtIndex(int inIndex)
        {
            if (inIndex >= 0)
            {
                if (currentTableView != null)
                {
                    selectTableView(currentTableView, false);
                }

                setCurrentTableAtIndex(inIndex);
                selectTableView(currentTableView, true);

                this.toolStripDropDownButtonTables.Text = editingFloorPlan.currentTable.Name;
                this.toolStripTextBoxTable.Text = editingFloorPlan.currentTable.Name;
                this.toolStripTextBoxTable.ToolTipText = @"Edit Table Name";

                this.toolStripTextBoxTable.Enabled = true;
                this.toolStripButtonDelTable.Enabled = true;

                showFloatingBtnForm();
            }
            else
            {
                if (currentTableView != null)
                {
                    selectTableView(currentTableView, false);
                }

                setCurrentTableAtIndex(inIndex);

                this.toolStripDropDownButtonTables.Text = @"None";
                this.toolStripTextBoxTable.Text = @"";
                this.toolStripTextBoxTable.ToolTipText = @"No selected table to edit";

                this.toolStripTextBoxTable.Enabled = false;
                this.toolStripButtonDelTable.Enabled = false;

                hideFloatingBtnForm();
            }
        }

        private void resetLocationView()
        {
            canvas.Controls.Clear();
            toolStripDropDownButtonTables.DropDownItems.Clear();
        }

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
                addItemsToToolStripDropDownButton(toolStripDropDownButtonTables, table.Name);
                //addItemsToToolStripDropDownButton(toolStripDropDownButtonTables, string.Format(@"{0} - {1}", table.Name, table.Number));
            }
        }

        private void selectTableView(ReservableTableView inTableView, Boolean inSelected)
        {
            inTableView.Selected = inSelected;
        }

        private void calculateXYOrigin(Control inControl)
        {
            switch (panelWorkshop.BackgroundImageLayout)
            {
                case ImageLayout.Center:
                    {
                        if (editingFloorPlan.currentLocation.Width < inControl.Width)
                        {
                            bckImgOrigin.X = (inControl.Width / 2) - Convert.ToInt16(editingFloorPlan.currentLocation.Width / 2);
                        }
                        else
                        {
                            bckImgOrigin.X = 0;
                        }

                        if (editingFloorPlan.currentLocation.Height < inControl.Height)
                        {
                            bckImgOrigin.Y = (inControl.Height / 2) - Convert.ToInt16(editingFloorPlan.currentLocation.Height / 2);
                        }
                        else
                        {
                            bckImgOrigin.Y = 0;
                        }
                    }
                    break;

                case ImageLayout.None:
                    {
                        bckImgOrigin.Y = 0;
                        bckImgOrigin.X = 0;
                    } break;

                case ImageLayout.Stretch:
                    {
                        bckImgOrigin.Y = 0;
                        bckImgOrigin.X = 0;
                    } break;

                case ImageLayout.Tile:
                    {
                        bckImgOrigin.Y = 0;
                        bckImgOrigin.X = 0;
                    } break;

                case ImageLayout.Zoom:
                    {
                        double maxClientHeight = inControl.Height;
                        double maxClientWidth = inControl.Width;

                        if (editingFloorPlan.currentLocation.Width < editingFloorPlan.currentLocation.Height)
                        {
                            double actualClientHeight = maxClientHeight;
                            double actualClientWidth = maxClientHeight / 1.33333333;

                            bckImgOrigin.Y = 0;
                            bckImgOrigin.X = Convert.ToInt16((maxClientWidth / 2)) - Convert.ToInt16((actualClientWidth / 2));
                        }
                        else
                        {
                            double actualClientHeight = maxClientHeight;
                            double actualClientWidth = maxClientHeight * 1.33333333;

                            bckImgOrigin.Y = 0;
                            bckImgOrigin.X = Convert.ToInt16((maxClientWidth / 2)) - Convert.ToInt16((actualClientWidth / 2));
                        }
                    } break;
            }
        }

        private void calculateScaleFactors(Control inCanvas, DTOLocation inLocation, ref double outXScale, ref double outYScale)
        {
            int maxXC = inCanvas.Width;
            int maxYC = inCanvas.Height;

            double maxXL = inLocation.Width;
            double maxYL = inLocation.Height;

            switch (panelWorkshop.BackgroundImageLayout)
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
                        if (editingFloorPlan.currentLocation.Width < editingFloorPlan.currentLocation.Height)
                        {
                            maxXC = Convert.ToInt16(inCanvas.Height / 1.33333333);
                            maxYC = inCanvas.Height; 
                        }
                        else
                        {
                            maxXC = Convert.ToInt16(inCanvas.Height * 1.33333333);
                            maxYC = inCanvas.Height;
                        }
                    } break;
            }

            outXScale = maxXC / maxXL;
            outYScale = maxYC / maxYL;

            calculateXYOrigin(panelWorkshop);
        }

        private void scaleTableViews(Control inCanvas, DTOLocation inLocation)
        {
            clearBckImgage();

            DTOReservable[] tablesInLocation = getTablesInLocation(inLocation);

            double xScale = 1; 
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);

            int i = 0;
            foreach (Control tableView in inCanvas.Controls)
            {
                try
                {
                    DTOReservable table = tablesInLocation[i++];
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

        private void scaleTableView(Control inCanvas, DTOLocation inLocation, DTOReservable inTable, ReservableTableView inTableView)
        {
            double xScale = 1;
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);

            try
            {
                inTableView.Left = bckImgOrigin.X + Convert.ToInt16(inTable.X * xScale);
                inTableView.Top = bckImgOrigin.Y + Convert.ToInt16(inTable.Y * yScale);

                inTableView.Width = Convert.ToInt16(inTable.Width * xScale);
                inTableView.Height = Convert.ToInt16(inTable.Height * yScale);
            }
            catch
            {
            }
        }

        private void scaleDTOTable(Control inCanvas, DTOLocation inLocation, DTOReservable inTable, ReservableTableView inTableView)
        {
            double xScale = 1;
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);

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

        private void clearBckImgage()
        {
            this.panelWorkshop.BackgroundImage = null;
        }

        private void refreshBckImage()
        {
            try
            {
                this.panelWorkshop.BackgroundImage = Helper.byteArrayToImage(editingFloorPlan.currentLocation.BackgroundImg);

                Size newSize = new Size(Convert.ToInt16(editingFloorPlan.currentLocation.Width), Convert.ToInt16(editingFloorPlan.currentLocation.Height));
                Image newImg = new Bitmap(this.panelWorkshop.BackgroundImage, newSize);

                this.panelWorkshop.BackgroundImage = newImg;

                editingFloorPlan.saveCurrentLocation();
            }
            catch
            {
                this.panelWorkshop.BackgroundImage = null;
            }
        }

        private void refreshSelectedTableView()
        {
            int index = canvas.Controls.IndexOf(currentTableView);

            if (index >= 0)
            {
                setTableViewAtIndex(index);

                //.................................

                setRectTracker(canvas.Controls[index]);
            }
        }


        private void showSquareTableInCanvas(Control inCanvas, DTOReservable inTable)
        {
            addTableToControl(inCanvas, inTable);

            //addTableToControl(inCanvas, inTable, @"round square lightgray lightocean 90x72 T.png");
            //addTableToControl(inCanvas, inTable, @"C:\software\source\menumate\5.3\MenuMate\Services\Menumate.Services\client\img\png\round square lightgray lightocean 90x72 T.png");
        }

        private void showRoundTableInCanvas(Control inCanvas, DTOReservable inTable)
        {
            addRoundTableToControl(inCanvas, inTable);

            //addTableToControl(inCanvas, inTable, @"circle lightgray orange 90x72 T.png");
        }

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

        private void addItemsToToolStripDropDownButton(ToolStripDropDownButton inDropDownButton, string inText)
        {
            inDropDownButton.DropDownItems.Add(inText);
        }

        private void updateCurrentLocationName(string inName)
        {
            if (editingFloorPlan.currentLocation.Name != inName)
            {
                editingFloorPlan.currentLocation.Name = inName;
                editingFloorPlan.signalLocationChanged();
            }

            toolStripDropDownButtonLocations.DropDownItems.Clear();
            foreach (DTOLocation loc in editingFloorPlan.locations)
            {
                addItemsToToolStripDropDownButton(toolStripDropDownButtonLocations, loc.Name);
            }

            toolStripDropDownButtonLocations.Text = inName;
        }

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

                    toolStripDropDownButtonTables.DropDownItems.Clear();

                    foreach (DTOReservable tb in editingFloorPlan.tablesInLocation)
                    {
                        addItemsToToolStripDropDownButton(toolStripDropDownButtonTables, tb.Name); 
                        //addItemsToToolStripDropDownButton(toolStripDropDownButtonTables, string.Format(@"{0} - {1}", tb.Name, tb.Number));
                    }

                    toolStripDropDownButtonTables.Text = inName;
                }
            }
            catch
            {
            }
        }

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

        private void selectTableView(ReservableTableView inTableView)
        {
            resetRectTracker();

            //.................................

            if (!inTableView.Selected)
            {
                setTableViewAtIndex(inTableView.TabIndex);
            }
            else
            {
                showFloatingBtnForm();
            }

            //.................................

            setRectTracker(inTableView);
        }

        private void showFloatingBtnForm()
        {
            ReservableTableView tableView = currentTableView;

            ((MainForm)this.MdiParent).showFloatingBtnForm(tableView.Caption, editingFloorPlan.currentTable.Number, tableView.Width, tableView.Height);
        }

        private void hideFloatingBtnForm()
        {
            ((MainForm)this.MdiParent).hideFloatingBtnForm();
        }

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

            addItemsToToolStripDropDownButton(toolStripDropDownButtonTables, inNewTable.Name);
 
            return newTableView;
        }

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

        private void refreshCanvasXY(double inMouseX, double inMouseY)
        {
            toolStripStatusLabelX.Text = string.Format(@"x: {0}", inMouseX);
            toolStripStatusLabelY.Text = string.Format(@"y: {0}", inMouseY);
        }

        private void refreshStatusBar(ReservableTableView inTableView)
        {
            //toolStripStatusLabelX.Text = string.Format(@"x: {0}", inTableView.Left);
            //toolStripStatusLabelY.Text = string.Format(@"y: {0}", inTableView.Top);

            toolStripStatusLabelTUpperCorner.Text = string.Format(@" |  Table: {0} ({1}, {2})", inTableView.Caption, inTableView.Left, inTableView.Top);
            toolStripStatusLabelWidth.Text = string.Format(@"Width: {0}", inTableView.Width);
            toolStripStatusLabelHeight.Text = string.Format(@"Height: {0}", inTableView.Height);
        }

        private void clearStatusBar()
        {
            //toolStripStatusLabelX.Text = @"";
            //toolStripStatusLabelY.Text = @"";

            toolStripStatusLabelTUpperCorner.Text = @"";
            toolStripStatusLabelWidth.Text = @"";
            toolStripStatusLabelHeight.Text = @"";
        }

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

        private Boolean doThis(string inText)
        {
            return MessageBox.Show(inText, @"FloorPlan", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes;
        }

        #region ToolStrip actions

        private void addNewLocation()
        {
            string locName = @"";

            DialogResult dialogResult = Helper.InputBox(@"New Location", @"Location Name", ref locName);

            switch (dialogResult)
            {
                case DialogResult.OK:
                    if (!locationExists(locName))
                    {
                        if(editingFloorPlan.floorPlanChanged)
                        {
                            askAndSave();
                        }

                        Image newImg = Image.FromFile(@"Blank.png");  

                        DTOLocation newLoc = new DTOLocation();
                        newLoc.Name = locName;
                        newLoc.Width = newImg.Width;
                        newLoc.Height = newImg.Height;

                        newLoc.BackgroundImg = Helper.imageToByteArray(newImg, ImageFormat.Jpeg);

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
                    else
                    {
                        if (editingFloorPlan.locations.Length > 0)
                        {
                            editingFloorPlan.setCurrentLocationAtIndex(editingFloorPlan.locations.Length - 1, false);
                        }
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

        private Image formatImage(string inFileName)
        {
            Image img = Image.FromFile(openFileDialogLocImg.FileName);

            editingFloorPlan.currentLocation.Width = img.Width;
            editingFloorPlan.currentLocation.Height = img.Height;

            return img;

            //Size newSize = new Size(Convert.ToInt16(editingFloorPlan.currentLocation.Width), Convert.ToInt16(editingFloorPlan.currentLocation.Height));
            //return new Bitmap(Image.FromFile(openFileDialogLocImg.FileName), newSize);
        }

        private void openImgFileDialog()
        {
            DialogResult dialogResult = openFileDialogLocImg.ShowDialog();

            switch (dialogResult)
            {
                case DialogResult.OK:
                    Image img = Image.FromFile(openFileDialogLocImg.FileName);

                    editingFloorPlan.currentLocation.BackgroundImg = Helper.imageToByteArray(img, fileFormatFromFilter(openFileDialogLocImg.FilterIndex));

                    editingFloorPlan.currentLocation.Width = img.Width;
                    editingFloorPlan.currentLocation.Height = img.Height;

                    editingFloorPlan.signalLocationChanged();

                    refreshBckImage();
                break;
            }

        }

        private void clearLocationImg()
        {
            DialogResult dResult = MessageBox.Show(@"Clear current location image", @"FloorPlan Editor", MessageBoxButtons.YesNo);

            switch (dResult)
            {
                case DialogResult.Yes:
                    {
                        editingFloorPlan.currentLocation.BackgroundImg = new byte[] { };
                        editingFloorPlan.signalLocationChanged();

                        refreshBckImage();
                    } break;
            }
        }

        private void addNewTable(DTOLocation inLocation, int inTableShape) 
        {
            int tableNumber = Helper.nextTableNumber();
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
                    newTable.X = 20;
                    newTable.Y = 20;
                    newTable.Width = TableWidth;
                    newTable.Height = TableHeight;

                    editingFloorPlan.addNewTable(newTable);
                    editingFloorPlan.signalTablesChanged();

                    selectTableView(addTableToCanvas(canvas, inLocation, newTable));
                    this.toolStripDropDownButtonTables.Enabled = true;

                    break;
            }
        }

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

                    this.toolStripDropDownButtonTables.Enabled = posIntegrationOK() && (editingFloorPlan.tablesInLocation.Length > 0);
                }
            }
        }

        private void saveFP()
        {
            try
            {
                editingFloorPlan.saveFP();
            }
            catch (PosIntegrationException exc)
            {
                MessageBox.Show(exc.Message);
            }
        }

        private void askAndSave()
        {
            if (editingFloorPlan.floorPlanChanged)
            {
                DialogResult dResult = MessageBox.Show(@"Save current changes", @"FloorPlan Editor", MessageBoxButtons.YesNo);

                switch (dResult)
                {
                    case DialogResult.Yes: saveFP(); break;
                    case DialogResult.No: editingFloorPlan.giveUpSaving(); break;
                }
            }
        }

        #endregion

        private void FPeditorForm_Shown(object sender, EventArgs e)
        {
            initFloorPlan();
        }

        #region Canvas Events

        private void canvas_MouseMove(object sender, MouseEventArgs e)
        {
            refreshCanvasXY(e.X, e.Y);
        }

        private void canvas_MouseDown(object sender, MouseEventArgs e)
        {
            resetRectTracker();
            clearSelectedTableView();
            clearStatusBar();
        }

        private void canvasTable_MouseDown(object sender, MouseEventArgs e)
        {
            selectTableView((ReservableTableView)sender);
            refreshStatusBar(currentTableView);
            //currentTableView.enableAntiFlicker = true;

            /*
                        resetRectTracker();

                        //.................................

                        Control control = (Control)sender;

                        ReservableTableView tableView = (ReservableTableView)sender;

                        if (!tableView.Selected)
                        {
                            setTableViewAtIndex(((Control)sender).TabIndex);
                        }

                        //.................................

                        setRectTracker(control);
            */
        }

        private void resetRectTracker()
        {
            canvas.Controls.Remove(rectTracker);
        }

        private void setRectTracker(Control inControl)
        {
            rectTracker = new RectTracker(inControl);
            rectTracker.MouseUp += new System.Windows.Forms.MouseEventHandler(this.RectTracker_MouseUp);
            rectTracker.MouseMove += new System.Windows.Forms.MouseEventHandler(this.RectTracker_MouseMove);

            canvas.Controls.Add(rectTracker);
        }

        private void RectTracker_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            scaleDTOTable(canvas, editingFloorPlan.currentLocation, editingFloorPlan.tablesInLocation[currentTableView.TabIndex], currentTableView);

            editingFloorPlan.signalTablesChanged();
            refreshStatusBar(currentTableView);

            //currentTableView.enableAntiFlicker = false;
            currentTableView.Width = currentTableView.Width;
        }

        private void RectTracker_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            refreshStatusBar(currentTableView);
        }

        #endregion

        private void panelWorkshop_Paint(object sender, PaintEventArgs e)
        {
            //bckImgGraphics = new Graphics();
            //e.Graphics;
        }

        private void panelWorkshop_Resize(object sender, EventArgs e)
        {
            try
            {
                bckImgGraphics = ((Control)sender).CreateGraphics();

                resetRectTracker();

                //.........................

                resetTimer(bckImgTimer);

                resizeCanvas();
                scaleTableViews(canvas, editingFloorPlan.currentLocation);
            }
            catch
            {
            }
        }

        #region ToolStrip

        private void btnQuit_Click(object sender, EventArgs e)
        {
            this.MdiParent.Close();
        }

        private void toolStripDropDownButtonLocations_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            ToolStripDropDownButton btn = (ToolStripDropDownButton)sender;
            int index = btn.DropDownItems.IndexOf(e.ClickedItem);

            setLocationViewAtIndex(index);
        }

        private void toolStripDropDownButtonTables_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            resetRectTracker();

            //.........................

            ToolStripDropDownButton btn = (ToolStripDropDownButton)sender;
            int index = btn.DropDownItems.IndexOf(e.ClickedItem);

            setTableViewAtIndex(index);

            //.................................

            setRectTracker(canvas.Controls[index]);
        }

        private void toolStripTextBoxLocation_TextChanged(object sender, EventArgs e)
        {
            ToolStripTextBox tsTextBox = (ToolStripTextBox)sender;

            updateCurrentLocationName(tsTextBox.Text);
            clearStatusBar();
        }

        private void toolStripDropDownButtonTables_TextChanged(object sender, EventArgs e)
        {
            resetTimer(tableNameChangeTimer);
        }

        private void toolStripButtonNewLocation_Click(object sender, EventArgs e)
        {
            addNewLocation();
        }

        private void toolStripButtonDelLocation_Click(object sender, EventArgs e)
        {
            deleteLocation();
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openImgFileDialog();
        }

        private void clearImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            clearLocationImg();
        }

        private void squareTableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            addNewTable(editingFloorPlan.currentLocation, 0);
        }

        private void roundTableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            addNewTable(editingFloorPlan.currentLocation, 1);
        }

        private void toolStripButtonNewTable_Click(object sender, EventArgs e)
        {
            
        }

        private void toolStripButtonDelTable_Click(object sender, EventArgs e)
        {
            deleteTable(currentTableView, editingFloorPlan.currentLocation, canvas);
        }

        private void toolStripButtonSave_Click(object sender, EventArgs e)
        {
            saveFP();
        }

        #endregion

        #region BtnForm

        public void setTableName(string inName)
        {
            updateCurrentTableName(inName);
        }

        public void setTableNumber(int inNumber)
        {
            updateCurrentTableNumber(inNumber);
        }

        public void setTableWidth(int inWidth)
        {
            currentTableView.Width = inWidth;
            refreshStatusBar(currentTableView);
        }

        public void setTableHeight(int inHeight)
        {
            currentTableView.Height = inHeight;
            refreshStatusBar(currentTableView);
        }
        
        public void startTableViewChanges()
        {
            resetRectTracker();
            //currentTableView.enableAntiFlicker = true;
        }

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

        private void FPEditorForm_FormClosing(object sender, FormClosingEventArgs e)
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
            catch(PosIntegrationException exc)
            {
                MessageBox.Show(exc.Message);
            }
            catch (Exception)
            {
            }
        }
    }
}
