   M S B A M L   `   `   `  ���� U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  +    %ProceduralTextureGenerator.MainWindow   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     :local(clr-namespace:ProceduralTextureGenerator       � Title$"  Procedural Texture Generator�����	#FF202020$	��840��$
��1600��.���  � RowDefinitions �� $��24 ��� $
��469* ��� $
��auto ��� $
��340* � � ColumnDefinitions �� $��1* ��� $
��auto ��� $��1* ��� $	��250 �.+�o�    MainMenuStyle#��[  $��0��$��0��$��4��.@�m� -     MainMenuItemStyle#��[ $��New Function Graph��}�.  m� Icon F� $��16��$��16�� �� Stretch$
 Fill=�
���  ��  NewFunctionGraph#��[  J� RenderTransformOrigin$ 0.5,0.5&�z�V� .~���  �� ScaleY$ -1Q��� �� T� m� -   #��[ $
��Open�� F� $��16��$��16��$ Uniform=�
���  ��  OpenFunctionGraph#��[ m� -   #��[ $
��Save�� F� $��16��$��16��$
 Fill=�
���  ��  Save#��[ +   %ProceduralTextureGenerator.ObjectView  -     Name$ 
objectView�� J� SnapsToDevicePixels . $��1��$��0��|�. �� ���	#FF282828	 K� ZIndex$	 1��$��1q� . $��1��$��0���� $��3��$��Center=�$��Stretch=����	#FF282828���	#FF282828���	#FF282828 . $��2��$��0��$��2��,   &ProceduralTextureGenerator.TextureView  -   
  Name$
 textureView�� . $��0��$��3��|�. �� ���	#FF282828$	 1��$��1q� . $��0��$��3���� $��3��$��Stretch=�$��Center=����	#FF282828���	#FF282828���	#FF282828 . $��1��$��1��$��3��*   $ProceduralTextureGenerator.GraphView  -     Name$ 	graphView�� . $��1��$��2��$��3��|�. �� ���	#FF282828$	 1��$��1q� . $��1��$��2��$��3��/   )ProceduralTextureGenerator.ParameterPanel  -     Name$ parameterPannel�� . $��1��$��3��$��3���� ���	#FF282828$	 1��$��1q� . $��1��$��3��$��3��