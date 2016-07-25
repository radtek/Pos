using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud.Types
{
    [Serializable]
    public sealed class PointsResponse
    {
        public PointsResponse()
        {
        }

        public string points
        {
            get;
            set;
        }
    }

    [Serializable]
    public sealed class PointsInfo
    {

        public PointsInfo()
        {
        }

        public PointsInfo(
            string uuid,
            Nullable<DateTime> start_date,
            Nullable<DateTime> end_date,
            string points_type)
        {
            this.uuid = uuid;
            this.start_date = start_date;
            this.end_date = end_date;
            this.points_type = points_type;
        }
        public string uuid
        {
            get;
            set;
        }
        public string points_type
        {
            get;
            set;
        }
        public Nullable<DateTime> start_date
        {
            get;
            set;
        }
        public Nullable<DateTime> end_date
        {
            get;
            set;
        }
        public string point_balance
        {
            get;
            set;
        }
    }
}
