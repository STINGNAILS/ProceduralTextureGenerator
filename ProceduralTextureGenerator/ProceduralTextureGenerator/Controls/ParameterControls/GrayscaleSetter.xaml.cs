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
	/// Логика взаимодействия для GrayscaleSetter.xaml
	/// </summary>
	public partial class GrayscaleSetter : UserControl
	{
		private int index;

		private float sPrevious;


		public GrayscaleSetter(string name, int index_)
		{
			InitializeComponent();

			grayscaleSetterName.Content = name;

			index = index_;

			float s = CoreDll.GraphViewGetSelectedNodeFloatParameter(index);

			sPrevious = s;

			grayscaleTextBox.Text = s.ToString("F4").Replace(",", ".");
			grayscaleTextBox.LostFocus += FocusLost;
			grayscaleTextBox.GotKeyboardFocus += SelectText;
			grayscaleTextBox.MouseDoubleClick += SelectText;
			grayscaleTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;

			grayscaleRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(s * 255)), (byte)((int)(s * 255)), (byte)((int)(s * 255))));
		}


		private void FocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				if(value != sPrevious)
				{
					CoreDll.GraphViewSetSelectedNodeFloatParameter(index, value);

					MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
					mainWindow?.OnFunctionGraphChanged();
					mainWindow?.InvalidateSaving();

					sPrevious = value;
				}
			}

			ParentHelper.GetParentParameterPanel(this)?.Update();
		}


		private void SelectText(object sender, KeyboardFocusChangedEventArgs e)
		{
			TextBox textBox = (TextBox)sender;
			textBox.SelectAll();
		}


		private void SelectText(object sender, MouseButtonEventArgs e)
		{
			TextBox textBox = (TextBox)sender;
			textBox.SelectAll();
		}


		private void IgnoreMouseButton(object sender, MouseButtonEventArgs e)
		{
			TextBox textBox = (TextBox)sender;
			if(textBox == null || textBox.IsKeyboardFocusWithin)
			{
				return;
			}

			e.Handled = true;
			textBox.Focus();
		}
	}
}
