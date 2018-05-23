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
		private Point rmbClickCoords;

		private ParameterPanel parameterPanel;

		private float scale;


		public GraphView()
        {
            InitializeComponent();

			view.Bind(2);

			scale = 1.0f;
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

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddBlendNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(6, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddRemapNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(7, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddGradientNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(8, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddPerlinNoiseNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(9, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddWorleyNoiseNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(10, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddNormalColorNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(11, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddBlurNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(12, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddDirectionalBlurNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(13, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddMetalReflectanceNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(14, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddHeightToNormalNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(15, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddShapeNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(16, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddTransformNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(17, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddWarpNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(18, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void AddSlopeBlurNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(19, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
		}


		private void OnMouseDown(object sender, MouseButtonEventArgs e)
		{
			Focus();

			if(e.ChangedButton == MouseButton.Left)
			{
				Point lmbClickCoords = e.GetPosition(this);
				CoreDll.GraphViewOnMouseDown((float)(lmbClickCoords.X / ActualWidth), (float)(1.0 - lmbClickCoords.Y / ActualHeight));
				ModifyStackPanel();
			}
		}


		private void OnMouseMove(object sender, MouseEventArgs e)
		{
			Point mouseCoords = e.GetPosition(this);
			CoreDll.GraphViewOnMouseMove((float)(mouseCoords.X / ActualWidth), (float)(1.0 - mouseCoords.Y / ActualHeight));

			HoveredPortDescriptor hoveredPortDescriptor = CoreDll.GraphViewGetHoveredPointDescriptor();
			hoveredPortName.Update(hoveredPortDescriptor);
			Canvas.SetLeft(hoveredPortName, hoveredPortDescriptor.x * ActualWidth - hoveredPortName.ActualWidth * scale / 2.0);
			Canvas.SetTop(hoveredPortName, (1.0 - hoveredPortDescriptor.y) * ActualHeight - hoveredPortName.ActualHeight * scale / 2.0);
		}


		private void OnMouseUp(object sender, MouseButtonEventArgs e)
		{
			if(e.ChangedButton == MouseButton.Right)
			{
				rmbClickCoords = e.GetPosition(this);
			}
			
			Point mouseCoords = e.GetPosition(this);
			CoreDll.GraphViewOnMouseUp((float)(mouseCoords.X / ActualWidth), (float)(1.0 - mouseCoords.Y / ActualHeight));
			if(CoreDll.GraphViewGraphWasChanged() == 1)
			{
				MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
				mainWindow?.OnFunctionGraphChanged();
				mainWindow?.InvalidateSaving();
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


		private void OnMouseWheel(object sender, MouseWheelEventArgs e)
		{
			scale *= ((float) Math.Pow(1.15, e.Delta / 120.0));

			HoveredPortDescriptor hoveredPortDescriptor = CoreDll.GraphViewGetHoveredPointDescriptor();
			hoveredPortName.Update(hoveredPortDescriptor);
			Canvas.SetLeft(hoveredPortName, hoveredPortDescriptor.x * ActualWidth - hoveredPortName.ActualWidth * scale / 2.0);
			Canvas.SetTop(hoveredPortName, (1.0 - hoveredPortDescriptor.y) * ActualHeight - hoveredPortName.ActualHeight * scale / 2.0);
			hoveredPortName.RenderTransform = new ScaleTransform(scale, scale);
		}


		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			if(e.Key == Key.Delete)
			{
				CoreDll.GraphViewRemoveSelected();
				if(CoreDll.GraphViewGraphWasChanged() == 1)
				{
					MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
					mainWindow?.OnFunctionGraphChanged();
					mainWindow?.InvalidateSaving();
				}
			}
		}


		private void ScopeGraph(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewScope();
		}
	}
}