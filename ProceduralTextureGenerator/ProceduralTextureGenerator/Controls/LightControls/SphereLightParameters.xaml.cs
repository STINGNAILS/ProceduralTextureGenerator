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
    /// Логика взаимодействия для SphereLightParameters.xaml
    /// </summary>
    public partial class SphereLightParameters : UserControl
	{
		private SphereLight sphereLight;


		public SphereLightParameters()
        {
            InitializeComponent();

			isEnabledCheckBox.Checked += EnableSphereLight;
			isEnabledCheckBox.Unchecked += DisableSphereLight;

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

			lightRadiusTextBox.LostFocus += FocusLostLightRadius;
			lightRadiusTextBox.GotKeyboardFocus += SelectText;
			lightRadiusTextBox.MouseDoubleClick += SelectText;
			lightRadiusTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			lightRadiusTextBox.KeyDown += OnEnterDown;

			sourceRadiusTextBox.LostFocus += FocusLostSourceRadius;
			sourceRadiusTextBox.GotKeyboardFocus += SelectText;
			sourceRadiusTextBox.MouseDoubleClick += SelectText;
			sourceRadiusTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			sourceRadiusTextBox.KeyDown += OnEnterDown;

			xTextBox.LostFocus += FocusLostX;
			xTextBox.GotKeyboardFocus += SelectText;
			xTextBox.MouseDoubleClick += SelectText;
			xTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			xTextBox.KeyDown += OnEnterDown;

			yTextBox.LostFocus += FocusLostY;
			yTextBox.GotKeyboardFocus += SelectText;
			yTextBox.MouseDoubleClick += SelectText;
			yTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			yTextBox.KeyDown += OnEnterDown;

			zTextBox.LostFocus += FocusLostZ;
			zTextBox.GotKeyboardFocus += SelectText;
			zTextBox.MouseDoubleClick += SelectText;
			zTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			zTextBox.KeyDown += OnEnterDown;
		}


		public void SetParameters(SphereLight sphereLight_, int sphereLightIndex)
		{
			sphereLight = sphereLight_;
			sphereLightNameLabel.Content += " " + sphereLightIndex.ToString();

			if(sphereLight.isEnabled != 0)
			{
				isEnabledCheckBox.IsChecked = true;
				sphereLightStackPanel.Visibility = Visibility.Visible;
			}
			else
			{
				isEnabledCheckBox.IsChecked = false;
				sphereLightStackPanel.Visibility = Visibility.Collapsed;
			}

			redColorTextBox.Text = sphereLight.r.ToString("F4").Replace(",", ".");
			greenColorTextBox.Text = sphereLight.g.ToString("F4").Replace(",", ".");
			blueColorTextBox.Text = sphereLight.b.ToString("F4").Replace(",", ".");
			colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(sphereLight.r * 255)), (byte)((int)(sphereLight.g * 255)), (byte)((int)(sphereLight.b * 255))));

			intensityTextBox.Text = sphereLight.intensity.ToString("F4").Replace(",", ".");

			lightRadiusTextBox.Text = sphereLight.lightRadius.ToString("F4").Replace(",", ".");

			sourceRadiusTextBox.Text = sphereLight.sourceRadius.ToString("F4").Replace(",", ".");

			xTextBox.Text = sphereLight.x.ToString("F4").Replace(",", ".");
			yTextBox.Text = sphereLight.y.ToString("F4").Replace(",", ".");
			zTextBox.Text = sphereLight.z.ToString("F4").Replace(",", ".");
		}


		public SphereLight GetParameters()
		{
			return sphereLight;
		}


		private void EnableSphereLight(object sender, RoutedEventArgs e)
		{
			sphereLight.isEnabled = 1;
			sphereLightStackPanel.Visibility = Visibility.Visible;
		}


		private void DisableSphereLight(object sender, RoutedEventArgs e)
		{
			sphereLight.isEnabled = 0;
			sphereLightStackPanel.Visibility = Visibility.Collapsed;
		}


		private void FocusLostRed(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				sphereLight.r = value;
				colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(sphereLight.r * 255)), (byte)((int)(sphereLight.g * 255)), (byte)((int)(sphereLight.b * 255))));
			}

			redColorTextBox.Text = sphereLight.r.ToString("F4").Replace(",", ".");
		}


		private void FocusLostGreen(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				sphereLight.g = value;
				colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(sphereLight.r * 255)), (byte)((int)(sphereLight.g * 255)), (byte)((int)(sphereLight.b * 255))));
			}

			greenColorTextBox.Text = sphereLight.g.ToString("F4").Replace(",", ".");
		}


		private void FocusLostBlue(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);

				sphereLight.b = value;
				colorRect.Fill = new SolidColorBrush(Color.FromArgb((byte)255, (byte)((int)(sphereLight.r * 255)), (byte)((int)(sphereLight.g * 255)), (byte)((int)(sphereLight.b * 255))));
			}

			blueColorTextBox.Text = sphereLight.b.ToString("F4").Replace(",", ".");
		}


		private void FocusLostIntensity(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1000000.0f);

				sphereLight.intensity = value;
			}

			intensityTextBox.Text = sphereLight.intensity.ToString("F4").Replace(",", ".");
		}


		private void FocusLostLightRadius(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1000000.0f);

				sphereLight.lightRadius = value;
			}

			lightRadiusTextBox.Text = sphereLight.lightRadius.ToString("F4").Replace(",", ".");
		}


		private void FocusLostSourceRadius(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1000000.0f);

				sphereLight.sourceRadius = value;
			}

			sourceRadiusTextBox.Text = sphereLight.sourceRadius.ToString("F4").Replace(",", ".");
		}


		private void FocusLostX(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, -100000000.0f), 100000000.0f);

				sphereLight.x = value;
			}

			xTextBox.Text = sphereLight.x.ToString("F4").Replace(",", ".");
		}


		private void FocusLostY(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, -100000000.0f), 100000000.0f);

				sphereLight.y = value;
			}

			yTextBox.Text = sphereLight.y.ToString("F4").Replace(",", ".");
		}


		private void FocusLostZ(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, -100000000.0f), 100000000.0f);

				sphereLight.z = value;
			}

			zTextBox.Text = sphereLight.z.ToString("F4").Replace(",", ".");
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
