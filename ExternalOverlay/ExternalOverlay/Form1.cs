using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace ExternalOverlay
{
    public partial class Form1 : Form
    {
        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        public static extern IntPtr GetForegroundWindow();

        public Form1()
        {
            InitializeComponent();

            label1.Text = "hello";


            IntPtr App = GetForegroundWindow();

            label1.Text = App.ToString();

            this.Refresh();
        }
    }
}
