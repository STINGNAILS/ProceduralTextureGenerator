using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Runtime.InteropServices;

namespace ProceduralTextureGenerator
{
	class MouseHelper
	{
		public static void SetCursorPosition(Point point)
		{
			SetCursorPos((int)point.X, (int)point.Y);
		}

		
		public static Point GetCursorPosition()
		{
			POINT point;
			GetCursorPos(out point);

			return new Point(point.x, point.y);
		}


		[StructLayout(LayoutKind.Sequential)]
		public struct POINT
		{
			public int x;
			public int y;

			public static implicit operator Point(POINT point)
			{
				return new Point(point.x, point.y);
			}
		}


		[DllImport("User32.dll")]
		private static extern bool SetCursorPos(int x, int y);


		[DllImport("user32.dll")]
		private static extern bool GetCursorPos(out POINT point);
	}
}
