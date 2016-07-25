using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Forms;
using System.Drawing;

using System.Runtime.InteropServices;
using System.Runtime;

using FloorPlanClient;
using System.Threading;


namespace FloorPlanIntegration
{
    public enum CanvasManagerError
    {
        NoError,
        NullCanvas
    };

    public class CanvasManager
    {
        protected Control _canvas;
        protected Point _bckImgOrigin;
        protected CanvasManagerError _canvasManagerError;

        protected TableInfo _tableToAdd;
        protected TableView _tableViewToSelect;
        protected RectTracker _rectTracker;

        /// <summary>
        /// 
        /// </summary>
        protected CanvasManager()
        {
            _canvas = null;
            pResetCanvasManagerError();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCanvas"></param>
        protected CanvasManager(Control inCanvas)
        {
            _canvas = inCanvas;
            pResetCanvasManagerError();

            pSetCanvasResizeEvent();
        }

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public CanvasManagerError LastError
        {
            get
            {
                return _canvasManagerError;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public Control Canvas
        {
            get
            {
                return _canvas;
            }
            set
            {
                _canvas = value;

                pSetCanvasResizeEvent();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int InitPosX
        {
            get
            {
                return _bckImgOrigin.X;
            }
            set
            {
                _bckImgOrigin.X = value;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int InitPosY
        {
            get
            {
                return _bckImgOrigin.Y;
            }
            set
            {
                _bckImgOrigin.Y = value;
            }
        }

        #endregion

        protected void pSetCanvasResizeEvent()
        {
            try
            {
                _canvas.Resize -= CanvasResize;
            }
            catch
            {
            }
            _canvas.Resize += CanvasResize;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected virtual void CanvasResize(object sender, EventArgs e)
        {
            //
        }

        #region Private

        /// <summary>
        /// 
        /// </summary>
        protected void pResetCanvasManagerError()
        {
            _canvasManagerError = CanvasManagerError.NoError;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inError"></param>
        protected void pSetCanvasManagerError(CanvasManagerError inError)
        {
            _canvasManagerError = inError;
        }

        #endregion
    }

    // Summary:
    //     Provides data for the System.Windows.Forms.Control.MouseUp, System.Windows.Forms.Control.MouseDown,
    //     and System.Windows.Forms.Control.MouseMove events.
    [ComVisible(true)]
    public class LocationInfoEventArgs : EventArgs
    {
        LocationInfo _locationInfo;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocationInfo"></param>
        [TargetedPatchingOptOut("Performance critical to inline this type of method across NGen image boundaries")]
        public LocationInfoEventArgs(LocationInfo inLocationInfo)
        {
            _locationInfo = inLocationInfo;
        }

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo Location 
        { 
            get
            {
                return _locationInfo;
            }     
        }
    }

    // Summary:
    //     Provides data for the System.Windows.Forms.Control.MouseUp, System.Windows.Forms.Control.MouseDown,
    //     and System.Windows.Forms.Control.MouseMove events.
    [ComVisible(true)]
    public class TableViewEventArgs : EventArgs
    {
        TableView _tableView;
        int       _tableNumber;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocationInfo"></param>
        [TargetedPatchingOptOut("Performance critical to inline this type of method across NGen image boundaries")]
        public TableViewEventArgs(TableView inTableView, int inTableNumber)
        {
            _tableView = inTableView;
            _tableNumber = inTableNumber;
        }

        /// <summary>
        /// 
        /// </summary>
        public TableView TableView
        {
            get
            {
                return _tableView;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int TableNumber
        {
            get
            {
                return _tableNumber;
            }
            set
            {
                _tableNumber = value;
            }
        }
    }

    // A delegate type for hooking up location info change notifications.
    public delegate void LocationInfoChangedEventHandler(object sender, LocationInfoEventArgs e);

    // A delegate type for hooking up table view change notifications.
    public delegate void TableViewChangedEventHandler(object sender, TableViewEventArgs e);
    
    public class LocationInfoCanvasManager : CanvasManager
    {
        FloorPlanManager _floorPlanManager;
        LocationInfo _location;

        /// <summary>
        /// 
        /// </summary>
        LocationInfoCanvasManager(): base()
        {
            _location = null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCanvas"></param>
        /// <param name="inLocation"></param>
        LocationInfoCanvasManager(Control inCanvas, FloorPlanManager inFloorPlanManager): base(inCanvas)
        {
            FPManager = inFloorPlanManager;         }

        /// <summary>
        /// 
        /// </summary>
        public static LocationInfoCanvasManager Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly LocationInfoCanvasManager instance = new LocationInfoCanvasManager();
        }

        // An event that clients can use to be notified whenever the background image changes.
        public event LocationInfoChangedEventHandler BackImageChanged;

        // An event that clients can use to be notified whenever a table changes.
        public event TableViewChangedEventHandler TableViewChanged;

        // An event that clients can use to be notified before whenever a table is selected.
        public event TableViewChangedEventHandler TableViewSelecting;

        // An event that clients can use to be notified whenever a table is selected.
        public event TableViewChangedEventHandler TableViewSelected;

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public FloorPlanManager FPManager
        {
            get
            {
                return _floorPlanManager;
            }
            set
            {
                _floorPlanManager = value;

                try
                {
                    Location = _floorPlanManager.FPlanRunner.LocationInUse;
                }
                catch
                {
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo Location
        {
            get
            {
                return _location;
            }
            set
            {
                _location = value;

                try
                {
                     pShowLocationInCanvas(_location);
                }
                catch
                {
                }
            }
        }

        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected override void CanvasResize(object sender, EventArgs e)
        {
            pShowTables();
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public void RefreshBckImage()
        {
            pResetCanvasManagerError();
            try
            {
                Canvas.BackgroundImage = _location.BackImage;
                pShowTables();

                OnBackImageChanged(new LocationInfoEventArgs(_location));
            }
            catch
            {
                pSetCanvasManagerError(CanvasManagerError.NullCanvas);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public void RefreshTables()
        {
            pShowTables();
        }

        /// <summary>
        /// 
        /// </summary>
        public void ShowTable(TableInfo inTable)
        {
            pShowCanvas(false);

            pShowTableInCanvas(inTable);
            selectTableView(pTableViewWithTable(inTable), true); 

            pShowCanvas(true);    
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        public void SelectTable(TableInfo inTable)
        {
            pShowCanvas(false);

            selectTableView(pTableViewWithTable(inTable), true);

            pShowCanvas(true);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        public void RemoveTableFromCanvas(TableInfo inTable)
        {
            try
            {
                TableView tableView = pTableViewWithTable(inTable);

                if(inTable == Location.SelectedTable)
                {
                    resetRectTracker(); // Remove selecting rect
                }

                Canvas.Controls.Remove(tableView);

                pAssignTableViewIndexes();
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableName"></param>
        /// <param name="inTableNumber"></param>
        /// <param name="inLeft"></param>
        /// <param name="inTop"></param>
        /// <param name="inWidth"></param>
        /// <param name="inHeight"></param>
        public void SubmitSelectedTableViewAttrs(string inTableName, int inTableNumber, int inLeft, int inTop, int inWidth, int inHeight)
        {
            TableView tableView = pTableViewWithTable(Location.SelectedTable);

            Location.SelectedTable.Number = inTableNumber;

            tableView.Caption = inTableName;
            tableView.Left    = inLeft;
            tableView.Top     = inTop;
            tableView.Width   = inWidth;
            tableView.Height  = inHeight;

            scaleTable(Canvas, Location, Location.SelectedTable, tableView);

            Location.SelectedTable.Name   = inTableName;
            Location.SelectedTable.Number = inTableNumber;
        }

        #endregion

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        /// <returns></returns>
        private TableView pTableViewWithTable(TableInfo inTable)
        {
            try
            {
                return pTableViewAtIndex(pIndexOfTable(inTable));
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        private TableView pTableViewAtIndex(int inIndex)
        {
            TableView result = null;

            //::::::::::::::::::::::::::::::::::

            foreach (Control control in Canvas.Controls)
            {
                if((control is TableView) && (control.TabIndex == inIndex))
                {
                    result = (TableView)control;
                }
            }

            //::::::::::::::::::::::::::::::::::

            return result;
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
                return Location.TableList[inIndex];
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        /// <returns></returns>
        private int pIndexOfTable(TableInfo inTable)
        {
            return Location.IndexOfTable(inTable);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableView"></param>
        /// <returns></returns>
        private int pIndexOfTableView(TableView inTableView)
        {
            return Canvas.Controls.IndexOf(inTableView);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        private void pShowLocationInCanvas(LocationInfo inLocation)
        {
            pResetCanvasManagerError();
            try
            {
                Canvas.BackgroundImage = inLocation.BackImage;
                pShowTables();
            }
            catch
            {
                pSetCanvasManagerError(CanvasManagerError.NullCanvas);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pShowTables()
        {
            pShowCanvas(false);

            pShowTablesInCanvas(Location.TableList);

            if (Location.SelectedTable != null)
            {
                selectTableView(pTableViewWithTable(Location.SelectedTable), true);
            }
            else
            {
                if (Canvas.Controls.Count > 0)
                {
                    selectTableView(pTableViewAtIndex(0), true);
                }
            }

            pShowCanvas(true);
        }

        delegate void canvasFunc();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inShow"></param>
        private void pShowCanvas(bool inShow)
        {
            if (CanvasInvokeRequired())
            {
                if(inShow)
                {
                    Canvas.Invoke(new canvasFunc(ShowCanvas));
                }
                else
                {
                    Canvas.Invoke(new canvasFunc(HideCanvas));
                }
            }
            else
            {
                Canvas.Visible = inShow;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool CanvasInvokeRequired()
        {
            return Canvas.InvokeRequired;
        }

        /// <summary>
        /// 
        /// </summary>
        private void ShowCanvas()
        {
            Canvas.Visible = true;
            Application.DoEvents();
        }

        /// <summary>
        /// 
        /// </summary>
        private void HideCanvas()
        {
            Canvas.Visible = false;
            Application.DoEvents();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableList"></param>
        private void pShowTablesInCanvas(TableInfo[] inTableList)
        {
            pResetCanvasManagerError();
            try
            {
                pClearCanvas();

                if(_location.TableList.Length > 0)
                {
                    foreach (TableInfo table in _location.TableList)
                    {
                        pShowTableInCanvas(table);
                    }
                }

            }
            catch
            {
                pSetCanvasManagerError(CanvasManagerError.NullCanvas);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pClearCanvas()
        {
            pResetCanvasManagerError();
            try
            {
                if (CanvasInvokeRequired())
                {
                    Canvas.Invoke(new canvasFunc(ClearCanvas));
                }
                else
                {
                    ClearCanvas();
                }
            }
            catch
            {
                pSetCanvasManagerError(CanvasManagerError.NullCanvas);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void ClearCanvas()
        {
            resetRectTracker();

            while (Canvas.Controls.Count > 0)
            {
                Control control = Canvas.Controls[0];

                Canvas.Controls.Remove(control);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        private void pShowTableInCanvas(TableInfo inTable)
        {
            int tableIndex = pIndexOfTable(inTable);

            if((Canvas.Controls.Count == 0) || (tableIndex >= (Canvas.Controls.Count - 1)))
            {
                pAddTableToCanvas(inTable);
            }
            else
            {
                pRefreshTableInCanvas(tableIndex);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        private void pAddTableToCanvas(TableInfo inTable)
        {
            _tableToAdd = inTable;

            if (CanvasInvokeRequired())
            {
                Canvas.Invoke(new canvasFunc(AddTableToCanvas));
            }
            else
            {
                AddTableToCanvas();
             }                  
        }

        /// <summary>
        /// 
        /// </summary>
        private void AddTableToCanvas()
        {
            TableView newTableView = (_tableToAdd.Shape == TableShape.Rectangle) ? new TableView() : new RoundTableView();

            newTableView.TabIndex = pNextTableIndex(Canvas);  //Location.TableList.Length - 1;
            newTableView.Caption = _tableToAdd.Name;
            newTableView.MouseDown += MouseDownOnTableView;

            scaleTableView(Canvas, _location, _tableToAdd, newTableView);

            Canvas.Controls.Add(newTableView);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCanvas"></param>
        private int pNextTableIndex(Control inCanvas)
        {
            if (_rectTracker == null) // No Table selecetd
            {
                return inCanvas.Controls.Count;
            }
            else
            {
                return inCanvas.Controls.Count == 0 ? 0 : inCanvas.Controls.Count - 1;
            }            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        private void pRefreshTableInCanvas(int inIndex)
        {
            try
            {
                TableView tableView = pTableViewAtIndex(inIndex);
                TableInfo table     = pTableAtIndex(inIndex);

                tableView.Caption = table.Name;

                scaleTableView(Canvas, Location, table, tableView);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void pAssignTableViewIndexes()
        {
            int index = 0;
            foreach(Control control in Canvas.Controls)
            {
                if(control is TableView)
                {
                    control.TabIndex = index++;
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCanvas"></param>
        /// <param name="inLocation"></param>
        /// <param name="inTable"></param>
        /// <param name="inTableView"></param>
        private void scaleTableView(Control inCanvas, LocationInfo inLocation, TableInfo inTable, TableView inTableView)
        {
            double xScale = 1;
            double yScale = 1;

            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);
            calculateXYOrigin(inCanvas, xScale, yScale);

            try
            {
                inTableView.Left = _bckImgOrigin.X + Convert.ToInt16(inTable.PosX * xScale);
                inTableView.Top = _bckImgOrigin.Y + Convert.ToInt16(inTable.PosY * yScale);

                inTableView.Width = Convert.ToInt16(inTable.Width * xScale);
                inTableView.Height = Convert.ToInt16(inTable.Height * yScale);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCanvas"></param>
        /// <param name="inLocation"></param>
        /// <param name="outXScale"></param>
        /// <param name="outYScale"></param>
        private void calculateScaleFactors(Control inCanvas, LocationInfo inLocation, ref double outXScale, ref double outYScale)
        {
            int maxXC = inCanvas.Width;
            int maxYC = inCanvas.Height;

            double maxXL = inLocation.Width;
            double maxYL = inLocation.Height;

            double locationRatio = resolutionRatio(inLocation);

            switch (_canvas.BackgroundImageLayout)
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

                        calculateImgDimensionsInCanvas(inLocation, inCanvas, ref imgWidth, ref imgHeight);

                        maxXC = Convert.ToInt16(imgWidth);
                        maxYC = Convert.ToInt16(imgHeight);
                    } break;
            }

            outXScale = maxXC / maxXL;
            outYScale = maxYC / maxYL;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inCanvas"></param>
        /// <returns></returns>
        private double resolutionRatio(LocationInfo inLocation)
        {
            double width  = Convert.ToDouble(inLocation.Width);
            double height = Convert.ToDouble(inLocation.Height);

            if (width >= height)
            {
                return width / height;
            }

            return height / width;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inCanvas"></param>
        /// <param name="inImgWidth"></param>
        /// <param name="inImgHeight"></param>
        private void calculateImgDimensionsInCanvas(LocationInfo inLocation, Control inCanvas, ref double inImgWidth, ref double inImgHeight)
        {
            if(inLocation.BackImageOrientation == ImageOrientation.Lanscape)
            {
                calculateLandscapeImgDimensionsInCanvas(inLocation, inCanvas, ref inImgWidth, ref inImgHeight);
            }
            else
            {
                calculatePortraitImgDimensionsInCanvas(inLocation, inCanvas, ref inImgWidth, ref inImgHeight);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inCanvas"></param>
        /// <param name="inImgWidth"></param>
        /// <param name="inImgHeight"></param>
        private void calculateLandscapeImgDimensionsInCanvas(LocationInfo inLocation, Control inCanvas, ref double inImgWidth, ref double inImgHeight)
        {
            double locationRatio = resolutionRatio(inLocation);

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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inCanvas"></param>
        /// <param name="inImgWidth"></param>
        /// <param name="inImgHeight"></param>
        private void calculatePortraitImgDimensionsInCanvas(LocationInfo inLocation, Control inCanvas, ref double inImgWidth, ref double inImgHeight)
        {
            double locationRatio = resolutionRatio(inLocation);

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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inControl"></param>
        /// <param name="inXScale"></param>
        /// <param name="inYScale"></param>
        private void calculateXYOrigin(Control inControl, double inXScale, double inYScale)
        {
            _bckImgOrigin.Y = 0;
            _bckImgOrigin.X = 0;

            switch (inControl.BackgroundImageLayout)
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

                        calculateImgDimensionsInCanvas(_location, inControl, ref imgWidth, ref imgHeight);

                        if(horizontalAdjustment(inControl, _location))
                        {
                            _bckImgOrigin.Y = 0;
                            _bckImgOrigin.X = Convert.ToInt16((inControl.Width / 2) - (imgWidth / 2));
                        }
                        else
                        {
                            _bckImgOrigin.Y = Convert.ToInt16((inControl.Height / 2) - (imgHeight / 2));
                            _bckImgOrigin.X = 0;
                        }

/*
                        if(inControl.Width <= inControl.Height)
                        {
                            _bckImgOrigin.Y = Convert.ToInt16((inControl.Height / 2) - (imgHeight / 2));
                            _bckImgOrigin.X = 0;
                        }
                        else
                        {
                            _bckImgOrigin.Y = 0;
                            _bckImgOrigin.X = Convert.ToInt16((inControl.Width / 2) - (imgWidth / 2));
                        }
*/
                    } break;
            }
        }

        private bool horizontalAdjustment(Control inControl, LocationInfo inLocation)
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::

            double cRatio = Convert.ToDouble(inControl.Width) / Convert.ToDouble(inControl.Height);

            result = cRatio >= 1.333333333333333;

            //::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableView"></param>
        /// <param name="inSelected"></param>
        private void selectTableView(TableView inTableView, Boolean inSelected)
        {
            try
            {
                 pUpdateSelectedTable();
            }
            catch(Exception exc)
            {
                MessageBox.Show(exc.Message);
            }

            if (inSelected)
            {
                foreach (Control control in Canvas.Controls)
                {
                    if (control is TableView)
                    {
                        TableView tableView = (TableView)control;

                        if (tableView == inTableView)
                        {
                            tableView.Selected = inSelected;

                            _tableViewToSelect = tableView;
                            setRectTracker();

                            OnTableViewSelected(new TableViewEventArgs(tableView, Location.SelectedTable.Number));
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

        /// <summary>
        /// 
        /// </summary>
        private void pUnselectAllTableViews()
        {
            foreach (Control control in _canvas.Controls)
            {
                if (control is TableView)
                {
                    ((TableView)control).Selected = false;
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void MouseDownOnTableView(object sender, MouseEventArgs e)
        {
            pUpdateSelectedTable();

            _tableViewToSelect = (TableView)sender;
            setRectTracker();

            Location.SelectedTable = Location.TableList[pIndexOfTableView(_tableViewToSelect)];
            OnTableViewSelected(new TableViewEventArgs(_tableViewToSelect, Location.SelectedTable.Number));
        }

        /// <summary>
        /// pUpdateSelectedTable is called everytime the TableViewSelecting event needs to be triggered
        /// It triggers the OnTableViewSelecting event, then updates the selected TableInfo
        /// </summary>
        private void pUpdateSelectedTable()
        {
            if (Location.SelectedTable != null)
            {
                TableView tv = pTableViewWithTable(Location.SelectedTable);
                TableViewEventArgs tvEventArgs = new TableViewEventArgs(tv, Location.SelectedTable.Number);

                OnTableViewSelecting(tvEventArgs);

                scaleTable(Canvas, Location, Location.SelectedTable, tv);

                Location.SelectedTable.Name = tvEventArgs.TableView.Caption;
                Location.SelectedTable.Number = tvEventArgs.TableNumber;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inControl"></param>
        private void setRectTracker()
        {
            if (CanvasInvokeRequired())
            {
                Canvas.Invoke(new canvasFunc(AddRecTrackerToCanvas));           
            }
            else
            {
                AddRecTrackerToCanvas();
            }       
        }

        /// <summary>
        /// 
        /// </summary>
        private void AddRecTrackerToCanvas()
        {
            resetRectTracker();

            _rectTracker = new RectTracker(_tableViewToSelect);

            //_rectTracker.TabIndex = -1;
            _rectTracker.MouseUp += new System.Windows.Forms.MouseEventHandler(this.RectTracker_MouseUp);
            _rectTracker.MouseMove += new System.Windows.Forms.MouseEventHandler(this.RectTracker_MouseMove);

            Canvas.Controls.Add(_rectTracker);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RectTracker_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            TableView tableView = (TableView)((RectTracker)sender).Control;
            scaleTable(Canvas, Location, Location.SelectedTable, tableView);

            OnTableViewChanged(new TableViewEventArgs(tableView, Location.SelectedTable.Number));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RectTracker_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            //refreshStatusBar(currentTableView);
        }

        private void scaleTable(Control inCanvas, LocationInfo inLocation, TableInfo inTable, TableView inTableView)
        {
            double xScale = 1;
            double yScale = 1;
            calculateScaleFactors(inCanvas, inLocation, ref xScale, ref yScale);
            calculateXYOrigin(inCanvas, xScale, yScale);

            try
            {
                inTable.PosX = (inTableView.Left - _bckImgOrigin.X) / xScale;
                inTable.PosY = (inTableView.Top  - _bckImgOrigin.Y) / yScale;

                inTable.Width = inTableView.Width / xScale;
                inTable.Height = inTableView.Height / yScale;
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void resetRectTracker()
        {
            if (_rectTracker != null)
            {
                Canvas.Controls.Remove(_rectTracker);

                _rectTracker = null;
                GC.Collect();
            }
        }

        #endregion

        #region events

        // Invoke the BackImageChanged event; called whenever the background image changes
        protected virtual void OnBackImageChanged(LocationInfoEventArgs e)
        {
            if (BackImageChanged != null)
            {
                BackImageChanged(this, e);
            }
        }

        // Invoke the TableViewChanged event; called whenever both the position and the size of a tableview change
        protected virtual void OnTableViewChanged(TableViewEventArgs e)
        {
            if(TableViewChanged != null)
            {
                TableViewChanged(this, e);
            }
        }

        // Invoke the TableViewChanged event; called before whenever both the position and the size of a tableview change
        protected virtual void OnTableViewSelecting(TableViewEventArgs e)
        {
            if(TableViewSelecting != null)
            {
                TableViewSelecting(this, e);
            }
        }

        // Invoke the TableViewChanged event; called whenever both the position and the size of a tableview change
        protected virtual void OnTableViewSelected(TableViewEventArgs e)
        {
            if(TableViewSelected != null)
            {
                TableViewSelected(this, e);
            }
        }

        #endregion
    }
}
