
using System;
using System.IO;
using PDServer;
using System.Collections.Generic;

namespace post
{

	public class postClient
	{
		TCPClient client;
		public postClient ()
		{
			client = new TCPClient ();
		}
		
		public void Connect (String IPAddress, int Port)
		{
			try 
			{				
				client.Connect (IPAddress, Port);
			} catch (Exception e) {				
				Console.WriteLine (e.Message);
			}
		}

		public Boolean IsConnected()
		{
			return this.client.Connected;
		}

		public String getdata ()
		{			
			return client.Receive ();
		}
		
		public void put (string Data)
		{			
			if(client.IsConnected())
			{			
				client.Send (Data);
			}
		}
		
		public void putFile (string FileName)
		{			
			if(client.IsConnected())
			{			
				if(File.Exists(FileName))
				{
					System.IO.FileStream file = File.OpenRead(FileName);

                    var fileContents = System.IO.File.ReadAllText(FileName);
                    fileContents = fileContents.Replace("????", Guid.NewGuid().ToString() );
					client.Send (fileContents);
				}
				else	
				{
					Console.WriteLine ("File not found");
				}
			}
		}
	}
}
