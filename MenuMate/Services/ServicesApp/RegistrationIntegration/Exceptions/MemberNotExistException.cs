using System;
namespace Loyaltymate.Exceptions
{
    public class MemberNotExistException:Exception
    {
        public MemberNotExistException()
            : base("Member Not Exist")
        {
            
        }
    }
}