using System;

namespace Safemate.Core.LogProxy.Mail
{
    public class Base
    {
        public Base()
        {
            type = "Base";
        }

        public string Type
        {
            get {
                return type;
            }
        }

        public virtual void Send(String message, String subject)
        {
        }

        protected String type;
    };
};
