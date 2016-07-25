using System;

namespace FileMate.Core.Xml.Exceptions {
	class UnsupportedRepositoryType : Exception {
		public UnsupportedRepositoryType(String uri)
		: base(String.Format("URI \"{0}\" matches no " +
		    "implemented repository type.", uri)) {
		}
	}
}
