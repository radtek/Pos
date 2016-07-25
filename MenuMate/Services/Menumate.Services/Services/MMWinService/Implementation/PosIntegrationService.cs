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

        #endregion
    }
}
