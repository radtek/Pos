using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate_tests
{
    class SampleMethodCalls
    {
        void execute()
        {
            /*
                ServicePointManager.ServerCertificateValidationCallback =
                    ignore_self_signed_certificates;

                Credentials creds =
                    new Credentials(
                        "test_account_test@pos.loylaty-mate.com",
                        "433ed0c9",
                        "ca65db49e5b521875487c2892a79bc7ded1497094e3b141753fcb6d13f3fdedf",
                        "34607fbeb9976430ac90d1ee451b1d0032e12837e205342d9f28a12f13bbbe88",
                        "test-group");
                Loyalty_mate_service service =
                  new Loyalty_mate_service(creds);
                Console.Write("Creating service and authenticating ...");
                Dictionary<string, Member> all_members =
                    service.Get_all_member_details();
                Console.WriteLine("done.");

                if (all_members.Count == 0)
                        Console.WriteLine("No members to purge.");
                foreach (KeyValuePair<string, Member> i in all_members) {
                        Console.Write("Deleting "
                                      + i.Key
                                      + " ("
                                      + i.Value.profile.name
                                      + ") ...");
                        service.Delete_member(i.Key);
                        Console.WriteLine("done.");
                }
                Console.WriteLine("All test stuff's done.");
                Console.ReadLine();
         * */

                //string u = "andrewm@menumate.com.dev";
                //string p = "18andrew18";
                //string t = "abcdef";

                //string w = u + ":" + p + ":" + t;
                //string ew = Convert.ToBase64String(Encoding.UTF8.GetBytes(w));
                //string x = Loyalty_mate.Utilities.Hash.Hash.Create_md5_checksum_from_string(ew);
                //string y = ew + ":" + x;
                //string ey = Convert.ToBase64String(Encoding.UTF8.GetBytes(y));
                //return;
        }

        //static bool
        //ignore_self_signed_certificates(
        //    object o,
        //    X509Certificate server_certificate,
        //    X509Chain trust_chain,
        //    SslPolicyErrors ssl_errors)
        //{
        //        return true;
        //}
        
    }
}
