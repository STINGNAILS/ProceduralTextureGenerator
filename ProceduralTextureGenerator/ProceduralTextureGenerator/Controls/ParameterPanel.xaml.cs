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
	public partial class ParameterPanel : UserControl
	{
		public ParameterPanel()
		{
			InitializeComponent();
		}


		private void AddNodeName(string nodeName)
		{
			parameterStackPanel.Children.Add(new NodeName(nodeName));
		}


		private void AddBaseParameters(int textureResolutionIndex, int bpcIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Base Parameters"));

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
			parameterStackPanel.Children.Add(new CategoryName("Color Parameters"));

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
			parameterStackPanel.Children.Add(new CategoryName("Blend Parameters"));

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
			parameterStackPanel.Children.Add(new CategoryName("Remap Parameters"));

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
			parameterStackPanel.Children.Add(new CategoryName("Noise Parameters"));

			parameterStackPanel.Children.Add(new IntegerParameter("Octaves", octavesIndex, 1, 12));

			parameterStackPanel.Children.Add(new FloatParameter("Persistence", persistenceindex, 0.0f, 5.0f));
		}


		private void AddPerlinNoiseParameters(int gridStartingSizeIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Perlin Noise Parameters"));

			parameterStackPanel.Children.Add(new IntegerParameter("Grid Starting Size", gridStartingSizeIndex, 1, 6));
		}


		private void AddWorleyNoiseParameters(int sitesStartingNumIndex, int distanceTypeIndex, int exponentIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Worley Noise Parameters"));

			parameterStackPanel.Children.Add(new IntegerParameter("Sites Starting Number", sitesStartingNumIndex, 10, 200));

			KeyValuePair<string, int>[] distanceTypeSelections = new KeyValuePair<string, int>[2]
			{
				new KeyValuePair<string, int>("Distance to the closest site", 1),
				new KeyValuePair<string, int>("Difference between distances to 2 closest sites", 2)
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Distance Type", distanceTypeIndex, distanceTypeSelections));

			parameterStackPanel.Children.Add(new FloatParameter("Minkowski Exponent", exponentIndex, 0.01f, 10.0f));
		}


		private void AddBlurParameters(int intensityIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Blur Parameters"));

			parameterStackPanel.Children.Add(new FloatParameter("Intensity", intensityIndex, 0.0f, 100.0f));
		}



		private void AddDirectionalBlurParameters(int intensityIndex, int angleIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Directional Blur Parameters"));

			parameterStackPanel.Children.Add(new FloatParameter("Intensity", intensityIndex, 0.0f, 100.0f));

			parameterStackPanel.Children.Add(new FloatParameter("Angle", angleIndex, 0.0f, 360.0f));
		}


		private void AddMetalReflectanceParameters(int metalIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Metal Reflectance Parameters"));

			KeyValuePair<string, int>[] metalSelections = new KeyValuePair<string, int>[10]
			{
				new KeyValuePair<string, int>("Iron", 0),
				new KeyValuePair<string, int>("Titanium", 1),
				new KeyValuePair<string, int>("Aluminium", 2),
				new KeyValuePair<string, int>("Nickel", 3),
				new KeyValuePair<string, int>("Silver", 4),
				new KeyValuePair<string, int>("Gold", 5),
				new KeyValuePair<string, int>("Platinum", 6),
				new KeyValuePair<string, int>("Copper", 7),
				new KeyValuePair<string, int>("Chromium", 8),
				new KeyValuePair<string, int>("Cobalt", 9)
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Metal", metalIndex, metalSelections));
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
				//Metal Reflectance
				case 14:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int metalIndex = 2;

					AddNodeName("Metal Reflectance");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddMetalReflectanceParameters(metalIndex);

					break;
				}
			}
		}
	}
}
