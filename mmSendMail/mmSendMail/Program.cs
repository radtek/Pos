using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using NLog;
using NLog.Config;
using NLog.Targets;

namespace mmSendMail
{
    using Params = Dictionary<string, string>;

    public enum SendMailAction
    {
        Unknown,
        Help,
        ProfileList,
        Send
    };

    public enum SendMailOutput
    {
        Console,
        File,
        All
    };

    class Program
    {
        static string _helpParam    = @"-h";
        static string _helpAltParam = @"-help";

        static string _pListParam    = @"-l";
        static string _pListAltParam = @"-plist";

        string[] _paramNames = {
                                      @"-s",  // Server URL or IP address
                                      @"-p",  // Port Number
                                      @"-n",  // Authentication: 0 - None, 1- Basic.  Optional, defaults to 0 - None
                                      @"-u",  // User name, if Authentication = 1
                                      @"-w",  // Password,  if Authentication = 1
                                      @"-f",  // From: (Mail Sender's address)
                                      @"-t",  // To: (Mail Recipient's address)
                                      @"-a",  // Subject (optional)
                                      @"-b",  // Body
                                      @"-m",  // Filename (optional) Use file as part of the Body of Message
                                      @"-o",  // Output: 0 - Console, 1 - File, 2 - All (Console & File). If missing, Output = 0
                                      @"-d",  // Debug (Show all mail server communication)
                                      @"-v",  // Save profile 
                                      @"-l"   // List of Profiles
                                 };

        string[] _paramAltNames = { // Alternative parameter's names
                                        @"-server",         // Server URL or IP address
                                        @"-port",           // Port Number
                                        @"-authentication", // Authentication: 0 - None, 1 - Basic.  Optional, defaults to 0 - None
                                        @"-username",       // User name, if Authentication is required
                                        @"-password",       // Password,  if Authentication is required
                                        @"-from",           // From: (Mail Sender's address)
                                        @"-to",             // To: (Mail Recipient's address)
                                        @"-subject",        // Subject (optional)
                                        @"-body",           // Body
                                        @"-filename",       // Filename (optional) Use file as part of the Body of Message
                                        @"-output",         // Output: 0 - Console, 1 - File, 2 - All (Console & File). Optional, defaults to 0 - Console
                                        @"-debug",          // Debug (Show all mail server communication)
                                        @"-save",           // Save profile
                                        @"-plist"           // List of Profiles
                                   };

        static void Main(string[] args)
        {
            try
            {
                initSSM();

                switch (GetSendMailAction(args))
                {
                    case SendMailAction.Help: ShowHelp(); break;
                    case SendMailAction.ProfileList: ShowPList(); break;
                    case SendMailAction.Send: SendEmail(GetInputParams(args)); break;

                    default: ; break;
                }
            }
            catch (Exception)
            {
                Console.WriteLine(@"");
            }
        }

        /// <summary>
        /// 
        /// </summary>
        static void initSSM()
        {
            if(!File.Exists(@"default.ssm"))
            {
                createDefaultProfile();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        static void createDefaultProfile()
        {
            createProfile(
                @"default.ssm",
                @"smtp.gmail.com",                         // Server URL or IP address
                587,                                       // Port Number
                NLog.Targets.SmtpAuthenticationMode.Basic, // Authentication: 0 - None, 1 - Basic.  Optional, defaults to 0 - None
                @"noreply@menumate.com",                   // User name, if Authentication is required
                @"1202matemenu",                           // Password,  if Authentication is required
                @"noreply@menumate.com",                   // From: (Mail Sender's address)
                @"support@menumate.com",                   // To: (Mail Recipient's address)
                @"Subject",                                // Subject (optional)
                @"Body",                                   // Body
                @"ssMailBody.txt",                         // Filename (optional) Use file as part of the Body of Message
                SendMailOutput.All,                        // Output: 0 - Console, 1 - File, 2 - All (Console & File). Optional, defaults to 0 - Console
                true                                       // Debug (Show all mail server communication)
              );
        }

        /// <summary>
        /// 
        /// </summary>
        static void createProfile(string inProfileName, string inServerURL, int inIPPort, NLog.Targets.SmtpAuthenticationMode inAuthMode, string inUserName, string inPassw,
                                  string inFrom, string inRcpt, string inSubject, string inBody, string inBodyFilename, SendMailOutput inOutput, bool inDebug)
        {
        }

        private Logger _createEMailLogger()
        {
            setMailLoggerSubject(@"Summa failed in location: ");
            return LogManager.GetLogger(@"eMail");
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSubject"></param>
        private void setMailLoggerSubject(string inSubject)
        {
            MailTarget target = new MailTarget();

            target.Name = @"errorGmail";
            target.SmtpServer = @"smtp.gmail.com";
            target.SmtpPort = 587;
            target.SmtpAuthentication = NLog.Targets.SmtpAuthenticationMode.Basic;
            target.EnableSsl = true;

            target.SmtpUserName = @"noreply@menumate.com";
            target.SmtpPassword = @"1202matemenu";
            //target.SmtpUserName = @"support@menumate.com";
            //target.SmtpPassword = @"matemenu1202";

            target.From = @"noreply@menumate.com";
            //            target.To = @"jorgel@menumate.com";
            //            target.From = @"support@menumate.com";
            target.To = @"support@menumate.com";

            //target.Subject = string.Format(@"{0}  Version 5.3.0.12", inSubject);
            target.Subject = string.Format(@"{0}  Version __MM_VERSION__", inSubject);
            target.Body = @"${message}";
            //target.Body = @"${date}: ${message}";

            LoggingConfiguration config = LogManager.Configuration;

            //......................................

            try
            {
                if (config.FindTargetByName(target.Name) != null)
                {
                    config.RemoveTarget(target.Name);
                    config = config.Reload();
                }

                //config.RemoveTarget(target.Name);
            }
            catch (Exception)
            {
            }

            //......................................

            config.AddTarget("errorGmail", target);
            config.LoggingRules.Add(new NLog.Config.LoggingRule("eMail", LogLevel.Error, target));

            //LogManager.Configuration.AddTarget("errorGmail", target);
            //LogManager.Configuration.LoggingRules.Add(new NLog.Config.LoggingRule("eMail", LogLevel.Error, target));

            LogManager.Configuration = config;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="?"></param>
        static SendMailAction GetSendMailAction(string[] args)
        {
            SendMailAction result = SendMailAction.Send;

            //:::::::::::::::::::::::::::::

            if ((args.Length == 0) || (args[0].ToLower() == _helpParam) || (args[0].ToLower() == _helpAltParam))
            {
                result = SendMailAction.Help;
            }
            else
            {
                if ((args[0].ToLower() == _pListParam) || (args[0].ToLower() == _pListAltParam))
                {
                    result = SendMailAction.ProfileList;
                }
            }

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static Params GetInputParams(string[] inArgs)
        {
            Params result = createParams();

            //:::::::::::::::::::::::::::::

            result["server"]         = readServer(inArgs);
            result["port"]           = readIPPort(inArgs);
            result["authentication"] = readAuthentication(inArgs);
            result["username"]       = readUsername(inArgs);
            result["password"]       = readPassword(inArgs);
            result["from"]           = readFrom(inArgs);
            result["rcpt"]           = readRcpt(inArgs);
            result["subject"]        = readSubject(inArgs);
            result["body"]           = readBody(inArgs);
            result["filename"]       = readFilename(inArgs);
            result["output"]         = readOutput(inArgs);
            result["debug"]          = readDebug(inArgs);
            result["profilename"]    = readProfileName(inArgs);
            result["plist"]          = readPList(inArgs);

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readServer(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"smtp.gmail.com";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readIPPort(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"587";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readAuthentication(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"1";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readUsername(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"noreply@menumate.com";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readPassword(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"1202matemenu";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readFrom(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"noreply@menumate.com";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readRcpt(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"jorgel@menumate.com";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readSubject(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"the subject";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readBody(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"the body";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readFilename(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"mmSendMail.log";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readOutput(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"all";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readDebug(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"1";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readProfileName(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"default.ssm";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inArgs"></param>
        /// <returns></returns>
        static string readPList(string[] inArgs)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::

            result = @"1";

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static Params createParams()
        {
            Params result = new Params();

            //::::::::::::::::::::::::::::::

            result.Add(@"server",         @"");
            result.Add(@"port",           @"25");
            result.Add(@"authentication", @"0");
            result.Add(@"username",       @"");
            result.Add(@"password",       @"");
            result.Add(@"from",           @"CompileBox@menumate.com");
            result.Add(@"rcpt",           @"");
            result.Add(@"subject",        @"Auto compile process complete. A new Menumate installer has been created");
            result.Add(@"body",           @"");
            result.Add(@"filename",       @"mmSendMail.log");
            result.Add(@"output",         @"console");
            result.Add(@"debug",          @"0");
            result.Add(@"profilename",    @"default.ssm");

            //::::::::::::::::::::::::::::::

            return result;
        }

       /// <summary>
       /// 
       /// </summary>
       /// <returns></returns>
        static string[] retrievePList()
        {
            List<string> result = new List<string>();

            //:::::::::::::::::::::::::::::

            result.Add(@"default.ssm");

            //:::::::::::::::::::::::::::::

            return result.ToArray<string>();
        }


        /// <summary>
        /// 
        /// </summary>
        static void ShowHelp()
        {
            Console.WriteLine(@"mmSendMail | Help");
            Console.WriteLine(@"");
            Console.WriteLine(@"-h (-help");
            Console.WriteLine(@"-s (-server)   Server URL or IP address");
            Console.WriteLine(@"-p (-port)     Port Number (Optional, defaults to 25");
            Console.WriteLine(@"-n (-authentication) (Optional, defaults to 0 - No authentication)");
            Console.WriteLine(@"-u (-username) Only if Authentication is required");
            Console.WriteLine(@"-w (-password) Only if Authentication is required");
            Console.WriteLine(@"-f (-from)     From: (Mail Sender's address");
            Console.WriteLine(@"-t (-to)       To: (Mail Recipient's address");
            Console.WriteLine(@"-a (-subject)  Subject (optional)");
            Console.WriteLine(@"-b (-body)     Body");
            Console.WriteLine(@"-f (-filename) Filename (optional) Use file as part of the Body of Message");
            Console.WriteLine(@"-o (-output)   Output: 0 -> Console, 1 -> File (defaults to mmSendMail.log), 2 -> All (Console & File). If missing, Output = 0");
            Console.WriteLine(@"-d (-debug)    Debug (Show all mail server communication)");
            Console.WriteLine(@"");
            Console.WriteLine(@"Examples:");
            Console.WriteLine(@"");
            Console.WriteLine(@"mmSendMail -h");
            Console.WriteLine(@"mmSendMail -help");
            Console.WriteLine(@"");
            Console.WriteLine("mmSendMail -s mail.gmail.com -p 25 -f sender@gamil.com -t recipient@gmail.com -a \"test mail subject\" -b \"this is the message body\"");
            Console.WriteLine("mmSendMail -s mail.gmail.com -p 25 -f sender@gamil.com -t recipient@gmail.com -a \"test mail subject\" -b \"this is the message body\" -o 0");
            Console.WriteLine("mmSendMail -s mail.gmail.com -p 25 -f sender@gamil.com -t recipient@gmail.com -a \"test mail subject\" -b \"this is the message body\" -o 1");
            Console.WriteLine("mmSendMail -s mail.gmail.com -p 25 -f sender@gamil.com -t recipient@gmail.com -a \"test mail subject\" -b \"this is the message body\" -o 2 -d");
            Console.WriteLine("mmSendMail -s mail.gmail.com -p 25 -f sender@gamil.com -t recipient@gmail.com -a \"test mail subject\" -b \"this is the message body\" -o 2 -debug");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output 2");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output All");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output all");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output all log.txt");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output Console");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output console");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output file");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output file log.txt");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output File log.txt");
            Console.WriteLine(@"");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -n 1 -u username -w password -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output File log.txt");
            Console.WriteLine(@"");
            Console.WriteLine("mmSendMail -server mail.gmail.com -p 25 -n basic -u username -w password -f sender@gamil.com -to recipient@gmail.com -subject \"test mail subject\" -b \"this is the message body\" -output File log.txt");

            Console.ReadLine();
        }

        /// <summary>
        /// 
        /// </summary>
        static void ShowPList()
        {
            string[] plist = retrievePList();

            Console.WriteLine(@"mmSendMail | Profile List");
            Console.WriteLine(@"");

            foreach(string profile in plist)
            {
                Console.WriteLine(string.Format(@"{0}: ", profile));
            }

            Console.WriteLine("");
            Console.Write("Press any key to continue ...");
            Console.ReadLine();
        }

        static void SendEmail(Params inParams)
        {
            Console.WriteLine(@"mmSendMail | Sending an email ...");

            Console.Write("");
            Console.Write("Press any key to continue ...");
            Console.ReadLine();
        }
    }
}
