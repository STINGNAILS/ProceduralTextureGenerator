using System;
using System.Collections.Generic;
using System.Diagnostics;
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
using System.Windows.Threading;
using Microsoft.Win32;

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Логика взаимодействия для MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private Stopwatch frameTime;
		private TimeSpan lastRenderTime;

		private string functionGraphName;


		private bool progressIsSaved;


		public MainWindow()
		{
			InitializeComponent();

			CoreDll.Init();

			CompositionTarget.Rendering += Update;

			graphView.SetParameterPanel(parameterPannel);

			frameTime = new Stopwatch();
			frameTime.Start();

			functionGraphName = "";

			progressIsSaved = true;
		}


		public void InvalidateSaving()
		{
			progressIsSaved = false;
		}


		private void Update(object sender, EventArgs e)
		{
			RenderingEventArgs args = (RenderingEventArgs)e;

			if(lastRenderTime != args.RenderingTime)
			{
				UpdateControls();
				objectView.Render();
				graphView.Render();
				textureView.Render();
				lastRenderTime = args.RenderingTime;
			}
		}


		private void UpdateControls()
		{
			frameTime.Stop();
			float dt = frameTime.ElapsedMilliseconds / 1000.0f;

			objectView.Update(dt);
			graphView.Update(dt);
			textureView.Update(dt);

			frameTime.Restart();
		}


		private void OnKeyDown(object sender, KeyEventArgs e)
		{

		}


		private void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if(!progressIsSaved)
			{
				NotSavedDialog notSavedDialog = new NotSavedDialog
				{
					Owner = Application.Current.MainWindow,
					WindowStartupLocation = WindowStartupLocation.CenterOwner
				};

				if(notSavedDialog.ShowDialog() == true)
				{
					if(notSavedDialog.SaveChanges)
					{
						string functionGraphFile = AppDomain.CurrentDomain.BaseDirectory + "Function Graphs\\" + functionGraphName + ".ptfg";
						CoreDll.SaveFunctionGraphToFile(functionGraphFile);
					}
				}
				else
				{
					e.Cancel = true;
				}
			}
		}


		private void OnExit(object sender, EventArgs e)
		{
			CoreDll.Release();
		}


		private void NewFunctionGraph(object sender, RoutedEventArgs e)
		{
			if(!progressIsSaved)
			{
				NotSavedDialog notSavedDialog = new NotSavedDialog
				{
					Owner = Application.Current.MainWindow,
					WindowStartupLocation = WindowStartupLocation.CenterOwner
				};

				if(notSavedDialog.ShowDialog() == true)
				{
					if(notSavedDialog.SaveChanges)
					{
						string functionGraphFile = AppDomain.CurrentDomain.BaseDirectory + "Function Graphs\\" + functionGraphName + ".ptfg";
						CoreDll.SaveFunctionGraphToFile(functionGraphFile);
					}
				}
				else
				{
					return;
				}
			}

			NewFunctionGraphDialog newFunctionGraphWindow = new NewFunctionGraphDialog
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			if(newFunctionGraphWindow.ShowDialog() == true)
			{
				CoreDll.ResetFunctionGraph();

				functionGraphName = newFunctionGraphWindow.FunctionGraphName;
				Title = functionGraphName + " - Procedural Texture Generator";

				progressIsSaved = false;
			}
		}


		private void OpenFunctionGraph(object sender, RoutedEventArgs e)
		{
			if(!progressIsSaved)
			{
				NotSavedDialog notSavedDialog = new NotSavedDialog
				{
					Owner = Application.Current.MainWindow,
					WindowStartupLocation = WindowStartupLocation.CenterOwner
				};

				if(notSavedDialog.ShowDialog() == true)
				{
					if(notSavedDialog.SaveChanges)
					{
						string functionGraphFile = AppDomain.CurrentDomain.BaseDirectory + "Function Graphs\\" + functionGraphName + ".ptfg";
						CoreDll.SaveFunctionGraphToFile(functionGraphFile);
					}
				}
				else
				{
					return;
				}
			}

			OpenFileDialog openFileDialog = new OpenFileDialog()
			{
				Filter = "PTFG Files (*.ptfg)|*.ptfg"
			};
			
			if(openFileDialog.ShowDialog() == true)
			{
				CoreDll.LoadFunctionGraphFromFile(openFileDialog.FileName);
				
				functionGraphName = System.IO.Path.GetFileNameWithoutExtension(openFileDialog.FileName);
				Title = functionGraphName + " - Procedural Texture Generator";

				progressIsSaved = true;
			}
		}


		private void SaveFunctionGraph(object sender, RoutedEventArgs e)
		{
			string functionGraphFile = AppDomain.CurrentDomain.BaseDirectory + "Function Graphs\\" + functionGraphName + ".ptfg";
			CoreDll.SaveFunctionGraphToFile(functionGraphFile);

			progressIsSaved = true;
		}
	}
}
