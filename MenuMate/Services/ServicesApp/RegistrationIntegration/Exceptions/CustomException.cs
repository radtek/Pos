using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Exceptions
{
    public class CustomException
    {
        public string ExceptionMessage { get; set; }
        public string Message { get; set; }
        public string StackTrace { get; set; }
        public string ExceptionType { get; set; }
    }
}
