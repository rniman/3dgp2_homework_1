//struct VS_Billboard_INPUT
//{
//    float3 positionW : POSITION;
//    float2 sizeW : SIZE;
//};

//struct VS_Billboard_OUTPUT
//{
//    float3 centerW : POSITION;
//    float2 sizeW : SIZE;
//};

//VS_Billboard_OUTPUT VSBillboard(VS_Billboard_INPUT input)
//{
//    VS_Billboard_OUTPUT output;
//    output.centerW = input.positionW;
//    output.sizeW = input.sizeW;
//	return output;
//}

struct VS_BILLBOARD_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float4x4 mtxTransform : WORLDMATRIX;
};

struct VS_BILLBOARD_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct MATERIAL
{
    float4 m_cAmbient;
    float4 m_cDiffuse;
    float4 m_cSpecular; //a = power
    float4 m_cEmissive;
};

// cbv
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
    matrix gmtxGameObject : packoffset(c0);
    MATERIAL gMaterial : packoffset(c4);
    uint gnTexturesMask : packoffset(c8);
};

VS_BILLBOARD_OUTPUT VSBillboard(VS_BILLBOARD_INPUT input)
{
    VS_BILLBOARD_OUTPUT output;

    output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
    output.uv = input.uv;

    return (output);
}