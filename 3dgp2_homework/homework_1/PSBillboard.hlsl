struct GS_Billboard_Output
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
    //uint primID : SV_PrimitiveID;
};

Texture2D gtxtTexture : register(t0);

SamplerState gWrapSamplerState : register(s0);

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

//#include "Light.hlsl"

float4 PSBillboard(GS_Billboard_Output input) : SV_TARGET
{
    float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);

    return (cColor);
}