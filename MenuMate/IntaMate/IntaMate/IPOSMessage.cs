using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace IntaMate {
    public interface IPOSMessage {
        void Process(XmlDocument document);
    }
}
