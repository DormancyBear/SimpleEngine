// global variables
// can be modified externally from the C++ code
// 通常将所有 globals 按照更新频率( 哪些 globals 会在一起修改 )进行划分, 分别放在不同的 cbuffer 里( HLSL 的一种 buffer object type )
// 显然 graphics card 会将同一个 buffer 里的所有变量存到一起( 类似数组 ), 存储效率更高
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


// 自定义结构体
// 语义的本质是寄存器 => Shader 通过与寄存器通信参与进 GPU 的硬件渲染管线
// GPU 从显存中取得一个顶点的不同类型数据( 颜色, 位置等 )分别填到对应的寄存器中
// 如下 Shader 先到 POSITION 寄存器中获取输入, 再将处理后的数据填充到 SV_POSITION 寄存器中
struct VertexInputType
{
    float4 position : POSITION;	// x, y, z, w
    float4 color : COLOR;		// red, green, blue, alpha
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


// Vertex Shader
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}