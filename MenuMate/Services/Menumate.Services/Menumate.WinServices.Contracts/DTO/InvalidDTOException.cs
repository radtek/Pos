using System;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    public class InvalidDTOException : Exception
    {
        public InvalidDTOException(string message) : base(message) { }
    }
}
