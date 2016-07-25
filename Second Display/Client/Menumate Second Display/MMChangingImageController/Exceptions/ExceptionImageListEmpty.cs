using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MMChangingImageController.Exceptions
{
    public class ExceptionImageListEmpty : Exception
    {
        public ExceptionImageListEmpty(string message) : base(message)
        {
        }
    }
}
