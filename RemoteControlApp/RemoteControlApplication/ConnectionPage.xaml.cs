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

        Client client = new Client();

        public ConnectionPage()
        {
            this.InitializeComponent();
            Window.Current.SizeChanged += Current_SizeChanged;
        }

        private void Current_SizeChanged(object sender, Windows.UI.Core.WindowSizeChangedEventArgs e) {
            int newWidth = ((int)e.Size.Width-320)/10;
            Thickness newMargin = new Thickness();
            newMargin.Top = newMargin.Bottom = 0;
            newMargin.Left = newMargin.Right = newWidth/8;

            ConnectButton.Width = newWidth;
            ConnectButton.Margin = newMargin;

            DisconnectButton.Width = newWidth;
            DisconnectButton.Margin = newMargin;

            ServernameBox.Width = newWidth;
            ServernameBox.Margin = newMargin;

            UsernameBox.Width = newWidth;
            UsernameBox.Margin = newMargin;

            PasswordBox.Width = newWidth;
            PasswordBox.Margin = newMargin;
        }
        

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            //bool output = TestProgram.Main();
            //StateText.Text = output.ToString();

            ConnectProgressRing.IsActive = true;
            String servername = ServernameBox.Text;
            String username = UsernameBox.Text;
            String password = PasswordBox.Password;
            String port = PortBox.Text;
            client.connect(servername, username, password, Int32.Parse(port));
            String state = client.GetState();
            if (state == "Connected")
            {
                DisconnectButton.Visibility = Visibility.Visible;
                //start session
            }
            StateText.Text = state;
            ConnectProgressRing.IsActive = false;
        }

        private void DisconnectButton_Click(object sender, RoutedEventArgs e)
        {
            client.disconnect();
        }

        private void ServernameBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            String text = e.ToString();
        }

        private void UsernameBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void PasswordBox_PasswordChanged(object sender, RoutedEventArgs e)
        {

        }
    }
}
