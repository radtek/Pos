using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Menumate.WinServices.Client.FloorPlan
{
    public class NoCurrentLocationException : Exception
    {
        public NoCurrentLocationException() : base(@"No current location")
        {
        }
    }

    //..................................................

    public class NoCurrentTableException : Exception
    {
        public NoCurrentTableException() : base(@"No current Table")
        {
        }
    }

    //..................................................

    public class LocationNotFoundException : Exception
    {
        public LocationNotFoundException(string inLocName) : base(string.Format(@"Location {0} not found", inLocName))
        {
        }
    }

    public class TableNotFoundException : Exception
    {
        public TableNotFoundException(string inTablename, string inLocName): base(string.Format(@"Table {0} not found in location {1}", inTablename, inLocName))
        {
        }
    }

    public class UnsavedLocationException : Exception
    {
        public UnsavedLocationException(string inLocName): base(string.Format(@"Location {0} not not saved", inLocName))
        {
        }
    }
    
    public class UnsavedTablesException : Exception
    {
        public UnsavedTablesException(string inLocName): base(string.Format(@"Tables not saved in location {0}", inLocName))
        {
        }
    }

    public class PosIntegrationException : Exception
    {
        public PosIntegrationException(string inMessage): base(string.Format(@"PosIntegration exception with message: {0}", inMessage))
        {
        }
    }

}
