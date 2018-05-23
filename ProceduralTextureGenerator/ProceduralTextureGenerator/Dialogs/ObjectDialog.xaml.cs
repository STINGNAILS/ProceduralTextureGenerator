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
    /// Логика взаимодействия для ObjectDialog.xaml
    /// </summary>
    public partial class ObjectDialog : Window
    {
        public ObjectDialog()
        {
            InitializeComponent();

			objectComboBox.Items.Add("Cube");
			objectComboBox.Items.Add("Sphere");
			objectComboBox.SelectedIndex = CoreDll.ObjectViewGetObjectType();
		}


		private void AcceptObject(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
		}
		

		public int ObjectType
		{
			get
			{
				return objectComboBox.SelectedIndex;
			}
		}
	}
}
