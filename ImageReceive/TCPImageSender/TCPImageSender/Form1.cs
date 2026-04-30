using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;
using System.Net.Sockets;
using System.Net;

namespace TCPImageSender
{
    public partial class Form1 : Form
    {
        private string selectedImagePath;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void btnSelectImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Image Files|*.jpg;*.jpeg;*.png;*.bmp";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                selectedImagePath = ofd.FileName;
                pictureBox1.Image = Image.FromFile(selectedImagePath);
                lblStatus.Text = "Image selected.";
            }
        }

        private void btnSendImage_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(selectedImagePath))
            {
                MessageBox.Show("Please select an image first.");
                return;
            }

            string receiverIP = "192.168.10.133"; 
            int port = 5000;

            try
            {
                byte[] imageBytes;
                using (MemoryStream ms = new MemoryStream())
                {
                    Image img = Image.FromFile(selectedImagePath);
                    img.Save(ms, ImageFormat.Png);
                    imageBytes = ms.ToArray();
                }

                using (TcpClient client = new TcpClient())
                {
                    client.Connect(IPAddress.Parse(receiverIP), port);
                    using (NetworkStream stream = client.GetStream())
                    {
                        // Send length first
                        byte[] lengthBytes = BitConverter.GetBytes(imageBytes.Length);
                        stream.Write(lengthBytes, 0, lengthBytes.Length);

                        // Send image bytes
                        stream.Write(imageBytes, 0, imageBytes.Length);

                        lblStatus.Text = "Image sent successfully.";
                    }
                }
            }
            catch (Exception ex)
            {
                lblStatus.Text = "Error: " + ex.Message;
            }
        }
    }
}

