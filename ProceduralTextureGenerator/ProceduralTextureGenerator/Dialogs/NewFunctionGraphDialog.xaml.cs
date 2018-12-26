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
    /// Логика взаимодействия для NewFunctionGraphWindow.xaml
    /// </summary>
    public partial class NewFunctionGraphDialog : Window
    {
        public NewFunctionGraphDialog()
        {
            InitializeComponent();

			functionGraphNameTextBox.PreviewTextInput += CheckTextInput;
		}


		private void OnContentRendered(object sender, EventArgs e)
		{
			functionGraphNameTextBox.Text = "Unnamed";
			functionGraphNameTextBox.SelectAll();
			functionGraphNameTextBox.Focus();
		}


		private void AcceptName(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
		}


		public string FunctionGraphName
		{
			get
			{
				return functionGraphNameTextBox.Text;
			}
		}


		private string restrictedSymbols = "\\/:*?\"<>|";


		private void CheckTextInput(object sender, TextCompositionEventArgs e)
		{
			if(restrictedSymbols.Contains(e.Text))
			{
				e.Handled = true;
			}
		}
	}
}
