using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;

using Castle.ActiveRecord;
using Castle.ActiveRecord.Framework;

using Menumate.WinServices.Implementation.Contracts;
using Menumate.WinServices.Implementation.Contracts.DTO;
using Menumate.WinServices.Implementation.Contracts.BusinessObjects;

using NHibernate;

using Menumate.WinServices.Tools;

namespace Menumate.WinServices.Implementation
{
    [ServiceBehavior(
        #if DEUBG
		    IncludeExceptionDetailInFaults = true,
        #endif

        InstanceContextMode = InstanceContextMode.Single,
        ConcurrencyMode = ConcurrencyMode.Multiple)
    ]

    /// <summary>
    /// Menumate POS Integration implementation.  This class is meant to be used in FloorPlan Service
    /// </summary>
    public class PosIntegrationService : IPosIntegration
    {

        public PosIntegrationService()
        {
            ReservationsServiceInitHelper.InitDB();
        }

        #region IPosIntegration Members

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public DTOLocation[] GetAllLocations()
        {
            DTOLocation[] returnList = null;

            using (SessionScope scope = new SessionScope(FlushAction.Auto))
            {
                try
                {
                    Location[] allLocations = Location.FindAll();
                    List<DTOLocation> dtoAllLocations = new List<DTOLocation>();
                    foreach (Location l in allLocations)
                    {
                        dtoAllLocations.Add(DTOFactory.BuildLocationDTO(l));
                    }
                    returnList = dtoAllLocations.ToArray<DTOLocation>();
                }
                catch (ActiveRecordException are)
                {
                    throw;
                }
            }
            return returnList;
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <param name="locationId"></param>
        /// <returns></returns>
        public DTOReservable[] GetTablesForLocation(int locationId)
        {
            List<DTOReservable> returnList = new List<DTOReservable>();

            using (new SessionScope(FlushAction.Auto))
            {
                try
                {
                    // transient
                    Terminal term = new Terminal("pos");
                    term.TableViewFilter = "day";
                    term.CurrentDate = DateTime.Now;

                    Location location = Location.Find(locationId);
                    foreach (Table t in location.Tables)
                    {
                        returnList.Add(DTOFactory.BuildReservableDTO(t, term));
                    }
                }
                catch (ActiveRecordException are)
                {
                    throw;
                }
            }
            return returnList.ToArray<DTOReservable>();
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public byte[] GetBackgroundImgDataForLocation(int locationid)
        {
            byte[] data = null;

            using (new SessionScope(FlushAction.Auto))
            {
                try
                {
                    Location location = null;
                    location = Location.Find(locationid);

                    data = location.BackgroundImg;

                }
                catch (NHibernate.ObjectNotFoundException onf)
                {
                    throw;
                    //Log.ErrorException(String.Format("Error getting background image data for location, id => {0}", locationid), onf);
                }
                catch (Castle.ActiveRecord.NotFoundException nfe)
                {
                    throw;
                    //Log.ErrorException(String.Format("Error getting background image data for location, id => {0}", locationid), nfe);
                }
                catch (ActiveRecordException are)
                {
                    throw;
                    //Log.ErrorException(String.Format("Error getting background image data for location, id => {0}", locationid), are);
                }
            }
            
            return data;
        }

        // ---------------------------------------------------------------------
 
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult AddLocation(DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            //::::::::::::::::::::::::::::::::::::::::::

            using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {
                    Location[] locations = Location.FindAll();

                    //.....................................

                    Location newLocation = new Location(locationDTO.Name);
                    newLocation.DefaultDuration = locationDTO.DefaultDuration;
                    newLocation.PlanWidth = locationDTO.Width;
                    newLocation.PlanHeight = locationDTO.Height;

                    if (locations.Contains<Location>(newLocation, new LocationComparer()))
                    {
                        throw new LocationAlreadyExistsException(String.Format("Location {0} already exists", newLocation.Name));
                    }

                    newLocation.Save();
                    result.Success = true;
                    result.ObjectAffected = newLocation.Id;
                    result.Message = "Location added successfully.";

                    ServiceLogger.Log(string.Format("Location {0} added sucessfully", newLocation.ToString()));

                }
                catch (ActiveRecordException are)
                {
                    scope.VoteRollBack();
                    result.Success = false;
                    result.Message = are.ToString();

                    ServiceLogger.LogException(string.Format("Exception while adding new location{0}{1}", Environment.NewLine, locationDTO.Name), are);
                }
                catch (LocationAlreadyExistsException laee)
                {

                    string message = string.Format("Location already exists; {0}.", locationDTO);

                    scope.VoteRollBack();
                    result.Success = false;
                    result.Message = message;
                    result.LongMessage = laee.ToString();

                    ServiceLogger.LogException(message, laee);
                }
                catch (InvalidDTOException ide)
                {
                    scope.VoteRollBack();
                    result.Success = false;
                    result.Message = ide.ToString();

                    ServiceLogger.LogException(string.Format("Error adding location;{0}{1}", Environment.NewLine, locationDTO.Name), ide);
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult AddReservable(DTOReservable tableDTO, DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            //::::::::::::::::::::::::::::::::::::::::::

            using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {
                    DTOHelpers.ValidateReservableDTO(tableDTO);
                    DTOHelpers.ValidateLocationDTO(locationDTO);

                    Location location = Location.Find(locationDTO.Id);
                    Table table = DTOHelpers.TableFromDTO(tableDTO);

                    location.AddTable(table);

                    result.Success = true;
                    result.ObjectAffected = table.Id;
                    result.Message = String.Format("{0} added successfully to {1}", tableDTO.ToString(), location.ToString());

                    ServiceLogger.Log(string.Format("Table {0} added to location {1}", tableDTO.Name, location.Name));
                }
                catch (ActiveRecordException are)
                {
                    scope.VoteRollBack();
                    result.Success = false;
                    result.Message = string.Format("Could not add table {0};{1}{2}", tableDTO.Name, Environment.NewLine, are.ToString());

                    ServiceLogger.LogException(string.Format("Could not add table {0}", tableDTO.Name), are);
                }
                catch (ReservableAlreadyExistsException rree)
                {
                    scope.VoteRollBack();
                    result.Success = false;
                    result.Message = string.Format("A table called => {0} already exists;{1}{2}", tableDTO.Name, Environment.NewLine, rree.ToString());

                    ServiceLogger.LogException(string.Format("A table called => {0} already exists", tableDTO.Name), rree);
                }
                catch (InvalidDTOException ide)
                {
                    scope.VoteRollBack();
                    result.Success = false;
                    result.Message = string.Format("Problem with supplied information, cannot add table {0}; {1}{2}", tableDTO.Name, Environment.NewLine, ide.ToString());

                    ServiceLogger.LogException(string.Format("Problem with supplied information, cannot add table {0}", tableDTO.Name), ide);
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult DeleteLocation(DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            //::::::::::::::::::::::::::::::::::::::::::

            using (TransactionScope txnScope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {
                    Location loc = Location.Find(locationDTO.Id);

                    loc.Delete();  // will cascade to linked objects; session, tables.


                    result.Success = true;
                    result.Message = string.Format("location => {0} deleted.", loc.Name);
                    result.ObjectAffected = locationDTO.Id;

                    ServiceLogger.Log(string.Format("Location {0} deleted", loc.Name));
                }
                catch (ActiveRecordException are)
                {
                    txnScope.VoteRollBack();

                    ServiceLogger.LogException(String.Format("Error trying to delete location => {0}", locationDTO.Name), are);

                    result.Success = false;
                    result.Message = string.Format("Error trying to delete location => {0};{1}{2}", locationDTO.Name, Environment.NewLine, are.ToString());
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult DeleteReservable(DTOReservable tableDTO, DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            //::::::::::::::::::::::::::::::::::::::::::

            using (TransactionScope txnScope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {
                    Table table = Table.Find(tableDTO.Id);

                    // remove the reference to the table from the location
                    Location loc = Location.Find(locationDTO.Id);
                    loc.RemoveTable(table);

                    result.Success = true;
                    result.Message = string.Format("Table => {0} deleted ok.", tableDTO.Name);
                    result.ObjectAffected = tableDTO.Id;

                    ServiceLogger.Log(result.Message);
                }
                catch (InvalidOperationException ioe)
                {
                    txnScope.VoteRollBack();
                    result.Success = false;
                    result.Message = string.Format("Cannot delete table => {0}{1}{2}.", tableDTO.ToString(), Environment.NewLine, ioe.ToString());
                    result.ObjectAffected = tableDTO.Id;

                    ServiceLogger.LogException(string.Format("Cannot delete table => {0}.", tableDTO.ToString()), ioe);
                }
                catch (ActiveRecordException are)
                {
                    txnScope.VoteRollBack();

                    result.Success = false;
                    result.Message = string.Format("Error when trying to delete Table => {0};{1}{2}", tableDTO.Name, Environment.NewLine, are.ToString());
                    result.ObjectAffected = tableDTO.Id;

                    ServiceLogger.LogException(string.Format("Error when trying to delete Table => {0}", tableDTO.Name), are);
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult UpdateLocation(DTOLocation locationDTO)
        {
            ServiceLogger.Log(string.Format("Updating location {0}...", locationDTO.Name));

            //..........................................

            OperationResult result = new OperationResult();

            //::::::::::::::::::::::::::::::::::::::::::

            Location updatedLocation = null;
            using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {

                try
                {
                    updatedLocation = Location.Find(locationDTO.Id);

                    updatedLocation.Name = locationDTO.Name;
                    updatedLocation.DefaultDuration = locationDTO.DefaultDuration;

                    updatedLocation.PlanWidth = locationDTO.Width;
                    updatedLocation.PlanHeight = locationDTO.Height;

                    updatedLocation.BackgroundImg = locationDTO.BackgroundImg;

                    result.Success = true;
                    result.ObjectAffected = updatedLocation.Id;
                    result.Message = string.Format("Location {0} updated ok.", updatedLocation.Name);

                    ServiceLogger.Log(result.Message);
                }
                catch (ObjectNotFoundException onfe)
                {
                    scope.VoteRollBack();

                    result.Success = false;
                    result.ObjectAffected = 0;
                    result.Message = string.Format("Location {0} not updated{1}{2}.", updatedLocation.Name, Environment.NewLine, onfe.ToString());

                    ServiceLogger.LogException(string.Format("Location {0} not updated.", updatedLocation.Name), onfe);
                }
                catch (ActiveRecordException are)
                {
                    scope.VoteRollBack();

                    result.Success = false;
                    result.ObjectAffected = 0;
                    result.Message = string.Format("Location {0} not updated{1}{2}.", updatedLocation.Name, Environment.NewLine, are.ToString());

                    ServiceLogger.LogException(string.Format("Location {0} not updated.", updatedLocation.Name), are);
                }
                catch (Exception exc)
                {
                    scope.VoteRollBack();

                    result.Success = false;
                    result.ObjectAffected = 0;
                    result.Message = string.Format("Location {0} not updated{1}{2}.", updatedLocation.Name, Environment.NewLine, exc.ToString());

                    ServiceLogger.LogException(string.Format("Location {0} not updated.", updatedLocation.Name), exc);
                }

            }

            //::::::::::::::::::::::::::::::::::::::::::

            ServiceLogger.Log(string.Format("Finished updating location {0}...", locationDTO.Name));

            //..........................................

            return result;
        }

        // ---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult UpdateReservable(DTOReservable tableDTO)
        {
            OperationResult result = new OperationResult();

            //::::::::::::::::::::::::::::::::::::::::::

            using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {
                    Table tableToUpdate = Table.Find(tableDTO.Id);
                    Location location = Location.Find(tableToUpdate.ParentLocation.Id);

                    location.UpdateTable(tableToUpdate, tableDTO.Name, tableDTO.MaxGuests, tableDTO.X, tableDTO.Y, tableDTO.Width, tableDTO.Height, tableDTO.Shape, tableDTO.Number);

                    result.Success = true;
                    result.ObjectAffected = tableDTO.Id;
                    result.Message = string.Format("Table id => {0} update ok", tableDTO.Id);

                    ServiceLogger.Log(result.Message);
                }
                catch (ReservableAlreadyExistsException rae)
                {
                    scope.VoteRollBack();

                    // TODO: logging, proper message in result

                    result.Success = false;
                    result.ObjectAffected = 0;
                    result.Message = rae.ToString();

                    ServiceLogger.LogException(string.Format("Table already exists => {0}.", tableDTO.ToString()), rae);
                }
                catch (Castle.ActiveRecord.NotFoundException nfe)
                {
                    scope.VoteRollBack();
                    // TODO: logging, proper message in result
                    result.Success = false;
                    result.ObjectAffected = 0;
                    result.Message = nfe.ToString();

                    ServiceLogger.LogException(string.Format("Table not found => {0}.", tableDTO.ToString()), nfe);
                }
                catch (ActiveRecordException are)
                {
                    scope.VoteRollBack();
                    // TODO: logging, proper message in result
                    result.Success = false;
                    result.ObjectAffected = 0;
                    result.Message = are.ToString();

                    ServiceLogger.LogException(string.Format("Table => {0}.", tableDTO.ToString()), are);
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        #endregion
    }
}
