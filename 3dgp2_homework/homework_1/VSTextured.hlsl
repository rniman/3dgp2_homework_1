struct VS_TEXTURED_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float4x4 mtxTransform : WORLDMATRIX;
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
    uint gnTexturesMask : packoffset(c8);
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

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
    VS_TEXTURED_OUTPUT output;
    
    float3 position = float3(input.mtxTransform._41_42_43);
    
    float4x4 worldMatrix = MatrixLookAt(position, gvCameraPosition, float3(0.0f, 1.0f, 0.0f));
    input.mtxTransform._11 = worldMatrix._11; input.mtxTransform._12 = worldMatrix._21;  input.mtxTransform._13 = worldMatrix._31;
    input.mtxTransform._21 = worldMatrix._12; input.mtxTransform._22 = worldMatrix._22;  input.mtxTransform._23 = worldMatrix._32;
    input.mtxTransform._31 = worldMatrix._13; input.mtxTransform._32 = worldMatrix._23;  input.mtxTransform._33 = worldMatrix._33;
    
    output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
    output.uv = input.uv;

    return (output);
}