using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.Windows.Threading;

namespace ProceduralTextureGenerator
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private DateTime timePrevious;
        private DateTime timeCurrent;

        private System.Drawing.Point mouseCoordsPrevious;
        private System.Drawing.Point mouseCoordsCurrent;


        public MainWindow()
        {
            InitializeComponent();

            CoreDll.Init();

            CompositionTarget.Rendering += Update;
            
            //graphView.Init();
            //textureView.Init();

            mouseCoordsPrevious = System.Windows.Forms.Cursor.Position;

            timePrevious = DateTime.Now;
        }


        private void Update(object sender, EventArgs e)
        {
            UpdateControls();
            CoreDll.Render();
        }


        private void UpdateControls()
        {
            mouseCoordsCurrent = System.Windows.Forms.Cursor.Position;
            timeCurrent = DateTime.Now;

            float dx = mouseCoordsCurrent.X - mouseCoordsPrevious.X;
            float dy = mouseCoordsCurrent.Y - mouseCoordsPrevious.Y;
            float dt = (timeCurrent - timePrevious).Ticks / 10000000.0f;

            objectView.OnMouseMove(dx, dy, dt);
            //graphView.OnMouseMove(dx, dy, dt);
            //textureView.OnMouseMove(dx, dy, dt);

            mouseCoordsPrevious = mouseCoordsCurrent;
            timePrevious = timeCurrent;
        }


        private void OnExit(object sender, EventArgs e)
        {
            CoreDll.Release();
        }
    }
}
