   M S B A M L   `   `   `  ����U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  5      ,    &ProceduralTextureGenerator.RemapSetter   5      W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      :local(clr-namespace:ProceduralTextureGenerator     5      .��5	      �   � RowDefinitions  5
   
   �� 5      $��32 �6   �� 5      $	��144 �6   �� 5      $��20 �6   �� 5      $��8 �6    � ColumnDefinitions 5   
   �� 5      $��1* �6   �� 5      $	��150 �6   .+�5   
   �� -   5         ��Name$ remapSetterName��5         ParameterNameLabelStyle#��[  6)   $��Remap Setter Name��6Z   $��0��6v   $��0��6�   $��3��6�   �� 5   
   -   5         ��Name$ renamedValueCanvas��5      $��50��6-   $��Right=�68   $��1��6T   $��0��6a   .+�5      �� -   5        $ remappedValueLabel��5        IndicatorLabelStyle#��[ 60   $
��0.50��6]   $��24��6l    �� Left$ 10��6x   
 �� Top$ 60��6�   5      �� 5   
   -   5        $ 
viewCanvas��5      ���	#FF202020$��2,2,8,2q�6<   $��1��6M   $��2��6Z   .+�5      �� -   5         ��Name$ remappingLine1��5      
������� �� StrokeDashArray$	 5 5R�69    K� ZIndex$ 1��6O   � 5      -   5        	 �Name$	 remappingCurve��5      
�������$ 1��6=   �� 5      -   5        $ remappingLine2��5      
�������$	 5 5R�69   $ 1��6O   A� 5      -   5        
 A�Name$
 x1Drag��5      $��10��6&   $��10��61   
���  ��$	 135��6L   $ -5��6]   $ 2��6n   A� 5      -	   5        $
 x2Drag��5      $��10��6&   $��10��61   
���  ��$ 65��6L   $ 65��6\   $ 2��6m   A� 5       -
   5        $
 x3Drag��5       $��10��6&   $��10��61   
���  ��$ -5��6L   $	 135��6\   $ 2��6n   �� 5!      $	��140��6   $	��140��6!   $��1q�6.   
�������6B   5"      �� 5$   
   -   5        $ valueCanvas��5$      $��0,0,8,2q�6&   $��2��67   $��1��6D   .+�5%      �� -   5        $ 
valueLabel��5%      #��[ 6(   $
��0.50��6U   $��24��6d   $ 52��6p   $ -5��6�   5&      �� 5(   
     ParameterSeparatorStyle#��[ 6   $��4��6E   $��0��6R   $��2��6b   5)      5*      