using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;

using NHibernate;
using NLog;
using StringProcessing;
using System.Timers;
using System.ServiceModel;

using Castle.ActiveRecord;
using Castle.ActiveRecord.Framework;
using Castle.ActiveRecord.Queries;

using Menumate.WinServices.Implementation.Contracts;
using Menumate.WinServices.Implementation.Contracts.DTO;
using Menumate.WinServices.Implementation.Contracts.BusinessObjects;

// TODO: Audit logging for every operation to give a 'security' log of operations, i.e.,
// what terminal did what and when. Later when a user may be identified with a terminal
// it would be possible to track user activity by terminal for reporting/time sheet etc purposes.

namespace Menumate.WinServices.Implementation
{
    /// <summary>
    ///  Reservation implementation.  This class is meant to be used in Reservation Service
    /// </summary>
    // ---------------------------------------------------------------------
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession, ConcurrencyMode = ConcurrencyMode.Reentrant
#if DEBUG
                    , IncludeExceptionDetailInFaults = true
#endif
)]
    public class ReservationsService : IReservations
    {
        #region Loggers
        protected static Logger Log = LogManager.GetLogger("Server");
        protected static Logger AuditLog = LogManager.GetLogger("Audit");
        #endregion

        #region ctors
        static ReservationsService()
        {
            // start heartbeat timer
            //_heartbeat.Elapsed += new ElapsedEventHandler(_heartbeat_Elapsed);
            //_heartbeat.Start();
            //Log.Info(string.Format("Heartbeat started, {0} second interval.", _heartbeat.Interval / 1000.0));
        }

        public ReservationsService()
        {
            Log.Info(string.Format("*** RESERVATIONS session starting for => {0}", OperationContext.Current.SessionId));
            ReservationsServiceInitHelper.InitDB();
            Log.Info(string.Format("Service Version {0}",
                        System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString()));
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Only used for testing at this stage.
        /// </summary>
        /// <param name="dataSource"></param>
        public ReservationsService(string dataSource)
        {
            throw new NotImplementedException();
        }
        #endregion

        #region dtors
        ~ReservationsService()
        {

        }
        #endregion


        /// <summary>
        /// Returns all locations in database. Note that this getter ASSUMES that the caller
        /// will handle the SessionScope/TransactionScope and any exceptions thrown by FindAll().
        /// </summary>
        private Location[] Locations
        {
            get
            {
                return Location.FindAll();
            }
        }

        #region IReservations Members

        #region static data
        // a list of connected clients
        private static List<IReservationsCallback> _clients = new List<IReservationsCallback>();

        // a map of clients and their associated terminal
        private static Dictionary<IReservationsCallback, Terminal> _clientsAndTerminals = new Dictionary<IReservationsCallback, Terminal>();

        // a map of table id's and their current interest level
        private static Dictionary<int, int> _tableInterestLevel = new Dictionary<int, int>();

        private static System.Timers.Timer _heartbeat = new System.Timers.Timer(6000);	// 6 second heartbeat

        #endregion

        private static object CLIENTLISTLOCK = new object();

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="terminalName"></param>
        /// <returns></returns>
        OperationResult IReservations.Logon(string terminalName)
        {
            OperationResult or = new OperationResult();

            // validate terminal name
            if (StringFunctions.IsAlphaNumeric(terminalName))
            {
                OperationContext ctx = OperationContext.Current;
                if (ctx != null)
                {
                    IReservationsCallback clientLoggingOn =
                        ctx.GetCallbackChannel<IReservationsCallback>();

                    // look up client in database
                    using (TransactionScope txn = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                    {
                        Terminal clientTerminal = null;
                        try
                        {
                            clientTerminal = Terminal.FindOne(NHibernate.Criterion.Expression.Eq("Name", terminalName));

                            if (clientTerminal == null)
                            {
                                Log.Info(string.Format("{0} not found in terminal list, creating a new one", terminalName));
                                clientTerminal = new Terminal(terminalName);

                                // try and set the current location and session
                                if (Locations != null)
                                {
                                    if (Locations.Length > 0)
                                    {
                                        clientTerminal.CurrentLocation = Locations[0];
                                        if (clientTerminal.CurrentLocation.Sessions.Count > 0)
                                        {
                                            clientTerminal.CurrentSession = clientTerminal.CurrentLocation.Sessions[0];
                                        }
                                    }
                                }
                                clientTerminal.Save();
                            }

                            // found (or created) client terminal
                            lock (ReservationsService.CLIENTLISTLOCK)
                            {
                                if (_clients.Contains(clientLoggingOn))
                                {
                                    or.Success = false;
                                    or.Message = "Already logged on.";
                                    Log.Error(string.Format("Already logged on => {0}", terminalName));
                                }
                                else
                                {
                                    if (clientTerminal == null)
                                    {
                                        or.Success = false;
                                        or.Message = string.Format("Logon failed => {0}, could not get terminal.", terminalName);
                                        Log.Error("Logon failed => {0}, could not get Terminal.", terminalName);

                                    }
                                    else
                                    {
                                        _clients.Add(clientLoggingOn);
                                        _clientsAndTerminals[clientLoggingOn] = clientTerminal;
                                        or.Success = true;
                                        or.Message = "Logon successfull.";
                                        or.ObjectAffected = clientTerminal.Id;
                                        Log.Info("Logon successfull => {0}, channel hash is => {1}", clientTerminal.Name,
                                            clientLoggingOn.GetHashCode());
                                    }
                                }
                            }
                        }
                        catch (ActiveRecordException are)
                        {
                            txn.VoteRollBack();
                            or.Success = false;
                            or.Message = string.Format("Exception when logging on for => {0};{1}", terminalName, are);
                            Log.ErrorException(string.Format("Exception when logging on for => {0}.", terminalName), are);
                        }
                    }
                }
                else
                {
                    or.Success = false;
                    or.Message = "Interalnal error, operation context is null.";
                    Log.Error(string.Format("The current OperationContext is null."));
                }
            }
            else
            {
                string m = string.Format("Terminal name had invalid characters => {0}", terminalName);
                or.Success = false;
                or.Message = m;
                Log.Error(m);
            }
            return or;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        OperationResult IReservations.LogOff()
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {

                try
                {

                    IReservationsCallback clientLoggingOn =
                            OperationContext.Current.GetCallbackChannel<IReservationsCallback>();

                    Terminal t = _clientsAndTerminals[clientLoggingOn];

                    lock (ReservationsService.CLIENTLISTLOCK)
                    {
                        _clients.Remove(clientLoggingOn);
                        _clientsAndTerminals.Remove(clientLoggingOn);
                    }
                    result.Success = true;
                    result.Message = string.Format("{0} logged off successfully.", t.Name);
                }
                catch (SystemException se)
                {
                    string message = "Error while logging off.";
                    Log.Error(string.Format("{0};{1}{2}", message, Environment.NewLine, se.ToString()));
                    result.Success = false;
                    result.Message = message;
                    result.LongMessage = se.ToString();
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on.";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Returns the terminals current location, will try to set it if not 
        /// already set.
        /// </summary>
        /// <returns>The DTO representation of the terminals current location.</returns>
        DTOLocation IReservations.GetCurrentLocation()
        {
            DTOLocation location = null;

            if (ClientLoggedIn())
            {
                using (TransactionScope txn = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {
                        Terminal clientsTerminal = GetLoggedInClientsTerminal();
                        if (clientsTerminal.CurrentLocation == null)
                        {      // no current location

                            // get one if possible
                            if (Locations != null && Locations.Length > 0)
                            {

                                clientsTerminal.CurrentLocation = Locations[0];
                                clientsTerminal.Save();

                                location = DTOFactory.BuildLocationDTO(clientsTerminal.CurrentLocation);
                            }
                        }
                        else
                        {
                            location = DTOFactory.BuildLocationDTO(clientsTerminal.CurrentLocation);
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        txn.VoteRollBack();
                        Log.ErrorException("Exception when getting current location.", are);
                    }
                }
            }

            return location;
        }

        // ---------------------------------------------------------------------
        DTOSession IReservations.GetCurrentSession()
        {

            DTOSession session = null;

            // verify client has logged in first
            if (ClientLoggedIn())
            {
                using (TransactionScope txn = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        Terminal clientsTerminal = GetLoggedInClientsTerminal();

                        // if no current session, get one if possible
                        if (clientsTerminal.CurrentSession == null)
                        {

                            if (clientsTerminal.CurrentLocation != null &&
                                clientsTerminal.CurrentLocation.Sessions.Count > 0)
                            {

                                // get the first one
                                clientsTerminal.CurrentSession = clientsTerminal.CurrentLocation.Sessions[0];
                                clientsTerminal.Save();
                                session = DTOFactory.BuildSessionDTO(clientsTerminal.CurrentSession);
                            }
                        }
                        else
                        {
                            session = DTOFactory.BuildSessionDTO(clientsTerminal.CurrentSession);
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        txn.VoteRollBack();
                        Log.ErrorException("Exception when getting current session.", are);
                    }
                }
            }
            return session;
        }

        // ---------------------------------------------------------------------
        public DateTime GetCurrentDate()
        {


            DateTime result = DateTime.MinValue;

            if (ClientLoggedIn())
            {
                using (new SessionScope())
                {
                    try
                    {
                        Terminal t = GetLoggedInClientsTerminal();
                        result = t.CurrentDate;
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException("Exception when getting current date.", are);
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult AddLocation(DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {
                        Location newLocation = new Location(locationDTO.Name);
                        newLocation.DefaultDuration = locationDTO.DefaultDuration;
                        newLocation.PlanWidth = locationDTO.Width;
                        newLocation.PlanHeight = locationDTO.Height;

                        if (Locations.Contains<Location>(newLocation, new LocationComparer()))
                        {
                            throw new LocationAlreadyExistsException(String.Format("Location {0} already exists", newLocation.Name));
                        }

                        newLocation.Save();
                        result.Success = true;
                        result.ObjectAffected = newLocation.Id;
                        result.Message = "Location added successfully.";

                        Terminal clientsTerminal = GetLoggedInClientsTerminal();
                        if (clientsTerminal != null)
                        {
                            AuditLog.Log(LogLevel.Info, string.Format("Location {0} added sucessfully by {1}",
                                newLocation.ToString(), GetLoggedInClientsTerminal().Name));
                        }

                        // notify clients
                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Locations);
                        });
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = are.ToString();

                        Log.ErrorException(string.Format("Exception while adding new location{0}{1}",
                            Environment.NewLine, locationDTO.Name), are);
                    }
                    catch (LocationAlreadyExistsException laee)
                    {

                        string message = string.Format("Location already exists; {0}.", locationDTO);

                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = message;
                        result.LongMessage = laee.ToString();

                        Log.ErrorException(message, laee);
                    }
                    catch (InvalidDTOException ide)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = ide.ToString();

                        Log.ErrorException(string.Format("Error adding location;{0}{1}",
                            Environment.NewLine, locationDTO.Name), ide);
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on.";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult AddReservable(DTOReservable tableDTO, DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();
            // verify client has logged in first
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {
                        Terminal term = GetLoggedInClientsTerminal();
                        DTOHelpers.ValidateReservableDTO(tableDTO);
                        DTOHelpers.ValidateLocationDTO(locationDTO);

                        Location location = Location.Find(locationDTO.Id);
                        Table table = DTOHelpers.TableFromDTO(tableDTO);

                        location.AddTable(table);

                        result.Success = true;
                        result.ObjectAffected = table.Id;
                        result.Message = String.Format("{0} added successfully to {1}",
                            tableDTO.ToString(), location.ToString());

                        DTOReservable tableDTOReturn = DTOFactory.BuildReservableDTO(table, term);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            TableStateChangedCallClientCallbacks(StateChangeType.Add,
                                new DTOReservable[] { tableDTOReturn });
                        });

                        AuditLog.Log(LogLevel.Info, string.Format("Table {0} added to location {1} by {2}",
                            tableDTO.Name, location.Name, GetLoggedInClientsTerminal().Name));
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Could not add table {0};{1}{2}",
                            tableDTO.Name, Environment.NewLine, are.ToString());
                    }
                    catch (ReservableAlreadyExistsException rree)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("A table called => {0} already exists;{1}{2}",
                            tableDTO.Name, Environment.NewLine, rree.ToString());
                    }
                    catch (InvalidDTOException ide)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Problem with supplied information, cannot add table;{1}{2}",
                            tableDTO.Name, Environment.NewLine, ide.ToString());
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult AddReservation(DTOReservation reservationDTO)
        {

            OperationResult result = new OperationResult();
            // verify client has logged in first
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {
                        DTOHelpers.ValidateReservationDTO(reservationDTO);

                        Reservation newReservation = DTOHelpers.ReservationFromDTO(reservationDTO);
                        newReservation.Session = Session.Find(reservationDTO.Session.Id);

                        Location location = Location.Find(reservationDTO.Location.Id);


                        IList<Table> tableList = new List<Table>();
                        foreach (DTOReservable tableDto in reservationDTO.Reservables)
                        {
                            Table tableToAdd = Table.Find(tableDto.Id);
                            ReservationsService.DecreaseInterestLevelForTable(tableToAdd.Id);
                            if (!tableList.Contains(tableToAdd))
                            {
                                tableList.Add(Table.Find(tableDto.Id));
                            }
                        }

                        newReservation.ReservedList = tableList;

                        Guest guest = null;

                        if (reservationDTO.Reserver.Id == 0)
                        {     // new guest
                            guest = new Guest(reservationDTO.Reserver.GuestName,
                                reservationDTO.Reserver.GuestPhone, reservationDTO.Reserver.GuestEmail);
                            if (string.IsNullOrEmpty(guest.Name))
                            {
                                guest.Name = "[no name]";
                            }
                            guest.Save();
                        }
                        else
                        {             // returning guest
                            try
                            {
                                guest = Guest.Find(reservationDTO.Reserver.Id); // throws Castle.ActiveRecord.NotFoundException

                                guest.Name = reservationDTO.Reserver.GuestName;
                                guest.Phone = reservationDTO.Reserver.GuestPhone;
                                guest.Email = reservationDTO.Reserver.GuestEmail;
                                guest.Update();

                            }
                            catch (Castle.ActiveRecord.NotFoundException onfe)
                            {
                                // omg! guest not found even with id
                                Log.Error(onfe.ToString());

                                // new guest as last resort
                                guest = new Guest(reservationDTO.Reserver.GuestName,
                                    reservationDTO.Reserver.GuestPhone, reservationDTO.Reserver.GuestEmail);
                                guest.Save();
                            }
                        }

                        // set the guest now
                        newReservation.Reserver = guest;

                        newReservation.PartyName = reservationDTO.PartyName;
                        newReservation.Note = reservationDTO.Note;

                        location.AddReservation(newReservation);

                        string message = string.Format("Reservation added ok for => {0} arriving at => {1}",
                            newReservation.Reserver.ToString(), newReservation.Arrival.ToString());

                        result.Success = true;
                        result.ObjectAffected = newReservation.Id;
                        result.Message = message;

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });

                        AuditLog.Log(LogLevel.Info, string.Format("{0}, by {1}.",
                            message,
                            GetLoggedInClientsTerminal().Name));
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();

                        string message = string.Format("Could not add reservation for => {0} arriving at => {1}",
                            reservationDTO.Reserver.ToString(), reservationDTO.Arrival.ToString());
                        result.Success = false;
                        result.Message = message;
                        result.LongMessage = are.ToString();
                        Log.ErrorException(message, are);
                    }
                    catch (ReservationException re)
                    {
                        scope.VoteRollBack();

                        string message = string.Format("Could not add reservation for => {0} arriving at => {1}",
                            reservationDTO.Reserver.ToString(), reservationDTO.Arrival.ToString());
                        result.Success = false;
                        result.Message = message;
                        result.LongMessage = re.ToString();
                        Log.ErrorException(message, re);
                    }
                    catch (InvalidDTOException ide)
                    {

                        scope.VoteRollBack();

                        string message = string.Format("Could not add reservation arriving at => {0}",
                            reservationDTO.Arrival.ToString());
                        result.Success = false;
                        result.Message = message;
                        result.LongMessage = ide.ToString();
                        Log.ErrorException(message, ide);
                    }
                    catch (InvalidOperationException ioe)
                    {
                        scope.VoteRollBack();

                        string message = string.Format("Could not add reservation arriving at => {0}",
                            reservationDTO.Arrival.ToString());
                        result.Success = false;
                        result.Message = message;
                        result.LongMessage = ioe.ToString();
                        Log.ErrorException(message, ioe);
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult AddReserver(DTOReserver guestDTO)
        {
            OperationResult result = new OperationResult();
            // verify client has logged in first
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {

                        Guest newGuest = DTOHelpers.GuestFromDTO(guestDTO);
                        newGuest.Save();

                        result.ObjectAffected = newGuest.Id;
                        result.Success = true;
                        result.Message = string.Format("Guest =>{0} added successfully", newGuest.ToString());
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = string.Format("Could not add guest => {0};{1}{2}",
                            guestDTO.ToString(), Environment.NewLine, are.ToString());
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sessionDTO"></param>
        /// <param name="locationDTO"></param>
        /// <returns></returns>
        public OperationResult AddSession(DTOSession sessionDTO, DTOLocation locationDTO)
        {


            // TODO: validate incomming dto's, i.e., check for sane values

            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {

                using (Castle.ActiveRecord.TransactionScope txScope =
                    new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {

                        Session newSession = new Session(sessionDTO.Name, sessionDTO.StartTime, sessionDTO.EndTime);
                        newSession.Save();

                        Location location = Location.Find(locationDTO.Id);
                        location.AddSession(newSession);


                        result.Success = true;
                        result.ObjectAffected = newSession.Id;
                        result.Message = String.Format("Session => {0} added successfully.", sessionDTO.Name);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Sessions);
                        });

                        WriteToAuditLog(result.Message);

                    }
                    catch (NHibernate.ObjectNotFoundException onfe)
                    {
                        txScope.VoteRollBack();
                        // TODO: logging

                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = String.Format("Could not find location => {0};" + Environment.NewLine + "{1}",
                            locationDTO.Name, onfe.Message);
                    }
                    catch (SessionAlreadyExistsException saee)
                    {
                        txScope.VoteRollBack();
                        result.Success = false;
                        result.Message = String.Format("Could not add session => {0}, a session with that name already exists;" +
                            Environment.NewLine + "{1}",
                                sessionDTO.Name, saee.Message);
                    }
                    catch (ActiveRecordException are)
                    {
                        // TODO: logging
                        txScope.VoteRollBack();

                        result.Success = false;
                        result.Message = String.Format("Problem adding the session => {0} to the location => {1};"
                            + Environment.NewLine + "{2}", sessionDTO.Name, locationDTO.Name, are.Message);
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult CancelReservation(DTOReservation resDTO, DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {

                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {

                        Reservation res = Reservation.Find(resDTO.Id);
                        Location l = Location.Find(locationDTO.Id);
                        l.CancelReservation(res);

                        result.Success = true;
                        result.ObjectAffected = res.Id;
                        result.Message = string.Format("Reservation successfully cancelled.");

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();

                        Log.Error(string.Format("Problem cancelling reservation;{0}{1}",
                                    Environment.NewLine, are.ToString()));

                        result.Success = false;
                        result.Message = string.Format("Problem cancelling reservation;{0}{1}",
                                    Environment.NewLine, are.ToString());
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult DeleteLocation(DTOLocation locationDTO)
        {

            OperationResult result = new OperationResult();

            using (TransactionScope txnScope =
                    new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {

                    Location loc = Location.Find(locationDTO.Id);

                    // TODO: are there any reservations? if so, don't delete

                    // TODO: why do this? should be defined in the model to be cleaned up after cascade
                    // clean up objects with references; terminal
                    Terminal[] terminals = Terminal.FindAll();
                    foreach (Terminal t in terminals)
                    {
                        if (t.CurrentLocation.Id == loc.Id)
                        {
                            t.CurrentLocation = null;
                            t.CurrentSession = null;
                            t.Save();
                        }
                    }

                    loc.Delete();  // will cascade to linked objects; session, tables.


                    result.Success = true;
                    result.Message = string.Format("location => {0} deleted.",
                        loc.Name);
                    result.ObjectAffected = locationDTO.Id;

                    ThreadPool.QueueUserWorkItem(delegate
                    {
                        CallClientCallbacks(CallbacksToActivate.Locations);
                    });

                    WriteToAuditLog(result.Message);

                }
                catch (ActiveRecordException are)
                {
                    txnScope.VoteRollBack();

                    Log.DebugException(String.Format("Error trying to delete location => {0}", locationDTO.Name), are);

                    result.Success = false;
                    result.Message = string.Format("Error trying to delete location => {0};{1}{2}",
                        locationDTO.Name, Environment.NewLine, are.ToString());
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult DeleteReservable(DTOReservable tableDTO, DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {
                using (TransactionScope txnScope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {
                        Table table = Table.Find(tableDTO.Id);
                        Terminal term = GetLoggedInClientsTerminal();

                        // remove the reference to the table from the location
                        Location loc = Location.Find(locationDTO.Id);
                        loc.RemoveTable(table);

                        result.Success = true;
                        result.Message = string.Format("Table => {0} deleted ok.", tableDTO.Name);
                        result.ObjectAffected = tableDTO.Id;

                        DTOReservable tableDTOReturn = DTOFactory.BuildReservableDTO(table, term);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            TableStateChangedCallClientCallbacks(StateChangeType.Delete,
                                new DTOReservable[] { tableDTOReturn });
                        });

                        WriteToAuditLog(result.Message);

                    }
                    catch (InvalidOperationException ioe)
                    {
                        txnScope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Cannot delete table => {0}{1}{2}.",
                            tableDTO.ToString(), Environment.NewLine, ioe.ToString());
                        result.ObjectAffected = tableDTO.Id;
                    }
                    catch (ActiveRecordException are)
                    {
                        txnScope.VoteRollBack();

                        result.Success = false;
                        result.Message = string.Format("Error when trying to delete Table => {0};{1}{2}", tableDTO.Name,
                            Environment.NewLine, are.ToString());
                        result.ObjectAffected = tableDTO.Id;

                        Log.ErrorException(string.Format("Error when trying to delete Table => {0}", tableDTO.Name), are);
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult DeleteSession(DTOSession sessionToDeleteDTO, DTOLocation locationDTO)
        {
            OperationResult result = new OperationResult();

            using (TransactionScope txnScope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
            {
                try
                {
                    // find the session
                    Session s = Session.Find(sessionToDeleteDTO.Id);

                    // get the parent location
                    Location loc = Location.Find(s.ParentLocation.Id);

                    // TODO: are there any reservations using this session? if so, cannot delete

                    loc.DeleteSession(s);


                    result.Success = true;
                    result.Message = string.Format("session => {0} deleted.", sessionToDeleteDTO.Name);
                    result.ObjectAffected = sessionToDeleteDTO.Id;

                    ThreadPool.QueueUserWorkItem(delegate
                    {
                        CallClientCallbacks(CallbacksToActivate.Sessions);
                    });

                    WriteToAuditLog(result.Message);
                }
                catch (InvalidOperationException ioe)
                {
                    txnScope.VoteRollBack();
                    result.Success = false;
                    result.Message = string.Format("Reservations exists for session => {0}, cannot delete.", sessionToDeleteDTO.Name);
                    result.ObjectAffected = sessionToDeleteDTO.Id;
                }
                catch (ActiveRecordException are)
                {
                    txnScope.VoteRollBack();
                    Log.DebugException(String.Format("Error deleteing session => {0}", sessionToDeleteDTO.Name), are);
                    result.Success = false;
                    result.Message = string.Format("Error trying to delete session => {0};{1}{2}",
                        sessionToDeleteDTO.Name, Environment.NewLine, are.ToString());
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public DTOReservation[] GetAllReservationsByLocation(int locationId)
        {
            List<DTOReservation> returnListDTO = new List<DTOReservation>();
            if (ClientLoggedIn())
            {
                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        IList<Reservation> returnList = Location.Find(locationId).Reservations;
                        if (returnList != null)
                        {
                            foreach (Reservation g in returnList)
                            {
                                returnListDTO.Add(DTOFactory.BuildReservationDTO(g));
                            }
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException(String.Format("Error getting Reservations by Location list"), are);
                    }
                }
            }
            return returnListDTO.ToArray<DTOReservation>();
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTOReserver[] IReservations.GetAllGuests()
        {
            List<DTOReserver> returnListDTO = new List<DTOReserver>();
            if (ClientLoggedIn())
            {
                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        Guest[] returnList = null;
                        returnList = Guest.FindAll();

                        if (returnList != null)
                        {
                            foreach (Guest g in returnList)
                            {
                                returnListDTO.Add(DTOFactory.BuildReserverDTO(g));
                            }
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.DebugException(String.Format("Error getting Guest list"), are);
                    }
                }
            }
            return returnListDTO.ToArray<DTOReserver>();
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Return an array of reservation DTO objects for the day, location and session specified.
        /// </summary>
        /// <param name="day"></param>
        /// <returns></returns>
        public DTOReservation[] GetReservationsForDay(DateTime day, int locationId, int sessionId)
        {
            DTOReservation[] returnList = null;
            if (ClientLoggedIn())
            {
                using (SessionScope scope = new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        Location location = null;
                        Session session = null;
                        location = Location.Find(locationId);
                        session = Session.Find(sessionId);

                        // setup the parameters
                        DateTime startOfDay = new DateTime(day.Year, day.Month, day.Day,
                            0, 0, 0);
                        DateTime endOfDay = new DateTime(day.Year, day.Month, day.Day,
                            23, 59, 59);

                        SimpleQuery<Reservation> q = new SimpleQuery<Reservation>(
                            @"from Reservation res
							where res.ParentLocation.id = :location_id
							and res.Session.id = :session_id
							and res.Arrival between :start_of_day and :end_of_day");
                        q.SetParameter("location_id", locationId);
                        q.SetParameter("session_id", sessionId);
                        q.SetParameter("start_of_day", startOfDay);
                        q.SetParameter("end_of_day", endOfDay);

                        Reservation[] reservationList = q.Execute();

                        IList<DTOReservation> dtoReservationList = new List<DTOReservation>();
                        foreach (Reservation r in reservationList)
                        {
                            dtoReservationList.Add(DTOFactory.BuildReservationDTO(r));
                        }
                        returnList = dtoReservationList.ToArray<DTOReservation>();

                    }
                    catch (NHibernate.ObjectNotFoundException onf)
                    {
                        Log.DebugException(string.Format("day => {0}, locationId => {1}, sessionId => {2}",
                            day, locationId, sessionId), onf);
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.DebugException(string.Format("day => {0}, locationId => {1}, sessionId => {2}",
                            day, locationId, sessionId), nfe);
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.DebugException("Error getting reservations for day", are);
                    }
                }
            }
            return returnList;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="searchString"></param>
        /// <returns></returns>
        public DTOReservation[] SearchForReservationsByGuestName(string searchString)
        {
            List<DTOReservation> reservations = new List<DTOReservation>();
            if (ClientLoggedIn())
            {
                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {

                        string s = "%" + searchString + "%";

                        SimpleQuery<Guest> q = new SimpleQuery<Guest>(
                            @"from Guest g
							where g.Name like :search_string");
                        q.SetParameter("search_string", s);
                        Guest[] guestList = q.Execute();

                        SimpleQuery<Reservation> qryReservationsForGuest = new SimpleQuery<Reservation>(
                            @"from Reservation res
								where res.Reserver.id=:guest_id");

                        foreach (Guest guest in guestList)
                        {
                            qryReservationsForGuest.SetParameter("guest_id", guest.Id);
                            Reservation[] reservationsResult = qryReservationsForGuest.Execute();
                            foreach (Reservation r in reservationsResult)
                            {
                                reservations.Add(DTOFactory.BuildReservationDTO(r));
                            }
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.DebugException(string.Format("Error while searching for reservations by guest => {0}",
                            searchString), are);
                    }
                }
            }
            return reservations.ToArray<DTOReservation>();
        }

        // ---------------------------------------------------------------------
        public DTOReservation[] GetReservationsByReservable(DTOReservable reservableDTO, DTOLocation locationDTO)
        {

            List<DTOReservation> reservations = new List<DTOReservation>();
            if (ClientLoggedIn())
            {

                using (new SessionScope())
                {
                    try
                    {

                        Table table = null;
                        IList<Reservation> reservationsUsedBy = null;
                        table = Table.Find(reservableDTO.Id);
                        reservationsUsedBy = table.ReservationsThatUseMe;

                        foreach (Reservation reservation in reservationsUsedBy)
                        {
                            reservations.Add(DTOFactory.BuildReservationDTO(reservation));
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException("Error getting reservation for table.", are);
                    }
                }
            }

            return reservations.ToArray<DTOReservation>();
        }

        // ---------------------------------------------------------------------
        public DTOReserver[] GetReserversByName(string guestName)
        {
            List<DTOReserver> matchesList = new List<DTOReserver>();
            if (ClientLoggedIn())
            {
                using (new SessionScope())
                {
                    Guest[] matches;
                    matches = Guest.FindAll(NHibernate.Criterion.Expression.Like("Name", guestName,
                        NHibernate.Criterion.MatchMode.Anywhere));
                    foreach (Guest g in matches)
                    {
                        matchesList.Add(DTOFactory.BuildReserverDTO(g));
                    }
                }
            }
            return matchesList.ToArray();
        }

        // ---------------------------------------------------------------------
        public DTOLocation[] GetAllLocations()
        {
            List<DTOLocation> returnList = new List<DTOLocation>();
            if (ClientLoggedIn())
            {

                using (new SessionScope())
                {
                    try
                    {
                        Location[] locations = Location.FindAll();
                        for (int i = 0; i < locations.Length; i++)
                        {
                            returnList.Add(DTOFactory.BuildLocationDTO(locations[i]));
                        }
                    }
                    catch (NHibernate.ObjectNotFoundException onf)
                    {
                        Log.ErrorException("Could not return list of locations.", onf);
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.ErrorException("Could not return list of locations.", nfe);
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException("Could not return list of locations.", are);
                    }
                }
            }
            return returnList.ToArray<DTOLocation>();
        }

        // ---------------------------------------------------------------------
        public DTOSession[] GetSessionsForLocation(int locationId)
        {
            List<DTOSession> returnList = new List<DTOSession>();
            if (ClientLoggedIn())
            {
                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        Location location = null;
                        location = Location.Find(locationId);
                        if (location != null)
                        {
                            foreach (Session s in location.Sessions)
                            {
                                returnList.Add(DTOFactory.BuildSessionDTO(s));
                            }
                        }
                    }
                    catch (NHibernate.ObjectNotFoundException onf)
                    {
                        Log.ErrorException(String.Format("Error getting Session list for location, id => {0}", locationId), onf);
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.ErrorException(String.Format("Error getting Session list for location, id => {0}", locationId), nfe);
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException(String.Format("Error getting Session list for location, id => {0}", locationId), are);
                    }
                }
            }
            return returnList.ToArray<DTOSession>();
        }

        // ---------------------------------------------------------------------
        public DTOReservable[] GetTablesForLocation(int locationId)
        {
            List<DTOReservable> returnList = new List<DTOReservable>();
            if (ClientLoggedIn())
            {
                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {

                        Terminal term = GetLoggedInClientsTerminal();
                        Location location = null;
                        location = Location.Find(locationId);
                        if (location != null)
                        {
                            foreach (Table t in location.Tables)
                            {
                                returnList.Add(DTOFactory.BuildReservableDTO(t, term));
                            }
                        }
                    }
                    catch (NHibernate.ObjectNotFoundException onf)
                    {
                        Log.ErrorException(String.Format("Error getting Table list for location, id => {0}", locationId), onf);
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.ErrorException(String.Format("Error getting Table list for location, id => {0}", locationId), nfe);
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException(String.Format("Error getting Table list for location, id => {0}", locationId), are);
                    }
                }
            }
            return returnList.ToArray<DTOReservable>();
        }

        public byte[] GetBackgroundImgDataForLocation(int locationid)
        {
            byte[] data = null;
            if (ClientLoggedIn())
            {
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
                        Log.ErrorException(String.Format("Error getting background image data for location, id => {0}", locationid), onf);
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.ErrorException(String.Format("Error getting background image data for location, id => {0}", locationid), nfe);
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.ErrorException(String.Format("Error getting background image data for location, id => {0}", locationid), are);
                    }
                }
            }
            return data;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="locationId"></param>
        /// <param name="inspectionTime"></param>
        /// <param name="excludedReservation"></param>
        /// <returns></returns>
        public DTOReservable[] GetTableAvailabilityForLocation(int locationId, DateTime inspectionTime,
                DTOReservation excludedReservation, TimeSpan duration)
        {
            List<DTOReservable> returnList = new List<DTOReservable>();
            if (ClientLoggedIn())
            {
                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        Location location = Location.Find(locationId);

                        Terminal term = GetLoggedInClientsTerminal();

                        Reservation excludedRes = null;
                        if (excludedReservation != null)
                        {
                            excludedRes = Reservation.Find(excludedReservation.Id);
                        }

                        foreach (Table table in location.Tables)
                        {
                            Reservability reservability = location.GetReservability(table.ReservationsThatUseMe, table,
                                inspectionTime, excludedRes, duration);
                            DTOReservable dtoTable = DTOFactory.BuildReservableDTO(table, term);
                            dtoTable.Available = reservability.FullyAvailable;
                            returnList.Add(dtoTable);
                        }
                    }
                    catch (NHibernate.ObjectNotFoundException onf)
                    {
                        Log.Error(string.Format("Error getting availability of table for location id => {0};{1}{2}",
                            locationId, Environment.NewLine, onf.ToString()));
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.Error(string.Format("Error getting availability of table for location id => {0};{1}{2}",
                            locationId, Environment.NewLine, nfe.ToString()));
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.Error(string.Format("Error getting availability of table for location id => {0};{1}{2}",
                            locationId, Environment.NewLine, are.ToString()));
                    }
                }
            }
            return returnList.ToArray<DTOReservable>();
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Returns true iff the table belongs to a reservation that is in
        /// progress now.
        /// TODO: not used?
        /// </summary>
        /// <param name="table"></param>
        /// <returns></returns>
        public bool DoesReservationIntersect(DTOReservable dtoTable)
        {
            bool result = false;
            DateTime comparisonDateTime = DateTime.Now;

            if (ClientLoggedIn())
            {

                using (new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        Table table = Table.Find(dtoTable.Id);
                        IList<Reservation> reservations = table.ReservationsThatUseMe;

                        foreach (Reservation reservation in reservations)
                        {
                            if ((comparisonDateTime >= reservation.Arrival) &&
                                    (comparisonDateTime <= (reservation.Arrival + reservation.Duration)))
                            {
                                // intersect found, bail now
                                result = true;
                                break;
                            }
                        }
                    }
                    catch (NHibernate.ObjectNotFoundException onf)
                    {
                        Log.Error(string.Format("Error find reservations intersect;{0}{1}",
                            Environment.NewLine, onf.ToString()));
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        Log.Error(string.Format("Error find reservations intersect;{0}{1}",
                            Environment.NewLine, nfe.ToString()));
                    }
                    catch (ActiveRecordException are)
                    {
                        Log.Error(string.Format("Error find reservations intersect;{0}{1}",
                            Environment.NewLine, are.ToString()));
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="table"></param>
        /// <returns></returns>
        public OperationResult IncreaseInterestLevel(int id)
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {
                using (new SessionScope())
                {
                    try
                    {
                        Table table = Table.Find(id);
                        ReservationsService.IncreaseInterestLevelForTable(table.Id);
                        Terminal term = GetLoggedInClientsTerminal();
                        result.Success = true;
                        result.Message = "Table interest level increased ok.";
                        result.ObjectAffected = table.Id;

                        DTOReservable tableDTO = DTOFactory.BuildReservableDTO(table, term);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            TableStateChangedCallClientCallbacks(StateChangeType.Update,
                                new DTOReservable[] { tableDTO });
                        });
                    }
                    catch (ActiveRecordException are)
                    {
                        result.Success = false;
                        result.Message = are.ToString();
                    }
                    catch (SystemException e)
                    {
                        result.Success = false;
                        result.Message = e.ToString();
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on.";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="table"></param>
        /// <returns></returns>
        public OperationResult DecreaseInterestLevel(int id)
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {

                using (new SessionScope())
                {

                    try
                    {
                        Table table = Table.Find(id);
                        ReservationsService.DecreaseInterestLevelForTable(table.Id);
                        Terminal term = GetLoggedInClientsTerminal();
                        result.Success = true;
                        result.Message = "Table interest level decreased ok.";
                        result.ObjectAffected = table.Id;

                        DTOReservable tableDTOReturn = DTOFactory.BuildReservableDTO(table, term);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            TableStateChangedCallClientCallbacks(StateChangeType.Update,
                                new DTOReservable[] { tableDTOReturn });
                        });
                    }
                    catch (ActiveRecordException are)
                    {
                        result.Success = false;
                        result.Message = are.ToString();
                    }
                    catch (SystemException e)
                    {
                        result.Success = false;
                        result.Message = e.ToString();
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on.";
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="res"></param>
        /// <returns></returns>
        public OperationResult DecreaseInterestLevelForReservation(DTOReservation res)
        {
            OperationResult result = new OperationResult();

            using (new SessionScope())
            {

                try
                {

                    bool reservationExists = Reservation.Exists<int>(res.Id);
                    if (reservationExists)
                    {
                        // only decrease interest level for a table if it was additional to the existing reservation
                        Reservation reservation = Reservation.Find(res.Id);
                        foreach (DTOReservable tableDto in res.Reservables)
                        {
                            Table t = Table.Find(tableDto.Id);
                            if (!reservation.ReservedList.Contains(t))
                            {
                                ReservationsService.DecreaseInterestLevelForTable(t.Id);
                            }
                        }
                    }
                    else
                    {
                        foreach (DTOReservable table in res.Reservables)
                        {
                            Table t = Table.Find(table.Id);
                            ReservationsService.DecreaseInterestLevelForTable(t.Id);
                        }
                    }

                    result.Success = true;
                    result.Message = "Interest decreased on all tables for reservation";
                    result.ObjectAffected = res.Id;

                }
                catch (ActiveRecordException are)
                {
                    result.Success = false;
                    result.Message = are.ToString();
                }
                catch (SystemException e)
                {
                    result.Success = false;
                    result.Message = e.ToString();
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="reservation"></param>
        /// <returns></returns>
        public OperationResult GetReservationStatus(DTOReservation reservationDto, bool excludeSelf)
        {
            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {

                using (SessionScope scope = new SessionScope(FlushAction.Auto))
                {

                    try
                    {

                        // assume that the availability is ok
                        result.Success = true;

                        Reservation res = null;
                        IList<Table> tables = new List<Table>();

                        if (excludeSelf)
                        {
                            res = Reservation.Find(reservationDto.Id);
                        }
                        else
                        {
                            res = DTOHelpers.ReservationFromDTO(reservationDto);
                            res.ParentLocation = Location.Find(reservationDto.Location.Id);
                        }

                        // get the real list of tables from db
                        foreach (Table table in res.ReservedList)
                        {
                            tables.Add(Table.Find(table.Id));
                        }

                        foreach (Table t in tables)
                        {
                            if (excludeSelf)
                            {  // reservation exists so exclude this one
                                if (!(res.ParentLocation.GetReservability(t.ReservationsThatUseMe, t, res.Arrival, res, res.Duration).FullyAvailable))
                                {
                                    result.Success = false;
                                    result.Message = string.Format("Clash on {0}", t.Name);
                                    //result.ObjectAffected = t.Id;
                                    break;
                                }
                            }
                            else
                            {
                                if (!(res.ParentLocation.GetReservability(t.ReservationsThatUseMe, t, res.Arrival, null, res.Duration).FullyAvailable))
                                {
                                    result.Success = false;
                                    result.Message = string.Format("Clash on {0}", t.Name);
                                    //result.ObjectAffected = t.Id;
                                    break;
                                }
                            }
                        }

                        result.Message = "Status OK";
                    }
                    catch (ObjectNotFoundException onfe)
                    {
                        result.Success = false;
                        result.Message = onfe.ToString();
                        // TODO: log
                    }
                    catch (ActiveRecordException are)
                    {
                        result.Success = false;
                        result.Message = are.ToString();
                        // TODO: log
                    }
                    catch (SystemException e)
                    {
                        result.Success = false;
                        result.Message = e.ToString();
                        // TODO: log
                        throw;
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "No logged on.";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public DTOReservability GetReservability(int locationId, DTOReservation[] reservations, DTOReservable tableDto,
                DateTime inspectionDateTime,
                DTOReservation excludedReservation, TimeSpan duration)
        {

            DTOReservability result = null;

            if (ClientLoggedIn())
            {

                using (SessionScope scope = new SessionScope(FlushAction.Auto))
                {
                    try
                    {
                        Location location = Location.Find(locationId);
                        Table t = Table.Find(tableDto.Id);

                        Reservation excludedRes = null;
                        if (excludedReservation != null)
                        {
                            excludedRes = Reservation.Find(excludedReservation.Id);
                        }

                        List<Reservation> reservationsList = new List<Reservation>();
                        if (reservations != null)
                        {
                            foreach (DTOReservation dtoRes in reservations)
                            {
                                reservationsList.Add(Reservation.Find(dtoRes.Id));
                            }
                        }

                        Reservability reservability =
                            location.GetReservability(reservationsList, t, inspectionDateTime, excludedRes,
                                duration);

                        result = DTOFactory.BuildReservabilityDTO(reservability);
                    }
                    catch (ObjectNotFoundException onfe)
                    {
                        // TODO: log
                    }
                    catch (ActiveRecordException are)
                    {
                        // TODO: log
                    }
                    catch (SystemException e)
                    {
                        // TODO: log
                        throw;
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult ReleaseReservation(DTOReservation reservation, DateTime releaseDateTime)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {

                using (TransactionScope scope = new TransactionScope(TransactionMode.New,
                        OnDispose.Commit))
                {

                    try
                    {

                        Reservation res = Reservation.Find(reservation.Id);
                        res.Release(releaseDateTime);

                        result.Success = true;
                        result.ObjectAffected = res.Id;
                        result.Message = "Reservation released successfully";

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (NHibernate.ObjectNotFoundException onfe)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = onfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = are.ToString();
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult SeatReservation(DTOReservation reservation)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {

                using (TransactionScope scope = new TransactionScope(TransactionMode.New,
                        OnDispose.Commit))
                {

                    try
                    {

                        Reservation res = Reservation.Find(reservation.Id);
                        Location location = Location.Find(reservation.Location.Id);
                        location.SeatReservation(res);

                        result.Success = true;
                        result.ObjectAffected = res.Id;
                        result.Message = "Reservation seated successfully.";

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (ReservationException re)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = re.ToString();
                    }
                    catch (NHibernate.ObjectNotFoundException onfe)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = onfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = are.ToString();
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult SetCurrentLocation(int locationId)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {

                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    Terminal clientsTerminal = GetLoggedInClientsTerminal();
                    Location newCurrentLocation = null;
                    try
                    {
                        newCurrentLocation = Location.Find(locationId);
                        clientsTerminal.CurrentLocation = newCurrentLocation;
                        clientsTerminal.Save();

                        result.Success = true;
                        result.ObjectAffected = newCurrentLocation.Id;
                        result.Message = "Current Location set successfully.";

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });
                    }
                    catch (NHibernate.ObjectNotFoundException onfe)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = onfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = are.ToString();
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult SetCurrentSession(int sessionId)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    Terminal clientsTerminal = GetLoggedInClientsTerminal();
                    Session newCurrentSession = null;

                    try
                    {
                        newCurrentSession = Session.Find(sessionId);
                        clientsTerminal.CurrentSession = newCurrentSession;
                        clientsTerminal.Save();

                        result.Success = true;
                        result.ObjectAffected = newCurrentSession.Id;
                        result.Message = string.Format("Current session set to => {0}",
                            newCurrentSession.ToString());

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });
                    }
                    catch (NHibernate.ObjectNotFoundException onfe)
                    {

                        // TODO: how to set the current session to nothing/null?

                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = onfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = are.ToString();
                    }
                }
            }
            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult SetCurrentDate(DateTime newDate)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {
                        Terminal clientsTerminal = GetLoggedInClientsTerminal();
                        clientsTerminal.CurrentDate = newDate;
                        clientsTerminal.Save();

                        result.Success = true;
                        result.Message = string.Format("Terminal =>{0} successfully set its current date to => {1}.",
                            clientsTerminal.Name, clientsTerminal.CurrentDate.ToLongTimeString());

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });

                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Unable to set current date;{0}{1}",
                                    Environment.NewLine, are.ToString());
                    }
                }
            }
            return result;
        }


        // ---------------------------------------------------------------------
        public OperationResult UpdateDefaultDuration(DTOLocation location)
        {
            OperationResult result = new OperationResult();
            Location updatedLocation = null;
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        updatedLocation = Location.Find(location.Id);

                        updatedLocation.DefaultDuration = location.DefaultDuration;

                        result.Success = true;
                        result.Message = string.Format("Location {0} updated ok.", updatedLocation.Name);

                        WriteToAuditLog(result.Message);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Sessions);
                        });

                    }
                    catch (ObjectNotFoundException onfe)
                    {
                        scope.VoteRollBack();

                        result.Success = false;
                        result.Message = string.Format("Location {0} not updated.",
                            updatedLocation.Name, Environment.NewLine, onfe.ToString());
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();

                        result.Success = false;
                        result.Message = string.Format("Location {0} not updated{1}{2}.",
                            updatedLocation.Name, Environment.NewLine, are.ToString());
                    }

                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }

            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult UpdateReservation(DTOReservation updatedReservationDto)
        {
            OperationResult result = new OperationResult();
            // verify client has logged in first
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {

                        DTOHelpers.ValidateReservationDTO(updatedReservationDto);

                        // get the reservation before any modification as the excluded reservation
                        Reservation excludedReservation = Reservation.Find(updatedReservationDto.Id);

                        // get it again, this one get modified
                        Reservation updatedReservation = Reservation.Find(updatedReservationDto.Id);

                        if (updatedReservationDto.Reservables.Length == 0)
                        {
                            throw new ReservationException("No tables.");
                        }

                        // compare table list of saved reservation and incomming dto and decrease
                        // interest level only on new ones
                        IList<Table> tableList = new List<Table>();
                        foreach (DTOReservable tableDto in updatedReservationDto.Reservables)
                        {
                            Table t = Table.Find(tableDto.Id);
                            tableList.Add(t);
                            if (!updatedReservation.ReservedList.Contains(t))
                            {
                                // this is a new table to the reservation, reduce is interest level
                                ReservationsService.DecreaseInterestLevelForTable(t.Id);
                            }
                        }

                        // update the fields
                        updatedReservation.ParentLocation = Location.Find(updatedReservationDto.Location.Id);
                        updatedReservation.Session = Session.Find(updatedReservationDto.Session.Id);
                        updatedReservation.Arrival = updatedReservationDto.Arrival;
                        updatedReservation.GuestCount = updatedReservationDto.GuestCount;
                        updatedReservation.ReservedList = tableList;

                        // TODO: might be new reserver? id will be zero...
                        if (updatedReservationDto.Reserver.Id == 0)
                        {
                            Guest g = new Guest(
                                updatedReservationDto.Reserver.GuestName,
                                updatedReservationDto.Reserver.GuestPhone,
                                updatedReservationDto.Reserver.GuestEmail);
                            g.Save();
                            updatedReservation.Reserver = g;
                        }
                        else
                        {

                            updatedReservation.Reserver = Guest.Find(updatedReservationDto.Reserver.Id);

                            // update reserver fields and save
                            updatedReservation.Reserver.Name = updatedReservationDto.Reserver.GuestName;
                            updatedReservation.Reserver.Phone = updatedReservationDto.Reserver.GuestPhone;
                            updatedReservation.Reserver.Email = updatedReservationDto.Reserver.GuestEmail;
                            updatedReservation.Reserver.Update();
                        }

                        updatedReservation.PartyName = updatedReservationDto.PartyName;
                        updatedReservation.Note = updatedReservationDto.Note;
                        updatedReservation.Duration = updatedReservationDto.Duration;

                        if (updatedReservation.GuestCount < 1)
                        {
                            throw new ReservationException("Guest count cannot be less than one (1).");
                        }

                        // check reservability!
                        Location location = excludedReservation.ParentLocation;
                        bool allOk = true;
                        foreach (Table table in tableList)
                        {
                            IList<Reservation> reservationsByTable = table.ReservationsThatUseMe;
                            Reservability availability = location.GetReservability(reservationsByTable, table,
                                updatedReservation.Arrival, excludedReservation, updatedReservation.Duration);

                            if (!availability.FullyAvailable)
                            {
                                scope.VoteRollBack();

                                allOk = false;
                                result.Success = false;
                                result.ObjectAffected = 0;
                                result.Message =
                                    string.Format("Reservation not updated because {0} is not available.",
                                        table.ToString());
                                break;
                            }
                        }

                        if (allOk)
                        {
                            // save
                            updatedReservation.Save();
                            result.Success = true;
                            result.ObjectAffected = updatedReservation.Id;
                            result.Message = string.Format("Reservation updated ok for => {0} arriving at => {1}",
                                updatedReservation.Reserver.ToString(), updatedReservation.Arrival.ToString());

                            ThreadPool.QueueUserWorkItem(delegate
                            {
                                CallClientCallbacks(CallbacksToActivate.Reservations);
                            });

                            WriteToAuditLog(result.Message);
                        }
                    }
                    catch (ObjectNotFoundException onfe)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Could not find reservation to update!;{0}{1}",
                            Environment.NewLine, onfe.ToString());
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Could not update reservation;{0}{1}",
                            Environment.NewLine, are.ToString());
                    }
                    catch (ReservationException re)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Could not add reservation;{0}{1}",
                            Environment.NewLine, re.ToString());
                    }
                }
            }

            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult UpdateLocation(DTOLocation locationDTO)
        {

            // TODO: exception handling and logging!

            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {
                Location updatedLocation = null;
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        updatedLocation = Location.Find(locationDTO.Id);

                        updatedLocation.Name = locationDTO.Name;
                        updatedLocation.DefaultDuration = locationDTO.DefaultDuration;

                        updatedLocation.BackgroundImg = locationDTO.BackgroundImg;

                        result.Success = true;
                        result.ObjectAffected = updatedLocation.Id;
                        result.Message = string.Format("Location {0} updated ok.", updatedLocation.Name);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Locations);
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (ObjectNotFoundException onfe)
                    {
                        scope.VoteRollBack();

                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = string.Format("Location {0} not updated.",
                            updatedLocation.Name, Environment.NewLine, onfe.ToString());
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();

                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = string.Format("Location {0} not updated{1}{2}.",
                            updatedLocation.Name, Environment.NewLine, are.ToString());
                    }

                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }

            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult UpdateSession(DTOSession session, DTOLocation location)
        {

            OperationResult result = new OperationResult();

            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {
                    try
                    {

                        Session sessionToUpdate = Session.Find(session.Id);
                        Location l = Location.Find(location.Id);

                        sessionToUpdate.StartTime = session.StartTime;
                        sessionToUpdate.EndTime = session.EndTime;
                        sessionToUpdate.ParentLocation = l;
                        sessionToUpdate.Name = session.Name;

                        sessionToUpdate.Save();

                        result.Success = true;
                        result.ObjectAffected = session.Id;
                        result.Message = string.Format("Updated session {0} successfully", session.Name);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Sessions);
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        result.Success = false;
                        result.Message = string.Format("Error updating session;{0}{1}",
                                    Environment.NewLine, are.ToString());
                    }
                }
            }
            else
            {
                result.Success = false;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }


        // ---------------------------------------------------------------------
        public OperationResult UpdateGuest(int guestId, string name, string phone,
            string email)
        {

            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {

                        if (guestId == 0)
                        {
                            throw new ReservationException("Guest ID cannot be zero.");
                        }

                        Terminal clientsTerminal = GetLoggedInClientsTerminal();


                        Guest g = Guest.Find(guestId);
                        g.Name = name;
                        g.Phone = phone;
                        g.Email = email;
                        g.Save();

                        result.Success = true;
                        result.ObjectAffected = g.Id;
                        result.Message = string.Format("Guest id => {0} update ok", guestId);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            CallClientCallbacks(CallbacksToActivate.Reservations);
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (ReservationException re)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = re.ToString();
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = nfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = are.ToString();
                    }
                }
            }
            else
            {
                result.Success = false;
                result.ObjectAffected = 0;
                result.Message = "Not logged on. No session?";
            }

            return result;
        }

        // ---------------------------------------------------------------------
        public OperationResult UpdateReservable(DTOReservable reservable)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {

                        //
                        Table tableToUpdate = Table.Find(reservable.Id);
                        Location location = Location.Find(tableToUpdate.ParentLocation.Id);
                        Terminal term = GetLoggedInClientsTerminal();

                        location.UpdateTable(tableToUpdate, reservable.Name, reservable.MaxGuests,
                            reservable.X, reservable.Y, reservable.Width, reservable.Height, reservable.Shape, reservable.Number);

                        result.Success = true;
                        result.ObjectAffected = reservable.Id;
                        result.Message = string.Format("Table id => {0} update ok", reservable.Id);

                        DTOReservable tableDTOReturn = DTOFactory.BuildReservableDTO(tableToUpdate, term);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            TableStateChangedCallClientCallbacks(StateChangeType.Update,
                                new DTOReservable[] { tableDTOReturn });
                        });

                        WriteToAuditLog(result.Message);
                    }
                    catch (ReservableAlreadyExistsException rae)
                    {
                        scope.VoteRollBack();

                        // TODO: logging, proper message in result

                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = rae.ToString();
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = nfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = 0;
                        result.Message = are.ToString();
                    }
                }
            }
            else
            {
                result.Success = false;
                result.ObjectAffected = 0;
                result.Message = "Not logged on. No session?";
            }
            return result;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Finds the reservation and sets the current view to that reservation. Sets the
        /// object affected to be the reservation's id.
        /// </summary>
        /// <param name="reservationId"></param>
        /// <returns></returns>
        public OperationResult LocateReservation(int reservationId)
        {
            OperationResult result = new OperationResult();
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        Reservation reservationToLocate = Reservation.Find(reservationId);
                        Terminal clientTerminal = GetLoggedInClientsTerminal();
                        clientTerminal.CurrentLocation = reservationToLocate.ParentLocation;
                        clientTerminal.CurrentSession = reservationToLocate.Session;
                        clientTerminal.CurrentDate = reservationToLocate.Arrival;
                        clientTerminal.Save();

                        result.Success = true;
                        result.Message = string.Format("Reservaton {0} located", reservationToLocate.ToString());
                        result.ObjectAffected = reservationToLocate.Id;
                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = reservationId;
                        result.Message = nfe.ToString();
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                        result.Success = false;
                        result.ObjectAffected = reservationId;
                        result.Message = are.ToString();
                    }
                }
            }
            else
            {
                result.Success = false;
                result.ObjectAffected = 0;
                result.Message = "Not logged on.";
            }
            return result;
        }

        public DTOReservation[] SetView(int locationid, int sessionid, DateTime dateToView)
        {
            DTOReservation[] result = null;
            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        Location location = Location.Find(locationid);
                        Session session = Session.Find(sessionid);
                        Terminal t = GetLoggedInClientsTerminal();
                        t.CurrentDate = dateToView;
                        t.CurrentLocation = location;
                        t.CurrentSession = session;
                        t.Save();

                        // setup the parameters
                        DateTime startOfDay = new DateTime(dateToView.Year, dateToView.Month, dateToView.Day,
                            0, 0, 0);
                        DateTime endOfDay = new DateTime(dateToView.Year, dateToView.Month, dateToView.Day,
                            23, 59, 59);

                        SimpleQuery<Reservation> q = new SimpleQuery<Reservation>(
                            @"from Reservation res
							where res.ParentLocation.id = :location_id
							and res.Session.id = :session_id
							and res.Arrival between :start_of_day and :end_of_day");
                        q.SetParameter("location_id", locationid);
                        q.SetParameter("session_id", sessionid);
                        q.SetParameter("start_of_day", startOfDay);
                        q.SetParameter("end_of_day", endOfDay);

                        Reservation[] reservationList = q.Execute();

                        IList<DTOReservation> dtoReservationList = new List<DTOReservation>();
                        foreach (Reservation r in reservationList)
                        {
                            dtoReservationList.Add(DTOFactory.BuildReservationDTO(r));
                        }
                        result = dtoReservationList.ToArray<DTOReservation>();

                    }
                    catch (Castle.ActiveRecord.NotFoundException nfe)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result


                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();
                        // TODO: logging, proper message in result
                    }
                }
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public OperationResult CycleCurrentLocation()
        {

            OperationResult result = null;

            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        Terminal clientTerminal = GetLoggedInClientsTerminal();
                        Location[] locations = this.Locations;

                        int index = 0;
                        // only cycle if there is more than one location!
                        if (locations.Length > 1)
                        {
                            for (int i = 0; i < locations.Length; i++)
                            {
                                if (clientTerminal.CurrentLocation == locations[i])
                                {
                                    index = i;
                                    break;	// for
                                }
                            }

                            // found it, now increment
                            index++;

                            // if oob wrap it
                            if (index >= locations.Length)
                            {
                                index = 0;
                            }

                            // set new location
                            clientTerminal.CurrentLocation = locations[index];

                            // update session
                            if (locations[index].Sessions != null && locations[index].Sessions.Count > 0)
                            {
                                clientTerminal.CurrentSession = locations[index].Sessions[0];
                            }
                            else
                            {
                                clientTerminal.CurrentSession = null;
                            }

                            clientTerminal.Save();

                            result = new OperationResult();
                            result.Success = true;
                            result.ObjectAffected = locations[index].Id;

                            // TODO: what callback to activate?
                            ThreadPool.QueueUserWorkItem(delegate
                            {
                                CallClientCallbacks(CallbacksToActivate.Locations);
                            });
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();

                        result.Success = false;
                        result.Message = "Cannot cycle locations.";
                        result.LongMessage = are.ToString();

                        // TODO: logging, proper message in result
                    }
                }
            }
            return result;

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="view"></param>
        /// <returns></returns>
        public OperationResult SetTableViewFilter(ReservationViews view)
        {
            OperationResult result = null;

            if (ClientLoggedIn())
            {
                using (TransactionScope scope = new TransactionScope(TransactionMode.New, OnDispose.Commit))
                {

                    try
                    {
                        Terminal clientTerminal = GetLoggedInClientsTerminal();

                        switch (view)
                        {
                            case ReservationViews.Occupancy:
                                clientTerminal.TableViewFilter = "occupancy";
                                break;

                            case ReservationViews.Day:
                                clientTerminal.TableViewFilter = "day";
                                break;

                            case ReservationViews.Session:
                                clientTerminal.TableViewFilter = "session";
                                break;
                        }
                        clientTerminal.Save();

                        result = new OperationResult();
                        result.Success = true;
                        result.Message = "View changed successfully.";

                        List<DTOReservable> updatedTables = new List<DTOReservable>();

                        if (clientTerminal.CurrentLocation != null)
                        {
                            foreach (Table table in clientTerminal.CurrentLocation.Tables)
                            {
                                updatedTables.Add(DTOFactory.BuildReservableDTO(table, clientTerminal));
                            }

                            // fire callback
                            ThreadPool.QueueUserWorkItem(delegate
                            {
                                TableStateChangedCallClientCallbacks(
                                    StateChangeType.Update, updatedTables.ToArray<DTOReservable>());
                            });
                        }
                    }
                    catch (ActiveRecordException are)
                    {
                        scope.VoteRollBack();

                        result.Success = false;
                        result.Message = "Cannot set table view filter.";
                        result.LongMessage = are.ToString();

                        // TODO: logging, proper message in result
                    }
                }
            }
            return result;
        }

        #endregion


        #region Private Methods



        // ---------------------------------------------------------------------
        private enum CallbacksToActivate { Locations = 1, Sessions, Tables, Reservations };

        private enum StateChangeType { Add = 1, Delete, Update };

        // ---------------------------------------------------------------------
        /// <summary>
        /// Invokes a callback to notify clients that a table or tables has been modified.
        /// This should be called on a seperate thread to avoid the service from blocking
        /// on disconnected/hung/etc. clients.
        /// </summary>
        /// <param name="changeType">The type of change i.e., added, updated, deleted.</param>
        /// <param name="tablesThatChanged">An array of changed items.</param>
        private void TableStateChangedCallClientCallbacks(StateChangeType changeType, DTOReservable[] tablesThatChanged)
        {
            IReservationsCallback[] clientsCopy = null;

            lock (_clientListLockObject)
            {
                clientsCopy = _clients.ToArray<IReservationsCallback>();
            }

            foreach (IReservationsCallback client in clientsCopy)
            {
                try
                {
                    switch (changeType)
                    {
                        case StateChangeType.Add:
                            client.OnTablesAdded(tablesThatChanged);
                            break;

                        case StateChangeType.Delete:
                            client.OnTablesDeleted(tablesThatChanged);
                            break;

                        case StateChangeType.Update:
                            client.OnTablesUpdated(tablesThatChanged);
                            break;
                    }
                }
                catch (CommunicationObjectAbortedException coa)
                {       // client disconnected
                    _clients.Remove(client);
                    _clientsAndTerminals.Remove(client);
                    Log.Debug("Removed aborted client; {0}, {1}",
                         client.GetHashCode(), coa.ToString());
                }
                catch (CommunicationObjectFaultedException cof)
                {       // client disconnected
                    _clients.Remove(client);
                    _clientsAndTerminals.Remove(client);
                    Log.Debug("Removed faulted client; {0}, {1}",
                        client.GetHashCode(), cof.ToString());
                }
                catch (CommunicationException ce)
                {
                    _clients.Remove(client);
                    _clientsAndTerminals.Remove(client);
                    Log.DebugException(String.Format("Communication Exception, removed broken client; {0}, {1}",
                        client.GetHashCode(), ce.ToString()), ce);
                }
                catch (TimeoutException te)
                {
                    _clients.Remove(client);
                    _clientsAndTerminals.Remove(client);
                    Log.DebugException(String.Format("Removed a client that timed out; {0}, {1}",
                        client.GetHashCode(), te.ToString()), te);
                }
                catch (SystemException e)
                {
                    _clients.Remove(client);
                    _clientsAndTerminals.Remove(client);
                    Log.DebugException(String.Format("System Exception, removed broken client; {0}, {1}",
                        client.GetHashCode(), e.ToString()), e);
                }
            }
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Notify all connected clients that something, indicated by CallbacsToActivate,
        /// has changed.
        /// </summary>
        /// <param name="o"></param>
        private void CallClientCallbacks(CallbacksToActivate o)
        {
            IReservationsCallback[] clientsCopy = null;
            lock (_clientListLockObject)
            {
                clientsCopy = _clients.ToArray<IReservationsCallback>();
            }
            ExecuteClientCalbackDelegate d = ExecuteClientCallback;

            foreach (IReservationsCallback client in clientsCopy)
            {

                switch (o)
                {
                    case CallbacksToActivate.Locations:
                        d.BeginInvoke(client, client.OnLocationsChanged, null, null);
                        break;

                    case CallbacksToActivate.Sessions:
                        d.BeginInvoke(client, client.OnSessionsChanged, null, null);
                        break;

                    case CallbacksToActivate.Tables:
                        throw new NotSupportedException();
                        break;

                    case CallbacksToActivate.Reservations:
                        d.BeginInvoke(client, client.OnReservationsChanged, null, null);
                        break;
                }
            }
        }

        delegate void ExecuteClientCalbackDelegate(IReservationsCallback i,
            ClientCallBackDelegate m);
        delegate void ClientCallBackDelegate();
        private void ExecuteClientCallback(IReservationsCallback client, ClientCallBackDelegate clientCallbackMethod)
        {

            try
            {
                clientCallbackMethod.Invoke();
            }
            catch (CommunicationObjectAbortedException coa)
            {       // client disconnected
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.Debug("Removed aborted client; {0}, {1}",
                     client.GetHashCode(), coa.ToString());

            }
            catch (CommunicationObjectFaultedException cof)
            {       // client disconnected
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);

                Log.Debug("Removed faulted client; {0}, {1}",
                    client.GetHashCode(), cof.ToString());
            }
            catch (CommunicationException ce)
            {
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.DebugException(String.Format("Communication Exception, removed broken client; {0}, {1}",
                    client.GetHashCode(), ce.ToString()), ce);
            }
            catch (TimeoutException te)
            {
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.DebugException(String.Format("Removed a client that timed out; {0}, {1}",
                    client.GetHashCode(), te.ToString()), te);

            }
            catch (SystemException e)
            {
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.DebugException(String.Format("Generic Exception, removed broken client; {0}, {1}",
                    client.GetHashCode(), e.ToString()), e);
            }
        }

        // ---------------------------------------------------------------------
        private bool ClientLoggedIn()
        {
            bool result = false;

            OperationContext ctx = OperationContext.Current;
            if (ctx == null)
            {
                result = false;
                Log.Debug("OperationContext object instance is null");
            }
            else
            {
                lock (ReservationsService.CLIENTLISTLOCK)
                {
                    if (_clients.Contains(ctx.GetCallbackChannel<IReservationsCallback>()))
                    {
                        result = true;
                    }
                }
            }
            return result;
        }



        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="p"></param>
        private void WriteToAuditLog(string p)
        {
            AuditLog.Log(LogLevel.Info, string.Format("{0}, by {1}.", p, GetLoggedInClientsTerminal().Name));
        }

        #endregion

        #region static methods

        // ---------------------------------------------------------------------
        internal static Terminal GetLoggedInClientsTerminal()
        {

            Terminal result = null;

            OperationContext ctx = OperationContext.Current;

            if (ctx == null)
            {
                result = null;
                Log.Error("OperationContext object instance is null");
            }
            else
            {
                try
                {
                    lock (ReservationsService.CLIENTLISTLOCK)
                    {
                        result = _clientsAndTerminals[ctx.GetCallbackChannel<IReservationsCallback>()];
                        result.Refresh();
                    }
                }
                catch (ActiveRecordException are)
                {
                    Log.Error(string.Format("Error check logged in status;{0}{1}",
                        Environment.NewLine, are.ToString()));
                }
                catch (SystemException e)
                {
                    Log.Error(string.Format("Error check logged in status;{0}{1}",
                         Environment.NewLine, e.ToString()));
                }
            }
            return result;
        }

        private static object _clientListLockObject = new object();

        // ---------------------------------------------------------------------
        /// <summary>
        /// Do heartbeat processing here.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void _heartbeat_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {

            // this will produce large logs!
            Log.Debug(string.Format("Heartbeat at {0}", e.SignalTime));

            IReservationsCallback[] clientsCopy = null;
            lock (_clientListLockObject)
            {
                clientsCopy = _clients.ToArray<IReservationsCallback>();
            }

            ExecuteClientCalbackDelegate d = ExecuteClientCallbackS;

            using (new SessionScope())
            {
                foreach (IReservationsCallback client in clientsCopy)
                {

                    Terminal term = _clientsAndTerminals[client];
                    term.Refresh();

                    if (term.TableViewFilter == "occupancy")
                    {
                        d.BeginInvoke(client, client.OnReservationsChanged, null, null);
                    }
                    else if (term.TableViewFilter == "day")
                    {
                        d.BeginInvoke(client, client.OnReservationsChanged, null, null);
                    }
                    else if (term.TableViewFilter == "session")
                    {
                        d.BeginInvoke(client, client.OnReservationsChanged, null, null);
                    }
                }
            }
        }


        /// <summary>
        /// Static version of client callback execution method.
        /// </summary>
        /// <param name="client"></param>
        /// <param name="clientCallbackMethod"></param>
        private static void ExecuteClientCallbackS(IReservationsCallback client, ClientCallBackDelegate clientCallbackMethod)
        {

            try
            {
                clientCallbackMethod.Invoke();
            }
            catch (CommunicationObjectAbortedException coa)
            {       // client disconnected
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.Debug("Removed aborted client; {0}, {1}",
                     client.GetHashCode(), coa.ToString());

            }
            catch (CommunicationObjectFaultedException cof)
            {       // client disconnected
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);

                Log.Debug("Removed faulted client; {0}, {1}",
                    client.GetHashCode(), cof.ToString());
            }
            catch (CommunicationException ce)
            {
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.DebugException(String.Format("Communication Exception, removed broken client; {0}, {1}",
                    client.GetHashCode(), ce.ToString()), ce);
            }
            catch (TimeoutException te)
            {
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.DebugException(String.Format("Removed a client that timed out; {0}, {1}",
                    client.GetHashCode(), te.ToString()), te);

            }
            catch (SystemException e)
            {
                _clients.Remove(client);
                _clientsAndTerminals.Remove(client);
                Log.DebugException(String.Format("Generic Exception, removed broken client; {0}, {1}",
                    client.GetHashCode(), e.ToString()), e);
            }
        }



        // ---------------------------------------------------------------------
        /// <summar>
        /// 
        /// </summary>
        /// <param name="tableId"></param>
        /// <returns></returns>
        public static int GetTableInterestLevel(int tableId)
        {
            int level = 0;
            if (_tableInterestLevel.ContainsKey(tableId))
            {
                level = _tableInterestLevel[tableId];
            }
            return level;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="p"></param>
        private static void IncreaseInterestLevelForTable(int p)
        {
            if (_tableInterestLevel.ContainsKey(p))
            {
                _tableInterestLevel[p]++;
            }
            else
            {
                _tableInterestLevel[p] = 1;
            }
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="i"></param>
        private static void DecreaseInterestLevelForTable(int i)
        {
            if (_tableInterestLevel.ContainsKey(i))
            {
                _tableInterestLevel[i]--;
            }
        }

        #endregion
    }
}
