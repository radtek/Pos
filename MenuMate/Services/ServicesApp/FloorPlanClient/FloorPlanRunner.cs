using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Menumate.WinServices.Implementation.Contracts;
using Menumate.WinServices.Implementation.Contracts.DTO;

using System.Drawing.Imaging;

namespace FloorPlanClient
{ 
    public class FloorPlanRunner
    {
        string _errorMsg;
        PosIntegrationClient _posIntegrationClient = null;

        LocationInfoBuilder _locationInfoBuilder = LocationInfoBuilder.Instance;
        TableInfoBuilder    _tableInfoBuilder    = TableInfoBuilder.Instance;

        int _locationIndex = -1;
        List<LocationInfo> _locationList = new List<LocationInfo>();

        List<TableInfo> _deletedTableList = new List<TableInfo>();

        /// <summary>
        /// 
        /// </summary>
        public FloorPlanRunner()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static FloorPlanRunner Instance
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

            internal static readonly FloorPlanRunner instance = new FloorPlanRunner();
        }

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public int LocationCount
        {
            get
            {
                return _locationList.Count;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo[] LocationList
        {
            get
            {
                return _locationList.ToArray<LocationInfo>();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo LocationInUse
        {
            get
            {
                try
                {
                    return LocationList[_locationIndex];
                }
                catch
                {
                    return null;
                }
            }

            set
            {
                _locationIndex = pIndexOfLocation(value);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int IndexOfLocationInUse
        {
            get
            {
                return _locationIndex;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int TableCount
        {
            get
            {
                return LocationInUse.TableCount;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo[] TableList
        {
            get
            {
                return LocationInUse.TableList;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo SelectedTable
        {
            get
            {
                return LocationInUse.SelectedTable;
            }
            set
            {
                LocationInUse.SelectedTable = value;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int IndexOfSelectedTable
        {
            get
            {
                return pIndexOfTable(SelectedTable);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int SelectedTableIndex
        {
            get
            {
                return LocationInUse.SelectedTableIndex;
            }
        }

        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool Init()
        {
            bool result = pInitPosIntegrationClient();

            //::::::::::::::::::::::::::::::

            if(result)
            {
                result = pInitLocations(_posIntegrationClient);
            }

            //::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public LocationInfo LocationAtIndex(int inIndex)
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
        public LocationInfo AddLocation()
        {
            return pAddLocation();           
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImgOrientation"></param>
        /// <returns></returns>
        public LocationInfo AddLocation(ImageOrientation inImgOrientation)
        {
            return pAddLocation(inImgOrientation); 
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        public void DelLocation(LocationInfo inLocation)
        {
            pDelLocation(inLocation);
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo AddRectTable()
        {
            return pAddRectTable();
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo AddElipseTable()
        {
            return pAddElipseTable();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inAppInfo"></param>
        public void DelTable(TableInfo inTable)
        {
            pDelTable(inTable);
        }

        /// <summary>
        /// 
        /// </summary>
        public void SaveFP()
        {
            if(LocationInUseIsNew())
            {
                pAddNewDTOLocation(LocationInUse);
            }
            else
            {
                pSaveDTOLocation(LocationInUse);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool LocationInUseIsNew()
        {
            return LocationInUse.New;
        }

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool pInitPosIntegrationClient()
        {
           return (_posIntegrationClient = pCreatePosIntegrationClient()) != null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private PosIntegrationClient pCreatePosIntegrationClient()
        {
            PosIntegrationClient result = null;

            //:::::::::::::::::::::::::::::::::::

            try
            {
                result = new PosIntegrationClient();
            }
            catch (Exception exc)
            {
                _errorMsg = exc.Message;
            }

            //::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosIntegrationClient"></param>
        /// <returns></returns>
        private bool pInitLocations(PosIntegrationClient inPosIntegrationClient)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::::::

            try
            {
                DTOLocation[] dtoLocations = inPosIntegrationClient.GetAllLocations();
                pClearLocationInfoList();

                foreach (DTOLocation dtoLocation in dtoLocations)
                {
                    _locationList.Add(_locationInfoBuilder.BuildLocationInfo(_posIntegrationClient, dtoLocation));
                }

                _locationIndex = (_locationList.Count == 0) ? -1 : 0;
            }
            catch
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        private void pClearLocationInfoList()
        {
            _locationList.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <returns></returns>
        private int pIndexOfLocation(LocationInfo inLocation)
        {
            try
            {
                int index = 0;

                foreach (LocationInfo location in LocationList)
                {
                    if (location == inLocation)
                    {
                        return index;
                    }

                    index++;
                }

                return -1;
            }
            catch (Exception)
            {
                return -1;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo pAddLocation()
        {
            _locationList.Add(new LocationInfo());

            return _locationList[_locationList.Count - 1];
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImgOrientation"></param>
        public LocationInfo pAddLocation(ImageOrientation inImgOrientation)
        {
            _locationList.Add(new LocationInfo(inImgOrientation));

            return _locationList[_locationList.Count - 1];
        }

        /// <summary>
        /// 
        /// </summary>
        public void pDelLocation(LocationInfo inLocation)
        {
            try
            {
                pRemoveDTOLocation(inLocation);

                _locationList.RemoveAt(_locationList.IndexOf(inLocation));

                GC.Collect(); 
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo pAddRectTable()
        {
            try
            {
                return LocationAtIndex(_locationIndex).AddTable(TableShape.Rectangle);
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo pAddElipseTable()
        {
            try
            {
                return LocationAtIndex(_locationIndex).AddTable(TableShape.Elipse);
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
        public void pDelTable(TableInfo inTable)
        {
            try
            {
                pCopyItToDeletedTables(inTable);
                LocationAtIndex(_locationIndex).DelTable(inTable);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <returns></returns>
        private int pIndexOfTable(TableInfo inTable)
        {
            try
            {
                int index = 0;

                foreach (TableInfo table in TableList)
                {
                    if (table == inTable)
                    {
                        return index;
                    }

                    index++;
                }

                return -1;
            }
            catch (Exception)
            {
                return -1;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="?"></param>
        private void pCopyItToDeletedTables(TableInfo inTable)
        {
            _deletedTableList.Add(inTable);
        }

        #endregion

        #region DTO

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        private void pRemoveDTOLocation(LocationInfo inLocation)
        {
            if(inLocation.ID > 0)
            {
                 _posIntegrationClient.DeleteLocation(pDTOLocationWithID(inLocation.ID));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        private DTOLocation pDTOLocationWithID(int inID)
        {
            DTOLocation result = null;

            //:::::::::::::::::::::::::::::::::

            DTOLocation[] dtoLocations = _posIntegrationClient.GetAllLocations();

            foreach(DTOLocation dtoLocation in dtoLocations)
            {
                if (dtoLocation.Id == inID)
                {
                    result = dtoLocation;
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        private DTOReservable pDTOTableWithID(int inID)
        {
            DTOReservable result = null;

            //:::::::::::::::::::::::::::::::::

            DTOReservable[] dtoTables = _posIntegrationClient.GetTablesForLocation(LocationInUse.ID);

            foreach (DTOReservable dtoTable in dtoTables)
            {
                if(dtoTable.Id == inID)
                {
                    result = dtoTable;
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        private void pAddNewDTOLocation(LocationInfo inLocation)
        {
            DTOLocation dtoLocation = new DTOLocation();

            dtoLocation.Name   = inLocation.Location;
            dtoLocation.Height = inLocation.Height;
            dtoLocation.Width  = inLocation.Width;

            _posIntegrationClient.AddLocation(dtoLocation);

            // Assign the new dtoLocation a background image
            DTOLocation[] dtoLocations = _posIntegrationClient.GetAllLocations();
            dtoLocation                = dtoLocations[dtoLocations.Length - 1];
            dtoLocation.BackgroundImg  = Helper.imageToByteArray(inLocation.BackImage, ImageFormat.Jpeg);

            _posIntegrationClient.UpdateLocation(dtoLocation);

            pSaveDTOTablesInLocation(inLocation.TableList, dtoLocation);

            _locationInfoBuilder.AssignLocationID(inLocation, dtoLocation.Id);
            _locationInfoBuilder.MakeLocationNoLongerNew(inLocation);
            _locationInfoBuilder.MakeLocationNoLongerModified(inLocation);
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="inLocation"></param>
        private void pSaveDTOLocation(LocationInfo inLocation)
        {
            DTOLocation[] dtoLocations = _posIntegrationClient.GetAllLocations();
            DTOLocation dtoLocation = dtoLocations[IndexOfLocationInUse];

            if (inLocation.Modified)
            {
                dtoLocation.Id     = inLocation.ID;
                dtoLocation.Name   = inLocation.Location;
                dtoLocation.Height = inLocation.Height;
                dtoLocation.Width  = inLocation.Width;
                dtoLocation.BackgroundImg = Helper.imageToByteArray(inLocation.BackImage, ImageFormat.Jpeg);

                _posIntegrationClient.UpdateLocation(dtoLocation);

                _locationInfoBuilder.MakeLocationNoLongerModified(inLocation);
            }

            pSaveDTOTablesInLocation(inLocation.TableList, dtoLocation);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableList"></param>
        /// <param name="inLocationID"></param>
        private void pSaveDTOTablesInLocation(TableInfo[] inTableList, DTOLocation inDTOLocation)
        {
            foreach(TableInfo table in inTableList)
            {
                if (table.New)
                {
                    pAddNewDTOTable(table, inDTOLocation);
                }
                else
                {
                    pSaveDTOTable(table, inDTOLocation);
                }
            }

            pDeleteDTOTables(_deletedTableList, inDTOLocation);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        private void pAddNewDTOTable(TableInfo inTable, DTOLocation inDTOLocation)
        {
            DTOReservable dtoTable = new DTOReservable();

            dtoTable.Name   = inTable.Name;
            dtoTable.Number = inTable.Number;
            dtoTable.X      = inTable.PosX;
            dtoTable.Y      = inTable.PosY;
            dtoTable.Height = inTable.Height;
            dtoTable.Width  = inTable.Width;
            dtoTable.Shape  = (inTable.Shape == TableShape.Rectangle) ? @"r" : @"e";

            _posIntegrationClient.AddReservable(dtoTable, inDTOLocation);

            DTOReservable[] dtoTables = _posIntegrationClient.GetTablesForLocation(inDTOLocation.Id);
            int tableID = dtoTables[dtoTables.Length - 1].Id;

            _tableInfoBuilder.AssignTableID(inTable, tableID);
            _tableInfoBuilder.MakeTableNoLongerNew(inTable);
            _tableInfoBuilder.MakeTableNoLongerModified(inTable);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        /// <param name="inLocation"></param>
        private void pSaveDTOTable(TableInfo inTable, DTOLocation inLocation)
        {
            if (inTable.Modified)
            {
                DTOReservable dtoTable = new DTOReservable();

                dtoTable.Id     = inTable.ID;
                dtoTable.Name   = inTable.Name;
                dtoTable.Number = inTable.Number;
                dtoTable.X      = inTable.PosX;
                dtoTable.Y      = inTable.PosY;
                dtoTable.Height = inTable.Height;
                dtoTable.Width  = inTable.Width;
                dtoTable.Shape  = (inTable.Shape == TableShape.Rectangle) ? @"r" : @"s";

                _posIntegrationClient.UpdateReservable(dtoTable);

                _tableInfoBuilder.MakeTableNoLongerModified(inTable);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="?"></param>
        private void pDeleteDTOTables(List<TableInfo> inTables, DTOLocation inDTOLocation)
        {
            foreach (TableInfo table in inTables)
            {
                if (table.ID > 0)
                {
                    _posIntegrationClient.DeleteReservable(pDTOTableWithID(table.ID), inDTOLocation);
                }
            }

            _deletedTableList.Clear();
        }

        #endregion
    }
}
