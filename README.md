# OpenGLDemo


本demo旨在汇总learnopengl的大部分内容，进行一个综合的应用，发现并解决一些问题









###文件说明

main函数在Scenario.cpp文件，着色器文件用到了floor，light，mount，skybox和shadow相关文件，关于calculateLight相关文件我想等阴影这部分做完再用

在实际工程中，这些着色器文件是怎么分配的，是否跟我这样将不同的物体相关的功能将着色器分开写，还是说只有一个或几个大的着色器文件，将功能写到一起，如果是分开的，那应该如何很配是一种比较好的选择。


头文件中的内容跟learnopengl中的基本一致，我只改了model.h的一小部分内容



问题：阴影贴图绘制不成功（全黑），我觉得问题在while循环里（正常的场景是能渲染出来的，只是阴影画不出来）

#####3.18 问题更新

绘制阴影贴图时，使用了绘制floor的renderQuad，范围大于1，阴影显示不出来，更换renderQuad后绘制阴影成功,但是方向不太对且好像只有地板（？），模型貌似没有绘制进去

![1742309698037](https://github.com/user-attachments/assets/93581f99-dd36-4fe2-bfb0-560b64fe5852)



