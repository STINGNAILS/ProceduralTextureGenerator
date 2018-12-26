using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ProceduralTextureGenerator
{
	public struct DirectionalLight
	{
		public float r;
		public float g;
		public float b;
		public float intensity;
		public float phi;
		public float psi;
		public int isEnabled;
	}


	public struct SphereLight
	{
		public float r;
		public float g;
		public float b;
		public float intensity;
		public float x;
		public float y;
		public float z;
		public float lightRadius;
		public float sourceRadius;
		public int isEnabled;
	}


	public struct HoveredPortDescriptor
	{
		public int hoveredPortFunctionIndex;
		public int hoveredPortIndex;
		public float x;
		public float y;
	}


	class CoreDll
	{
		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Init")]
		public static extern int Init();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ResizeView")]
		public static extern int ResizeView(int viewIndex, IntPtr viewResource);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Navigate")]
		public static extern void Navigate(int viewIndex, float dx, float dy, float dt);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Zoom")]
		public static extern void Zoom(int viewIndex, float x, float y, float dz);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Render")]
		public static extern void Render(int viewIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ProcessEnvironmentTasks")]
		public static extern void ProcessEnvironmentTasks();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Release")]
		public static extern void Release();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewGetObjectType")]
		public static extern int ObjectViewGetObjectType();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewSetObjectType")]
		public static extern void ObjectViewSetObjectType(int objectType);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewGetDirectionalLight")]
		public static extern DirectionalLight ObjectViewGetDirectionalLight();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewGetSphereLight")]
		public static extern SphereLight ObjectViewGetSphereLight(int sphereLightIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewGetEnvironmentMapIndex")]
		public static extern int ObjectViewGetEnvironmentMapIndex();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewSetDirectionalLight")]
		public static extern void ObjectViewSetDirectionalLight(DirectionalLight directionalLight);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewSetSphereLight")]
		public static extern void ObjectViewSetSphereLight(int sphereLightIndex, SphereLight sphereLight);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewSetEnvironmentMap")]
		public static extern void ObjectViewSetEnvironmentMap(int environmentMapIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ObjectViewScope")]
		public static extern void ObjectViewScope();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewAddNode")]
		public static extern void GraphViewAddNode(int functionIndex, float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGraphWasChanged")]
		public static extern int GraphViewGraphWasChanged();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseDown")]
		public static extern void GraphViewOnMouseDown(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseMove")]
		public static extern void GraphViewOnMouseMove(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseUp")]
		public static extern void GraphViewOnMouseUp(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewOnMouseDoubleClick")]
		public static extern void GraphViewOnMouseDoubleClick(float x, float y);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewRemoveSelected")]
		public static extern void GraphViewRemoveSelected();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGetHoveredPointDescriptor")]
		public static extern HoveredPortDescriptor GraphViewGetHoveredPointDescriptor();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewGetScale")]
		public static extern float GraphViewGetScale();

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

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewScope")]
		public static extern void GraphViewScope();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewSaveMaterialTexturesToFiles")]
		public static extern void GraphViewSaveMaterialTexturesToFiles([MarshalAs(UnmanagedType.LPWStr)] string baseColorFileName, [MarshalAs(UnmanagedType.LPWStr)] string metallicFileName, [MarshalAs(UnmanagedType.LPWStr)] string roughnessFileName, [MarshalAs(UnmanagedType.LPWStr)] string normalFileName, int formatIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewProcess")]
		public static extern void GraphViewProcess();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GraphViewAbortProcessing")]
		public static extern void GraphViewAbortProcessing();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureViewScope")]
		public static extern void TextureViewScope();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureViewSaveTrackedTextureToFile")]
		public static extern void TextureViewSaveTrackedTextureToFile([MarshalAs(UnmanagedType.LPWStr)] string fileName, int formatIndex);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ResetFunctionGraph")]
		public static extern void ResetFunctionGraph();

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SaveFunctionGraphToFile")]
		public static extern void SaveFunctionGraphToFile([MarshalAs(UnmanagedType.LPStr)] string fileName);

		[DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "LoadFunctionGraphFromFile")]
		public static extern void LoadFunctionGraphFromFile([MarshalAs(UnmanagedType.LPStr)] string fileName);
	}
}
