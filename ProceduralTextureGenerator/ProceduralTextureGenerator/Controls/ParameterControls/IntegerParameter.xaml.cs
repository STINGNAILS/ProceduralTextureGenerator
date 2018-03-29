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
    /// Логика взаимодействия для IntegerParameter.xaml
    /// </summary>
    public partial class IntegerParameter : UserControl
	{
		int index;
		int minValue;
		int maxValue;


		public IntegerParameter()
        {
            InitializeComponent();
        }


		public IntegerParameter(string name, int index_, int minValue_, int maxValue_)
		{
			InitializeComponent();

			integerParameterName.Content = name;

			index = index_;
			minValue = minValue_;
			maxValue = maxValue_;

			int value = CoreDll.GraphViewGetSelectedNodeIntParameter(index);

			integerParameterTextBox.Text = value.ToString();
			integerParameterTextBox.LostFocus += FocusLost;
			integerParameterTextBox.GotKeyboardFocus += SelectText;
			integerParameterTextBox.MouseDoubleClick += SelectText;
			integerParameterTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			//integerParameterTextBox.PreviewTextInput += CheckInput;
		}


		private void FocusLost(object sender, RoutedEventArgs e)
		{
			int value;
			if(int.TryParse(((TextBox)sender).Text, out value))
			{
				value = Math.Min(Math.Max(value, minValue), maxValue);

				CoreDll.GraphViewSetSelectedNodeIntParameter(index, value);
				CoreDll.GraphViewProcess();
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
