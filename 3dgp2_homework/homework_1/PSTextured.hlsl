
struct VS_TEXTURED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D gtxtTexture : register(t0);

SamplerState gWrapSamplerState : register(s0);

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);

    return (cColor);
}