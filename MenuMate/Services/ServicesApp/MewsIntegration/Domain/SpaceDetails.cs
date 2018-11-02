using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class SpaceDetails
    {
        [DataMember]
        public List<Space> Spaces { get; set; }
    }
}
