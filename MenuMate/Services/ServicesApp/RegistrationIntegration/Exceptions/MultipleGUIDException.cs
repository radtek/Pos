﻿using System;

namespace RegistrationIntegration.Exceptions
{
    public class MultipleGUIDException :Exception
    {
        public MultipleGUIDException()
            : base("300 : Multiple GUID Exist")
        {
            
        }
    }
}