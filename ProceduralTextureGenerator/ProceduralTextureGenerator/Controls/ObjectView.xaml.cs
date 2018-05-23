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


		private void ChangeObject(object sender, RoutedEventArgs e)
		{
			ObjectDialog objectDialog = new ObjectDialog()
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			if(objectDialog.ShowDialog() == true)
			{
				CoreDll.ObjectViewSetObjectType(objectDialog.ObjectType);
			}
		}


		private void ChangeLighting(object sender, RoutedEventArgs e)
		{
			LightingDialog lightingDialog = new LightingDialog()
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			lightingDialog.ShowDialog();
		}


		private void ChangeEnvironment(object sender, RoutedEventArgs e)
		{
			EnvironmentDialog environmentDialog = new EnvironmentDialog()
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			if(environmentDialog.ShowDialog() == true)
			{
				CoreDll.ObjectViewSetEnvironmentMap(environmentDialog.EnvironmentIndex);
			}
		}


		private void ScopeObject(object sender, RoutedEventArgs e)
		{
			CoreDll.ObjectViewScope();
		}
	}
}
