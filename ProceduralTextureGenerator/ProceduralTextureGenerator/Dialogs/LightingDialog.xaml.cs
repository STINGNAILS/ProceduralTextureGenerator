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
using System.Windows.Shapes;

namespace ProceduralTextureGenerator
{
    /// <summary>
    /// Логика взаимодействия для LightingDialog.xaml
    /// </summary>
    public partial class LightingDialog : Window
    {
        public LightingDialog()
        {
            InitializeComponent();

			directionalLightParameters.SetParameters(CoreDll.ObjectViewGetDirectionalLight());
			sphereLight1Parameters.SetParameters(CoreDll.ObjectViewGetSphereLight(0), 1);
			sphereLight2Parameters.SetParameters(CoreDll.ObjectViewGetSphereLight(1), 2);
			sphereLight3Parameters.SetParameters(CoreDll.ObjectViewGetSphereLight(2), 3);
			sphereLight4Parameters.SetParameters(CoreDll.ObjectViewGetSphereLight(3), 4);
		}


		private void AcceptLighting(object sender, RoutedEventArgs e)
		{
			DialogResult = true;

			CoreDll.ObjectViewSetDirectionalLight(directionalLightParameters.GetParameters());
			CoreDll.ObjectViewSetSphereLight(0, sphereLight1Parameters.GetParameters());
			CoreDll.ObjectViewSetSphereLight(1, sphereLight2Parameters.GetParameters());
			CoreDll.ObjectViewSetSphereLight(2, sphereLight3Parameters.GetParameters());
			CoreDll.ObjectViewSetSphereLight(3, sphereLight4Parameters.GetParameters());
		}
	}
}
