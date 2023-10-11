struct VS_TERRAIN_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

SamplerState gssWrap : register(s0);

Texture2D gtxtTerrainTexture : register(t8);
Texture2D gtxtDetailTexture1 : register(t9);
Texture2D gtxtDetailTexture2 : register(t10);
Texture2D gtxtAlphaTexture : register(t11);

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
    float4 cBaseTexColor = gtxtTerrainTexture.Sample(gssWrap, input.uv0);
    float4 cDetailTexColor1 = gtxtDetailTexture1.Sample(gssWrap, input.uv1);
    float4 cDetailTexColor2 = gtxtDetailTexture2.Sample(gssWrap, input.uv0);
    float fAlpha = gtxtAlphaTexture.Sample(gssWrap, input.uv0);
    
	float4 cColor = cBaseTexColor * 0.5f + cDetailTexColor1 * 0.5f;
    cColor = saturate(lerp(cColor, cDetailTexColor2, fAlpha));
    return (cColor);
}
