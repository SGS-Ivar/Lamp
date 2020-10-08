struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer CBuffer
{
	matrix u_Model;
	//matrix u_Projection;
};

VSOut main(float3 pos : POSITION, float3 color : COLOR)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.f), u_Model);
	vso.color = color;

	return vso;
}