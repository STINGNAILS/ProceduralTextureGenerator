   M S B A M L   `   `   `  ����U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  5      ,    &ProceduralTextureGenerator.ColorSetter   5      W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      :local(clr-namespace:ProceduralTextureGenerator     5      .��5	      �   � RowDefinitions  5
   
   �� 5      $��32 �6   �� 5      $��32 �6   �� 5      $��32 �6   �� 5      $��32 �6   �� 5      $��8 �6    � ColumnDefinitions 5   
   �� 5      $��60 �6   �� 5      $��1* �6   �� 5      $	��102 �6   .+�5   
   �� -   5         ��Name$ colorSetterName��5         ParameterNameLabelStyle#��[  6)   $��Color Setter Name��6Z   $��0��6v   $��0��6�   $��3��6�   �� 5   
   -   5        $ redColorLabel��5        IndicatorLabelStyle#��[ 6'   $��R��6T   $��1��6`   $��0��6m   �� 5   
   -   5         ��Name$ redColorTextBox��5      $��24��6+   $��Segoe UI)�67   $��14(�6M   
����������	#FF202020
�������$��2,0,2,0q�6�    �� TextAlignment$ Center=�6�   $��1��6�   $��1��6�   �� 5   
   -   5        $ greenColorLabel��5      #��[ 6)   $��G��6V   $��2��6b   $��0��6o   �� 5   
   -   5        $ greenColorTextBox��5      $��24��6-   $��Segoe UI)�69   $��14(�6O   
����������	#FF202020
�������$��2,0,2,0q�6�   $ Center=�6�   $��2��6�   $��1��6�   �� 5   
   -   5        $ blueColorLabel��5      #��[ 6(   $��B��6U   $��3��6a   $��0��6n   �� 5    
   -   5        $ blueColorTextBox��5       $��24��6,   $��Segoe UI)�68   $��14(�6N   
����������	#FF202020
�������$��2,0,2,0q�6�   $ Center=�6�   $��3��6�   $��1��6�   �� 5"   
   -   5         ��Name$ 	colorRect��5"      
�������$��2,2,8,2q�67   $��2��6H   $��1��6X   $��3��6e   �� 5$   
     ParameterSeparatorStyle#��[ 6   $��4��6E   $��0��6R   $��3��6b   5%      5&      