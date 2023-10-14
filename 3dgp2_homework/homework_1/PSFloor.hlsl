struct VS_FLOOR_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float2 uv : TEXCOORD;
};

Texture2D gtxAlbedo : register(t0);
Texture2D gtxDetailAlbedo : register(t1);

SamplerState gssWrap : register(s0);
SamplerState gssClamp : register(s1);

float4 PSFloor(VS_FLOOR_OUTPUT input) : SV_TARGET
{
    float4 albedoColor = gtxAlbedo.Sample(gssWrap, input.uv);
    float4 detailColor = gtxDetailAlbedo.Sample(gssWrap, input.uv);
    
    float4 cColor = lerp(albedoColor, detailColor, 0.0f);
    
    return cColor;
}