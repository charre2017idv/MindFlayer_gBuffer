#include "mfLightPass.h"
#include "mfSwapchain.h"
#include "mfDepthStencilView.h"

mfLightPass::mfLightPass()
{
}


mfLightPass::~mfLightPass()
{
}

void mfLightPass::Init(mfLightPassDesc _Desc, mfSwapchain & _Swapchain)
{
  m_descriptor = _Desc;
  m_lightDescriptor = _Desc;

  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateShaderResourceView(m_lightDescriptor.RenderTargetsTex_Ref[0], NULL, &m_gBufferTex[0]);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateShaderResourceView(m_lightDescriptor.RenderTargetsTex_Ref[1], NULL, &m_gBufferTex[1]);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateShaderResourceView(m_lightDescriptor.RenderTargetsTex_Ref[2], NULL, &m_gBufferTex[2]);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateShaderResourceView(m_lightDescriptor.RenderTargetsTex_Ref[3], NULL, &m_gBufferTex[3]);

  mfBufferDesc ShaderAttributesDesc;
  ShaderAttributesDesc.Usage = mf_USAGE_DEFAULT;
  ShaderAttributesDesc.ByteWidth = sizeof(mfLightShaderAttributes);
  ShaderAttributesDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ShaderAttributesDesc.CPUAccessFlags = 0;

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

  // Set Back buffer
  _Swapchain.getInterface().ID->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backbuffer);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateRenderTargetView(m_backbuffer, NULL, &m_BackBufferRT.getInterfaceRT().ID);

  m_ShaderAttribtuesBuffer.Init(ShaderAttributesDesc);
  
  m_LightAttributes.Position = { 200, 0,0,0 };
  m_LightAttributes.lightDir = { 1, 0, 0, 1 };
  m_LightAttributes.lightDirColor = { 1.0f,1.0f,1.0f,1.0f };
  m_LightAttributes.ambientColor = { 1.0f,1.0f,1.0f,1.0f };
  m_LightAttributes.specularColor = { 1, 0, 0, 1 };
  m_LightAttributes.specPower = 15.0f;
  m_LightAttributes.kDiffuse = 1.0f;
  m_LightAttributes.kAmbient = 0.14f;
  m_LightAttributes.kSpecular = 1.0f;
}

void mfLightPass::Update(mfDepthStencilView & _DepthStencilView, Vector4 _Eye)
{
  // Set Render Targets
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->OMSetRenderTargets(1, &m_BackBufferRT.getInterfaceRT().ID, _DepthStencilView.getInterface().ID);
  // Clear Render Targets
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->ClearRenderTargetView(m_BackBufferRT.getInterfaceRT().ID, whiteClearcolor);
  // Clear Depth Stencil View
  mfGraphic_API::getSingleton().ClearDepthStencilView(_DepthStencilView);
  // Set Shader resources
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->PSSetShaderResources(0, 4, m_gBufferTex);
  m_ShaderAttribtuesBuffer.Update(&m_LightAttributes);
  // Set GameObject
  m_GameObject.Update(mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 0);
  // Set Vertex Shader
  m_VertexShader.Update();
  // Set Pixel Shader
  m_PixelShader.Update();
  // Set Samplers
  m_SamplerState.Update();
  // Set Rasterizer
  m_Rasterizer.Update();
}

void mfLightPass::Render()
{
  m_ShaderAttribtuesBuffer.Render(0, 1, false);
  // Draw Game Object
  m_GameObject.Render();
}

void mfLightPass::Destroy()
{
  for (int i = 0; i < 4; i++)
  {
    m_lightDescriptor.RenderTargetsTex_Ref[i]->Release();
    m_gBufferTex[i]->Release();
  }
  m_BackBufferRT.Destroy();
  m_ShaderAttribtuesBuffer.Destroy();
  m_VertexShader.Destroy();
  m_PixelShader.Destroy();
  m_InputLayout.Destroy();

  m_GameObject.Destroy();
  m_SamplerState.Destroy();
  m_Rasterizer.Destroy();
}

mfLightPassDesc & mfLightPass::getLightDescriptor()
{
  return m_lightDescriptor;
}
