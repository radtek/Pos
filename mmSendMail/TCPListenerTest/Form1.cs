using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Threading;
using System.Net.Sockets;
using System.Net;

namespace TCPListenerTest
{
    public partial class Form1 : Form
    {
        private List<Socket> _clients = new List<Socket>();
        private Thread _listening_thread;
        private TcpListener _listener;
        private bool _listenerAlive;

        public Form1()
        {
            InitializeComponent();

            this._listening_thread = new Thread(new ThreadStart(this.ListeningThread));
            this._listening_thread.Start();

            _listenerAlive = true;
        }

        private void ListeningThread() // let's listen in another thread instead!!
        {
            int port = 12345; // change as required

            this._listener = new TcpListener(IPAddress.Any, port);

            try
            {
                this._listener.Start();
            }
            catch (Exception e) { showText(richTextBox1, "couldn't bind to port " + port + " -> " + e.Message); return; }
            //catch (Exception e) { MessageBox.Show("couldn't bind to port " + port + " -> " + e.Message); return; }

            showText(richTextBox1, "Liten to port " + port);

            while(_listenerAlive)
            {
                if (this._listener.Pending())
                    this._clients.Add(this._listener.AcceptSocket()); // won't block because pending was true

                foreach (Socket sock in this._clients)
                    if (sock.Poll(0, SelectMode.SelectError))
                        _clients.Remove(sock);
                    else if (sock.Poll(0, SelectMode.SelectRead))
                        ParserFunction(sock);

                Thread.Sleep(30);
            }
        }

        private void ParserFunction(Socket sock)
        {

        }

        //::::::::::::::::::::::::::::::::::::

        delegate void notifFunc();
        RichTextBox _rtBox;
        string _txt;

        private void showText(RichTextBox aRTBox , string aText)
        {
            if (aRTBox.InvokeRequired)
            {
                _rtBox = aRTBox;
                _txt = aText;
 
                _rtBox.Invoke(new notifFunc(onShowTxtFuncInvoke));

                return;
            }

            aRTBox.Clear();
            aRTBox.AppendText(aText);
        }

        private void onShowTxtFuncInvoke()
        {
            _rtBox.Clear();
            _rtBox.AppendText(_txt);
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            _listenerAlive = false;
        }

        //::::::::::::::::::::::::::::::::::::

    }
}

