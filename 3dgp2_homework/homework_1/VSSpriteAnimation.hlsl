struct VS_TEXTURED_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
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
    uint gnTexturesMask : packoffset(c8.x);
    uint columns : packoffset(c8.y);
    uint rows : packoffset(c8.z);
    uint column : packoffset(c8.w);
    uint row : packoffset(c9);
};

VS_TEXTURED_OUTPUT VSSpriteAnimation(VS_TEXTURED_INPUT input)
{
    VS_TEXTURED_OUTPUT output;
        
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);

    float3x3 mtx3x3Texture = float3x3(1.0f / float(rows),       0.0f,                           0.0f,
                                      0.0f,                     1.0f / float(columns),          0.0f,
                                      float(row) / float(rows), float(column) / float(columns), 1.0f);
    
    output.uv = mul(float3(input.uv, 1.0f), mtx3x3Texture).xy;

    return (output);
}