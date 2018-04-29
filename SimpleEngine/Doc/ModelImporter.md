# ModelImporter
利用`Assimp`导入多种模型格式, 即将多种建模软件独有的 model format 转换为引擎自用 xml 格式.

## 编译`Assimp`
默认情况下, Assimp 项目会生成 DLL, 但我想要静态编译( 省的一个.dll文件搞来搞去 ), 所以在 Cmake 生成项目时, 设置了 BUILD_SHARED_LIBS 为 false.

generate 成功后, 在 VS2017 中编译, 得到 lib 文件后导入我自己的项目.
