using System;

namespace FileMate.Core.Exceptions {
	public class BadConfiguration : Base {
		public BadConfiguration(String message,
		    Exception innerException)
			: base(message, innerException) {
		}
	};
};
