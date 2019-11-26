#include "mfPass.h"
#include "mfLoadModel.h"
#include "mfDepthStencilView.h"


mfPass::mfPass()
{
}


mfPass::~mfPass()
{
}

void mfPass::Init(mfBasePassDesc _Desc)
{
  m_descriptor = _Desc;

//   mfBaseTextureDesc RenderTargetDesc;
//   RenderTargetDesc.Tag = "RENDER_TARGET";
//   RenderTargetDesc.ID = mf_RENDER_TARGET_TEXTURE;
//   RenderTargetDesc.Width = SCREEN_WIDTH;
//   RenderTargetDesc.Height = SCREEN_HEIGHT;
//   RenderTargetDesc.MipLevels = 1;
//   RenderTargetDesc.Arraysize = 1;
//   RenderTargetDesc.TexFormat = mf_FORMAT_R32G32B32A32_FLOAT;
//   RenderTargetDesc.SamplerDescCount = 1;
//   RenderTargetDesc.SamplerDescQuality = 0;
//   RenderTargetDesc.Usage = mf_USAGE_DEFAULT;
//   RenderTargetDesc.Bindflags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//   RenderTargetDesc.CPUAccessFlags = 0;
//   RenderTargetDesc.MiscFlags = 0;
//   RenderTargetDesc.Filepath = NULL;
// 
//   m_RenderTarget.Init(RenderTargetDesc);
 // mfRenderManager::getSingleton().GetDevice().CreateRenderTargets(m_RenderTarget);
  // Initialize Render Targets

  D3D11_TEXTURE2D_DESC textureDesc;
  ZeroMemory(&textureDesc, sizeof(textureDesc));

  textureDesc.Width = SCREEN_WIDTH;
  textureDesc.Height = SCREEN_HEIGHT;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;
  
  for (int i = 0; i < 4; i++)
  {
    mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateTexture2D(&textureDesc, NULL, &m_gBuffer.RenderTargetTextures[i]);
  }

  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc32;
  ZeroMemory(&renderTargetViewDesc32, sizeof(renderTargetViewDesc32));
  renderTargetViewDesc32.Format = textureDesc.Format;
  renderTargetViewDesc32.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  renderTargetViewDesc32.Texture2D.MipSlice = 0;

  //Create Render target view
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateRenderTargetView(m_gBuffer.RenderTargetTextures[0], &renderTargetViewDesc32, &m_gBuffer.RenderTargets[0]);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateRenderTargetView(m_gBuffer.RenderTargetTextures[1], &renderTargetViewDesc32, &m_gBuffer.RenderTargets[1]);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateRenderTargetView(m_gBuffer.RenderTargetTextures[2], &renderTargetViewDesc32, &m_gBuffer.RenderTargets[2]);
  mfGraphic_API::getSingleton().GetDevice().getInterface().ID->CreateRenderTargetView(m_gBuffer.RenderTargetTextures[3], &renderTargetViewDesc32, &m_gBuffer.RenderTargets[3]);

  // Initialize Shader Resources
  D3DX11CreateShaderResourceViewFromFile
  (
    mfGraphic_API::getSingleton().GetDevice().getInterface().ID,
    L"Gun_albedo.png",
    NULL,
    NULL,
    &m_Textures[0],
    NULL
  );

  D3DX11CreateShaderResourceViewFromFile
  (
    mfGraphic_API::getSingleton().GetDevice().getInterface().ID,
    L"Gun_normal.png",
    NULL,
    NULL,
    &m_Textures[1],
    NULL
  );

  D3DX11CreateShaderResourceViewFromFile
  (
    mfGraphic_API::getSingleton().GetDevice().getInterface().ID,
    L"Gun_specular.png",
    NULL,
    NULL,
    &m_Textures[2],
    NULL
  );


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
  // Initialize View Const Buffer
  m_ViewBuffer.Init(_Desc.ViewBufferDesc);

  // Initialize Projection Const Buffer
  m_ProjBuffer.Init(_Desc.ProjBufferDesc);

  // Initialize Camera
  m_Camera.Init(_Desc.CameraDesc);

  m_Camera.setViewMatrix();

  m_Camera.setProjMatrix(mf_PIDIV4, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 10000.0f);
}

void mfPass::Update(mfDepthStencilView & _DepthStencilView, float _Time)
{
  // Set Render Targets
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->OMSetRenderTargets(4, m_gBuffer.RenderTargets, _DepthStencilView.getInterface().ID);
  // Clear the render target buffers
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->ClearRenderTargetView(m_gBuffer.RenderTargets[0], whiteClearcolor);
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->ClearRenderTargetView(m_gBuffer.RenderTargets[1], whiteClearcolor);
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->ClearRenderTargetView(m_gBuffer.RenderTargets[2], whiteClearcolor);
  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->ClearRenderTargetView(m_gBuffer.RenderTargets[3], whiteClearcolor);
  // Clear Depth Stencil View
  mfGraphic_API::getSingleton().ClearDepthStencilView(_DepthStencilView);

  mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->PSSetShaderResources(0, 3, m_Textures);

  // Set View buffer
  View.mView = XMMatrixTranspose(m_Camera.getViewMatrix());
  m_ViewBuffer.Update(&View);
  // Set Projection Buffer
  Projection.mProjection = XMMatrixTranspose(m_Camera.getProjMatrix());
  m_ProjBuffer.Update(&Projection);

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

void mfPass::Render()
{
  // Set view Const Buffer
  m_ViewBuffer.Render(0, 1, false);
  // Set projection Const Buffer
  m_ProjBuffer.Render(1, 1, false);

  // Draw Game Object
  m_GameObject.Render();
}

void mfPass::Destroy()
{
  m_ViewBuffer.Destroy();
  m_ProjBuffer.Destroy();
  //mfBasePass::Destroy();

  for (int i = 0; i < 4; i++)
  {
    m_gBuffer.RenderTargetTextures[i]->Release();
    m_gBuffer.RenderTargets[i]->Release();
  }
  m_Textures[0]->Release();
  m_Textures[1]->Release();
  m_Textures[2]->Release();

  m_VertexShader.Destroy();
  m_PixelShader.Destroy();
  m_InputLayout.Destroy();
  m_GameObject.Destroy();
  m_SamplerState.Destroy();
  m_Rasterizer.Destroy();
}

mfgBufferPassID & mfPass::getInterface()
{
  return m_gBuffer;
}

mfCamera & mfPass::getCamera()
{
  return m_Camera;
}
