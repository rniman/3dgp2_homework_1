
struct VS_OCEAN_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct MATERIAL
{
    float4 m_cAmbient;
    float4 m_cDiffuse;
    float4 m_cSpecular; //a = power
    float4 m_cEmissive;
};

struct VS_OCEAN_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float2 uv : TEXCOORD;
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

VS_OCEAN_OUTPUT VSOcean(VS_OCEAN_INPUT input)
{
    VS_OCEAN_OUTPUT output;

    output.positionW = (float3) mul(float4(input.position, 1.0f), gmtxGameObject);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.uv = input.uv;

    return (output);
}