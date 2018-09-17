using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Category
    {
        [DataMember]
        public string Code { get; set; }
    }
}
