using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Exceptions
{
    public class CardCodeInUse:Exception
    {
        public CardCodeInUse()
            : base("Card code already in use.")
        {
            
        }
    }
}
