struct VS_TERRAIN_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

SamplerState gssWrap : register(s0);

Texture2D gtxtTerrainTexture : register(t8);
Texture2D gtxtDetailTexture : register(t9);
Texture2D gtxtAlphaTexture : register(t10);

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
    float4 cBaseTexColor = gtxtTerrainTexture.Sample(gssWrap, input.uv0);
    float4 cDetailTexColor = gtxtDetailTexture.Sample(gssWrap, input.uv1);
    float fAlpha = gtxtTerrainTexture.Sample(gssWrap, input.uv0);

	//float4 cColor = cBaseTexColor * 0.5f + cDetailTexColor * 0.5f;
    float4 cColor = saturate(lerp(cBaseTexColor, cDetailTexColor, fAlpha));

    return (cColor);
}
