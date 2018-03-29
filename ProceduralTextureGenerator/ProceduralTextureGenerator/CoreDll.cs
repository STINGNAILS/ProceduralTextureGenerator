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

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewAddNode")]
		public static extern void GraphViewAddNode(int functionIndex, float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseDown")]
		public static extern void GraphViewOnMouseDown(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseMove")]
		public static extern void GraphViewOnMouseMove(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseUp")]
		public static extern void GraphViewOnMouseUp(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewRemoveSelected")]
		public static extern void GraphViewRemoveSelected();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGetSelectedNodeIndex")]
		public static extern int GraphViewGetSelectedNodeIndex();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGetSelectedNodeFunctionIndex")]
		public static extern int GraphViewGetSelectedNodeFunctionIndex();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGetSelectedNodeIntParameter")]
		public static extern int GraphViewGetSelectedNodeIntParameter(int parameterIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGetSelectedNodeFloatParameter")]
		public static extern float GraphViewGetSelectedNodeFloatParameter(int parameterIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewSetSelectedNodeIntParameter")]
		public static extern void GraphViewSetSelectedNodeIntParameter(int parameterIndex, int value);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewSetSelectedNodeFloatParameter")]
		public static extern void GraphViewSetSelectedNodeFloatParameter(int parameterIndex, float value);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewProcess")]
		public static extern void GraphViewProcess();
	}
}
