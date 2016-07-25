using System;

namespace FileMate.Core.Exceptions {
	public class Base : Exception {
		public Base(String message, Exception innerException)
		    : base(message, innerException) {
		}

		public Base(String message, String _targetName,
                    Exception innerException)
		        : base(message, innerException) {
			targetName = _targetName;
		}

		protected String targetName = null;

		public String TargetName {
			get {
				return targetName;
			}
		}
	}
}
