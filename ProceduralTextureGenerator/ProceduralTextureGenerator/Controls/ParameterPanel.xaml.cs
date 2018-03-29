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
	/// Логика взаимодействия для ParameterPanel.xaml
	/// </summary>

	public struct FloatTextBoxData
	{
		public int index;
		public float minValue;
		public float maxValue;


		public FloatTextBoxData(int index_, float minValue_, float maxValue_)
		{
			index = index_;
			minValue = minValue_;
			maxValue = maxValue_;
		}
	}


	public struct IntTextBoxData
	{
		public int index;
		public int minValue;
		public int maxValue;


		public IntTextBoxData(int index_, int minValue_, int maxValue_)
		{
			index = index_;
			minValue = minValue_;
			maxValue = maxValue_;
		}
	}


	public partial class ParameterPanel : UserControl
	{
		public ParameterPanel()
		{
			InitializeComponent();
		}


		private void AddNodeName(string nodeName)
		{
			Label baseColorLabel = new Label
			{
				Content = nodeName,
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 32,
				Foreground = Brushes.White,
				Background = Brushes.Gray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(baseColorLabel);
		}


		private void AddCategoryName(string categoryName)
		{
			Label label = new Label
			{
				Content = categoryName,
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(label);
		}


		private void AddBaseParameters(int textureResolutionIndex, int bpcIndex)
		{
			Label baseColorLabel = new Label
			{
				Content = "Base Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(baseColorLabel);

			KeyValuePair<string, int>[] resolutionSelections = new KeyValuePair<string, int>[9]
			{
				new KeyValuePair<string, int>("16x16 px", 16),
				new KeyValuePair<string, int>("32x32 px", 32),
				new KeyValuePair<string, int>("64x64 px", 64),
				new KeyValuePair<string, int>("128x128 px", 128),
				new KeyValuePair<string, int>("256x256 px", 256),
				new KeyValuePair<string, int>("512x512 px", 512),
				new KeyValuePair<string, int>("1024x1024 px", 1024),
				new KeyValuePair<string, int>("2048x2048 px", 2048),
				new KeyValuePair<string, int>("4096x4096 px", 4096)
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Resolution", textureResolutionIndex, resolutionSelections));

			KeyValuePair<string, int>[] formatSelections = new KeyValuePair<string, int>[3]
			{
				new KeyValuePair<string, int>("8 bits per channel", 8),
				new KeyValuePair<string, int>("16 bits per channel", 16),
				new KeyValuePair<string, int>("32 bits per channel", 32)
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Format", bpcIndex, formatSelections));
		}


		private void AddColorParameters(int colorModeIndex, int rIndex, int gIndex, int bIndex)
		{
			Label colorParametersLabel = new Label
			{
				Content = "Color Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(colorParametersLabel);

			KeyValuePair<string, int>[] colorModeSelections = new KeyValuePair<string, int>[2]
			{
				new KeyValuePair<string, int>("Color", 1),
				new KeyValuePair<string, int>("Grayscale", 0),
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Color Mode", colorModeIndex, colorModeSelections));

			if(CoreDll.GraphViewGetSelectedNodeIntParameter(colorModeIndex) == 1)
			{
				parameterStackPanel.Children.Add(new ColorSetter("Color Value", rIndex, gIndex, bIndex));
			}
			else
			{
				parameterStackPanel.Children.Add(new GrayscaleSetter("Grayscale Value", rIndex));
			}
		}


		private void AddBlendParameters(int blendModeIndex, int kIndex)
		{
			Label blendParametersLabel = new Label
			{
				Content = "Blend Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(blendParametersLabel);

			KeyValuePair<string, int>[] blendModeSelections = new KeyValuePair<string, int>[6]
			{
				new KeyValuePair<string, int>("Merge", 0),
				new KeyValuePair<string, int>("Add", 1),
				new KeyValuePair<string, int>("Sub", 2),
				new KeyValuePair<string, int>("Multiply", 3),
				new KeyValuePair<string, int>("Min", 4),
				new KeyValuePair<string, int>("Max", 5)
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Blend Mode", blendModeIndex, blendModeSelections));

			parameterStackPanel.Children.Add(new FloatParameter("Blend Coefficient", kIndex, 0.0f, 1.0f));
		}


		private void AddRemapParameters(int remapModeIndex, int x1Index, int y1Index, int x2Index, int y2Index, int x3Index, int y3Index)
		{
			Label levelsParametersLabel = new Label
			{
				Content = "Remap Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(levelsParametersLabel);

			KeyValuePair<string, int>[] remapModeSelections = new KeyValuePair<string, int>[2]
			{
				new KeyValuePair<string, int>("Linear", 1),
				new KeyValuePair<string, int>("Quadratic", 2)
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Remap Mode", remapModeIndex, remapModeSelections));

			parameterStackPanel.Children.Add(new RemapSetter("Remap Curve", remapModeIndex, x1Index, y1Index, x2Index, y2Index, x3Index, y3Index));
		}


		private void AddNoiseParameters(int octavesIndex, int persistenceindex)
		{
			Label noiseParametersLabel = new Label
			{
				Content = "Noise Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(noiseParametersLabel);

			parameterStackPanel.Children.Add(new IntegerParameter("Octaves", octavesIndex, 1, 12));

			parameterStackPanel.Children.Add(new FloatParameter("Persistence", persistenceindex, 0.0f, 5.0f));
		}


		private void AddPerlinNoiseParameters(int gridStartingSizeIndex)
		{
			Label perlinNoiseParametersLabel = new Label
			{
				Content = "Perlin Noise Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(perlinNoiseParametersLabel);

			parameterStackPanel.Children.Add(new IntegerParameter("Grid Starting Size", gridStartingSizeIndex, 1, 6));
		}


		private void AddWorleyNoiseParameters(int sitesStartingNumIndex, int distanceTypeIndex, int exponentIndex)
		{
			Label worleyNoiseParametersLabel = new Label
			{
				Content = "Worley Noise Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(worleyNoiseParametersLabel);

			parameterStackPanel.Children.Add(new IntegerParameter("Sites Starting Number", sitesStartingNumIndex, 10, 200));

			KeyValuePair<string, int>[] distanceTypeSelections = new KeyValuePair<string, int>[2]
			{
				new KeyValuePair<string, int>("Distance to the closest site", 1),
				new KeyValuePair<string, int>("Difference between distances to 2 closest sites", 2),
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Distance Type", distanceTypeIndex, distanceTypeSelections));

			parameterStackPanel.Children.Add(new FloatParameter("Minkowski Exponent", exponentIndex, 0.01f, 10.0f));
		}


		private void AddBlurParameters(int intensityIndex)
		{
			Label blurParametersLabel = new Label
			{
				Content = "Blur Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(blurParametersLabel);

			parameterStackPanel.Children.Add(new FloatParameter("Intensity", intensityIndex, 0.0f, 100.0f));
		}



		private void AddDirectionalBlurParameters(int intensityIndex, int angleIndex)
		{
			Label directionalBlurParametersLabel = new Label
			{
				Content = "Directional Blur Parameters",
				FontFamily = new FontFamily("Segoe UI Semibold"),
				FontSize = 16,
				Height = 30,
				Foreground = Brushes.White,
				Background = Brushes.DarkGray,
				Margin = new Thickness(2.0, 2.0, 0.0, 0.0)
			};
			parameterStackPanel.Children.Add(directionalBlurParametersLabel);

			parameterStackPanel.Children.Add(new FloatParameter("Intensity", intensityIndex, 0.0f, 100.0f));

			parameterStackPanel.Children.Add(new FloatParameter("Angle", angleIndex, 0.0f, 360.0f));
		}


		public void Update()
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
				//Base Color
				case 0:
				{
					int textureResolutionIndex =0;
					int bpcIndex = 1;

					AddNodeName("Base Color");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					
					break;
				}
				//Metallic
				case 1:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;

					AddNodeName("Metallic");
					AddBaseParameters(textureResolutionIndex, bpcIndex);

					break;
				}
				//Roughness
				case 2:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;

					AddNodeName("Roughness");
					AddBaseParameters(textureResolutionIndex, bpcIndex);

					break;
				}
				//Normal
				case 3:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;

					AddNodeName("Normal");
					AddBaseParameters(textureResolutionIndex, bpcIndex);

					break;
				}
				//Uniform Color
				case 5:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int colorModeIndex = 2;
					int rIndex = 0;
					int gIndex = 1;
					int bIndex = 2;

					AddNodeName("Uniform Color");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddColorParameters(colorModeIndex, rIndex, gIndex, bIndex);

					break;
				}
				//Blend
				case 6:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int blendModeIndex = 2;
					int kIndex = 0;

					AddNodeName("Blend");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddBlendParameters(blendModeIndex, kIndex);

					break;
				}
				//Remap
				case 7:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int remapModeIndex = 2;
					int x1Index = 0;
					int y1Index = 1;
					int x2Index = 2;
					int y2Index = 3;
					int x3Index = 4;
					int y3Index = 5;

					AddNodeName("Remap");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddRemapParameters(remapModeIndex, x1Index, y1Index, x2Index, y2Index, x3Index, y3Index);

					break;
				}
				//Gradient
				case 8:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;

					AddNodeName("Gradient");
					AddBaseParameters(textureResolutionIndex, bpcIndex);

					break;
				}
				//Perlin Noise
				case 9:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int octavesIndex = 2;
					int gridStartingSizeIndex = 3;
					int persistenceIndex = 0;

					AddNodeName("Perlin Noise");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddNoiseParameters(octavesIndex, persistenceIndex);
					AddPerlinNoiseParameters(gridStartingSizeIndex);

					break;
				}
				//Worley Noise
				case 10:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int octavesIndex = 2;
					int sitesStartingNumIndex = 3;
					int distanceTypeIndex = 4;
					int persistenceIndex = 0;
					int exponentIndex = 1;

					AddNodeName("Worley Noise");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddNoiseParameters(octavesIndex, persistenceIndex);
					AddWorleyNoiseParameters(sitesStartingNumIndex, distanceTypeIndex, exponentIndex);

					break;
				}
				//Normal Color
				case 11:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;

					AddNodeName("Normal Color");
					AddBaseParameters(textureResolutionIndex, bpcIndex);

					break;
				}
				//Blur
				case 12:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int intensityIndex = 0;

					AddNodeName("Blur");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddBlurParameters(intensityIndex);

					break;
				}
				//Directional Blur
				case 13:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int intensityIndex = 0;
					int angleIndex = 1;

					AddNodeName("Directional Blur");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddDirectionalBlurParameters(intensityIndex, angleIndex);

					break;
				}
			}
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


		private void X1TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(0, value);
			}
			Update();
		}


		private void X2TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(1, value);
			}
			Update();
		}


		private void X3TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(2, value);
			}
			Update();
		}


		private void X4TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(3, value);
			}
			Update();
		}


		private void X5TextBoxOnFocusLost(object sender, RoutedEventArgs e)
		{
			float value;
			if(float.TryParse(((TextBox)sender).Text.Replace(".", ","), out value))
			{
				value = Math.Min(Math.Max(value, 0.0f), 1.0f);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(4, value);
			}
			Update();
		}
	}
}
