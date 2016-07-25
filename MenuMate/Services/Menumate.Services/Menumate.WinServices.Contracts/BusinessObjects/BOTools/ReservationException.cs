using System;
using System.Runtime.Serialization;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{

    public class ReservationException : SystemException
    {
        public ReservationException() { }
        public ReservationException(String message) : base(message) { }
        public ReservationException(string message, Exception inner) : base(message, inner) { }
        protected ReservationException(SerializationInfo info, StreamingContext context) : base(info, context) { }
    }

    public class AlreadyReservedException : ReservationException
    {
        public AlreadyReservedException() { }
        public AlreadyReservedException(String message) : base(message) { }
        public AlreadyReservedException(string message, Exception inner) : base(message, inner) { }
        protected AlreadyReservedException(SerializationInfo info, StreamingContext context) : base(info, context) { }
    }

    public class LocationAlreadyExistsException : ReservationException
    {
        public LocationAlreadyExistsException() { }
        public LocationAlreadyExistsException(String message) : base(message) { }
        public LocationAlreadyExistsException(string message, Exception inner) : base(message, inner) { }
        protected LocationAlreadyExistsException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }

    public class ReservableAlreadyExistsException : ReservationException
    {
        public ReservableAlreadyExistsException() { }
        public ReservableAlreadyExistsException(String message) : base(message) { }
        public ReservableAlreadyExistsException(string message, Exception inner) : base(message, inner) { }
        protected ReservableAlreadyExistsException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }

    public class SessionAlreadyExistsException : ReservationException
    {
        public SessionAlreadyExistsException() { }
        public SessionAlreadyExistsException(String message)
            : base(message) { }
        public SessionAlreadyExistsException(string message, Exception inner) : base(message, inner) { }
        protected SessionAlreadyExistsException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }
}
