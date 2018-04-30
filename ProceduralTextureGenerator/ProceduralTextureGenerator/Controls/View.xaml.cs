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
using System.Windows.Interop;
using System.Runtime.InteropServices;
using Microsoft.Wpf.Interop.DirectX;

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Interaction logic for View.xaml
	/// </summary>
	public partial class View : UserControl
	{
		private int viewIndex = 0;

		private bool isNavigating;

		private Point mouseClickCoords;


		public View()
		{
			InitializeComponent();

			directXView.OnRender = OnRender;

			isNavigating = false;
		}


		private void OnLoaded(object sender, RoutedEventArgs e)
		{
			directXView.WindowOwner = (new System.Windows.Interop.WindowInteropHelper(MainWindow.GetWindow(this))).Handle;
		}


		private void OnResize(object sender, SizeChangedEventArgs e)
		{
			double dpiScale = 1.0;

			if(PresentationSource.FromVisual(this).CompositionTarget is HwndTarget hwndTarget)
			{
				dpiScale = hwndTarget.TransformToDevice.M11;
			}

			int surfWidth = (int)(ActualWidth < 0 ? 0 : Math.Ceiling(ActualWidth * dpiScale));
			int surfHeight = (int)(ActualHeight < 0 ? 0 : Math.Ceiling(ActualHeight * dpiScale));

			directXView.SetPixelSize(surfWidth, surfHeight);
		}


		private void OnRender(IntPtr surface, bool isNewSurface)
		{
			if(isNewSurface)
			{
				int result = CoreDll.ResizeView(viewIndex, surface);
				if(result == 1)
				{
					MessageBox.Show("View initialization error; the program will be shut down");
					ParentHelper.GetParentMainWindow(this).Close();
				}
			}

			CoreDll.Render(viewIndex);
		}


		public void Bind(int viewIndex_)
		{
			viewIndex = viewIndex_;
		}


		public void Update(float dt)
		{
			if(isNavigating)
			{
				Point currentMouseCoords = MouseHelper.GetCursorPosition();

				float dx = (float)(currentMouseCoords.X - mouseClickCoords.X);
				float dy = (float)(currentMouseCoords.Y - mouseClickCoords.Y);

				CoreDll.Navigate(viewIndex, dx, dy, dt);
				MouseHelper.SetCursorPosition(mouseClickCoords);
			}
		}


		public void Render()
		{
			directXView.RequestRender();
		}


		private void OnMouseDown(object sender, MouseButtonEventArgs e)
		{
			if(e.ChangedButton == MouseButton.Middle)
			{
				mouseClickCoords = MouseHelper.GetCursorPosition();
				
				Mouse.Capture(this);
				this.Cursor = Cursors.None;
				isNavigating = true;
			}
		}


		private void OnMouseUp(object sender, MouseButtonEventArgs e)
		{
			if(e.ChangedButton == MouseButton.Middle)
			{
				Mouse.Capture(null);
				this.Cursor = Cursors.Arrow;
				isNavigating = false;
			}
		}


		private void OnMouseWheel(object sender, MouseWheelEventArgs e)
		{
			Point mouseCoords = e.GetPosition(this);

			CoreDll.Zoom(viewIndex, (float)(mouseCoords.X / ActualWidth), (float)(1.0 - mouseCoords.Y / ActualHeight), e.Delta);
		}
	}
}
