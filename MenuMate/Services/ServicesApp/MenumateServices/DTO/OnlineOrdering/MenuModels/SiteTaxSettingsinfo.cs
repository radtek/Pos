﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.OnlineOrdering.MenuModels
{
    public class SiteTaxSettingsinfo
    {
        public long SiteId { get; set; }
        public List<TaxSettingsInfo> SiteTaxSettings { get; set; }
    }
}
