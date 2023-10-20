struct VS_Billboard_INPUT
{
    float3 positionW : POSITION;
    float2 sizeW : SIZE;
};

struct VS_Billboard_OUTPUT
{
    float3 centerW : POSITION;
    float2 sizeW : SIZE;
};

VS_Billboard_OUTPUT VSBillboard(VS_Billboard_INPUT input)
{
    VS_Billboard_OUTPUT output;
    output.centerW = input.positionW;
    output.sizeW = input.sizeW;
	return output;
}