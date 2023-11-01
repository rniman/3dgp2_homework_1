struct VS_USERINTERFACE_INPUT
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

struct VS_USERINTERFACE_OUTPUT
{
    float4 position : SV_POSITION;
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
    uint gnTexturesMask : packoffset(c8.x);
    uint columns : packoffset(c8.y);
    uint rows : packoffset(c8.z);
    uint column : packoffset(c8.w);
    uint row : packoffset(c9);
};

VS_USERINTERFACE_OUTPUT VSUserInterface(VS_USERINTERFACE_INPUT input)
{
    VS_USERINTERFACE_OUTPUT output;
    float4 positionW = mul(float4(input.position, 1.0f), gmtxGameObject);
    output.position = float4(positionW.xy, 0.0f, 1.0f);
    output.uv = input.uv;

    return (output);
}