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
    /// Логика взаимодействия для HoveredPortName.xaml
    /// </summary>
    public partial class HoveredPortName : UserControl
    {
        public HoveredPortName()
        {
            InitializeComponent();
        }

		public void Update(HoveredPortDescriptor hoveredPortDescriptor)
		{
			if(hoveredPortDescriptor.hoveredPortFunctionIndex == -1)
			{
				Visibility = Visibility.Collapsed;
				return;
			}
			else
			{
				Visibility = Visibility.Visible;
			}

			switch(hoveredPortDescriptor.hoveredPortFunctionIndex)
			{
				//Base Color
				case 0:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Metallic
				case 1:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Roughness
				case 2:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Normal
				case 3:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Blend
				case 6:
				{
					switch(hoveredPortDescriptor.hoveredPortIndex)
					{
						case 0:
						{
							hoveredPortNameTextBlock.Text = "Foreground";
							break;
						}
						case 1:
						{
							hoveredPortNameTextBlock.Text = "Background";
							break;
						}
						case 2:
						{
							hoveredPortNameTextBlock.Text = "Blend Mask";
							break;
						}
					}

					break;
				}
				//Remap
				case 7:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Gradient
				case 8:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Blur
				case 12:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Directional Blur
				case 13:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Height To Normal
				case 15:
				{
					hoveredPortNameTextBlock.Text = "Height";
					break;
				}
				//Transform
				case 17:
				{
					hoveredPortNameTextBlock.Text = "Input";
					break;
				}
				//Warp
				case 18:
				{
					switch(hoveredPortDescriptor.hoveredPortIndex)
					{
						case 0:
						{
							hoveredPortNameTextBlock.Text = "Input";
							break;
						}
						case 1:
						{
							hoveredPortNameTextBlock.Text = "Slope";
							break;
						}
					}

					break;
				}
				//Slope Blur
				case 19:
				{
					switch(hoveredPortDescriptor.hoveredPortIndex)
					{
						case 0:
						{
							hoveredPortNameTextBlock.Text = "Input";
							break;
						}
						case 1:
						{
							hoveredPortNameTextBlock.Text = "Slope";
							break;
						}
					}

					break;
				}
				//Directional Warp
				case 21:
				{
					switch(hoveredPortDescriptor.hoveredPortIndex)
					{
						case 0:
						{
							hoveredPortNameTextBlock.Text = "Input";
							break;
						}
						case 1:
						{
							hoveredPortNameTextBlock.Text = "Intensity";
							break;
						}
					}

					break;
				}
				default:
				{
					break;
				}
			}
		}
    }
}
