using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;

namespace MewsIntegration.Domain
{
    public enum Classification
    {
        PaymasterAccount = 1,
        Blacklist,
        Media,
        LoyaltyProgram,
        PreviousComplaint,
        Returning,
        Staff,
        FriendOrFamily,
        TopManagement,
        Important,
        VeryImportant,
        Problematic,
        Cashlist,
        Disabled,
        Military
    };
}
