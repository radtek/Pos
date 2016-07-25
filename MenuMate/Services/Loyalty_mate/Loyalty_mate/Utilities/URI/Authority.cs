namespace Loyalty_mate.Utilities.URI {

public sealed class Authority {
        public
        Authority()
        {
        }

        public
        Authority(string host)
        {
                this.host_ = host;
        }

        public
        Authority(string host,
                  string username,
                  string password)
        {
                this.host_ = host;
                this.username_ = username;
                this.password_ = password;
        }

        public Authority
        Credentials(string username, string password)
        {
                this.username_ = username;
                this.password_ = password;
                return this;
        }

        public Authority
        Host(string host)
        {
                this.host_ = host;
                return this;
        }

        public static
        implicit operator string(Authority a)
        {
                string s = "";

                if (a.username_ != null
                    && a.username_.Length != 0
                    && a.password_ != null
                    && a.password_.Length != 0)
                    s += a.username_ + ":" + a.password_ + "@";

                return s + a.host_;
        }

        string host_;
        string username_;
        string password_;
} /* Authority */

} /* Utilities.URI */
