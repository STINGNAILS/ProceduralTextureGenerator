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
    /// Логика взаимодействия для FloatParameter.xaml
    /// </summary>
    public partial class FloatParameter : UserControl
    {
		private int index;
		private float minValue;
		private float maxValue;
		
		private float previousValue;


		public FloatParameter(string name, int index_, float minValue_, float maxValue_)
		{
			InitializeComponent();

			floatParameterName.Content = name;

			index = index_;
			minValue = minValue_;
			maxValue = maxValue_;

			float value = CoreDll.GraphViewGetSelectedNodeFloatParameter(index);

			previousValue = value;

			floatParameterTextBox.Text = value.ToString("F4").Replace(",", ".");
			floatParameterTextBox.LostFocus += FocusLost;
			floatParameterTextBox.GotKeyboardFocus += SelectText;
			floatParameterTextBox.MouseDoubleClick += SelectText;
			floatParameterTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
		}


		private void FocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, minValue), maxValue);

				if(value != previousValue)
				{
					CoreDll.GraphViewSetSelectedNodeFloatParameter(index, value);
					CoreDll.GraphViewProcess();

					ParentHelper.GetParentMainWindow(this)?.InvalidateSaving();

					previousValue = value;
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
