struct VS_Billboard_OUTPUT
{
    float3 centerW : POSITION;
    float2 sizeW : SIZE;
};

struct GS_Billboard_Output
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
    //uint primID : SV_PrimitiveID;
};

cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gvCameraPosition : packoffset(c8);
};

[maxvertexcount(4)]
void GSBillboard(point VS_Billboard_OUTPUT input[1],/* uint primID : SV_PrimitiveID,*/ inout TriangleStream<GS_Billboard_Output> outputStream)
{
    float3 vUp = float3(0.0f, 1.0f, 0.0f);
    float vLook = gvCameraPosition.xyz - input[0].centerW;
    vLook = normalize(vLook);
    float3 vRight = cross(vUp, vLook);
    
    float fHalfW = input[0].sizeW.x * 0.5f;
    float fHalfH = input[0].sizeW.y * 0.5f;
    
    float4 pVertices[4];
    pVertices[0] = float4(input[0].centerW + fHalfW * vRight - fHalfH * vUp, 1.0f);
    pVertices[1] = float4(input[0].centerW + fHalfW * vRight + fHalfH * vUp, 1.0f);
    pVertices[2] = float4(input[0].centerW - fHalfW * vRight - fHalfH * vUp, 1.0f);
    pVertices[3] = float4(input[0].centerW - fHalfW * vRight + fHalfH * vUp, 1.0f);
    
    float2 pUVs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

    GS_Billboard_Output output;
    for(int i = 0; i < 4; ++i)
    {
        output.posW = pVertices[i].xyz;
        output.posH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
        output.normalW = vLook;
        output.uv = pUVs[i];
        //output.primID = primID;
        
        outputStream.Append(output);
    }

}