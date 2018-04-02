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
	/// Логика взаимодействия для RemapSetter.xaml
	/// </summary>
	public partial class RemapSetter : UserControl
	{
		private int x1Index;
		private int y1Index;
		private int x2Index;
		private int y2Index;
		private int x3Index;
		private int y3Index;

		private int remapMode;

		private int dragIndex;


		public RemapSetter(string name, int remapModeIndex_, int x1Index_, int y1Index_, int x2Index_, int y2Index_, int x3Index_, int y3Index_)
		{
			InitializeComponent();

			remapSetterName.Content = name;
			
			x1Index = x1Index_;
			y1Index = y1Index_;
			x2Index = x2Index_;
			y2Index = y2Index_;
			x3Index = x3Index_;
			y3Index = y3Index_;

			remapMode = CoreDll.GraphViewGetSelectedNodeIntParameter(remapModeIndex_);

			float x1 = CoreDll.GraphViewGetSelectedNodeFloatParameter(x1Index);
			float y1 = CoreDll.GraphViewGetSelectedNodeFloatParameter(y1Index);
			float x2 = CoreDll.GraphViewGetSelectedNodeFloatParameter(x2Index);
			float y2 = CoreDll.GraphViewGetSelectedNodeFloatParameter(y2Index);
			float x3 = CoreDll.GraphViewGetSelectedNodeFloatParameter(x3Index);
			float y3 = CoreDll.GraphViewGetSelectedNodeFloatParameter(y3Index);

			Canvas.SetLeft(x1Drag, 140.0 * x1 - 5.0);
			Canvas.SetTop(x1Drag, 140.0 * (1.0 - y1) - 5.0);

			Canvas.SetLeft(x2Drag, 140.0 * x2 - 5.0);
			Canvas.SetTop(x2Drag, 140.0 * (1.0 - y2) - 5.0);

			Canvas.SetLeft(x3Drag, 140.0 * x3 - 5.0);
			Canvas.SetTop(x3Drag, 140.0 * (1.0 - y3) - 5.0);

			x1Drag.MouseDown += DragMouseDown;
			x2Drag.MouseDown += DragMouseDown;
			x3Drag.MouseDown += DragMouseDown;

			viewCanvas.MouseMove += DragMouseMove;

			viewCanvas.MouseUp += DragMouseUp;

			if(remapMode == 1)
			{
				x2Drag.Visibility = Visibility.Hidden;
			}
			remappedValueLabel.Visibility = Visibility.Hidden;
			valueLabel.Visibility = Visibility.Hidden;

			dragIndex = 0;

			RedrawCurve();
		}


		private void RedrawCurve()
		{
			double x1 = (Canvas.GetLeft(x1Drag) + 5.0) / 140.0;
			double y1 = 1.0 - (Canvas.GetTop(x1Drag) + 5.0) / 140.0;
			double x2 = (Canvas.GetLeft(x2Drag) + 5.0) / 140.0;
			double y2 = 1.0 - (Canvas.GetTop(x2Drag) + 5.0) / 140.0;
			double x3 = (Canvas.GetLeft(x3Drag) + 5.0) / 140.0;
			double y3 = 1.0 - (Canvas.GetTop(x3Drag) + 5.0) / 140.0;

			remappingLine1.X1 = 0.0;
			remappingLine1.Y1 = 140.0 * (1.0 - y1);
			remappingLine1.X2 = 140.0 * x1;
			remappingLine1.Y2 = remappingLine1.Y1;

			remappingLine2.X1 = 140.0;
			remappingLine2.Y1 = 140.0 * (1.0 - y3);
			remappingLine2.X2 = 140.0 * x3;
			remappingLine2.Y2 = remappingLine2.Y1;

			double a = 0.0;
			double b = 0.0;
			double c = 0.0;

			switch(remapMode)
			{
				case 1:
				{
					a = 0.0f;
					b = (y1 - y3) / (x1 - x3);
					c = y1 - b * x1;

					break;
				}
				case 2:
				{
					double z1 = x1 * x1 - x3 * x3;
					double z2 = x1 - x3;
					double z3 = x2 * x2 - x3 * x3;
					double z4 = x2 - x3;
					double w1 = y1 - y3;
					double w2 = y2 - y3;

					a = (w1 - w2 * z2 / z4) / (z1 - z3 * z2 / z4);
					b = (w1 - a * z1) / z2;
					c = y1 - b * x1 - a * x1 * x1;

					break;
				}
			}

			double step = (x3 - x1) / 100;

			remappingCurve.Points.Clear();
			for(int i = 0; i < 99; i++)
			{
				double x = x1 + step * i;
				double y = Math.Min(Math.Max(c + x * (b + x * a), 0.0), 1.0);

				remappingCurve.Points.Add(new Point(140.0 * x, 140.0 * (1.0 - y)));
			}
			remappingCurve.Points.Add(new Point(140.0 * x3, 140.0 * (1.0 - y3)));
		}


		private void DragMouseDown(object sender, MouseButtonEventArgs e)
		{
			if(sender == x1Drag)
			{
				dragIndex = 1;
			}
			else if(sender == x2Drag)
			{
				dragIndex = 2;
			}
			else if(sender == x3Drag)
			{
				dragIndex = 3;
			}

			remappedValueLabel.Visibility = Visibility.Visible;
			valueLabel.Visibility = Visibility.Visible;

			Mouse.Capture(viewCanvas);
		}


		private void DragMouseMove(object sender, MouseEventArgs e)
		{
			switch(dragIndex)
			{
				case 1:
				{
					Point coords = e.GetPosition(viewCanvas);

					double x;
					double y;
					if(remapMode == 1)
					{
						x = Math.Min(Math.Max(coords.X, 0.0), Canvas.GetLeft(x3Drag) + 3.0);
						y = Math.Min(Math.Max(coords.Y, 0.0), 140.0);
					}
					else
					{
						x = Math.Min(Math.Max(coords.X, 0.0), Canvas.GetLeft(x2Drag) + 4.0);
						y = Math.Min(Math.Max(coords.Y, 0.0), 140.0);
					}

					Canvas.SetLeft(x1Drag, x - 5.0);
					Canvas.SetTop(x1Drag, y - 5.0);

					Canvas.SetLeft(valueLabel, x - 20.0);
					Canvas.SetTop(remappedValueLabel, y - 15.0);

					valueLabel.Content = (x / 140.0).ToString("F2").Replace(",", ".");
					remappedValueLabel.Content = (1.0 - y / 140.0).ToString("F2").Replace(",", ".");

					break;
				}
				case 2:
				{
					Point coords = e.GetPosition(viewCanvas);
					
					double x = Math.Min(Math.Max(coords.X, Canvas.GetLeft(x1Drag) + 6), Canvas.GetLeft(x3Drag) + 4);
					double y = Math.Min(Math.Max(coords.Y, 0.0), 140.0);

					Canvas.SetLeft(x2Drag, x - 5.0);
					Canvas.SetTop(x2Drag, y - 5.0);

					Canvas.SetLeft(valueLabel, x - 20.0);
					Canvas.SetTop(remappedValueLabel, y - 15.0);

					valueLabel.Content = (x / 140.0).ToString("F2").Replace(",", ".");
					remappedValueLabel.Content = (1.0 - y / 140.0).ToString("F2").Replace(",", ".");

					break;
				}
				case 3:
				{
					Point coords = e.GetPosition(viewCanvas);

					double x;
					double y;
					if(remapMode == 1)
					{
						x = Math.Min(Math.Max(coords.X, Canvas.GetLeft(x1Drag) + 7.0), 140.0);
						y = Math.Min(Math.Max(coords.Y, 0.0), 140.0);
					}
					else
					{
						x = Math.Min(Math.Max(coords.X, Canvas.GetLeft(x2Drag) + 6.0), 140.0);
						y = Math.Min(Math.Max(coords.Y, 0.0), 140.0);
					}

					Canvas.SetLeft(x3Drag, x - 5.0);
					Canvas.SetTop(x3Drag, y - 5.0);

					Canvas.SetLeft(valueLabel, x - 20.0);
					Canvas.SetTop(remappedValueLabel, y - 15.0);

					valueLabel.Content = (x / 140.0).ToString("F2").Replace(",", ".");
					remappedValueLabel.Content = (1.0 - y / 140.0).ToString("F2").Replace(",", ".");

					break;
				}
				default:
				{
					break;
				}
			}

			RedrawCurve();
		}


		private void DragMouseUp(object sender, MouseButtonEventArgs e)
		{
			dragIndex = 0;
			Mouse.Capture(null);

			remappedValueLabel.Visibility = Visibility.Hidden;
			valueLabel.Visibility = Visibility.Hidden;

			CoreDll.GraphViewSetSelectedNodeFloatParameter(x1Index, (float)((Canvas.GetLeft(x1Drag) + 5.0) / 140.0));
			CoreDll.GraphViewSetSelectedNodeFloatParameter(y1Index, (float)(1.0 - (Canvas.GetTop(x1Drag) + 5.0) / 140.0));
			CoreDll.GraphViewSetSelectedNodeFloatParameter(x3Index, (float)((Canvas.GetLeft(x3Drag) + 5.0) / 140.0));
			CoreDll.GraphViewSetSelectedNodeFloatParameter(y3Index, (float)(1.0 - (Canvas.GetTop(x3Drag) + 5.0) / 140.0));

			if(remapMode == 1)
			{
				CoreDll.GraphViewSetSelectedNodeFloatParameter(x2Index, (float)((Canvas.GetLeft(x1Drag) + Canvas.GetLeft(x3Drag) + 10.0) / 280.0));
				CoreDll.GraphViewSetSelectedNodeFloatParameter(y2Index, (float)(1.0 - (Canvas.GetTop(x2Drag) + 5.0) / 140.0));
			}
			else
			{
				CoreDll.GraphViewSetSelectedNodeFloatParameter(x2Index, (float)((Canvas.GetLeft(x2Drag) + 5.0) / 140.0));
				CoreDll.GraphViewSetSelectedNodeFloatParameter(y2Index, (float)(1.0 - (Canvas.GetTop(x2Drag) + 5.0) / 140.0));
			}

			CoreDll.GraphViewProcess();

			ParentHelper.GetParentParameterPanel(this)?.Update();
		}
	}
}
