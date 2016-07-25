using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud.Types
{
    [Serializable]
    public sealed class MiscInfo
    {
        public MiscInfo()
        {
        }

        public MiscInfo(Nullable<DateTime> birthday_benefit_date,
            string tier_id, Nullable<DateTime> last_visit_date,
            string home_site, Nullable<DateTime> year_start_date,
            string card_number, string member_card_code)
        {
            this.birthday_benefit_date = birthday_benefit_date;
            this.last_visit_date = last_visit_date;
            this.home_site = home_site;
            this.year_start_date = year_start_date;
            this.card_number = card_number;
            this.tier_id = tier_id;
            this.member_card_code = member_card_code;
        }

        public Nullable<DateTime> birthday_benefit_date
        {
            get;
            set;
        }
        public string tier_id
        {
            get;
            set;
        }
        public Nullable<DateTime> last_visit_date
        {
            get;
            set;
        }
        public string home_site
        {
            get;
            set;
        }
        public Nullable<DateTime> year_start_date
        {
            get;
            set;
        }
        public string card_number { get; set; }
        public string member_card_code { get; set; }
    }
}
