using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud.Types
{
    [Serializable]
    public sealed class Tier
    {
        public Tier()
        {
        }

        public Tier(string siteid, string tierid, string name,
                    string level, string points_required, string birthday_bonus,
                    string priced_redemption_rate, string weighed_redemption_points,
                    string weighed_redemption_weight, bool send_mail_to_headoffice,
                    bool send_mail_to_member, bool change_card)
        {
            this.siteid = siteid;
            this.tierid = tierid;
            this.name = name;
            this.level = level;
            this.points_required = points_required;
            this.birthday_bonus_points = birthday_bonus;
            this.priced_redemption_rate = priced_redemption_rate;
            this.weighed_redemption_points = weighed_redemption_points;
            this.weighed_redemption_weight = weighed_redemption_weight;
            this.send_mail_to_headoffice = send_mail_to_headoffice;
            this.send_mail_to_member = send_mail_to_member;
            this.change_card = change_card;
            this.id = tierid;
        }

        public string id
        {
            get;
            set;
        }

        public string siteid
        {
            get;
            set;
        }

        public string tierid
        {
            get;
            set;
        }
        public string name
        {
            get;
            set;
        }
        public string level
        {
            get;
            set;
        }
        public string points_required
        {
            get;
            set;
        }
        public string birthday_bonus_points
        {
            get;
            set;
        }
        public string priced_redemption_rate
        {
            get;
            set;
        }
        public string weighed_redemption_points
        {
            get;
            set;
        }
        public string weighed_redemption_weight
        {
            get;
            set;
        }
        public bool send_mail_to_headoffice
        {
            get;
            set;
        }
        public bool send_mail_to_member
        {
            get;
            set;
        }
        public bool change_card
        {
            get;
            set;
        }

    }
}
