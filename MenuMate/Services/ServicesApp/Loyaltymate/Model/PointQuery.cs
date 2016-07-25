using System;
using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class PointQuery
    {
        public DateTime StartDate { get; set; }
        public DateTime EndDate { get; set; }
        public PointType PointType { get; set; }
        public string UniqueId { get; set; }
        public double Balance { get; set; }
    }
}