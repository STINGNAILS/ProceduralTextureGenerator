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


		public GraphView()
        {
            InitializeComponent();

			view.Bind(2);
		}


		public void Update(float dt)
		{
			view.Update(dt);

			HoveredPortDescriptor hoveredPortDescriptor = CoreDll.GraphViewGetHoveredPointDescriptor();
			float scale = CoreDll.GraphViewGetScale();
			hoveredPortName.Update(hoveredPortDescriptor);
			Canvas.SetLeft(hoveredPortName, hoveredPortDescriptor.x * ActualWidth - hoveredPortName.ActualWidth * scale / 2.0);
			Canvas.SetTop(hoveredPortName, (1.0 - hoveredPortDescriptor.y) * ActualHeight - hoveredPortName.ActualHeight * scale / 2.0);
			hoveredPortName.RenderTransform = new ScaleTransform(scale, scale);
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


		private void AddLinearGradientNode(object sender, RoutedEventArgs e)
		{
			CoreDll.GraphViewAddNode(20, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

			MainWindow mainWindow = ParentHelper.GetParentMainWindow(this);
			mainWindow?.OnFunctionGraphChanged();
			mainWindow?.InvalidateSaving();
        }


        private void AddDirectionalWarpNode(object sender, RoutedEventArgs e)
        {
            CoreDll.GraphViewAddNode(21, (float)(rmbClickCoords.X / ActualWidth), (float)(1.0 - rmbClickCoords.Y / ActualHeight));

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


		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			if(e.Key == Key.Delete)
			{
				CoreDll.GraphViewRemoveSelected();
				ModifyStackPanel();
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


		private void SaveMaterial(object sender, RoutedEventArgs e)
		{
			SaveMaterialDialog saveMaterialDialog = new SaveMaterialDialog()
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			if(saveMaterialDialog.ShowDialog() == true)
			{
				string folderName = saveMaterialDialog.FolderName;
				int formatIndex = saveMaterialDialog.FormatIndex;

				string format;
				switch(formatIndex)
				{
					case 1:
					{
						format = ".jpg";
						break;
					}
					case 2:
					{
						format = ".png";
						break;
					}
					case 3:
					{
						format = ".bmp";
						break;
					}
					case 4:
					{
						format = ".dds";
						break;
					}
					default:
					{
						format = "";
						break;
					}
				}

				string functionGraphName = ParentHelper.GetParentMainWindow(this).FunctionGraphName;
				string baseColorFileName = folderName + functionGraphName + "_BaseColor" + format;
				string metallicFileName = folderName + functionGraphName + "_Metallic" + format;
				string roughnessFileName = folderName + functionGraphName + "_Roughness" + format;
				string normalFileName = folderName + functionGraphName + "_Normal" + format;

				CoreDll.GraphViewSaveMaterialTexturesToFiles(baseColorFileName, metallicFileName, roughnessFileName, normalFileName, formatIndex);
			}
		}
	}
}