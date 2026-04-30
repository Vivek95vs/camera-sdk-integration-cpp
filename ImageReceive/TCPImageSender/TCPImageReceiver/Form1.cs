using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace TCPImageReceiver
{
    public partial class Form1 : Form
    {
        private TcpListener listener;
        private Thread listenThread;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void btnStartListening_Click(object sender, EventArgs e)
        {
            listenThread = new Thread(StartListening);
            listenThread.IsBackground = true;
            listenThread.Start();
            UpdateStatus("Listening for image...");
        }

        private void StartListening()
        {
            int port = 5000;
            listener = new TcpListener(IPAddress.Any, port);
            listener.Start();

            try
            {
                using (TcpClient client = listener.AcceptTcpClient())
                using (NetworkStream stream = client.GetStream())
                {
                    // Read image length (first 4 bytes)
                    byte[] lengthBytes = new byte[4];
                    stream.Read(lengthBytes, 0, 4);
                    int imageLength = BitConverter.ToInt32(lengthBytes, 0);

                    // Read image data
                    byte[] imageBytes = new byte[imageLength];
                    int bytesRead = 0;
                    while (bytesRead < imageLength)
                    {
                        int read = stream.Read(imageBytes, bytesRead, imageLength - bytesRead);
                        if (read == 0) break;
                        bytesRead += read;
                    }

                    // Convert and display image
                    using (MemoryStream ms = new MemoryStream(imageBytes))
                    {
                        Image receivedImage = Image.FromStream(ms);
                        ShowImage(receivedImage);
                        UpdateStatus("Image received.");
                    }
                }
            }
            catch (Exception ex)
            {
                UpdateStatus("Error: " + ex.Message);
            }
            finally
            {
                listener.Stop();
            }
        }

        private void ShowImage(Image img)
        {
            if (pictureBox1.InvokeRequired)
                pictureBox1.Invoke(new Action<Image>(ShowImage), img);
            else
                pictureBox1.Image = img;
        }

        private void UpdateStatus(string msg)
        {
            if (lblStatus.InvokeRequired)
                lblStatus.Invoke(new Action<string>(UpdateStatus), msg);
            else
                lblStatus.Text = msg;
        }
    }
}
