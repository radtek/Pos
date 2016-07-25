using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace FloorPlanClient
{
    public enum TableShape 
    {
        Rectangle,
        Elipse
    };

    public class TableInfo
    {
        internal int  _id       = 0; // Database ID 
        internal bool _new      = true;
        internal bool _modified = false;

        string _name = @"";
        int _number = 1;

        TableShape _shape;

        double _posX = 0;
        double _posY = 0;

        double _width = 0;
        double _height = 0;

        /// <summary>
        /// 
        /// </summary>
        public TableInfo()
        {
            pInitTableInfo(DefaultName, DefaultNumber, DefaultShape, DefaultPosX, DefaultPosY, DefaultWidth, DefaultHeight, DefaultModified, DefaultDeleted);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inNumber"></param>
        public TableInfo(string inName, int inNumber)
        {
            pInitTableInfo(inName, inNumber, DefaultShape, DefaultPosX, DefaultPosY, DefaultWidth, DefaultHeight, DefaultModified, DefaultDeleted);
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
        public string DefaultName
        {
            get
            {
                return @"";
            }
        }
        
        /// <summary>
        /// 
        /// </summary>
        public int DefaultNumber
        {
            get
            {
               return 1;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableShape DefaultShape
        {
            get
            {
                return TableShape.Rectangle;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public double DefaultPosX
        {
            get
            {
               return 0.0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public double DefaultPosY
        {
            get
            {
               return 0.0;
            }
        }
     
        /// <summary>
        /// 
        /// </summary>
        public double DefaultWidth
        {
            get
            {
               return 80.0;
            }
        }
     
        /// <summary>
        /// 
        /// </summary>
        public double DefaultHeight
        {
            get
            {
               return 60.0;
            }
        }
    
        /// <summary>
        /// 
        /// </summary>
        public bool DefaultModified
        {
            get
            {
                return false;
            }
        }
            
        /// <summary>
        /// 
        /// </summary>
        public bool DefaultDeleted
        {
            get
            {
               return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                _name = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public int Number
        {
            get
            {
                return _number;
            }
            set
            {
                _number = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public TableShape Shape
        {
            get
            {
                return _shape;
            }
            set
            {
                _shape = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public double PosX
        {
            get
            {
                return _posX;
            }
            set
            {
                _posX = value;
                iSetModified(true);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public double PosY
        {
            get
            {
                return _posY;
            }
            set
            {
                _posY = value;
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

        #endregion

        #region Private

        public void pInitTableInfo(string inName, int inNumber, TableShape inShape, double inPosX, double inPosY, double inWidth, double inHeight, bool inModified, bool inDeleted)
        {
            Name = inName;
            Number = inNumber;

            Shape = inShape;

            PosX = inPosX;
            PosY = inPosY;

            Width = inWidth;
            Height = inHeight;
        }

        #endregion

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

    public class TableInfoBuilder
    {
        /// <summary>
        /// 
        /// </summary>
        public TableInfoBuilder()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static TableInfoBuilder Instance
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

            internal static readonly TableInfoBuilder instance = new TableInfoBuilder();
        }

        public TableInfo BuildTableInfo(DTOReservable inDTOReservable)
        {
            TableInfo result = new TableInfo();

            //::::::::::::::::::::::::::::::::::::::::::::::::::

            result.Shape = (inDTOReservable.Shape.ToUpper() == @"R") ? TableShape.Rectangle : TableShape.Elipse;

            result._new      = false;
            result._id       = inDTOReservable.Id;
            result.Name      = inDTOReservable.Name;
            result.Number    = inDTOReservable.Number;
            result.PosX      = inDTOReservable.X;
            result.PosY      = inDTOReservable.Y;
            result.Width     = inDTOReservable.Width;
            result.Height    = inDTOReservable.Height;
            result._modified = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        /// <param name="inTableID"></param>
        public void AssignTableID(TableInfo inTable, int inTableID)
        {
            inTable._id = inTableID;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTable"></param>
        public void MakeTableNoLongerNew(TableInfo inTable)
        {
            inTable._new = false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLocation"></param>
        public void MakeTableNoLongerModified(TableInfo inTable)
        {
            inTable.iSetModified(false);
        }
    }
}
