using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

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

        public void connect(String servername, String username, String password, Int32 port = 13000)
        {
            tcpClient.ConnectAsync(servername, port);
            if  (!tcpClient.Connected)
            {
                State="Can't connect to server";
                return;
            }

            Port = port;
            Servername = servername;
            stream = tcpClient.GetStream();

            Byte[] data_un = Encoding.ASCII.GetBytes(username);
            Byte[] data_pw = Encoding.ASCII.GetBytes(password);
            stream.Write(data_un, 0, data_un.Length);
            stream.Write(data_pw, 0, data_pw.Length);

            Byte[] data_responce = new Byte[256];

            Int32 bytes = stream.Read(data_responce, 0, data_responce.Length);

            String responce = Encoding.ASCII.GetString(data_responce);
            State = responce;

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
