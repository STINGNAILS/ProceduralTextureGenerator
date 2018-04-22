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
	/// Логика взаимодействия для NotSavedDialog.xaml
	/// </summary>
	public partial class NotSavedDialog : Window
	{
		public NotSavedDialog()
		{
			InitializeComponent();

			SaveChanges = true;
		}


		private void Save(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
			SaveChanges = true;
		}


		private void Discard(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
			SaveChanges = false;
		}


		public bool SaveChanges
		{
			get;
			private set;
		}
	}
}
