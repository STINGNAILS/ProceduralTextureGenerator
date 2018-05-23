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
	/// Логика взаимодействия для EnvironmentDialog.xaml
	/// </summary>
	public partial class EnvironmentDialog : Window
	{
		public EnvironmentDialog()
		{
			InitializeComponent();

			environmentComboBox.Items.Add("Citadella");
			environmentComboBox.Items.Add("DallasW");
			environmentComboBox.Items.Add("GoldenGateBridge");
			environmentComboBox.Items.Add("MarriottMadisonWest");
			environmentComboBox.Items.Add("Storforsen");
			environmentComboBox.Items.Add("Storforsen2");
			environmentComboBox.Items.Add("SwedishRoyalCastle");
			environmentComboBox.Items.Add("Tenerife");
			environmentComboBox.Items.Add("UnionSquare");
			environmentComboBox.Items.Add("Yokohama");
			environmentComboBox.SelectedIndex = CoreDll.ObjectViewGetEnvironmentMapIndex();
		}


		private void AcceptEnvironment(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
		}


		public int EnvironmentIndex
		{
			get
			{
				return environmentComboBox.SelectedIndex;
			}
		}
	}
}
