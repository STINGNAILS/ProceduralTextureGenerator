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
    /// Interaction logic for ObjectView.xaml
    /// </summary>
    public partial class ObjectView : UserControl
    {
        public ObjectView()
        {
            InitializeComponent();

			view.Bind(1);
		}


        public void Update(float dt)
        {
            view.Update(dt);
        }


        public void Render()
        {
            view.Render();
        }
    }
}
