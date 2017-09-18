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

namespace ProceduralTextureGenerator
{
    /// <summary>
    /// Interaction logic for GraphView.xaml
    /// </summary>
    public partial class GraphView : UserControl
    {
        private bool isNavigating;


        public GraphView()
        {
            InitializeComponent();

            isNavigating = false;
        }


        public void Init()
        {
            //view.Init(2);
        }


        private void OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Right)
            {
                isNavigating = true;
            }
        }


        public void OnMouseMove(float dx, float dy, float dt)
        {
            if (isNavigating)
            {
                //view.Navigate(2, dx, dy, dt);
            }
        }


        private void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            //view.Resize(2);
        }


        private void OnMouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Right)
            {
                isNavigating = false;
            }
        }


        private void OnMouseWheel(object sender, MouseWheelEventArgs e)
        {
            
        }
    }
}
