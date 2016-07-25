using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using FloorPlanClient;

namespace FloorPlanIntegration
{
    public partial class MainForm : Form
    {
        bool _editorSaved = true;

        bool _locationAttrSubmitted = true;
        bool _tableAttrSubmitted = true;

        UInt32 _MIN_TABLE_NUMBER = 1;
        UInt32 _MAX_TABLE_NUMBER = UInt32.MaxValue;

        FloorPlanManager _floorPlanManager = FloorPlanManager.Instance;
        LocationInfoCanvasManager _canvasManager = LocationInfoCanvasManager.Instance;

        public MainForm()
        {
            InitializeComponent();
        }

        #region Properties

        public FloorPlanRunner FPlanRunner
        {
            get
            {
                return _floorPlanManager.FPlanRunner;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int LocationCount
        {
            get
            {
                return FPlanRunner.LocationCount;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public LocationInfo[] LocationList
        {
            get
            {
                return FPlanRunner.LocationList;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo LocationInUse
        {
            get
            {
                return FPlanRunner.LocationInUse;
            }
            set
            {
                FPlanRunner.LocationInUse = value;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int IndexOfLocationInUse
        {
            get
            {
                return FPlanRunner.IndexOfLocationInUse;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int TableCount
        {
            get
            {
                return FPlanRunner.TableCount;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private TableInfo[] TableList
        {
            get
            {
                return FPlanRunner.TableList;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private TableInfo SelectedTable
        {
            get
            {
                return FPlanRunner.SelectedTable;
            }
            set
            {
                FPlanRunner.SelectedTable = value;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int IndexOfSelectedTable
        {
            get
            {
                return FPlanRunner.IndexOfSelectedTable;
            }
        }

        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            pInitFPEditor();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            pCheckAndSave();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAddLocation_Click(object sender, EventArgs e)
        {
            pCheckAndSubmitLocationAttr();
            pAddLocationAndShow();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnEditLocation_Click(object sender, EventArgs e)
        {
            pCheckAndShowLocationInfo();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tvLocations_DoubleClick(object sender, EventArgs e)
        {
            if(pSelectedNode(tvLocations.SelectedNode) != null)
            {
                pCheckAndShowLocationInfo();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDelLocation_Click(object sender, EventArgs e)
        {
            pDelLocationAtIndex(IndexOfLocationInUse);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSave_Click(object sender, EventArgs e)
        {
             pSaveLocationInfo();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbLocationName_TextChanged(object sender, EventArgs e)
        {
            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnBckImage_Click(object sender, EventArgs e)
        {
            pChangeLocationBackImage();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnNewSquareTable_Click(object sender, EventArgs e)
        {
            pAddTableAndShow(TableShape.Rectangle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnNewRoundTable_Click(object sender, EventArgs e)
        {
            pAddTableAndShow(TableShape.Elipse);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSelectTable_Click(object sender, EventArgs e)
        {
            pCheckAndShowTableInfo();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tvTables_DoubleClick(object sender, EventArgs e)
        {
            if (pSelectedNode(tvTables.SelectedNode) != null)
            {
                pCheckAndShowTableInfo();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDelTable_Click(object sender, EventArgs e)
        {
            pDelTableAtIndex(IndexOfSelectedTable);
            pEnableSelectedTableAttrPanel(_canvasManager.Location.TableCount > 0);
            pCheckAndClearSelectedTableAttrPanel();

            pSetEditorSaved(false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            lbMouseX.Text = Convert.ToString(e.X);
            lbMouseY.Text = Convert.ToString(e.Y);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rbPortrait_Click(object sender, EventArgs e)
        {
            LocationInUse.BackImageOrientation = ImageOrientation.Portrait;
            pSetCanvasBckImage();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rbLandscape_Click(object sender, EventArgs e)
        {
            LocationInUse.BackImageOrientation = ImageOrientation.Lanscape;
            pSetCanvasBckImage();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbLocationName_KeyUp(object sender, KeyEventArgs e)
        {
            pSetLocationAttrSubmitted(false);
            pSetEditorSaved(false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbTableNumber_KeyDown(object sender, KeyEventArgs e)
        {
            e.SuppressKeyPress = !Helper.keyIsANumber(e.KeyCode) &&
                                 !Helper.keyIsBackSpace(e.KeyCode) &&
                                 !Helper.keyIsDelete(e.KeyCode);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbTableName_KeyUp(object sender, KeyEventArgs e)
        {
            pSetTableAttrSubmitted(false);
            pSetEditorSaved(false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLocationSubmit_Click(object sender, EventArgs e)
        {
            pSubmitLocationAttr();
            pPopulateLocationTreeView(tvLocations, LocationList);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnTableSubmit_Click(object sender, EventArgs e)
        {
            pSubmitSelectedTableAttr();
            pPopulateTableTreeView(tvTables, LocationInUse.TableList);
        }

        #region Private

        /// <summary>
        /// 
        /// </summary>
        private void pInitFPEditor()
        {
            lbVersion.Text = @"Version VERSTR";

            if(pInitFPIntegration())
            {
                pStartFPEditor();
            }
            else
            {
                pShowFPFailedToStart();
                pEnableFP(false);
            }

            pSetLocationAttrSubmitted(true);
            pSetTableAttrSubmitted(true);
        }

        /// <summary>
        /// 
        /// </summary>
        private void pStartFPEditor()
        {
            _canvasManager.Canvas = pCanvas;
            _canvasManager.BackImageChanged   += CanvasBackImageChanged;
            _canvasManager.TableViewChanged   += CanvasTableViewChanged;
            _canvasManager.TableViewSelecting += CanvasTableViewSelecting;
            _canvasManager.TableViewSelected  += CanvasTableViewSelected;

            if(LocationCount > 0)
            {
                pPopulateLocationTreeView(tvLocations, LocationList);
                pSelectNodeInTreeView(tvLocations, IndexOfLocationInUse);

                pShowLocationInfo(LocationInUse);
                pSetEditorSaved(true);
            }
            else
            {
                pInitFPEditorWithNoLocation();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pShowFPFailedToStart()
        {
            lbFailedToConnectToFPService.Text = @"Failed to connect to Floor Plan service. Click on the upper right corner button to close application.  Make sure POSIntegration service is running.";

            //pShowErrorMsg(@"Failed to connect to Floor Plan service ...");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEnabled"></param>
        private void pEnableFP(bool inEnabled)
        {
            tlpMain.Enabled = inEnabled;
        }

        /// <summary>
        /// 
        /// </summary>
        private void pInitFPEditorWithNoLocation()
        {
            LocationInUse = pAddNewLocation();

            pPopulateLocationTreeView(tvLocations, LocationList);
            pSelectNodeInTreeView(tvLocations, IndexOfLocationInUse);

            pShowLocationInfo(LocationInUse);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool pInitFPIntegration()
        {
            return _floorPlanManager.Init() == FloorPlanClientError.NoError;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CanvasBackImageChanged(object sender, EventArgs e)
        {
            pSetEditorSaved(false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CanvasTableViewChanged(object sender, TableViewEventArgs e)
        {
            pShowTableViewAttr(e.TableView, e.TableNumber);
            pSetEditorSaved(false);
        }

        /// <summary>
        /// This event is triggered just before a TableView is selected.
        /// Allows for any action before a TableView is selected e.g save the attributes
        /// of the previous selected TableView.
        /// The attributes in the e paramater correspond to the previous selected TableView
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CanvasTableViewSelecting(object sender, TableViewEventArgs e)
        {
            pCheckAndSubmitTableAttr(e);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CanvasTableViewSelected(object sender, TableViewEventArgs e)
        {
            pShowTableViewAttr(e.TableView, e.TableNumber);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableView"></param>
        /// <param name="inTableNumber"></param>
        private void pShowTableViewAttr(TableView inTableView, int inTableNumber)
        {
            pUpdateSelTablePos(inTableView.Left, inTableView.Top);

            tbTableX.Text      = Convert.ToString(inTableView.Left);
            tbTableY.Text      = Convert.ToString(inTableView.Top);
            tbTableWidth.Text  = Convert.ToString(inTableView.Width);
            tbTableHeight.Text = Convert.ToString(inTableView.Height);

            tbTableName.Text   = inTableView.Caption;
            tbTableNumber.Text = Convert.ToString(inTableNumber);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inList"></param>
        private void pPopulateLocationTreeView(TreeView inTreeView, LocationInfo[] inList)
        {
            inTreeView.Nodes.Clear();
            inTreeView.Nodes.Add(@"Locations");

            foreach (LocationInfo locItem in inList)
            {
                inTreeView.Nodes[0].Nodes.Add(locItem.Location);
            }

            inTreeView.TopNode.Expand();
        }

        /// <summary>
        /// 
        /// </summary>
        private void pAddLocationAndShow()
        {
            LocationInUse = pAddNewLocation();

            pPopulateLocationTreeView(tvLocations, LocationList);
            pShowLocationInfo(LocationInUse);
        }

        /// <summary>
        /// 
        /// </summary>
        private LocationInfo pAddNewLocation()
        {
            LocationInfo result = rbPortrait.Checked ? FPlanRunner.AddLocation(ImageOrientation.Portrait) : FPlanRunner.AddLocation(ImageOrientation.Lanscape);
        
            result.Location = @"My Location";
            
            pSetEditorSaved(false);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inIndex"></param>
        private void pDelLocationAtIndex(int inIndex)
        {
            FPlanRunner.DelLocation(pLocationAtIndex(inIndex));
            pDelTreeNodeAtIndex(inIndex, tvLocations);

            if (LocationCount == 0)
            {
                pAddLocationAndShow();
            }
            else
            {
                if (inIndex >= LocationCount)
                {
                    inIndex = LocationCount - 1;
                }

                pSelectNodeInTreeView(tvLocations, inIndex);

                LocationInUse = pLocationAtIndex(inIndex);
                pShowLocationInfo(LocationInUse);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inMakeItInUse"></param>
        private void pShowLocationInfo(LocationInfo inLocation)
        {
            tbLocationName.Text = inLocation.Location;

            pPopulateTableTreeView(tvTables, TableList);

            _canvasManager.Location = inLocation;

            pEnableSelectedTableAttrPanel(_canvasManager.Location.TableCount > 0);
            pCheckAndClearSelectedTableAttrPanel();
        }

        /// <summary>
        /// 
        /// </summary>
        private void pRefreshTablesInCanvas()
        {
            _canvasManager.RefreshTables();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImage"></param>
        private void pSetCanvasBckImage()
        {
            _canvasManager.RefreshBckImage();

            rbPortrait.Checked = LocationInUse.BackImageOrientation == ImageOrientation.Portrait;
            rbLandscape.Checked = !rbPortrait.Checked;
        }

        /// <summary>
        /// 
        /// </summary>
        private void pSaveLocationInfo()
        {
            FPlanRunner.SaveFP();

            pSetEditorSaved(true);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        /// <returns></returns>
        private LocationInfo pLocationAtIndex(int inIndex)
        {
            try
            {
                return LocationList[inIndex];
            }
            catch (Exception)
            {
                return null;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pChangeLocationBackImage()
        {
            DialogResult dialogResult = openFileDialogLocImg.ShowDialog();

            if (dialogResult == DialogResult.OK)
            {
                pRefreshLocationImage(Image.FromFile(openFileDialogLocImg.FileName));
                pRefreshLocationImage(LocationInUse.BackImage);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImage"></param>
        private void pRefreshLocationImage(Image inImage)
        {
            LocationInUse.BackImage = inImage;
            pSetCanvasBckImage();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inList"></param>
        private void pPopulateTableTreeView(TreeView inTreeView, TableInfo[] inList)
        {
            inTreeView.Nodes.Clear();
            inTreeView.Nodes.Add(@"Tables");

            foreach (TableInfo tableItem in inList)
            {
                inTreeView.Nodes[0].Nodes.Add(tableItem.Name);
            }

            inTreeView.TopNode.Expand();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inShape"></param>
        private void pAddTableAndShow(TableShape inShape)
        {
            if (TableCount <= _MAX_TABLE_NUMBER)
            {
                TableInfo table = pAddNewTable(inShape);

                pPopulateTableTreeView(tvTables, TableList);
                pShowTableInfo(table);

                pEnableSelectedTableAttrPanel(_canvasManager.Location.TableCount > 0);
                pCheckAndClearSelectedTableAttrPanel();

                pCheckForInvalidTableNumber(table);
            }
            else
            {
                pShowErrorMsg(string.Format(@"Failed to add a new table. You have reached the maximum amount of table for a location [{0}]", _MAX_TABLE_NUMBER));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private TableInfo pAddNewTable(TableShape inShape)
        {
            return (inShape == TableShape.Rectangle) ? pAddRectTable() : pAddElipseTable();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private TableInfo pAddRectTable()
        {
            TableInfo result = FPlanRunner.AddRectTable();
 
            pSetEditorSaved(false);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private TableInfo pAddElipseTable()
        {
            TableInfo result = FPlanRunner.AddElipseTable();

            pSetEditorSaved(false);

            return result;
        }

        /// <summary>
        /// Must be only used to check new table's number (just added to a location)
        /// </summary>
        /// <param name="inTableNumber"></param>
        private void pCheckForInvalidTableNumber(TableInfo inTable)
        {
            int tableNumber = inTable.Number;

            if ((tableNumber < _MIN_TABLE_NUMBER) || (tableNumber > _MAX_TABLE_NUMBER))
            {
                LocationInUse.ReArrangeTableNumbers();
                inTable.Number = LocationInUse.NextTableNumber();
                inTable.Name   = (inTable.Shape == TableShape.Rectangle) ? string.Format(@"S{0}", inTable.Number) : string.Format(@"R{0}", inTable.Number);

                pShowErrorMsg(@"An invalid table number was found in the DB. The table numbers have been re-assigned");
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableNumber"></param>
        /// <returns></returns>
        private bool pCheckForDuplicateTableNumber(int inTableNumber, TableInfo inTable)
        {
            return LocationInUse.SearchForDuplicateTableNumber(inTableNumber, inTable);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableNamer"></param>
        /// <param name="inTable"></param>
        /// <returns></returns>
        private bool pCheckForDuplicateTableName(string inTableName, TableInfo inTable)
        {
            return LocationInUse.SearchForDuplicateTableName(inTableName, inTable);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        private void pDelTableAtIndex(int inIndex)
        {
            try
            {
                TableInfo table = pTableAtIndex(inIndex);

                _canvasManager.RemoveTableFromCanvas(table);

                FPlanRunner.DelTable(table);
                pDelTreeNodeAtIndex(inIndex, tvTables);

                if (TableList.Length > 0)
                {
                    if (inIndex >= TableList.Length)
                    {
                        inIndex = TableList.Length - 1;
                    }

                    pSelectNodeInTreeView(tvTables, inIndex);

                    pSelectTable(pTableAtIndex(inIndex));
                }
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        /// <returns></returns>
        private TableInfo pTableAtIndex(int inIndex)
        {
            try
            {
                return TableList[inIndex];
            }
            catch (Exception)
            {
                return null;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        private void pShowTableInfo(TableInfo inTable)
        {
            _canvasManager.ShowTable(inTable);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        private void pSelectTable(TableInfo inTable)
        {
            SelectedTable = inTable;
 
            _canvasManager.SelectTable(SelectedTable);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTreeView"></param>
        /// <returns></returns>
        private int pIndexOfSelectedNode(TreeView inTreeView)
        {
            try
            {
                int result = -1;

                //:::::::::::::::::::::::::::::::

                result = inTreeView.TopNode.Nodes.IndexOf(pSelectedNode(inTreeView.SelectedNode));

                //:::::::::::::::::::::::::::::::

                return result;
            }
            catch (Exception)
            {
                return -1;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEnable"></param>
        private void pEnableSelectedTableAttrPanel(bool inEnabled)
        {
            gbTableAttributes.Enabled = inEnabled;
        }

        /// <summary>
        /// 
        /// </summary>
        private void pCheckAndClearSelectedTableAttrPanel()
        {
            if(TableCount == 0)
            {
                pClearSelectedTableAttrPanel();
                pClearSelectedTablePosPanel();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pClearSelectedTableAttrPanel()
        {
            tbTableName.Text   = @"";
            tbTableNumber.Text = @"";
            tbTableX.Text      = @"";
            tbTableY.Text      = @"";
            tbTableWidth.Text  = @"";
            tbTableHeight.Text = @"";
        }

        /// <summary>
        /// 
        /// </summary>
        private void pClearSelectedTablePosPanel()
        {
            lbSelTableX.Text = @"";
            lbSelTableY.Text = @"";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTreeView"></param>
        /// <param name="inCompanyIndex"></param>
        private void pSelectNodeInTreeView(TreeView inTreeView, int inNodeIndex)
        {
            try
            {
                inTreeView.SelectedNode = inTreeView.Nodes[0].Nodes[inNodeIndex];
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNode"></param>
        /// <returns></returns>
        private TreeNode pSelectedNode(TreeNode inNode)
        {
            return pIsHeadNode(inNode) ? null : inNode;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNode"></param>
        /// <returns></returns>
        private bool pIsHeadNode(TreeNode inNode)
        {
            return inNode.TreeView.TopNode == inNode;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        private void pDelTreeNodeAtIndex(int inIndex, TreeView inTreeView)
        {
            try
            {
                inTreeView.TopNode.Nodes.RemoveAt(inIndex);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pCheckAndShowLocationInfo()
        {
            if (!_editorSaved && pConfirmAction(string.Format(@"Save {0}", LocationInUse.Location)))
            {
                pSubmitLocationAttr();
                pSaveLocationInfo();
            }

            LocationInUse = pLocationAtIndex(pIndexOfSelectedNode(tvLocations));

            if (LocationInUse != null)
            {
                pShowLocationInfo(LocationInUse);
            }
            else
            {
                pShowLocationInfoError();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pCheckAndShowTableInfo()
        {
            TableInfo table = pTableAtIndex(pIndexOfSelectedNode(tvTables));
            pSelectTable(table);
        }

        /// <summary>
        /// 
        /// </summary>
        private void pCheckAndSave()
        {
            if(!_editorSaved && (MessageBox.Show(TextManager.Instance.SaveEditorText(), TextManager.Instance.TitleText(), MessageBoxButtons.YesNo) == DialogResult.Yes))
            {
                pSaveEditor();
            }
            pSetEditorSaved(true);
        }

        /// <summary>
        /// 
        /// </summary>
        private void pCheckAndSubmitLocationAttr()
        {
            if (!_tableAttrSubmitted && (MessageBox.Show(TextManager.Instance.SubmitLocationAttrText(), TextManager.Instance.TitleText(), MessageBoxButtons.YesNo) == DialogResult.Yes))
            {
                pSubmitLocationAttr();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pCheckAndSubmitTableAttr(TableViewEventArgs inTableAttr)
        {
            if (!_tableAttrSubmitted && (MessageBox.Show(TextManager.Instance.SubmitTableAttrText(), TextManager.Instance.TitleText(), MessageBoxButtons.YesNo) == DialogResult.Yes))
            {
                pSubmitTableAttr(inTableAttr);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pSaveEditor()
        {
           
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSaved"></param>
        private void pSetEditorSaved(bool inSaved)
        {
            _editorSaved = inSaved;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="?"></param>
        private void pSubmitLocationAttr()
        {
            LocationInUse.Location = tbLocationName.Text;

            pSetLocationAttrSubmitted(true);
        }

        /// <summary>
        /// This fuction is called from the btnSubmit click event only.
        /// It will submit the table view attributes' values 
        /// </summary>
        private void pSubmitSelectedTableAttr()
        {
            if (pCheckForDuplicateTableName(tbTableName.Text, SelectedTable))
            {
                pShowErrorMsg(@"The table name already exists. Cannot duplicate table names");
            }
            else
            {
                int tableNumber = Convert.ToInt16(tbTableNumber.Text);

                if (pCheckForDuplicateTableNumber(tableNumber, SelectedTable))
                {
                    pShowErrorMsg(@"The table number already exists. Cannot duplicate table numbers");
                }
                else
                {
                    _canvasManager.SubmitSelectedTableViewAttrs(
                        tbTableName.Text,
                        tableNumber,
                        Convert.ToInt16(tbTableX.Text),
                        Convert.ToInt16(tbTableY.Text),
                        Convert.ToInt16(tbTableWidth.Text),
                        Convert.ToInt16(tbTableHeight.Text));

                    pUpdateSelTablePos(Convert.ToInt16(tbTableX.Text), Convert.ToInt16(tbTableY.Text));

                    pSetTableAttrSubmitted(true);

                }
            }
        }

        /// <summary>
        /// This fuction is called from the TableViewSelecting event only.
        /// It will submit the table view attributes' values 
        /// </summary>
        private void pSubmitTableAttr(TableViewEventArgs inTableAttr)
        {
            inTableAttr.TableView.Caption = tbTableName.Text;
            inTableAttr.TableNumber       = Convert.ToInt16(tbTableNumber.Text);
            inTableAttr.TableView.Left    = Convert.ToInt16(tbTableX.Text);
            inTableAttr.TableView.Top     = Convert.ToInt16(tbTableY.Text);
            inTableAttr.TableView.Width   = Convert.ToInt16(tbTableWidth.Text);
            inTableAttr.TableView.Height  = Convert.ToInt16(tbTableHeight.Text);

            pUpdateSelTablePos(Convert.ToInt16(tbTableX.Text), Convert.ToInt16(tbTableY.Text));

            pSetTableAttrSubmitted(true);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSubmitted"></param>
        private void pSetLocationAttrSubmitted(bool inSubmitted)
        {
            _locationAttrSubmitted = inSubmitted;
            pEnableLocationSubmitBtn(!_locationAttrSubmitted);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEnable"></param>
        private void pEnableLocationSubmitBtn(bool inEnable)
        {
            btnLocationSubmit.Enabled   = inEnable;
            btnLocationSubmit.BackColor = inEnable ? Color.DarkOrange : Color.DimGray;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSaved"></param>
        private void pSetTableAttrSubmitted(bool inSubmitted)
        {
            _tableAttrSubmitted = inSubmitted;
            pEnableTableSubmitBtn(!_tableAttrSubmitted);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEnable"></param>
        private void pEnableTableSubmitBtn(bool inEnable)
        {
            btnTableSubmit.Enabled   = inEnable;
            btnTableSubmit.BackColor = inEnable ? Color.DarkOrange : Color.DimGray;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLeft"></param>
        /// <param name="inTop"></param>
        private void pUpdateSelTablePos(int inLeft, int inTop)
        {
            lbSelTableX.Text = Convert.ToString(inLeft);
            lbSelTableY.Text = Convert.ToString(inTop);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inText"></param>
        /// <returns></returns>
        private bool pConfirmAction(string inText)
        {
            return MessageBox.Show(inText, @"FloorPlan Editor", MessageBoxButtons.OKCancel) == DialogResult.OK;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inErrorMsg"></param>
        private void pShowErrorMsg(string inErrorMsg)
        {
            MessageBox.Show(inErrorMsg);
        }

        /// <summary>
        /// 
        /// </summary>
        private void pShowLocationInfoError()
        {
            pShowErrorMsg(@"No location selected");
        }

        #endregion


    }
}
