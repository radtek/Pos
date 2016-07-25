using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Drawing;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace FloorPlanClient
{
    public class LocationInfo
    {
        internal int _id        = 0; // Database Id
        internal bool _new      = true;
        internal bool _modified = false;

        string _location = @"";

        LocationBackImage _locBckImage;

        double _width = 100;
        double _height = 100;

        List<TableInfo> _tableList;
        int _selectedTableIndex = -1;

        TableInfoBuilder _tableInfoBuilder = TableInfoBuilder.Instance;

        /// <summary>
        /// 
        /// </summary>
        public LocationInfo()
        {
            _locBckImage = new LocationBackImage();

            Width  = _locBckImage.BackImage.Width;
            Height = _locBckImage.BackImage.Height;

            _tableList = new List<TableInfo>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImgOrientation"></param>
        public LocationInfo(ImageOrientation inImgOrientation)
        {
            _locBckImage = new LocationBackImage(inImgOrientation);
            

            Width  = _locBckImage.BackImage.Width;
            Height = _locBckImage.BackImage.Height;

            _tableList = new List<TableInfo>();
        }

        #region Properties

        /// <summary>
        /// Database ID
        /// </summary>
        public int ID
        {
            get
            {
                return _id;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool New
        {
            get
            {
                return _new;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool Modified
        {
            get
            {
                return _modified;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public string Location
        {
            get
            {
                return _location;
            }
            set
            {
                _location = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public double Width
        {
            get
            {
                return _width;
            }
            set
            {
                _width = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public double Height
        {
            get
            {
                return _height;
            }
            set
            {
                _height = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public Image BackImage
        {
            get
            {
                return _locBckImage.BackImage;
            }
            set
            {
                _locBckImage.BackImage = value;

                Width  = _locBckImage.BackImage.Width;
                Height = _locBckImage.BackImage.Height;

                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public ImageOrientation BackImageOrientation
        {
            get
            {
                return _locBckImage.Orientation;
            }
            set
            {
                _locBckImage.Orientation = value;

                Width  = _locBckImage.BackImage.Width;
                Height = _locBckImage.BackImage.Height;

                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int TableCount
        {
            get
            {
                return _tableList.Count;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo[] TableList
        {
            get
            {
                return _tableList.ToArray<TableInfo>();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableInfo SelectedTable
        {
            get
            {
                try
                {
                    return _tableList[_selectedTableIndex];
                }
                catch
                {
                    return null;
                }
            }
            set
            {
                _selectedTableIndex = _tableList.IndexOf(value);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int SelectedTableIndex
        {
            get
            {
                return _selectedTableIndex;
            }
        }

        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableShape"></param>
        /// <returns></returns>
        public TableInfo AddTable(TableShape inTableShape)
        {
            TableInfo table = new TableInfo();
            table.Shape  = inTableShape;
            table.Number = NextTableNumber();
            table.Name   = (table.Shape == TableShape.Rectangle) ? string.Format(@"S{0}", table.Number) : string.Format(@"R{0}", table.Number);

            _tableList.Add(table);

            SelectedTable = table;

            return table;
        }

        ///
        public TableInfo AddTable(TableInfo inTable)
        {
            _tableList.Add(inTable);

            SelectedTable = inTable;

            return inTable;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        public void DelTable(TableInfo inTable)
        {
            try
            {
                _tableList.Remove(inTable);

                GC.Collect();
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        /// <returns></returns>
        public int IndexOfTable(TableInfo inTable)
        {
            int result = -1;

            //:::::::::::::::::::::::::::::::::

            int index = 0;
            foreach(TableInfo table in TableList)
            {
                if(table == inTable)
                {
                    result = index;
                    break;
                }

                index++;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int NextTableNumber()
        {
            int max = 0;
            if (TableCount > 0)
            {
                max = TableList[0].Number;
                foreach (TableInfo table in TableList)
                {
                    if (table.Number > max)
                    {
                        max = table.Number;
                    }
                }
            }

            return max + 1;
        }

        /// <summary>
        /// 
        /// </summary>
        public void ReArrangeTableNumbers()
        {
            for (int i = 0; i < TableCount; i++)
            {
                TableList[i].Number = i + 1;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableNumber"></param>
        /// <returns></returns>
        public bool SearchForDuplicateTableNumber(int inTableNumber, TableInfo inTable)
        {
            bool result = false; // No duplication

            //::::::::::::::::::::::::::::

            foreach(TableInfo table in TableList)
            {
                if((table.Number == inTableNumber) && table != inTable)
                {
                    result = true; // Found a duplicate
                    break;
                }
            }

            //::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableName"></param>
        /// <param name="inTable"></param>
        /// <returns></returns>
        public bool SearchForDuplicateTableName(string inTableName, TableInfo inTable)
        {
            bool result = false; // No duplication

            //::::::::::::::::::::::::::::

            foreach (TableInfo table in TableList)
            {
                if ((table.Name == inTableName) && table != inTable)
                {
                    result = true; // Found a duplicate
                    break;
                }
            }

            //::::::::::::::::::::::::::::

            return result;
        }

        #region Internal

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inModified"></param>
        internal void iSetModified(bool inModified)
        {
            _modified = inModified;
        }

        #endregion
    }

    public class LocationInfoBuilder
    {
        /// <summary>
        /// 
        /// </summary>
        public LocationInfoBuilder()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static LocationInfoBuilder Instance
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

            internal static readonly LocationInfoBuilder instance = new LocationInfoBuilder();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDTOLocation"></param>
        /// <returns></returns>
        public LocationInfo BuildLocationInfo(PosIntegrationClient inPosIntegrationClient, DTOLocation inDTOLocation)
        {
            LocationInfo result = new LocationInfo();

            //::::::::::::::::::::::::::::::::::::

            result._new      = false;
            result._id       = inDTOLocation.Id;
            result.Location  = inDTOLocation.Name;
            result.Width     = inDTOLocation.Width;
            result.Height    = inDTOLocation.Height;

            byte[] byteImg = inPosIntegrationClient.GetBackgroundImgDataForLocation(inDTOLocation.Id);
            //if(byteImg.Length > 0)
            if (byteImg != null)
            {
                Image img = Helper.byteArrayToImage(byteImg);

                result.BackImageOrientation = (img.Width > img.Height) ? ImageOrientation.Lanscape : ImageOrientation.Portrait;
                result.BackImage = img;
            }

            result._modified = false;

            pAddTables(inPosIntegrationClient, result);
 
            //::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        /// <param name="inLocID"></param>
        public void AssignLocationID(LocationInfo inLocation, int inLocID)
        {
            inLocation._id = inLocID;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        public void MakeLocationNoLongerNew(LocationInfo inLocation)
        {
            inLocation._new = false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        public void MakeLocationNoLongerModified(LocationInfo inLocation)
        {
            inLocation.iSetModified(false);
        }

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <param name="?"></param>
        /// <param name="?"></param>
        private void pAddTables(PosIntegrationClient inPosIntegrationClient, LocationInfo inLocation)
        {
            DTOReservable[] dtoReservableTables = inPosIntegrationClient.GetTablesForLocation(inLocation._id);

            foreach(DTOReservable reservableTable in dtoReservableTables)
            {
                inLocation.AddTable(TableInfoBuilder.Instance.BuildTableInfo(reservableTable));
            }
        }

        #endregion
    }
}
