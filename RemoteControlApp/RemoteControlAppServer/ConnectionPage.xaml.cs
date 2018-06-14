using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace RemoteControlAppServer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ConnectionPage : Page
    {

        Server server=new Server();

        public ConnectionPage()
        {
            this.InitializeComponent();
        }

        public void MainButton_Click(object sender, RoutedEventArgs e)
        {
            if (((Button)sender).Content.Equals("START"))
            {
                ((Button)sender).Content = "STOP";
                MainText.Visibility = PasswordBox.Visibility = Visibility.Collapsed;

                String ip = server.GetIPAddress().ToString();
                IPText.Text = "Local ip address:\n" + ip;

                int port = server.GetPort();
                PortText.Text = "Port:\n" + port.ToString();

                String password = PasswordBox.Password;
//                PassText.Text = "Password:\n" + password;
                server.SetPassword(password);

                MainText2.Visibility = Visibility.Visible;

                server.Start();
                string state = server.state;
                MainText2.Text = state;
            } else
            {
                ((Button)sender).Content = "START";
                PasswordBox.Password="";
                MainText.Visibility = PasswordBox.Visibility = Visibility.Visible;

                IPText.Text = "";
                PortText.Text = "";

                MainText2.Visibility = Visibility.Collapsed;

                server.Stop();
            }
        }

    }

}
