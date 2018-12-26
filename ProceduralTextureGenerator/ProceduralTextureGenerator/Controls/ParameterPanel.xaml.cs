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


		private void AddGradientParameters(int gradientSizeIndex, int gradient0Index)
		{
			parameterStackPanel.Children.Add(new CategoryName("Gradient Parameters"));

			parameterStackPanel.Children.Add(new GradientSetter("Gradient", gradientSizeIndex, gradient0Index));
		}


		private void AddNoiseParameters(int seedIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Noise Parameters"));

			parameterStackPanel.Children.Add(new IntegerParameter("Seed", seedIndex, 0, 32767));
		}


		private void AddPerlinNoiseParameters(int minimalOctaveIndex, int maximalOctaveIndex, int persistenceIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Perlin Noise Parameters"));

			int minimalOctaveMaxValue = CoreDll.GraphViewGetSelectedNodeIntParameter(maximalOctaveIndex);
			int maximalOctaveMinValue = CoreDll.GraphViewGetSelectedNodeIntParameter(minimalOctaveIndex);

			parameterStackPanel.Children.Add(new IntegerParameter("Minimal Octave", minimalOctaveIndex, 1, minimalOctaveMaxValue));

			parameterStackPanel.Children.Add(new IntegerParameter("Maximal Octave", maximalOctaveIndex, maximalOctaveMinValue, 10));

			parameterStackPanel.Children.Add(new FloatParameter("Persistence", persistenceIndex, 0.01f, 5.0f));
		}


		private void AddWorleyNoiseParameters(int sitesNumIndex, int patternTypeIndex, int distanceTypeIndex, int borderWidthIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Worley Noise Parameters"));

			parameterStackPanel.Children.Add(new IntegerParameter("Sites Number", sitesNumIndex, 10, 5000))
				;
			KeyValuePair<string, int>[] patternTypeSelections = new KeyValuePair<string, int>[5]
			{
				new KeyValuePair<string, int>("F1", 1),
				new KeyValuePair<string, int>("F2", 2),
				new KeyValuePair<string, int>("F2 - F1", 3),
				new KeyValuePair<string, int>("Border", 4),
				new KeyValuePair<string, int>("Random Color", 5),
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Pattern Type", patternTypeIndex, patternTypeSelections));

			if(CoreDll.GraphViewGetSelectedNodeIntParameter(patternTypeIndex) == 4)
			{
				parameterStackPanel.Children.Add(new FloatParameter("Border Width", borderWidthIndex, 0.001f, 0.05f));
			}

			KeyValuePair<string, int>[] distanceTypeSelections = new KeyValuePair<string, int>[3]
			{
				new KeyValuePair<string, int>("Manhattan", 1),
				new KeyValuePair<string, int>("Euclidean", 2),
				new KeyValuePair<string, int>("Chebyshev", 3),
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Distance Type", distanceTypeIndex, distanceTypeSelections));
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


		private void AddHeightToNormalParameters(int heightIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Height To Normal Parameters"));

			parameterStackPanel.Children.Add(new FloatParameter("Height", heightIndex, 0.0f, 1.0f));
		}


		private void AddShapeParameters(int shapeIndex, int sidesNumIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Shape Parameters"));

			KeyValuePair<string, int>[] shapeSelections = new KeyValuePair<string, int>[2]
			{
				new KeyValuePair<string, int>("Polygon", 0),
				new KeyValuePair<string, int>("Disc", 1),
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Shape", shapeIndex, shapeSelections));

			parameterStackPanel.Children.Add(new IntegerParameter("Number of sides", sidesNumIndex, 3, 32));
		}


		private void AddTransformParameters(int tilingModeIndex, int xScaleIndex, int yScaleIndex, int rotationIndex, int xTranslateIndex, int yTranslateIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Transform Parameters"));

			KeyValuePair<string, int>[] tilingModeSelections = new KeyValuePair<string, int>[2]
			{
				new KeyValuePair<string, int>("Wrap", 0),
				new KeyValuePair<string, int>("Clamp", 1),
			};
			parameterStackPanel.Children.Add(new SelectionParameter("Tiling Mode", tilingModeIndex, tilingModeSelections));

			parameterStackPanel.Children.Add(new FloatParameter("Stretch X", xScaleIndex, -100.0f, 100.0f));

			parameterStackPanel.Children.Add(new FloatParameter("Stretch Y", yScaleIndex, -100.0f, 100.0f));

			parameterStackPanel.Children.Add(new FloatParameter("Rotation", rotationIndex, 0.0f, 360.0f));

			parameterStackPanel.Children.Add(new FloatParameter("Offset X", xTranslateIndex, -1000000.0f, 1000000.0f));

			parameterStackPanel.Children.Add(new FloatParameter("Offset Y", yTranslateIndex, -1000000.0f, 1000000.0f));
		}


		private void AddWarpParameters(int intensityIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Warp Parameters"));

			parameterStackPanel.Children.Add(new FloatParameter("Intensity", intensityIndex, 0.0f, 100.0f));
		}


		private void AddSlopeBlurParameters(int intensityIndex, int samplesNumIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Slope Blur Parameters"));

			parameterStackPanel.Children.Add(new FloatParameter("Intensity", intensityIndex, 0.0f, 100.0f));

			parameterStackPanel.Children.Add(new IntegerParameter("Number of Samples", samplesNumIndex, 0, 32));
		}


		private void AddLinearGradientParameters(int angleIndex)
		{
			parameterStackPanel.Children.Add(new CategoryName("Linear Gradient Parameters"));

			parameterStackPanel.Children.Add(new FloatParameter("Rotation", angleIndex, 0.0f, 360.0f));
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
				//Height : 4
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
					int gradientSizeIndex = 2;
					int gradient0Index = 0;

					AddNodeName("Gradient");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddGradientParameters(gradientSizeIndex, gradient0Index);

					break;
				}
				//Perlin Noise
				case 9:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int seedIndex = 2;
					int minimalOctaveIndex = 3;
					int maximalOctaveIndex = 4;
					int persistenceIndex = 0;

					AddNodeName("Perlin Noise");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddNoiseParameters(seedIndex);
					AddPerlinNoiseParameters(minimalOctaveIndex, maximalOctaveIndex, persistenceIndex);

					break;
				}
				//Worley Noise
				case 10:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int seedIndex = 2;
					int sitesNumIndex = 3;
					int patternTypeIndex = 4;
					int distanceTypeIndex = 5;
					int borderWidthIndex = 0;

					AddNodeName("Worley Noise");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddNoiseParameters(seedIndex);
					AddWorleyNoiseParameters(sitesNumIndex, patternTypeIndex, distanceTypeIndex, borderWidthIndex);

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
				//Height To Normal
				case 15:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int heightIndex = 0;

					AddNodeName("Height To Normal");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddHeightToNormalParameters(heightIndex);

					break;
				}
				//Shape
				case 16:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int shapeIndex = 2;
					int sidesNumIndex = 3;

					AddNodeName("Shape");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddShapeParameters(shapeIndex, sidesNumIndex);

					break;
				}
				//Transform
				case 17:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int tilingModeIndex = 2;
					int xScaleIndex = 0;
					int yScaleIndex = 1;
					int rotationIndex = 2;
					int xTranslateIndex = 3;
					int yTranslateIndex = 4;

					AddNodeName("Transform");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddTransformParameters(tilingModeIndex, xScaleIndex, yScaleIndex, rotationIndex, xTranslateIndex, yTranslateIndex);

					break;
				}
				//Warp
				case 18:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int intensityIndex = 0;

					AddNodeName("Warp");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddWarpParameters(intensityIndex);

					break;
				}
				//Slope Blur
				case 19:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int samplesNumIndex = 2;
					int intensityIndex = 0;

					AddNodeName("Slope Blur");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddSlopeBlurParameters(intensityIndex, samplesNumIndex);

					break;
				}
				//Linear Gradient
				case 20:
				{
					int textureResolutionIndex = 0;
					int bpcIndex = 1;
					int angleIndex = 0;

					AddNodeName("Linear Gradient");
					AddBaseParameters(textureResolutionIndex, bpcIndex);
					AddLinearGradientParameters(angleIndex);

					break;
				}
			}
		}
	}
}
