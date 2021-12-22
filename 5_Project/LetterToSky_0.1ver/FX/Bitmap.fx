//=============================================================================
// Bitmap.fx by Hacgeum
//
// Effect used to shade Bitmap
//=============================================================================

cbuffer cbPerFrame
{
    float4x4 gWorldViewProj;
};

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

BlendState AlphaBlend
{
    // 테두리 처리 False일 때
    AlphaToCoverageEnable = false;

    // 투명처리
    BlendEnable[0] = true; // 블렌드 모드 사용, 8개의 이미지까지 사용 가능

    /*
    블렌딩 함수

    INV_SRC_ALPHA : 소스 텍스쳐의 알파를 역전시킨 것
    즉, 1에서 소스 텍스쳐의 알파만큼 뺀 것.

    ex) 만약 소스의 알파가 0.3이라면 목표의 알파는 0.7로 보고 목표 픽셀의 70%를 사용
    */

    DestBlend[0] = INV_SRC_ALPHA; // 이미 그 자리에 그려져 있던 목표 픽셀의 색상
    SrcBlend[0] = SRC_ALPHA; // 소스 텍스쳐의 색상을 계산하는데 사용, (블렌딩 함수)SRC_ALPHA : 텍스쳐가 갖고있는 알파값 그대로사용
    BlendOp[0] = Add; // 연산 방법, Add : 더해서 최종 픽셀 구함

    SrcBlendAlpha[0] = One; // 한 개 쓰겠다.
    DestBlendAlpha[0] = One; // 한 개 쓰겠다.
    RenderTargetWriteMask[0] = 0x0f;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float2 Tex  : TEXCOORD0;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex  : TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout = (VertexOut) 0;
    
    // Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;
    
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 textureColor;
    
    textureColor = shaderTexture.Sample(SampleType, pin.Tex);
    
    return textureColor;
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        // 투명 처리 (텍스쳐 외의 뒷 배경색 제거)
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF); // 0xFFFFFFFF = 0xFF 알아서 개수 맞춰넣어줌
    }
}
