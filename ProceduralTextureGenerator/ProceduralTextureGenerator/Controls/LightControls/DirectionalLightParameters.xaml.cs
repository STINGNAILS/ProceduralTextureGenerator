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
    /// Логика взаимодействия для DirectionalLightParameters.xaml
    /// </summary>
    public partial class DirectionalLightParameters : UserControl
	{
		private DirectionalLight directionalLight;


		public DirectionalLightParameters()
        {
            InitializeComponent();

			isEnabledCheckBox.Checked += EnableDirectionalLight;
			isEnabledCheckBox.Unchecked += DisableDirectionalLight;

			redColorTextBox.LostFocus += FocusLostRed;
			redColorTextBox.GotKeyboardFocus += SelectText;
			redColorTextBox.MouseDoubleClick += SelectText;
			redColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			redColorTextBox.KeyDown += OnEnterDown;

			greenColorTextBox.LostFocus += FocusLostGreen;
			greenColorTextBox.GotKeyboardFocus += SelectText;
			greenColorTextBox.MouseDoubleClick += SelectText;
			greenColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			greenColorTextBox.KeyDown += OnEnterDown;

			blueColorTextBox.LostFocus += FocusLostBlue;
			blueColorTextBox.GotKeyboardFocus += SelectText;
			blueColorTextBox.MouseDoubleClick += SelectText;
			blueColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			blueColorTextBox.KeyDown += OnEnterDown;

			intensityTextBox.LostFocus += FocusLostIntensity;
			intensityTextBox.GotKeyboardFocus += SelectText;
			intensityTextBox.MouseDoubleClick += SelectText;
			intensityTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			intensityTextBox.KeyDown += OnEnterDown;

			phiTextBox.LostFocus += FocusLostPhi;
			phiTextBox.GotKeyboardFocus += SelectText;
			phiTextBox.MouseDoubleClick += SelectText;
			phiTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			phiTextBox.KeyDown += OnEnterDown;

			psiTextBox.LostFocus += FocusLostPsi;
			psiTextBox.GotKeyboardFocus += SelectText;
			psiTextBox.MouseDoubleClick += SelectText;
			psiTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			psiTextBox.KeyDown += OnEnterDown;
		}


		public void SetParameters(DirectionalLight directionalLight_)
		{
			directionalLight = directionalLight_;

			if(directionalLight.isEnabled != 0)
			{
				isEnabledCheckBox.IsChecked = true;
				directionalLightStackPanel.Visibility = Visibility.Visible;
			}
			else
			{
				isEnabledCheckBox.IsChecked = false;
				directionalLightStackPanel.Visibility = Visibility.Collapsed;
			}

			redColorTextBox.Text = directionalLight.r.ToString("F4").Replace(",", ".");
			greenColorTextBox.Text = directionalLight.g.ToString("F4").Replace(",", ".");
			blueColorTextBox.Text = directionalLight.b.ToString("F4").Replace(",", ".");
			colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(directionalLight.r * 255)), (byte)((int)(directionalLight.g * 255)), (byte)((int)(directionalLight.b * 255))));

			intensityTextBox.Text = directionalLight.intensity.ToString("F4").Replace(",", ".");

			phiTextBox.Text = directionalLight.phi.ToString("F4").Replace(",", ".");
			psiTextBox.Text = directionalLight.psi.ToString("F4").Replace(",", ".");
		}


		public DirectionalLight GetParameters()
		{
			return directionalLight;
		}


		private void EnableDirectionalLight(object sender, RoutedEventArgs e)
		{
			directionalLight.isEnabled = 1;
			directionalLightStackPanel.Visibility = Visibility.Visible;
		}


		private void DisableDirectionalLight(object sender, RoutedEventArgs e)
		{
			directionalLight.isEnabled = 0;
			directionalLightStackPanel.Visibility = Visibility.Collapsed;
		}


		private void FocusLostRed(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				directionalLight.r = value;
				colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(directionalLight.r * 255)), (byte)((int)(directionalLight.g * 255)), (byte)((int)(directionalLight.b * 255))));
			}

			redColorTextBox.Text = directionalLight.r.ToString("F4").Replace(",", ".");
		}


		private void FocusLostGreen(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				directionalLight.g = value;
				colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(directionalLight.r * 255)), (byte)((int)(directionalLight.g * 255)), (byte)((int)(directionalLight.b * 255))));
			}

			greenColorTextBox.Text = directionalLight.g.ToString("F4").Replace(",", ".");
		}


		private void FocusLostBlue(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				directionalLight.b = value;
				colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(directionalLight.r * 255)), (byte)((int)(directionalLight.g * 255)), (byte)((int)(directionalLight.b * 255))));
			}

			blueColorTextBox.Text = directionalLight.b.ToString("F4").Replace(",", ".");
		}


		private void FocusLostIntensity(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1000000.0f);

				directionalLight.intensity = value;
			}

			intensityTextBox.Text = directionalLight.intensity.ToString("F4").Replace(",", ".");
		}


		private void FocusLostPhi(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 180.0f);

				directionalLight.phi = value;
			}

			phiTextBox.Text = directionalLight.phi.ToString("F4").Replace(",", ".");
		}


		private void FocusLostPsi(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 360.0f);

				directionalLight.psi = value;
			}

			psiTextBox.Text = directionalLight.psi.ToString("F4").Replace(",", ".");
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
