using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class GiftCardApiViewModel
    {
        public double PointBalance { get; set; }
        public string GiftCardNumber { get; set; }
        public DateTime? ExpiryDate { get; set; }
        public DateTime? StartDate { get; set; }
        public GiftCardStatus Result { get; set; }
    }
}
