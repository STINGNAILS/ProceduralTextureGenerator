   M S B A M L   `   `   `  ����U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  5      *    $ProceduralTextureGenerator.GraphView   5      W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      :local(clr-namespace:ProceduralTextureGenerator     5      -   5        
������ |�. .��5	      �   � RowDefinitions  5
   
   �� 5      $��24 �6   �� 5      $��* �6   .+�5   
   ��  �� Orientation$ 
Horizontal=�6   ���	#BF404040 K� ZIndex$ 2��6E   .+�5      �� -   5         ��Name$ ObjectViewTitleName��5      $��
Graph View��65   $��Segoe UI Semibold)�6G   $��14(�6f   $
��Left=�6t   
�������$��	15,2,15,0q�6�   $��0��6�   �� 5      $��24��6   $��1,0,1,0q�6!   
�������6;   o� 5      $ 2��6   $��2,0,0,0q�6:   ��6$   Z� .@�5      m� -   5        $��Scope��5      $��Segoe UI)�6=   $��14(�6S   
�������$��1,0,3,0q�6u   $��0,1,0,0q�6�    m� Icon 5      F� 5      $��15��6   $��15��6*    �� Stretch$ Uniform=�66   
���  �� 	  Scope#��[  6W   5      m� 5      -   5        $��Save Material��5      $��Segoe UI)�6G   $��14(�6]   
�������$��1,0,3,0q�6   $��0,1,0,0q�6�    5      F� 5      $��15��6   $��15��6*   $ Uniform=�66   
���  ��  Save#��[ 6W   5      5      5      �� 5    
   $��2��6   .��5!      %   ProceduralTextureGenerator.View  -   5          Name$
 view��5!       � ContextMenu 5"      �� 5#        ContextMenuStyle#��[ 6   .@�5$      m� -   5        $��Uniform Color��5$        ContextMenuItemStyle#��[ 66   6�   m� 5%      -   5        $��Blend��5%      #��[ 6.   6s   m� 5&      -   5        $��Remap��5&      #��[ 6.   6s   m� 5'      -	   5        $��Gradient��5'      #��[ 61   6y   m� 5(      -
   5        $��Perlin Noise��5(      #��[ 65   6�   m� 5)      -   5        $��Worley Noise��5)      #��[ 65   6�   m� 5*      -   5        $��Normal Color��5*      #��[ 65   6�   m� 5+      -   5        $
��Blur��5+      #��[ 6-   6q   m� 5,      -   5        $��Directional Blur��5,      #��[ 69   6�   m� 5-      -   5        $��Metal Reflectance��5-      #��[ 6:   6�   m� 5.      -   5        $��Height To Normal��5.      #��[ 69   6�   m� 5/      -   5        $��Shape��5/      #��[ 6.   6s   m� 50      -   5        $��	Transform��50      #��[ 62   6{   m� 51      -   5        $
��Warp��51      #��[ 6-   6q   m� 52      -   5        $��
Slope Blur��52      #��[ 63   6|   m� 53      -   5        $��Linear Gradient��53      #��[ 68   6�   54      56      �� 58   
   -   5         ��Name$ hoveredPortNameCanvas��58      $��2��60   .+�59      0   *ProceduralTextureGenerator.HoveredPortName  -   5        	  Name$	 hoveredPortName��59   $   $y�	Collapsed=�6=   $ 1��6T   5:      5;      5<      