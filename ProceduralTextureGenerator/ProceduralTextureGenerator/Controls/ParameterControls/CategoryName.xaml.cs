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
	/// Логика взаимодействия для CategoryName.xaml
	/// </summary>
	public partial class CategoryName : UserControl
	{
		public CategoryName(string name)
		{
			InitializeComponent();

			categoryNameLabel.Content = name;
		}
	}
}
