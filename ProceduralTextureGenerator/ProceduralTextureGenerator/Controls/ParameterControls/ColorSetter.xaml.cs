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
	/// Логика взаимодействия для ColorSetter.xaml
	/// </summary>
	public partial class ColorSetter : UserControl
	{
		private int rIndex;
		private int gIndex;
		private int bIndex;


		private float rPrevious;
		private float gPrevious;
		private float bPrevious;


		public ColorSetter(string name, int rIndex_, int gIndex_, int bIndex_)
		{
			InitializeComponent();

			colorSetterName.Content = name;

			rIndex = rIndex_;
			gIndex = gIndex_;
			bIndex = bIndex_;

			float r = CoreDll.GraphViewGetSelectedNodeFloatParameter(rIndex);
			float g = CoreDll.GraphViewGetSelectedNodeFloatParameter(gIndex);
			float b = CoreDll.GraphViewGetSelectedNodeFloatParameter(bIndex);

			rPrevious = r;
			gPrevious = g;
			bPrevious = b;

			redColorTextBox.Text = r.ToString("F4").Replace(",", ".");
			redColorTextBox.LostFocus += FocusLostRed;
			redColorTextBox.GotKeyboardFocus += SelectText;
			redColorTextBox.MouseDoubleClick += SelectText;
			redColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			redColorTextBox.KeyDown += OnEnterDown;

			greenColorTextBox.Text = g.ToString("F4").Replace(",", ".");
			greenColorTextBox.LostFocus += FocusLostGreen;
			greenColorTextBox.GotKeyboardFocus += SelectText;
			greenColorTextBox.MouseDoubleClick += SelectText;
			greenColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			greenColorTextBox.KeyDown += OnEnterDown;

			blueColorTextBox.Text = b.ToString("F4").Replace(",", ".");
			blueColorTextBox.LostFocus += FocusLostBlue;
			blueColorTextBox.GotKeyboardFocus += SelectText;
			blueColorTextBox.MouseDoubleClick += SelectText;
			blueColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			blueColorTextBox.KeyDown += OnEnterDown;

			colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(r * 255)), (byte)((int)(g * 255)), (byte)((int)(b * 255))));
		}


		private void FocusLostRed(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				if(value != rPrevious)
				{
					CoreDll.GraphViewSetSelectedNodeFloatParameter(rIndex, value);

					MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
					mainWindow?.OnFunctionGraphChanged();
					mainWindow?.InvalidateSaving();

					rPrevious = value;
				}
			}

			ParentHelper.GetParentParameterPanel(this)?.Update();
		}


		private void FocusLostGreen(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				if(value != gPrevious)
				{
					CoreDll.GraphViewSetSelectedNodeFloatParameter(gIndex, value);

					MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
					mainWindow?.OnFunctionGraphChanged();
					mainWindow?.InvalidateSaving();

					gPrevious = value;
				}
			}

			ParentHelper.GetParentParameterPanel(this)?.Update();
		}


		private void FocusLostBlue(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				if(value != bPrevious)
				{
					CoreDll.GraphViewSetSelectedNodeFloatParameter(bIndex, value);

					MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
					mainWindow?.OnFunctionGraphChanged();
					mainWindow?.InvalidateSaving();

					bPrevious = value;
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


		private void OnEnterDown(object sender, KeyEventArgs e)
		{
			if(e.Key == Key.Enter)
			{
				ParentHelper.GetParentParameterPanel(this)?.Focus();
			}
		}
	}
}
