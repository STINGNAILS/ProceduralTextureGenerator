   M S B A M L   `   `   `  ����U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  5   	   +    %ProceduralTextureGenerator.MainWindow   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   :local(clr-namespace:ProceduralTextureGenerator     5   	     � Title$"  Procedural Texture Generator��5   	   ���	#FF202020$	��840��6E   $
��1600��6R   .��5	      �  � RowDefinitions 5
   
   �� 5      $��24 �6   �� 5      $
��469* �6   �� 5      $
��auto �6   �� 5      $
��340* �6    � ColumnDefinitions 5   
   �� 5      $��1* �6   �� 5      $
��auto �6   �� 5      $��1* �6   �� 5      $	��250 �6   .+�5   
   o�    MainMenuStyle#��[  6   $��0��66   $��0��6C   $��4��6S   .@�5      m� -   5          MainMenuItemStyle#��[ 5      $��New Function Graph��6B   }�.  m� Icon 5      F� 5      $��16��6   $��16��6&    �� Stretch$
 Fill=�62   
���  ��  NewFunctionGraph#��[ 6P    J� RenderTransformOrigin$ 0.5,0.5&�6y   z�5      V� 5      .~�5   "   ��  �� ScaleY$ -1Q�61   �� 5   "   �� 5   "   T� 5    "   5!      5#      5%      m� 5&      -   5        #��[ 5&      $
��Open��6B    5'      F� 5(      $��16��6   $��16��6&   $ Uniform=�62   
���  ��  OpenFunctionGraph#��[ 6S   5*      m� 5+      -   5        #��[ 5+      $
��Save��6B    5,      F� 5-      $��16��6   $��16��6&   $
 Fill=�62   
���  ��  Save#��[ 6P   5/      50      +   %ProceduralTextureGenerator.ObjectView  52   
   -   5          Name$ 
objectView��52       J� SnapsToDevicePixels . $��1��6J   $��0��6W   |�. 6g   �� 53   
   ���	#FF282828	 K� ZIndex$	 1��6)   $��1q�6:    . $��1��6i   $��0��6v   �� 55   
   $��3��6   $��Center=�6"   $��Stretch=�6=   ���	#FF282828���	#FF282828���	#FF282828 . $��2��6�   $��0��6�   $��2��6�   ,   &ProceduralTextureGenerator.TextureView  57   
   -   5        
  Name$
 textureView��57       . $��0��6L   $��3��6\   |�. 6i   �� 58   
   ���	#FF282828$	 1��6)   $��1q�6:    . $��0��6i   $��3��6y   �� 5:   
   $��3��6   $��Stretch=�6!   $��Center=�6=   ���	#FF282828���	#FF282828���	#FF282828 . $��1��6�   $��1��6�   $��3��6�   *   $ProceduralTextureGenerator.GraphView  5<   
   -   5          Name$ 	graphView��5<       . $��1��6H   $��2��6U   $��3��6e   |�. 6v   �� 5=   
   ���	#FF282828$	 1��6)   $��1q�6:    . $��1��6i   $��2��6v   $��3��6�   /   )ProceduralTextureGenerator.ParameterPanel  5?   
   -   5          Name$ parameterPannel��5?       . $��1��6S   $��3��6`   $��3��6p   �� 5@   
   ���	#FF282828$	 1��6)   $��1q�6:    . $��1��6i   $��3��6v   $��3��6�   5A      5B      