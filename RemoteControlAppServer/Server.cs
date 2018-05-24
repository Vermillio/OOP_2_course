using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;

namespace RemoteControlAppServer
{
    class Server
    {
        private TcpListener server;

        String password;

        public Server()
        {
            server = new TcpListener(IPAddress.Any, 8888);
        }

        public void Start()
        {
            server.Start();
            while (true)
            {
                //waiting for connection
                TcpClient client = server.AcceptTcpClient();
                NetworkStream stream = client.GetStream();
            } 
        }

        public void ProcessRequest()
        {

        }

        public void SetPassword(String t_password)
        {
            this.password = t_password;
        }

        public IPAddress GetIPAddress()
        {
            IPEndPoint point = (IPEndPoint)server.LocalEndpoint;
            return point.Address;
        }

        public int GetPort()
        {
            IPEndPoint point = (IPEndPoint)server.LocalEndpoint;
            return point.Port;
        }
    }
}
