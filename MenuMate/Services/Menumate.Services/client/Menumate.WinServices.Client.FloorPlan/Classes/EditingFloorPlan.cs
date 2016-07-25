using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Menumate.WinServices.Implementation.Contracts.DTO;

using System.Windows.Forms;

// "http://192.168.42.78:8732/MenumateWinServices/service"

namespace Menumate.WinServices.Client.FloorPlan
{
    class EditingFloorPlan
    {
        Boolean _posIntegrationFailed = false;
        PosIntegrationClient _posIntegrationClient;

        List<DTOLocation> _locations;
        List<DTOReservable> _tablesInLocation;

        List<DTOLocation> _removedLocations;
        List<DTOReservable> _removedTables;

        int _currentLocIndex;
        int _currentTableIndex;

        Boolean _currentLocationChanged;
        Boolean _tablesChanged;

        Boolean _locationDeleted = false;
        Boolean _tableDeleted = false;

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        ///     Creates an instance of EditingFloorPlan class
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        /// </exceptions>
        /// <returns></returns>
        EditingFloorPlan()
        {
        }

        /// <summary>
        ///     Creates a singleton instance of the EditingFloorPlan class
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        /// </exceptions>
        /// <returns></returns>
        public static EditingFloorPlan Instance
        {
            get
            {
                return Nested.instance;
/*
                try
                {
                    return Nested.instance;
                }
                catch(TypeInitializationException e)
                {
                    throw e.InnerException;  // It must be a PostIntegrationExeception
                }
*/
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly EditingFloorPlan instance = new EditingFloorPlan();
        }

        /// <summary>
        ///     Initializes EditingFloorPlan class
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void Init()
        {
            try
            {
                _posIntegrationFailed = false;

                _currentLocationChanged = false;
                _tablesChanged = false;

                initPosIntegrationClient();
                initLocations();
                initTablesInLocations();
            }
            catch (NoCurrentLocationException)
            {
                // Do nothing
            }
            catch (IndexOutOfRangeException) // Location list is empty
            {
                // Do nothing
            }
            catch (PosIntegrationException)
            {
                throw;
            }
        }

        /// <summary>
        ///     Creates a PosIntegrationClient object to communicate with PosIntegration Service
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected virtual void initPosIntegrationClient()
        {
            if (_posIntegrationClient == null) 
            {
                try
                {
                    _posIntegrationClient = new PosIntegrationClient();
                }
                catch (Exception exc)
                {
                    _posIntegrationFailed = true;
                    throw new PosIntegrationException(exc.Message);
                }
            }
        }

        /// <summary>
        ///     Initializes a new instance of the Floor Plan locations (System.Collections.Generic.List<DTOLocation> class).  Retrieve the locations info from the PosIntegration service
        ///     Initializes a new instance of the Floor Plan removed locations (System.Collections.Generic.List<DTOLocation> class). 
        ///        This is a list of locations that are removed by the user.  It is used by ... FINISH THIS LINE!!!
        /// </summary>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="IndexOutOfRangeException" description="Location list is empty"></exception>
        /// </exceptions>
        /// <param name="document"></param>
        /// <returns></returns>
        protected virtual void initLocations()
        {
            try
            {
                if (_locations == null) 
                {
                    _locations = new List<DTOLocation>();
                }
                if (_removedLocations == null)
                {
                    _removedLocations = new List<DTOLocation>();
                }

                //.............................

                _currentLocationChanged = false;

                resetLocations(false);
             }
             catch (PosIntegrationException)
             {
                throw;
             }
        }

        /// <summary>
        ///     Initializes a new instance of the Floor Plan tables in the current location (System.Collections.Generic.List<DTOReservable> class).  Retrieve the tables info from the PosIntegration service
        ///     Initializes a new instance of the Floor Plan removed tables in the current location (System.Collections.Generic.List<DTOReservable> class). 
        ///        This is a list of locations that are removed by the user.  It is used by saveTables method
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected virtual void initTablesInLocations()
        {
            try
            {
                if(_tablesInLocation == null) 
                {
                    _tablesInLocation = new List<DTOReservable>();
                }
                if(_removedTables == null) 
                {
                    _removedTables = new List<DTOReservable>();
                }
                
                //...............................

                _tablesChanged = false;

                resetTablesInLocation();
             }
             catch (PosIntegrationException)
             {
                throw;
             }
        }

        /// <summary>
        ///     Resets the Floor Plan locations (System.Collections.Generic.List<DTOLocation> class).  Retrieve the locations info from the PosIntegration service
        ///     Resets the Floor Plan removed locations (System.Collections.Generic.List<DTOLocation> class). Clears the removed location list.
        ///        This is a list of locations that are removed by the user.  It is used by ... FINISH THIS LINE!!!
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="IndexOutOfRangeException" description="Could not Reset Locations as changes in the current location have not been saved"></exception>
        ///    <exception name="UnsavedTablesException" description="Could not Reset Locations as changes in the current location's tables have not been saved"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected virtual void resetLocations(Boolean inCheckForSaved)
        {
            refreshLocations();

            try
            {
                setCurrentLocationAtIndex(0, inCheckForSaved);
            }
            catch (IndexOutOfRangeException)
            {
                clearCurrentLocation();
            }
            catch (UnsavedLocationException)
            {
                throw;
            }
            catch (UnsavedTablesException)
            {
                throw;
            }
            catch (PosIntegrationException)
            {
                throw;
            }
        }

        /// <summary>
        ///     Resets the Floor Plan tables in the current location (System.Collections.Generic.List<DTOReservable> class).  Retrieve the tables info from the PosIntegration service
        ///     Resets the Floor Plan removed tables in the current location (System.Collections.Generic.List<DTOReservable> class). Clears the removed tables list.
        ///        This is a list of tables that are removed by the user.  It is used by saveTables method
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="UnsavedTablesException" description="Could not Reset Locations as changes in the current location's tables have not been saved"></exception>
        ///    <exception name="NoCurrentLocationException" description="Could not Reset current location's tables as no current location has been set"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected virtual void resetTablesInLocation()
        {
            if (_tablesChanged)
            {
                throw new UnsavedTablesException(this.currentLocation.Name);
            }

            //........................

            try
            {
                refreshTablesInLocation();
                clearCurrentTable();
            }
            catch (NoCurrentLocationException)
            {
                throw;
            }
            catch (PosIntegrationException)
            {
                throw;
            }
        }

        /// <summary>
        ///     Resets the Floor Plan locations (System.Collections.Generic.List<DTOLocation> class).  Retrieve the locations info from the PosIntegration service
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected virtual void refreshLocations()
        {
            try
            {
                _locations.Clear();

                DTOLocation[] locations = _posIntegrationClient.GetAllLocations();

                foreach (DTOLocation loc in locations)
                {
                    _locations.Add(loc);
                }
            }
            catch (Exception exc) // Pos Integration Exception
            {
                _posIntegrationFailed = true;
                throw new PosIntegrationException(exc.Message);
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="NoCurrentLocationException" description="Could not Reset current location's tables as no current location has been set"></exception>
        /// </exceptions>
        /// <returns></returns>
        public virtual void refreshTablesInLocation()
        {
            try
            {
                _tablesInLocation.Clear();

                if (this.currentLocation.Id > 0) // Not a just added and unsaved location 
                {
                    DTOReservable[] tablesInLocation = _posIntegrationClient.GetTablesForLocation(this.currentLocation.Id);

                    foreach (DTOReservable table in tablesInLocation)
                    {
                        _tablesInLocation.Add(table);
                    }
                }
            }
            catch (NoCurrentLocationException)
            {
                throw;
            }
            catch (Exception exc) // Pos Integration Exception
            {
                _posIntegrationFailed = true;
                throw new PosIntegrationException(exc.Message);
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="IndexOutOfRangeException" description="Set an invalid location"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected void doSetCurrentLocationAtIndex(int inIndex, Boolean inCheckForSaved)
        {
            try
            {
                if (inIndex < _locations.Count)
                {
                    _currentLocIndex = inIndex;

                    if (this.currentLocation.Id > 0) // Not a just added and unsaved location 
                    {
                        this.currentLocation.BackgroundImg = _posIntegrationClient.GetBackgroundImgDataForLocation(this.currentLocation.Id);
                    }

                    //MessageBox.Show(string.Format("Set Current Location: {0}, at Index\n\nWidth: {1}      Height: {2}", this.currentLocation.Name, this.currentLocation.Width, this.currentLocation.Height));

                    if (this.currentLocation.Height == 0)
                    {
                        this.currentLocation.Height = 800;
                    }

                    if (this.currentLocation.Width == 0)
                    {
                        this.currentLocation.Width = 600;
                    }
                }
            }
            catch (PosIntegrationException)
            {
                throw;
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="NoCurrentLocationException" description="Could not save a table as no current location has been set"></exception>
        /// </exceptions>
        /// <returns></returns>
        protected void saveTable(DTOReservable inTable)
        {
            // Exceptions are caught in saveTables method !!!!
            if (inTable.Id == 0)
            {
                _posIntegrationClient.AddReservable(inTable, this.currentLocation);
            }
            else
            {
                _posIntegrationClient.UpdateReservable(inTable);
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public bool posIntegrationFailed
        {
            get
            {
                return _posIntegrationFailed;
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public bool floorPlanChanged
        {
            get
            {
                return _currentLocationChanged || _tablesChanged || _locationDeleted || _tableDeleted;
            }
        }


        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public DTOLocation[] locations
        {
            get
            {
                return _locations.ToArray();
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public DTOReservable[] tablesInLocation
        {
            get
            {
                return _tablesInLocation.ToArray();
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="NoCurrentLocationException" description="Set an invalid location"></exception>
        /// </exceptions>
        /// <returns></returns>
        public DTOLocation currentLocation
        {
            get
            {
                if ((_currentLocIndex >= 0) && (_currentLocIndex < _locations.Count))
                {
                    return _locations[_currentLocIndex];
                }
                else
                {
                    throw new NoCurrentLocationException();
                }
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="NoCurrentTableException" description="Set an invalid table"></exception>
        /// </exceptions>
        /// <returns></returns>
        public DTOReservable currentTable
        {
            get
            {
                if (_currentTableIndex >= 0)
                {
                    return _tablesInLocation[_currentTableIndex];
                }
                else
                {
                    throw new NoCurrentTableException();
                }
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void clearCurrentLocation()
        {
            _currentLocIndex = -1;
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void clearCurrentTable()
        {
            _currentTableIndex = -1;
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="IndexOutOfRangeException" description="Set an invalid location"></exception>
        ///    <exception name="UnsavedLocationException" description="Could not set the location as te current location's info changes have not been saved"></exception>
        ///    <exception name="UnsavedTablesException" description=""Could not set the location as te current location's tables info changes have not been saved"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void setCurrentLocationAtIndex(int inIndex, Boolean inCheckForSaved)
        {
            if(inCheckForSaved)
            {
                if(_currentLocationChanged)
                {
                    throw new UnsavedLocationException(this.currentLocation.Name);
                }
                
                if(_tablesChanged)
                {
                    throw new UnsavedTablesException(this.currentLocation.Name);
                }
            }

            doSetCurrentLocationAtIndex(inIndex, inCheckForSaved);
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="IndexOutOfRangeException" description="Set an invalid table"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void setCurrentTableAtIndex(int inIndex)
        {
            if ((inIndex >= 0) && (inIndex < _tablesInLocation.Count))
            {
                _currentTableIndex = inIndex;
            }
            else
            {
                throw new IndexOutOfRangeException(@"Invalid table index");
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="LocationNotFoundException" description="Unknown location"></exception>
        /// </exceptions>
        /// <returns></returns>
        public int indexOfLocation(DTOLocation inLocation)
        {
            int result = -1;

            //...................................

            int i = 0;
            foreach(DTOLocation loc in _locations)
            {
                if(inLocation.Id == loc.Id)
                {
                    result = i;
                    break;
                }
                i++;
            }

            //...................................

            if(result > -1)
            {
                return result;
            }
            else
            {
                throw new LocationNotFoundException(inLocation.Name);
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="TableNotFoundException" description="Unknown table for the curent location"></exception>
        /// </exceptions>
        /// <returns></returns>
        public int indexOfTable(DTOReservable inTable)
        {
            int result = -1;

            //...................................

            int i = 0;
            foreach (DTOReservable table in _tablesInLocation)
            {
                if (inTable.Id == table.Id)
                {
                    result = i;
                    break;
                }
                i++;
            }

            //...................................

            if (result > -1)
            {
                return result;
            }
            else
            {
                throw new TableNotFoundException(inTable.Name, this.currentLocation.Name);
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="NoCurrentLocationException" description="No current location has neen set"></exception>
        /// </exceptions>
        /// <returns></returns>
        public DTOReservable[] getTablesInLocation(DTOLocation inLocation)
        {
            try
            {
                if(inLocation.Id == this.currentLocation.Id)
                {
                    return this.tablesInLocation;
                }
                else
                {
                    return _posIntegrationClient.GetTablesForLocation(inLocation.Id);
                }
            }
            catch (NoCurrentLocationException)
            {
                throw;
            }
            catch (Exception exc) // Pos Integration Exception
            {
                _posIntegrationFailed = true;
                throw new PosIntegrationException(exc.Message);
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void addNewLocation(DTOLocation inNewLocation)
        {
            inNewLocation.Id = 0;

            _locations.Add(inNewLocation);
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="NoCurrentLocationException" description="No current location has neen set"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void removeCurrentLocation()
        {
            try
            {
                if(this.currentLocation.Id > 0)
                {
                    _removedLocations.Add(this.currentLocation);
                }
                
                _locations.Remove(this.currentLocation);
            }
            catch (NoCurrentLocationException)
            {
                throw;
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void addNewTable(DTOReservable inNewTable)
        {
            inNewTable.Id = 0;

            _tablesInLocation.Add(inNewTable);
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="Exception" description="Try to remove an unknown table"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void removeTable(DTOReservable inTable)
        {
            try
            {
                if (inTable.Id > 0)
                {
                    _removedTables.Add(inTable);
                }

                _tablesInLocation.Remove(inTable);
            }
            catch (Exception)
            {
                throw;
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void signalLocationChanged()
        {
            _currentLocationChanged = true;
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void signalTablesChanged()
        {
            _tablesChanged = true;
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void signalLocationDeleted()
        {
            _locationDeleted = true;
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void signalTableDeleted()
        {
            _tableDeleted = true;
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public void giveUpSaving()
        {
            refreshLocations();

            _currentLocationChanged = false;
            _tablesChanged = false;

            _locationDeleted = false;
            _tableDeleted = false;
         }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="NoCurrentLocationException" description="No current location has been set"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void saveFP()
        {
            try
            {
                foreach (DTOLocation loc in _removedLocations)
                {
                    _posIntegrationClient.DeleteLocation(loc);
                }
                _locationDeleted = false;

                saveCurrentLocation();
                saveTables();
            }
            catch(Exception)
            {
                throw;
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="NoCurrentLocationException" description="No current location has been set"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void saveCurrentLocation()
        {
            if (_currentLocationChanged)
            {
                try
                {
                    if (currentLocation.Id == 0)
                    {
                        byte[] imgBytes = this.currentLocation.BackgroundImg;

                        _posIntegrationClient.AddLocation(this.currentLocation);
                        refreshLocations();

                        doSetCurrentLocationAtIndex(_locations.Count - 1, false);

                        this.currentLocation.BackgroundImg = imgBytes;

                        //MessageBox.Show(string.Format("Save Current Location: {0}\n\nWidth: {1}      Height: {2}\n\ncurrentLocation.Id = 0", this.currentLocation.Name, this.currentLocation.Width, this.currentLocation.Height));

                        _posIntegrationClient.UpdateLocation(this.currentLocation);
                    }
                    else
                    {
                        //MessageBox.Show(string.Format("Save Current Location: {0}\n\nWidth: {1}      Height: {2}", this.currentLocation.Name, this.currentLocation.Width, this.currentLocation.Height));

                        _posIntegrationClient.UpdateLocation(this.currentLocation);
                    }

                    _currentLocationChanged = false;

                    saveTables();
                }
                catch (NoCurrentLocationException)
                {
                    throw;
                }
                catch (Exception exc) // Pos Integration Exception
                {
                    _posIntegrationFailed = true;
                    throw new PosIntegrationException(exc.Message);
                }
            }
        }

        /// <summary>
        ///    
        /// </summary>
        /// <param name="document"></param>
        /// <exceptions>
        ///    <exception name="PosIntegration client Exception" description="Any Exception coming from PosIntegration client"></exception>
        ///    <exception name="NoCurrentLocationException" description="No current location has been set"></exception>
        /// </exceptions>
        /// <returns></returns>
        public void saveTables()
        {
            if (_tablesChanged || _tableDeleted)
            {
                try
                {
                    foreach (DTOReservable table in _removedTables)
                    {
                        _posIntegrationClient.DeleteReservable(table, this.currentLocation);
                    }
                    _tableDeleted = false;

                    if (_tablesChanged)
                    {
                        foreach (DTOReservable table in this.tablesInLocation)
                        {
                            saveTable(table);
                        }
                        _tablesChanged = false;
                    }

                    refreshTablesInLocation();
                }
                catch (NoCurrentLocationException)
                {
                    throw;
                }
                catch (Exception exc) // Pos Integration Exception
                {
                    _posIntegrationFailed = true;
                    throw new PosIntegrationException(exc.Message);
                }
            }
        }

    }
}

