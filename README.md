# SimpleEngine

个人学习用项目.

## Model Format

利用了`rapidxml`库来构建我自己的模型格式( xml 形式 ),

1. 写了一个`ModelImporter`工具, 可以将 .obj, .fbx 等模型格式转换成引擎自用的 xml 文件;
2. 引擎这边运行时通过加载上面格式固定的 xml 文件来得到模型数据.

以上对各种模型格式的解析工作是借助了`Assimp`库来完成.

## GUI Framework

搭建了一个 GUI 框架, 支持分辨率适配.
