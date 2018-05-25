using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;

using System.IO;
using System.ComponentModel;
using System.Threading;

namespace RemoteControlAppServer
{
    class Server
    {
        private TcpListener server;

        String password;

        BackgroundWorker worker = new BackgroundWorker();
        int tcpClientsConnected;
        public string state="";
        public static ManualResetEvent tcpClientConnected =
        new ManualResetEvent(false);

        public Server()
        {
            var ipAddress = IPAddress.Parse("127.0.0.1");
            server = new TcpListener(ipAddress, 2104);

        }

        public void Start()
        {
            worker.WorkerReportsProgress = true;
            worker.WorkerSupportsCancellation = true;

            worker.ProgressChanged += (s, args) =>
            {
            };

            worker.DoWork += (s, args) =>
            {
                // startup the server on localhost 
                server.Start();

                while (!worker.CancellationPending)
                {

                    // as long as we're not pending a cancellation, let's keep accepting requests 
                    TcpClient attachedClient = server.AcceptTcpClient();
                    state = "connected";
                    StreamReader clientIn = new StreamReader(attachedClient.GetStream());
                    StreamWriter clientOut = new StreamWriter(attachedClient.GetStream());
                    clientOut.AutoFlush = true;

                    string msg="accept";
                    clientOut.WriteLine(string.Format("The server replied with: {0}", msg));
                }
            };

            worker.RunWorkerAsync();

     }

        public void Stop()
        {
            worker.CancelAsync();
        }

        public void acceptRequest(IAsyncResult ar)
        {
            TcpListener listener = (TcpListener)ar.AsyncState;
            TcpClient client = listener.EndAcceptTcpClient(ar);

            Byte[] bytes = new Byte[256];
            NetworkStream stream = client.GetStream();

            int i;
            String data = "";

            //receive and process client data
            while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
            {
                data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);

                if (data!=password)
                {
                    data = "Incorrect password";
                }
                else
                {
                    data = "Correct password";
                }

                byte[] msg = System.Text.Encoding.ASCII.GetBytes(data);

                // Send back a response.
                stream.Write(msg, 0, msg.Length);
            }

            client.Close();
            ++tcpClientsConnected;
            tcpClientConnected.Set();
        }

        public void noClientsCallback()
        {
            tcpClientConnected.Set();
        }

        public void SetPassword(String t_password)
        {
            this.password = t_password;
        }

        public IPAddress GetIPAddress()
        {
            if (server==null)
                return null;
            IPEndPoint point = (IPEndPoint)server.LocalEndpoint;
            return point.Address;
        }

        public int GetPort()
        {
            if (server == null)
                return 0;
            IPEndPoint point = (IPEndPoint)server.LocalEndpoint;
            return point.Port;
        }

    }
}
