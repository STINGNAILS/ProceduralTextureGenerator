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
using System.Windows.Forms;

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Логика взаимодействия для SaveMaterialDialog.xaml
	/// </summary>
	public partial class SaveMaterialDialog : Window
	{
		public SaveMaterialDialog()
		{
			InitializeComponent();

			formatComboBox.Items.Add("JPEG (*.jpg)");
			formatComboBox.Items.Add("Portable Network Graphics (*.png)");
			formatComboBox.Items.Add("Bitmap (*.bmp)");
			formatComboBox.Items.Add("DirectDraw Surface (*.dds)");
			formatComboBox.SelectedIndex = 0;

			functionGraphNameTextBox.Text = AppDomain.CurrentDomain.BaseDirectory;
		}


		private void OpenFolderBrowserDialog(object sender, RoutedEventArgs e)
		{
			FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog()
			{
				Description = "Select destination directory"
			};

			if(folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				functionGraphNameTextBox.Text = folderBrowserDialog.SelectedPath;
			}
		}


		private void AcceptMaterial(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
		}


		public string FolderName
		{
			get
			{
				return functionGraphNameTextBox.Text;
			}
		}


		public int FormatIndex
		{
			get
			{
				return formatComboBox.SelectedIndex + 1;
			}
		}
	}
}
