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

		ParameterPanel parameterPanel;


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


		public void SetParameterPanel(ParameterPanel parameterPanel_)
		{
			parameterPanel = parameterPanel_;
		}


		public void ModifyStackPanel()
		{
			parameterPanel.Update();
		}


		private void AddUniformColorNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(5, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddBlendNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(6, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddRemapNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(7, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddGradientNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(8, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddPerlinNoiseNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(9, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddWorleyNoiseNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(10, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddNormalColorNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(11, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddBlurNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(12, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddDirectionalBlurNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(13, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
		}


		private void AddMetalReflectanceNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(14, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));
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


		private void OnMouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if(e.ChangedButton == MouseButton.Left)
			{
				Point mouseCoords = e.GetPosition(this);
				CoreDll.GraphViewOnMouseDoubleClick((float)(mouseCoords.X / ActualWidth), (float)(1.0 - mouseCoords.Y / ActualHeight));
			}
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