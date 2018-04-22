using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ProceduralTextureGenerator
{
    class WindowDragBehavior
    {
		public static Window GetLeftMouseButtonDrag(DependencyObject obj)
		{
			return (Window) obj.GetValue(LeftMouseButtonDrag);
		}


		public static void SetLeftMouseButtonDrag(DependencyObject obj, Window window)
		{
			obj.SetValue(LeftMouseButtonDrag, window);
		}


		public static readonly DependencyProperty LeftMouseButtonDrag = DependencyProperty.RegisterAttached("LeftMouseButtonDrag", typeof(Window), typeof(WindowDragBehavior), new UIPropertyMetadata(null, OnLeftMouseButtonDragChanged));


		private static void OnLeftMouseButtonDragChanged(object sender, DependencyPropertyChangedEventArgs e)
		{
			if(sender is UIElement element)
			{
				element.MouseLeftButtonDown += ButtonDown;
			}
		}


		private static void ButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			UIElement element = sender as UIElement;
			
			if(element.GetValue(LeftMouseButtonDrag) is Window targetWindow)
			{
				targetWindow.DragMove();
			}
		}
	}
}