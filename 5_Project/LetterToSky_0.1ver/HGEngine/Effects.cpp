#include "Effects.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	:mFX(0)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &mFX));
}

Effect::~Effect()
{
	ReleaseCOM(mFX);
}
#pragma endregion

#pragma region ColorEffect
ColorEffect::ColorEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	LightTech = mFX->GetTechniqueByName("ColorTech");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

ColorEffect::~ColorEffect()
{

}
#pragma endregion

#pragma region LightingEffect
LightingEffect::LightingEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	LightTech = mFX->GetTechniqueByName("LightTech");

	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights = mFX->GetVariableByName("gDirLight");
	PointLights = mFX->GetVariableByName("gPointLight");
	SpotLights = mFX->GetVariableByName("gSpotLight");
	Mat = mFX->GetVariableByName("gMaterial");
}

LightingEffect::~LightingEffect()
{

}
#pragma endregion

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	Light1Tech = mFX->GetTechniqueByName("Light1");
	Light2Tech = mFX->GetTechniqueByName("Light2");
	Light3Tech = mFX->GetTechniqueByName("Light3");

	Light0TexTech = mFX->GetTechniqueByName("Light0Tex");
	Light1TexTech = mFX->GetTechniqueByName("Light1Tex");
	Light2TexTech = mFX->GetTechniqueByName("Light2Tex");
	Light3TexTech = mFX->GetTechniqueByName("Light3Tex");

	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights = mFX->GetVariableByName("gDirLights");
	Mat = mFX->GetVariableByName("gMaterial");
	DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{

}
#pragma endregion

#pragma region BlurEffect
BlurEffect::BlurEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	HorzBlurTech = mFX->GetTechniqueByName("HorzBlur");
	VertBlurTech = mFX->GetTechniqueByName("VertBlur");

	Weights = mFX->GetVariableByName("gWeights")->AsScalar();
	InputMap = mFX->GetVariableByName("gInput")->AsShaderResource();
	OutputMap = mFX->GetVariableByName("gOutput")->AsUnorderedAccessView();
}

BlurEffect::~BlurEffect()
{

}
#pragma endregion

#pragma region SkyEffect
SkyEffect::SkyEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	LightTech = mFX->GetTechniqueByName("SkyTech");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	CubeMap = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
}

SkyEffect::~SkyEffect()
{

}
#pragma endregion

#pragma region BitmapEffect
BitmapEffect::BitmapEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();

	DiffuseMap = mFX->GetVariableByName("shaderTexture")->AsShaderResource();

	LightTech = mFX->GetTechniqueByName("LightTech");
}

BitmapEffect::~BitmapEffect()
{

}
#pragma endregion

#pragma region Effects
ColorEffect* Effects::ColorFX = 0;
LightingEffect* Effects::LightingFX = 0;
BasicEffect* Effects::BasicFX = 0;
BlurEffect* Effects::BlurFX = 0;
SkyEffect* Effects::SkyFX = 0;
BitmapEffect* Effects::BitmapFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	ColorFX = new ColorEffect(device, L"../FX/Color.cso");
	LightingFX = new LightingEffect(device, L"../FX/Lighting.cso");
	BasicFX = new BasicEffect(device, L"../FX/Basic.cso");
	BlurFX = new BlurEffect(device, L"../FX/Blur.cso");
	SkyFX = new SkyEffect(device, L"../FX/Sky.cso");
	BitmapFX = new BitmapEffect(device, L"../FX/Bitmap.cso");
}

void Effects::DestroyAll()
{
	SafeDelete(ColorFX);
	SafeDelete(LightingFX);
	SafeDelete(BasicFX);
	SafeDelete(BlurFX);
	SafeDelete(SkyFX);
	SafeDelete(BitmapFX);
}
#pragma endregion