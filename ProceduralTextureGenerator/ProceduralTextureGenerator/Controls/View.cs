using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace ProceduralTextureGenerator
{
    public partial class View: UserControl
    {
        public bool isInitialized = false;


        public View()
        {
            InitializeComponent();
        }


        public void Resize(int viewIndex)
        {
            if (isInitialized)
            {
                CoreDll.ResizeView(viewIndex, (int)Handle);
            }
            else
            {
                CoreDll.BindView(viewIndex, (int)Handle);
                isInitialized = true;
            }
        }


        public void Navigate(int viewIndex, float dx, float dy, float dt)
        {
            CoreDll.Navigate(viewIndex, dx, dy, dt);
        }
    }
}


public class CoreDll
{
    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Init")]
    public static extern void Init();

    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "BindView")]
    public static extern void BindView(int viewIndex, int hwnd);

    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ResizeView")]
    public static extern void ResizeView(int viewIndex, int hwnd);

    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Navigate")]
    public static extern void Navigate(int viewIndex, float dx, float dy, float dt);

    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Zoom")]
    public static extern void Zoom(int viewIndex, float dz);

    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Render")]
    public static extern void Render();

    [DllImport("ProceduralTextureGeneratorCore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Release")]
    public static extern void Release();
}