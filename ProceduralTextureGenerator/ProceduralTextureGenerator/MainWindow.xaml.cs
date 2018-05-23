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
using System.Threading;

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Логика взаимодействия для MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private AutoResetEvent processGraphEvent;
		private AutoResetEvent terminateThreadEvent;
		private WaitHandle[] waitHandles;
		private Thread graphProcessingThread;

		private Stopwatch frameTime;
		private TimeSpan lastRenderTime;

		private string functionGraphName;

		private bool functionGraphIsSet;
		private bool progressIsSaved;


		public MainWindow()
		{
			InitializeComponent();

			int result = CoreDll.Init();
			if(result == 1)
			{
				MessageBox.Show("Initialization error; the program will be shut down");
				Close();
			}

			processGraphEvent = new AutoResetEvent(true);
			terminateThreadEvent = new AutoResetEvent(false);
			waitHandles = new WaitHandle[]
			{
				processGraphEvent,
				terminateThreadEvent
			};
			graphProcessingThread = new Thread(ProcessGraph);
			graphProcessingThread.Start();

			graphView.SetParameterPanel(parameterPannel);

			frameTime = new Stopwatch();

			functionGraphName = "";

			functionGraphIsSet = false;
			progressIsSaved = true;
			
			Closing += OnClosing;
			Closed += OnExit;
		}


		public void OnFunctionGraphChanged()
		{
			CoreDll.GraphViewAbortProcessing();

			processGraphEvent.Set();
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
				CoreDll.ProcessEnvironmentTasks();
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


		private void ProcessGraph()
		{
			while(true)
			{
				CoreDll.GraphViewProcess();

				int eventIndex = WaitHandle.WaitAny(waitHandles);
				if(eventIndex == 1)
				{
					break;
				}
			}
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
			terminateThreadEvent.Set();
			graphProcessingThread.Join();

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

			NewFunctionGraphDialog newFunctionGraphDialog = new NewFunctionGraphDialog
			{
				Owner = Application.Current.MainWindow,
				WindowStartupLocation = WindowStartupLocation.CenterOwner
			};

			if(newFunctionGraphDialog.ShowDialog() == true)
			{
				CoreDll.GraphViewAbortProcessing();
				terminateThreadEvent.Set();
				graphProcessingThread.Join();

				CoreDll.ResetFunctionGraph();

				graphProcessingThread = new Thread(ProcessGraph);
				graphProcessingThread.Start();

				OnFunctionGraphChanged();
				InvalidateSaving();

				functionGraphName = newFunctionGraphDialog.FunctionGraphName;
				Title = functionGraphName + " - Procedural Texture Generator";

				if(!functionGraphIsSet)
				{
					CompositionTarget.Rendering += Update;
					frameTime.Start();

					functionGraphIsSet = true;
				}
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
				
				OnFunctionGraphChanged();

				functionGraphName = System.IO.Path.GetFileNameWithoutExtension(openFileDialog.FileName);
				Title = functionGraphName + " - Procedural Texture Generator";

				parameterPannel.Update();

				progressIsSaved = true;

				if(!functionGraphIsSet)
				{
					CompositionTarget.Rendering += Update;
					frameTime.Start();

					functionGraphIsSet = true;
				}
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
