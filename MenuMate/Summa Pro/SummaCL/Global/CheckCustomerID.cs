using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;

namespace SummaCL.Global
{
    public class DuplicateIDException : SystemException
    {
        public DuplicateIDException() { }
        public DuplicateIDException(string message) : base(message) { }
        public DuplicateIDException(string message, Exception inner) : base(message, inner) { }
        protected DuplicateIDException(SerializationInfo info, StreamingContext context) : base(info, context) { }
    }

    class CheckCustomerID
    {
        static List<string> _idList = new List<string>();

        public static void Clear()
        {
            _idList.Clear();
        }

        //............................................

        public static void Check(string inID)
        {
            foreach(string id in _idList)
            {
                if(id == inID)
                {
                    throw new DuplicateIDException(string.Format(@"ID {0} already exists.", inID));
                }
            }

            if(inID.Trim() != @"")
            {
                _idList.Add(inID);
            }
        }

    }
}
