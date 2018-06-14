using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

using System.IO;
using System.ComponentModel;
using System.Threading;


namespace RemoteControlAppServer
{
    class Client
    {
        private TcpClient tcpClient;
        private String Servername;
        private Int32 Port;
        private NetworkStream stream;

        private string State="";

        public Client()
        {
            tcpClient = new TcpClient();
        }

        public void connect(String servername, String username, String password, Int32 port = 60060)
        {
            TcpClient client = new TcpClient();

            for (int i = 0; i < 3; i++)
            {
                var result = client.BeginConnect(servername, port, null, null);

                // give the client 5 seconds to connect
                result.AsyncWaitHandle.WaitOne(5000);

                if (!client.Connected)
                {
                    try {
                        client.EndConnect(result);
                        State = "Can't connect to server.";
                    }
                    catch (SocketException e) {
                        State = "SocketException: " + e.Message;
                    }
                    continue;
                }

                break;
            }

            if (client.Connected)
            {
                StreamReader clientIn = new StreamReader(client.GetStream());
                StreamWriter clientOut = new StreamWriter(client.GetStream());

                clientOut.AutoFlush = true;

                string key="request";
                clientOut.WriteLine(key);
                State = "Connected";
            }
            else
            {
            }
        }

    public void disconnect()
        {
            if (!tcpClient.Connected)
                return;
            IAsyncResult result=null;
            tcpClient.EndConnect(result);
        }

        public String GetState()
        {
            return State;
        }

        public bool isValidState()
        {
            return State == "Connected";
        }

        public void StartReceivingData()
        {

        }
    }
}

