using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyalty_mate_test_console.LoyaltyMateWCFServiceRef;

namespace Loyalty_mate_test_console
{
    class Program
    {
        static int siteId = 0;
        static string syndicateCode = "";

        static void Main(string[] args)
        {
            seperator();
            Console.WriteLine("Starting..............");
            if (init())
            {
                int option = getUserChoice();
                while (option != -1)
                {
                    try
                    {
                        switch (option)
                        {
                            case 1:
                                createUser();
                                break;
                            case 2:
                                updateUsers();
                                break;
                            case 3:
                                getUserDetails();
                                break;
                            case 4:
                                getAllUsers();
                                break;
                            case 5:
                                removeUser();
                                break;
                        }
                        option = getUserChoice();
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("An error occurred");
                    }
                }
            }
            else
                Console.WriteLine("Invalid site id or syndicate code.");
            Console.WriteLine("Exiting..........");
            seperator();
            Console.ReadKey();
        }

        static bool init()
        {
            Console.WriteLine("Set site id : ");
            if (int.TryParse(Console.ReadLine(), out siteId))
            {
                Console.WriteLine("Set syndicate code: ");
                syndicateCode = Console.ReadLine();
                return true;
            }
            else
                return false;
        }

        static int getUserChoice()
        {
            int option = -1;
            Console.WriteLine("Select an option from 1-5 : press -1 to quit");
            Console.WriteLine("1. Create | 2. Update | 3. Get | 4. GetAll | 5. Delete");
            int.TryParse(Console.ReadLine(), out option);
            return option;
        }

        static void createUser()
        {
            DTO_MemberInfo info = new DTO_MemberInfo();
            getMemberInformation(ref info);
            DTO_LoyaltyMemberResponse response = CloudOps.CreateMember(siteId, syndicateCode, info);
            if (response.Succesful)
                Console.WriteLine("User creation success");
            else
                Console.WriteLine("User creation failed : {0}", response.Message);
        }

        static void removeUser()
        {
            DTO_MemberInfo member = getMemberFromList();
            if (member != null)
            {
                DTO_LoyaltyResponse response = CloudOps.DeleteMember(siteId, syndicateCode, member.UUID);
                if (response.Succesful)
                    Console.WriteLine("Member removed successfully");
                else
                    Console.WriteLine("Member removal failed : {0}", response.Message);
            }
            else
                Console.WriteLine("No member to remove");
        }

        static void getAllUsers()
        {
            DTO_MemberInfo[] members = getAllUsersFromCloud();
            if (members.Length > 0)
                displayUsers(members);
            else
                Console.WriteLine("No members in the cloud yet");
        }

        static void updateUsers()
        {      
            DTO_MemberInfo member = getMemberFromList();
            if (member != null)
            {                
                updateSingleUser(member);
            }
            else
                Console.WriteLine("No member to update.");            
        }

        static void getUserDetails()
        {
            DTO_MemberInfo member = getMemberFromList();
            if(member != null)
                displayUser(member);
            else
                Console.WriteLine("No member to display");
        }

        private static void seperator()
        {
            Console.WriteLine("--------------------------------------------------------------------------------------------------------------------------");
        }

        private static DTO_MemberInfo getMemberFromList()
        {
            DTO_MemberInfo[] members = getAllUsersFromCloud();
            if (members.Length > 0)
            {
                Console.WriteLine("Select a member. press -1 to go back to main menu.");
                displayUsers(members);   
                int userIndex = -1;
                if (int.TryParse(Console.ReadLine(), out userIndex) && userIndex < members.Length)
                    return members[userIndex];
            }
            else
                Console.WriteLine("No members in the cloud yet");

            return null;
        }

        private static void displayUsers(DTO_MemberInfo[] members)
        {
            for (int i = 0; i < members.Length; i++)
            {
                displayUser(i, members[i]);
            }
        }

        private static void updateSingleUser(DTO_MemberInfo info)
        {
            int choice = -1;
            do
            {
                displayUser(info);
                Console.WriteLine("Select a field to update : Type field number (Eg: 2) and then followed by the new value. Type -1 to complete update.");
                string response = Console.ReadLine();
                string[] arr = response.Split(' ');
                if (int.TryParse(arr[0], out choice) && arr.Length > 1)
                {
                    try
                    {
                        string input = string.Join(" ", arr, 1, arr.Length - 1);
                        switch (choice)
                        {
                            case 1:
                            case 4:
                                Console.WriteLine("Cannot update this field");
                                break;
                            case 2:
                                info.Name = input;
                                break;
                            case 3:
                                info.CellPhone = input;
                                break;
                            case 5:
                                info.DateOfBirth = DateTime.Parse(input);
                                break;
                            case 6:
                                info.LoadedPoints = double.Parse(input);
                                break;
                            case 7:
                                info.EarnedPoints = double.Parse(input);
                                break;
                            default:
                                throw new Exception("Invalid input");
                        }
                    }
                    catch
                    {
                        Console.WriteLine("Invalid input");
                    }
                }
            }
            while (choice != -1);

            DTO_LoyaltyMemberResponse resp = CloudOps.UpdateMember(siteId, syndicateCode, info, false);
            if (resp.Succesful)
                Console.WriteLine("Update successfull");
            else
                Console.WriteLine("Update failed : {0}", resp.Message);
        }

        private static DTO_MemberInfo[] getAllUsersFromCloud()
        {
            DTO_LoyaltyMemberListResponse response = CloudOps.GetMemberList(siteId, syndicateCode);
            if (response.Succesful)
                return response.MemberList.ToArray();
            else
                return new DTO_MemberInfo[] { };
        }

        private static void displayUser(int number, DTO_MemberInfo info)
        {
            seperator();
            Console.WriteLine(@"Number: {0} | Id:{1} | Name: {2} | Cellphone: {3} | Email: {4} | DateOfBirth: {5} | LoadedPoints: {6} | EarnedPoints: {7} | ActivationToken: {8} ",
                number, info.UUID, info.Name, info.CellPhone, info.EMailAddress, getDateString(info.DateOfBirth), info.LoadedPoints, info.EarnedPoints, info.ActivationToken
                );
            seperator();
        }

        private static void displayUser(DTO_MemberInfo info)
        {
            seperator();
            Console.WriteLine(@"1. Id: {0} | 2. Name: {1} | 3. Cellphone: {2} | 4. Email: {3} | 5. DateOfBirth: {4} | 6. LoadedPoints: {5} | 7. EarnedPoints: {6}",
                info.UUID, info.Name, info.CellPhone, info.EMailAddress, getDateString(info.DateOfBirth), info.LoadedPoints, info.EarnedPoints
                );
            seperator();
        }

        private static string getDateString(DateTime? date)
        {
            return date.HasValue ? date.Value.ToShortDateString() : "";
        }

        private static void getMemberInformation(ref DTO_MemberInfo info)
        {
            info.Name = getLineFromConsole<string>("Name");
            info.DateOfBirth = DateTime.Parse(getLineFromConsole<DateTime>("BirthDate"));
            info.CellPhone = getLineFromConsole<string>("Cellphone");
            info.EarnedPoints = Convert.ToDouble(getLineFromConsole<Double>("Earned Points"));
            info.EMailAddress = getLineFromConsole<string>("Email");
            info.LoadedPoints = Convert.ToDouble(getLineFromConsole<Double>("Loaded Points"));
        }

        private static string getLineFromConsole<T>(string message)
        {
            Console.WriteLine(message);
            string input = Console.ReadLine();

            if (typeof(T) == typeof(System.Int32))
            {
                int val = 0;
                input = int.TryParse(input, out val) ? val.ToString() : "0";
            }
            else if (typeof(T) == typeof(System.Double))
            {
                double val = 0;
                input = double.TryParse(input, out val) ? val.ToString() : "0.0";
            }
            else if (typeof(T) == typeof(System.DateTime))
            {
                DateTime dt = DateTime.Now;
                input = DateTime.TryParse(input, out dt) ? dt.ToString("o") : DateTime.Now.ToString("o");
            }

            return input;
        }
    }
}
