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

float4x4 MatrixLookAt(float3 f3Eye, float3 f3At, float3 f3Up)
{
    float3 look = normalize(f3At - f3Eye);
    float3 right = normalize(cross(f3Up, look));
    //float3 up = normalize(cross(look, right));
    
    float NegEyeDotX = -dot(f3Eye, right);
    float NegEyeDotY = -dot(f3Eye, f3Up);
    float NegEyeDotZ = -dot(f3Eye, look);

    float4x4 resultMatrix = float4x4(
        float4(right.x, f3Up.x, look.x, 0.0f),
        float4(right.y, f3Up.y, look.y, 0.0f),
        float4(right.z, f3Up.z, look.z, 0.0f),
        float4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    resultMatrix[3] = float4(NegEyeDotX, NegEyeDotY, NegEyeDotZ, 1);
    
    return resultMatrix;
}

VS_TEXTURED_OUTPUT VSSpriteAnimation(VS_TEXTURED_INPUT input)
{
    VS_TEXTURED_OUTPUT output;
         
    float3 position = gmtxGameObject._41_42_43;
    
    float4x4 worldMatrix = MatrixLookAt(position, gvCameraPosition, float3(0.0f, 1.0f, 0.0f));
    float4x4 mtx = gmtxGameObject;
    mtx._11 = worldMatrix._11;
    mtx._12 = worldMatrix._21;
    mtx._13 = worldMatrix._31;
    mtx._21 = worldMatrix._12;
    mtx._22 = worldMatrix._22;
    mtx._23 = worldMatrix._32;
    mtx._31 = worldMatrix._13;
    mtx._32 = worldMatrix._23;
    mtx._33 = worldMatrix._33;
    
    output.position = mul(mul(mul(float4(input.position, 1.0f), mtx), gmtxView), gmtxProjection);

    float3x3 mtx3x3Texture = float3x3(1.0f / float(rows),       0.0f,                           0.0f,
                                      0.0f,                     1.0f / float(columns),          0.0f,
                                      float(row) / float(rows), float(column) / float(columns), 1.0f);
    
    output.uv = mul(float3(input.uv, 1.0f), mtx3x3Texture).xy;

    return (output);
}