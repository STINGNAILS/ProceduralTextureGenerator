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
using System.Windows.Controls.Primitives;

namespace ProceduralTextureGenerator
{
    /// <summary>
    /// Interaction logic for GraphView.xaml
    /// </summary>
    public partial class GraphView : UserControl
    {
		private bool isInteracting;
		private Point rmbClickCoords;

		StackPanel parameterStackPanel;

		public GraphView()
        {
            InitializeComponent();

			isInteracting = false;

		}


		private void OnLoaded(object sender, RoutedEventArgs e)
		{
			view.Init(2);
		}


		public void Update(float dt)
		{
			view.Update(dt);
		}


		public void Render()
		{
			view.Render();
		}


		public void SetParameterStackPanel(StackPanel stackPanel)
		{
			parameterStackPanel = stackPanel;
		}

		
		public void ModifyStackPanel()
		{
			parameterStackPanel.Children.Clear();

			int functionIndex = CoreDll.GraphViewGetSelectedNodeFunctionIndex();
			switch(functionIndex)
			{
				//No function
				case -1:
				{
					break;
				}
				//Base - Color
				case 0:
				{
					break;
				}
				//Base - Grayscale
				case 1:
				{
					break;
				}
				//Uniform Color
				case 2:
				{
					int textureTypeIndex = CoreDll.GraphViewGetSelectedNodeIntParameter(1);
					float r = CoreDll.GraphViewGetSelectedNodeFloatParameter(0);
					float g = CoreDll.GraphViewGetSelectedNodeFloatParameter(1);
					float b = CoreDll.GraphViewGetSelectedNodeFloatParameter(2);
					float a = CoreDll.GraphViewGetSelectedNodeFloatParameter(3);

					Label uniformColorLabel = new Label
					{
						Content = "Uniform Color",
						FontFamily = new FontFamily("Segoe UI Semibold"),
						FontSize = 16,
						Height = 32,
						Foreground = Brushes.White,
						Background = Brushes.Gray,
						Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(uniformColorLabel);
					
					Label colorModeLabel = new Label
					{
						Content = "Color Mode",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(colorModeLabel);

					StackPanel colorModeStackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 0.0)
					};

					ToggleButton colorModeColorToggleButton = new ToggleButton
					{
						Content = "Color",
						IsChecked = textureTypeIndex == 1 ? true : false,
						Width = 90,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 2.0, 0.0f)
					};
					colorModeColorToggleButton.Click += ToggleColorModeColor;
					colorModeStackPanel.Children.Add(colorModeColorToggleButton);

					ToggleButton colorModeGrayscaleToggleButton = new ToggleButton
					{
						Content = "Grayscale",
						IsChecked = textureTypeIndex == 0 ? true : false,
						Width = 90,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 2.0, 0.0f)
					};
					colorModeGrayscaleToggleButton.Click += ToggleColorModeGrayscale;
					colorModeStackPanel.Children.Add(colorModeGrayscaleToggleButton);

					parameterStackPanel.Children.Add(colorModeStackPanel);

					Separator colorSeparator = new Separator
					{
						Width = 220,
						HorizontalAlignment = HorizontalAlignment.Center,
						Margin = new Thickness(0.0, 7.0, 0.0, 0.0f)
					};
					parameterStackPanel.Children.Add(colorSeparator);

					Label colorLabel = new Label
					{
						Content = "Color Value",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(colorLabel);

					StackPanel redColorStackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					Label redColorLabel = new Label
					{
						Content = "R",
						FontFamily = new FontFamily("Consolas"),
						FontSize = 14,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(0.0, 0.0, 0.0, 0.0)
					};
					redColorStackPanel.Children.Add(redColorLabel);

					TextBox redColorTextBox = new TextBox
					{
						Text = r.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					redColorTextBox.LostFocus += RedColorTextBoxOnFocusLost;
					redColorTextBox.GotKeyboardFocus += SelectText;
					redColorTextBox.MouseDoubleClick += SelectText;
					redColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					redColorStackPanel.Children.Add(redColorTextBox);

					parameterStackPanel.Children.Add(redColorStackPanel);

					StackPanel greenColorStackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					Label greenColorLabel = new Label
					{
						Content = "G",
						FontFamily = new FontFamily("Consolas"),
						FontSize = 14,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(0.0, 0.0, 0.0, 0.0)
					};
					greenColorStackPanel.Children.Add(greenColorLabel);

					TextBox greenColorTextBox = new TextBox
					{
						Text = g.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					greenColorTextBox.LostFocus += GreenColorTextBoxOnFocusLost;
					greenColorTextBox.GotKeyboardFocus += SelectText;
					greenColorTextBox.MouseDoubleClick += SelectText;
					greenColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					greenColorStackPanel.Children.Add(greenColorTextBox);

					parameterStackPanel.Children.Add(greenColorStackPanel);

					StackPanel blueColorStackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					Label blueColorLabel = new Label
					{
						Content = "B",
						FontFamily = new FontFamily("Consolas"),
						FontSize = 14,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(0.0, 0.0, 0.0, 0.0)
					};
					blueColorStackPanel.Children.Add(blueColorLabel);

					TextBox blueColorTextBox = new TextBox
					{
						Text = b.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					blueColorTextBox.LostFocus += BlueColorTextBoxOnFocusLost;
					blueColorTextBox.GotKeyboardFocus += SelectText;
					blueColorTextBox.MouseDoubleClick += SelectText;
					blueColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					blueColorStackPanel.Children.Add(blueColorTextBox);

					parameterStackPanel.Children.Add(blueColorStackPanel);

					break;
				}
				//Blend
				case 3:
				{
					int blendModeIndex = CoreDll.GraphViewGetSelectedNodeIntParameter(1);
					float k = CoreDll.GraphViewGetSelectedNodeFloatParameter(0);

					Label blendLabel = new Label
					{
						Content = "Blend",
						FontFamily = new FontFamily("Segoe UI Semibold"),
						FontSize = 16,
						Height = 32,
						Foreground = Brushes.White,
						Background = Brushes.Gray,
						Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(blendLabel);

					Label blendModeLabel = new Label
					{
						Content = "Blend Mode",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(blendModeLabel);

					ComboBox blendModeComboBox = new ComboBox
					{
						HorizontalContentAlignment = HorizontalAlignment.Center,
						Width = 95,
						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(42.0, 0.0, 0.0, 0.0)
					};
					blendModeComboBox.Items.Add("Merge");
					blendModeComboBox.Items.Add("Add");
					blendModeComboBox.Items.Add("Sub");
					blendModeComboBox.Items.Add("Multiply");
					blendModeComboBox.Items.Add("Min");
					blendModeComboBox.Items.Add("Max");
					blendModeComboBox.SelectedIndex = blendModeIndex;
					blendModeComboBox.SelectionChanged += BlendModeComboBoxSelectionChanged;
					parameterStackPanel.Children.Add(blendModeComboBox);

					Separator kSeparator = new Separator
					{
						Width = 220,
						HorizontalAlignment = HorizontalAlignment.Center,
						Margin = new Thickness(0.0, 7.0, 0.0, 0.0f)
					};
					parameterStackPanel.Children.Add(kSeparator);

					Label kLabel = new Label
					{
						Content = "Blend Coefficient",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(kLabel);

					TextBox kTextBox = new TextBox
					{
						Text = k.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(42.0, 0.0, 0.0, 0.0)
					};
					kTextBox.LostFocus += BlendCoefficientTextBoxOnFocusLost;
					kTextBox.GotKeyboardFocus += SelectText;
					kTextBox.MouseDoubleClick += SelectText;
					kTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					parameterStackPanel.Children.Add(kTextBox);

					break;
				}
				//Levels
				case 4:
				{
					float x1 = CoreDll.GraphViewGetSelectedNodeFloatParameter(0);
					float x2 = CoreDll.GraphViewGetSelectedNodeFloatParameter(1);
					float x3 = CoreDll.GraphViewGetSelectedNodeFloatParameter(2);
					float x4 = CoreDll.GraphViewGetSelectedNodeFloatParameter(3);
					float x5 = CoreDll.GraphViewGetSelectedNodeFloatParameter(4);

					Label levelsLabel = new Label
					{
						Content = "Levels",
						FontFamily = new FontFamily("Segoe UI Semibold"),
						FontSize = 16,
						Height = 32,
						Foreground = Brushes.White,
						Background = Brushes.Gray,
						Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(levelsLabel);

					Label quadraticInterpolationLabel = new Label
					{
						Content = "Quadratic Interpolation",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(quadraticInterpolationLabel);

					StackPanel x1StackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					TextBox x1TextBox = new TextBox
					{
						Text = x1.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x1TextBox.LostFocus += X1TextBoxOnFocusLost;
					x1TextBox.GotKeyboardFocus += SelectText;
					x1TextBox.MouseDoubleClick += SelectText;
					x1TextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					x1StackPanel.Children.Add(x1TextBox);
					
					Rectangle x1Rectangle = new Rectangle
					{
						Width = 24,
						Height = 24,
						Fill = Brushes.Black,
						Stroke = Brushes.Silver,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x1StackPanel.Children.Add(x1Rectangle);

					parameterStackPanel.Children.Add(x1StackPanel);

					StackPanel x2StackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					TextBox x2TextBox = new TextBox
					{
						Text = x2.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x2TextBox.LostFocus += X2TextBoxOnFocusLost;
					x2TextBox.GotKeyboardFocus += SelectText;
					x2TextBox.MouseDoubleClick += SelectText;
					x2TextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					x2StackPanel.Children.Add(x2TextBox);

					Rectangle x2Rectangle = new Rectangle
					{
						Width = 24,
						Height = 24,
						Fill = Brushes.Gray,
						Stroke = Brushes.Silver,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x2StackPanel.Children.Add(x2Rectangle);

					parameterStackPanel.Children.Add(x2StackPanel);

					StackPanel x3StackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					TextBox x3TextBox = new TextBox
					{
						Text = x3.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x3TextBox.LostFocus += X3TextBoxOnFocusLost;
					x3TextBox.GotKeyboardFocus += SelectText;
					x3TextBox.MouseDoubleClick += SelectText;
					x3TextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					x3StackPanel.Children.Add(x3TextBox);

					Rectangle x3Rectangle = new Rectangle
					{
						Width = 24,
						Height = 24,
						Fill = Brushes.White,
						Stroke = Brushes.Silver,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x3StackPanel.Children.Add(x3Rectangle);

					parameterStackPanel.Children.Add(x3StackPanel);

					Separator linearReinterpolationSeparator = new Separator
					{
						Width = 220,
						HorizontalAlignment = HorizontalAlignment.Center,
						Margin = new Thickness(0.0, 7.0, 0.0, 0.0f)
					};
					parameterStackPanel.Children.Add(linearReinterpolationSeparator);

					Label linearReinterpolationLabel = new Label
					{
						Content = "Linear Reinterpolation",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(linearReinterpolationLabel);

					StackPanel x4StackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					Rectangle x4Rectangle = new Rectangle
					{
						Width = 24,
						Height = 24,
						Fill = Brushes.Black,
						Stroke = Brushes.Silver,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x4StackPanel.Children.Add(x4Rectangle);

					TextBox x4TextBox = new TextBox
					{
						Text = x4.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x4TextBox.LostFocus += X4TextBoxOnFocusLost;
					x4TextBox.GotKeyboardFocus += SelectText;
					x4TextBox.MouseDoubleClick += SelectText;
					x4TextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					x4StackPanel.Children.Add(x4TextBox);

					parameterStackPanel.Children.Add(x4StackPanel);

					StackPanel x5StackPanel = new StackPanel
					{
						Orientation = Orientation.Horizontal,

						Height = 24,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(40.0, 0.0, 0.0, 2.0)
					};

					Rectangle x5Rectangle = new Rectangle
					{
						Width = 24,
						Height = 24,
						Fill = Brushes.White,
						Stroke = Brushes.Silver,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x5StackPanel.Children.Add(x5Rectangle);

					TextBox x5TextBox = new TextBox
					{
						Text = x5.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(2.0, 0.0, 0.0, 0.0)
					};
					x5TextBox.LostFocus += X5TextBoxOnFocusLost;
					x5TextBox.GotKeyboardFocus += SelectText;
					x5TextBox.MouseDoubleClick += SelectText;
					x5TextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					x5StackPanel.Children.Add(x5TextBox);

					parameterStackPanel.Children.Add(x5StackPanel);

					break;
				}
				//Gradient
				case 5:
				{
					Label gradientLabel = new Label
					{
						Content = "Gradient",
						FontFamily = new FontFamily("Segoe UI Semibold"),
						FontSize = 16,
						Height = 32,
						Foreground = Brushes.White,
						Background = Brushes.Gray,
						Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(gradientLabel);

					break;
				}
				//Perlin Noise
				case 6:
				{
					int octaves = CoreDll.GraphViewGetSelectedNodeIntParameter(1);
					int gridStartingSize = CoreDll.GraphViewGetSelectedNodeIntParameter(2);
					float persistence = CoreDll.GraphViewGetSelectedNodeFloatParameter(0);

					Label perlinNoiseLabel = new Label
					{
						Content = "Perlin Noise",
						FontFamily = new FontFamily("Segoe UI Semibold"),
						FontSize = 16,
						Height = 32,
						Foreground = Brushes.White,
						Background = Brushes.Gray,
						Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(perlinNoiseLabel);

					Label octavesLabel = new Label
					{
						Content = "Octaves",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(octavesLabel);
					
					TextBox octavesTextBox = new TextBox
					{
						Text = octaves.ToString(),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(42.0, 0.0, 0.0, 0.0)
					};
					octavesTextBox.LostFocus += OctavesTextBoxOnFocusLost;
					octavesTextBox.GotKeyboardFocus += SelectText;
					octavesTextBox.MouseDoubleClick += SelectText;
					octavesTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					parameterStackPanel.Children.Add(octavesTextBox);

					Separator gridStartingSizeSeparator = new Separator
					{
						Width = 220,
						HorizontalAlignment = HorizontalAlignment.Center,
						Margin = new Thickness(0.0, 7.0, 0.0, 0.0f)
					};
					parameterStackPanel.Children.Add(gridStartingSizeSeparator);

					Label gridStartingSizeLabel = new Label
					{
						Content = "Grid Starting Size",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(gridStartingSizeLabel);

					TextBox gridStartingSizeTextBox = new TextBox
					{
						Text = gridStartingSize.ToString(),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(42.0, 0.0, 0.0, 0.0)
					};
					gridStartingSizeTextBox.LostFocus += GridStartingSizeTextBoxOnFocusLost;
					gridStartingSizeTextBox.GotKeyboardFocus += SelectText;
					gridStartingSizeTextBox.MouseDoubleClick += SelectText;
					gridStartingSizeTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					parameterStackPanel.Children.Add(gridStartingSizeTextBox);

					Separator persistenceSeparator = new Separator
					{
						Width = 220,
						HorizontalAlignment = HorizontalAlignment.Center,
						Margin = new Thickness(0.0, 7.0, 0.0, 0.0f)
					};
					parameterStackPanel.Children.Add(persistenceSeparator);

					Label persistenceLabel = new Label
					{
						Content = "Persistence",
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Foreground = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(30.0, -2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(persistenceLabel);

					TextBox persistenceTextBox = new TextBox
					{
						Text = persistence.ToString("F4").Replace(",", "."),
						TextAlignment = TextAlignment.Center,
						FontFamily = new FontFamily("Segoe UI"),
						FontSize = 16,
						Width = 80,
						Foreground = Brushes.Silver,
						Background = new SolidColorBrush(Color.FromArgb((byte)255, (byte)32, (byte)32, (byte)32)),
						BorderBrush = Brushes.Silver,
						HorizontalAlignment = HorizontalAlignment.Left,
						Margin = new Thickness(42.0, 0.0, 0.0, 0.0)
					};
					persistenceTextBox.LostFocus += PersistenceTextBoxOnFocusLost;
					persistenceTextBox.GotKeyboardFocus += SelectText;
					persistenceTextBox.MouseDoubleClick += SelectText;
					persistenceTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
					parameterStackPanel.Children.Add(persistenceTextBox);

					break;
				}
				//Normal Color
				case 7:
				{
					Label normalColorLabel = new Label
					{
						Content = "Normal Color",
						FontFamily = new FontFamily("Segoe UI Semibold"),
						FontSize = 16,
						Height = 32,
						Foreground = Brushes.White,
						Background = Brushes.Gray,
						Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
					};
					parameterStackPanel.Children.Add(normalColorLabel);

					break;
				}
			}
		}


		private void AddUniformColorNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(2, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddBlendNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(3, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddLevelsNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(4, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddGradientNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(5, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddPerlinNoiseNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(6, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddNormalColorNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(7, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void OnMouseDown(object sender, MouseButtonEventArgs e)
		{
			Focus();

			if(e.ChangedButton == MouseButton.Left)
			{
				Point lmbClickCoords = e.GetPosition(this);
				CoreDll.GraphViewOnMouseDown((float)(lmbClickCoords.X / ActualWidth), (float)(1.0 - lmbClickCoords.Y / ActualHeight));
				ModifyStackPanel();
				isInteracting = true;
			}
		}


		private void OnMouseMove(object sender, MouseEventArgs e)
		{
			if(isInteracting)
			{
				Point mouseCoords = e.GetPosition(this);
				CoreDll.GraphViewOnMouseMove((float)(mouseCoords.X / ActualWidth), (float)(1.0 - mouseCoords.Y / ActualHeight));
			}
		}


		private void OnMouseUp(object sender, MouseButtonEventArgs e)
		{
			if(e.ChangedButton == MouseButton.Right)
			{
				rmbClickCoords = e.GetPosition(this);
			}

			if(isInteracting)
			{
				Point mouseCoords = e.GetPosition(this);
				CoreDll.GraphViewOnMouseUp((float)(mouseCoords.X / ActualWidth), (float)(1.0 - mouseCoords.Y / ActualHeight));
				isInteracting = false;
			}
		}


		private void ToggleColorModeColor(object sender, RoutedEventArgs e)
		{
			((ToggleButton)((StackPanel)parameterStackPanel.Children[2]).Children[0]).IsChecked = true;
			((ToggleButton)((StackPanel)parameterStackPanel.Children[2]).Children[1]).IsChecked = false;

			CoreDll.GraphViewSetSelectedNodeIntParameter(1, 1);
			ModifyStackPanel();
		}


		private void ToggleColorModeGrayscale(object sender, RoutedEventArgs e)
		{
			((ToggleButton)((StackPanel)parameterStackPanel.Children[2]).Children[0]).IsChecked = false;
			((ToggleButton)((StackPanel)parameterStackPanel.Children[2]).Children[1]).IsChecked = true;

			CoreDll.GraphViewSetSelectedNodeIntParameter(1, 0);
			ModifyStackPanel();
		}


		private void RedColorTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(0, value);
			}
			ModifyStackPanel();
		}


		private void GreenColorTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(1, value);
			}
			ModifyStackPanel();
		}


		private void BlueColorTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(2, value);
			}
			ModifyStackPanel();
		}


		private void BlendModeComboBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			switch((string)((ComboBox)sender).SelectedItem)
			{
				case "Merge":
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(1, 0);
					break;
				}
				case "Add":
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(1, 1);
					break;
				}
				case "Sub":
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(1, 2);
					break;
				}
				case "Multiply":
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(1, 3);
					break;
				}
				case "Min":
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(1, 4);
					break;
				}
				case "Max":
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(1, 5);
					break;
				}
				default:
				{
					break;
				}
			}
			ModifyStackPanel();
		}


		private void BlendCoefficientTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(0, value);
			}
			ModifyStackPanel();
		}


		private void X1TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(0, value);
			}
			ModifyStackPanel();
		}


		private void X2TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(1, value);
			}
			ModifyStackPanel();
		}


		private void X3TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(2, value);
			}
			ModifyStackPanel();
		}


		private void X4TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(3, value);
			}
			ModifyStackPanel();
		}


		private void X5TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(4, value);
			}
			ModifyStackPanel();
		}


		private void OctavesTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			int value;
			if(int.TryParse(((TextBox)sender).Text, out value))
			{
				value = Math.Min(Math.Max(value, 1), 12);
				CoreDll.GraphViewSetSelectedNodeIntParameter(1, value);
			}
			ModifyStackPanel();
		}


		private void GridStartingSizeTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			int value;
			if(int.TryParse(((TextBox)sender).Text, out value))
			{
				value = Math.Min(Math.Max(value, 1), 6);
				CoreDll.GraphViewSetSelectedNodeIntParameter(2, value);
			}
			ModifyStackPanel();
		}


		private void PersistenceTextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Max(value, 0.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(0, value);
			}
			ModifyStackPanel();
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


		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			if(e.Key == Key.Delete)
			{
				CoreDll.GraphViewRemoveSelected();
			}
		}
	}
}