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
                server.SetPassword(PasswordBox.Password);
                MainText.Visibility = PasswordBox.Visibility = Visibility.Collapsed;

                String ip = server.GetIPAddress().ToString();
                IPText.Text = "Your current local ip address is\n" + ip;

                int port = server.GetPort();
                PortText.Text = "Your current port is\n" + port.ToString();
                MainText2.Visibility = Visibility.Visible;
            } else
            {
                ((Button)sender).Content = "START";
                PasswordBox.Password="";
                MainText.Visibility = PasswordBox.Visibility = Visibility.Visible;

                IPText.Text = "";
                PortText.Text = "";

                MainText2.Visibility = Visibility.Collapsed;
            }
        }

    }

}
