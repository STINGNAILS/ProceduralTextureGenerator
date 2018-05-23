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
	public class Grad
	{
		public float k;
		public float r;
		public float g;
		public float b;


		public Grad()
		{
			k = 0.0f;
			r = 0.0f;
			g = 0.0f;
			b = 0.0f;
		}


		public Grad(float k_, float r_, float g_, float b_)
		{
			k = k_;
			r = r_;
			g = g_;
			b = b_;
		}
	}

	/// <summary>
	/// Логика взаимодействия для GradientSetter.xaml
	/// </summary>
	public partial class GradientSetter : UserControl
	{
		private int gradientSizeIndex;
		private int gradient0Index;

		private int gradientSize;
		private List<Grad> gradients;

		
		public GradientSetter(string name, int gradientSizeIndex_, int gradient0Index_)
		{
			InitializeComponent();

			gradientSetterName.Content = name;

			gradientSizeIndex = gradientSizeIndex_;
			gradient0Index = gradient0Index_;

			gradientSize = CoreDll.GraphViewGetSelectedNodeIntParameter(gradientSizeIndex);
			
			GradientStopCollection gradientStops = new GradientStopCollection(gradientSize);
			gradients = new List<Grad>(gradientSize);

			for(int i = 0; i < gradientSize; i++)
			{
				int offset = gradient0Index + i * 4;

				float k = CoreDll.GraphViewGetSelectedNodeFloatParameter(offset + 0);
				float r = CoreDll.GraphViewGetSelectedNodeFloatParameter(offset + 1);
				float g = CoreDll.GraphViewGetSelectedNodeFloatParameter(offset + 2);
				float b = CoreDll.GraphViewGetSelectedNodeFloatParameter(offset + 3);

				gradients.Add(new Grad(k, r, g, b));
				gradientStops.Add(new GradientStop(Color.FromArgb((byte)255, (byte)((int)(r * 255)), (byte)((int)(g * 255)), (byte)((int)(b * 255))), k));
			}

			LinearGradientBrush brush = new LinearGradientBrush(gradientStops)
			{
				StartPoint = new Point(0.0, 0.0),
				EndPoint = new Point(1.0, 0.0)
			};
			
			gradientRectangle.Fill = brush;
		}


		private void OnClick(object sender, MouseButtonEventArgs e)
		{
			GradientDialog gradientDialog = new GradientDialog(gradientSize, gradients)
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			gradientDialog.ShowDialog();

			gradientSize = gradientDialog.GradientSize;

			CoreDll.GraphViewSetSelectedNodeIntParameter(gradientSizeIndex, gradientSize);

			for(int i = 0; i < gradientSize; i++)
			{
				int offset = gradient0Index + i * 4;

				CoreDll.GraphViewSetSelectedNodeFloatParameter(offset + 0, gradients[i].k);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(offset + 1, gradients[i].r);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(offset + 2, gradients[i].g);
				CoreDll.GraphViewSetSelectedNodeFloatParameter(offset + 3, gradients[i].b);
			}

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
			ParentHelper.GetParentParameterPanel(this)?.Update();
		}
	}
}
