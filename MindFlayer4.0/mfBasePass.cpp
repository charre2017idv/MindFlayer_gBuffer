#include "mfBasePass.h"
#include "mfLoadModel.h"
#include "mfDepthStencilView.h"


mfBasePass::mfBasePass()
{
}


mfBasePass::~mfBasePass()
{
}

void mfBasePass::Init(mfBasePassDesc _Desc)
{
  // Initialize Vertex Shader
  mfBaseShaderDesc VertexShaderDesc;
  VertexShaderDesc.FileName = _Desc.VertexShaderFileName;
  VertexShaderDesc.EntryPoint = "VS";
  VertexShaderDesc.ShaderModel = "vs_4_0";
#ifdef mfDIRECTX
  VertexShaderDesc.BlobOut = &m_VertexShader.getInterface().VSBlob;
#endif // mfDIRECTX
  m_VertexShader.Init(VertexShaderDesc);

  // Initialize Input Layout
  m_InputLayout.Init(_Desc.InputLayoutDesc, m_VertexShader);

#ifdef mfDIRECTX
  m_VertexShader.getInterface().VSBlob->Release();
#endif // mfDIRECTX
  m_InputLayout.Render();
  // Initialize Pixel Shader
  mfBaseShaderDesc PixelShaderDesc;
  PixelShaderDesc.FileName = _Desc.PixelShaderFileName;
  PixelShaderDesc.EntryPoint = "PS";
  PixelShaderDesc.ShaderModel = "ps_4_0";
#ifdef mfDIRECTX
  PixelShaderDesc.BlobOut = &m_PixelShader.getInterface().PSBlob;
#endif // mfDIRECTX

  m_PixelShader.Init(PixelShaderDesc);


  // Initialize GameObject
  m_GameObject.Init(_Desc.RawData, _Desc.ModelBufferDesc);

  // Initialize Sampler state
  m_SamplerState.Init(_Desc.SamplerDesc);

  // Initialize Rasterizer 
  m_Rasterizer.Init(_Desc.RasterizerDesc);



}

void mfBasePass::Update(float _Time)
{

  // Set GameObject
  m_GameObject.Update(mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST, _Time);
  // Set Vertex Shader
  m_VertexShader.Update();
  // Set Pixel Shader
  m_PixelShader.Update();
  // Set Samplers
  m_SamplerState.Update();
  // Set Rasterizer
  m_Rasterizer.Update();
}

void mfBasePass::Render()
{
  // Draw Game Object
  m_GameObject.Render();
}

void mfBasePass::Destroy()
{
  m_VertexShader.Destroy();
  m_PixelShader.Destroy();
  m_InputLayout.Destroy();
 
  m_GameObject.Destroy();
  m_SamplerState.Destroy();
  m_Rasterizer.Destroy();
}
