using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ProceduralTextureGenerator
{
	class CoreDll
	{
		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Init")]
		public static extern void Init();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "BindView")]
		public static extern void BindView(int viewIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "OverrideView")]
		public static extern void OverrideView(int viewIndex, IntPtr viewResource);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Navigate")]
		public static extern void Navigate(int viewIndex, float dx, float dy, float dt);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Zoom")]
		public static extern void Zoom(int viewIndex, float dz);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Render")]
		public static extern void Render(int viewIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Release")]
		public static extern void Release();
	}
}
