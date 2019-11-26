#include "mfRenderManager.h"



mfRenderManager::mfRenderManager()
{
}


mfRenderManager::~mfRenderManager()
{
}

HRESULT mfRenderManager::Init()
{
  HRESULT hr = S_OK;
#ifdef mfDIRECTX
  RECT rc;
  GetClientRect(m_Window.getInterface().ID, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

#endif // mfDIRECTX


  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  mf_DRIVER_TYPE driverTypes[] =
  { 
      mf_DRIVER_TYPE_HARDWARE,
      mf_DRIVER_TYPE_WARP,
      mf_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE(driverTypes);

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  // 3) InitSwapchain
  mfSwapchainDesc swapchainDesc;
  swapchainDesc.BufferCount = 1;
  swapchainDesc.Width = SCREEN_WIDTH;
  swapchainDesc.Height = SCREEN_HEIGHT;
  swapchainDesc.Format = mf_FORMAT_R8G8B8A8_UNORM;
  swapchainDesc.Numerator = 60;
  swapchainDesc.Denominator = 1;
  swapchainDesc.BufferUsage = mf_USAGE_RENDER_TARGET_OUTPUT;
#ifdef mfDIRECTX
  swapchainDesc.OutputWindow = m_Window.getInterface().ID;
#endif // mfDIRECTX
  swapchainDesc.Count = 1;
  swapchainDesc.Quality = 0;
  swapchainDesc.Windowed = TRUE;

  m_Swapchain.Init(swapchainDesc);

  // 4) Set Device and Swap chain
#ifdef mfDIRECTX
  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
  {
    m_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain
    (
      NULL,
      (D3D_DRIVER_TYPE)m_driverType,
      NULL,
      createDeviceFlags,
      featureLevels,
      numFeatureLevels,
      D3D11_SDK_VERSION,
      &m_Swapchain.getInterface().Desc,
      &m_Swapchain.getInterface().ID,
      &mfGraphic_API::getSingleton().GetprtDev(),
      &m_featureLevel,
      &mfGraphic_API::getSingleton().GetprtDevCtx()
    );
    if (SUCCEEDED(hr))
      break;
  }
  if (FAILED(hr))
    return hr;
#endif // mfDIRECTX

   /**
   * @brief : Initialization of Depth Stencil
   */
  mfBaseTextureDesc depthStencilDesc;
  depthStencilDesc.Tag = "DEPTH_STENCIL";
  depthStencilDesc.ID = mf_DEPTH_STENCIL_TEXTURE;
  depthStencilDesc.Width = SCREEN_WIDTH;
  depthStencilDesc.Height = SCREEN_HEIGHT;
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.Arraysize = 1;
  depthStencilDesc.TexFormat = mf_FORMAT_D24_UNORM_S8_UINT;
  depthStencilDesc.SamplerDescCount = 1;
  depthStencilDesc.SamplerDescQuality = 0;
  depthStencilDesc.Usage = mf_USAGE_DEFAULT;
  depthStencilDesc.Bindflags = mf_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags = 0;
  depthStencilDesc.Filepath = NULL;

  m_DepthStencil.Init(depthStencilDesc);

  mfBufferDesc ViewBufferDesc;
  ViewBufferDesc.Usage = mf_USAGE_DEFAULT;
  ViewBufferDesc.ByteWidth = sizeof(CBNeverChanges);
  ViewBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ViewBufferDesc.CPUAccessFlags = 0;

  mfBufferDesc ProjBufferDesc;
  ProjBufferDesc.Usage = mf_USAGE_DEFAULT;
  ProjBufferDesc.ByteWidth = sizeof(CBChangeOnResize);
  ProjBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ProjBufferDesc.CPUAccessFlags = 0;

  mfBufferDesc ModelBufferDesc;
  ModelBufferDesc.Usage = mf_USAGE_DEFAULT;
  ModelBufferDesc.ByteWidth = sizeof(CBChangesEveryFrame);
  ModelBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ModelBufferDesc.CPUAccessFlags = 0;

  mfBufferDesc SAQBufferDesc;
  SAQBufferDesc.Usage = mf_USAGE_DEFAULT;
  SAQBufferDesc.ByteWidth = sizeof(CBChangesEveryFrame);
  SAQBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  SAQBufferDesc.CPUAccessFlags = 0;

  mfCameraDesc gBufferCameraDesc;
  gBufferCameraDesc.Eye = Vector4{ 200.0f, 0.0f, 0.0f, 0.0f };
  gBufferCameraDesc.Up = Vector4{ 0.0f, 1.0f, 0.0f, 0.0f };
  gBufferCameraDesc.LookAt = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };

  gBufferCameraDesc.Front = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
  gBufferCameraDesc.Right = Vector4{ 1.0f, 0.0f, 0.0f, 0.0f };

  mfSamplerDesc SamplerStateDesc;
  SamplerStateDesc.Filter = mf_FILTER_MIN_MAG_MIP_LINEAR;
  SamplerStateDesc.AddressU = mf_TEXTURE_ADDRESS_WRAP;
  SamplerStateDesc.AddressV = mf_TEXTURE_ADDRESS_WRAP;
  SamplerStateDesc.AddressW = mf_TEXTURE_ADDRESS_WRAP;
  SamplerStateDesc.ComparisionFunc = mf_COMPARISON_NEVER;
  SamplerStateDesc.MinLOD = 0;
  SamplerStateDesc.MaxLOD = mf_FLOAT32_MAX;

  mfRasterizerDesc RasterizerDesc;
  RasterizerDesc.CullMode = mf_CULL_NONE;
  RasterizerDesc.FillMode = mf_FILL_SOLID;

  mfRasterizerDesc RasterizerLightDesc;
  RasterizerLightDesc.CullMode = mf_CULL_NONE;
  RasterizerLightDesc.FillMode = mf_FILL_SOLID;

  mfInputLayoutDesc InputlayoutDesc;
  InputlayoutDesc.Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };

  mfInputLayoutDesc Light_InputlayoutDesc;
  Light_InputlayoutDesc.Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  Light_InputlayoutDesc.Desc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  Light_InputlayoutDesc.Desc[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };
  Light_InputlayoutDesc.Desc[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };
  /**
   * @brief : Load Model from file
   */
  //m_LoadModel.Init("Meshes/drakefire-pistol/drakefire_pistol_low.obj");
  //m_LoadModel.Init("Meshes/ice/source/LP.obj");
  //m_LoadModel.Init("Meshes/M-FF_iOS_HERO_Peter_Parker_Spider-Man_All_New/AllNew.obj");
  m_LoadModel.Init("Meshes/Axe/gun.obj");
  /**
   * @brief : Load SAQ from file
   */
  m_loadSAQ.Init("Meshes/drakefire-pistol/ScreenAlignedQuad.3ds");

  /**
   * @brief : Initialization of the viewport
   */
  mfViewportDesc ViewportDesc;
  ViewportDesc.Width = (FLOAT)SCREEN_WIDTH;
  ViewportDesc.Height = (FLOAT)SCREEN_HEIGHT;
  ViewportDesc.MinDepth = 0.0f;
  ViewportDesc.MaxDepth = 1.0f;
  ViewportDesc.TopLeftX = 0;
  ViewportDesc.TopLeftY = 0;

  m_Viewport.Init(ViewportDesc);

  mfOpenFileDesc ModelFileDesc;
  ModelFileDesc.hwndOwner = NULL;
  ModelFileDesc.lpstrFilter = L" Obj Files\0*.obj\0 FBX Files\0*.fbx\0 3DS Files\0*.3ds\0  All files\0*.*\0";
  ModelFileDesc.nMaxFile = MAX_PATH;
  ModelFileDesc.lpstrTitle = L"Select a model file";
  ModelFileDesc.lpstrInitialDir = L"Meshes\\";
  ModelFileDesc.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

  OpenModelFile.Init(ModelFileDesc);

  mfOpenFileDesc TextureFileDesc;
  TextureFileDesc.hwndOwner = NULL;
  TextureFileDesc.lpstrFilter = L" PNG Files\0*.png\0 JPEG Files\0*.jpeg\0 JPG Files\0*.jpg\0";
  TextureFileDesc.nMaxFile = MAX_PATH;
  TextureFileDesc.lpstrTitle = L"Select a texture file";
  TextureFileDesc.lpstrInitialDir = L"Textures\\";
  TextureFileDesc.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

  OpenTextureFile.Init(TextureFileDesc);

  /**
   * @brief : Initialize gBuffer Pass
   */
  mfBasePassDesc gBuffer_Pass_Desc;
  gBuffer_Pass_Desc.VertexShaderFileName = L"GBuffer.fx";
  gBuffer_Pass_Desc.PixelShaderFileName = L"GBuffer.fx";
  gBuffer_Pass_Desc.RawData = m_LoadModel.getInterface();
  gBuffer_Pass_Desc.ViewBufferDesc = ViewBufferDesc;
  gBuffer_Pass_Desc.ProjBufferDesc = ProjBufferDesc;
  gBuffer_Pass_Desc.ModelBufferDesc = ModelBufferDesc;
  gBuffer_Pass_Desc.CameraDesc = gBufferCameraDesc;
  gBuffer_Pass_Desc.SamplerDesc = SamplerStateDesc;
  gBuffer_Pass_Desc.RasterizerDesc = RasterizerDesc;
  gBuffer_Pass_Desc.InputLayoutDesc = InputlayoutDesc;

  m_gBuffer_Pass.Init(gBuffer_Pass_Desc);


  mfLightPassDesc Light_Pass_Desc1;
  Light_Pass_Desc1.RenderTargetsTex_Ref[0] = m_gBuffer_Pass.getInterface().RenderTargetTextures[0];
  Light_Pass_Desc1.RenderTargetsTex_Ref[1] = m_gBuffer_Pass.getInterface().RenderTargetTextures[1];
  Light_Pass_Desc1.RenderTargetsTex_Ref[2] = m_gBuffer_Pass.getInterface().RenderTargetTextures[2];
  Light_Pass_Desc1.RenderTargetsTex_Ref[3] = m_gBuffer_Pass.getInterface().RenderTargetTextures[3];
  Light_Pass_Desc1.VertexShaderFileName = L"Light.fx";
  Light_Pass_Desc1.PixelShaderFileName = L"Light.fx";
  Light_Pass_Desc1.RawData = m_loadSAQ.getInterface();
  Light_Pass_Desc1.ViewBufferDesc = ViewBufferDesc;
  Light_Pass_Desc1.ProjBufferDesc = ProjBufferDesc;
  Light_Pass_Desc1.CameraDesc = gBufferCameraDesc;
  Light_Pass_Desc1.ModelBufferDesc = SAQBufferDesc;
  Light_Pass_Desc1.SamplerDesc = SamplerStateDesc;
  Light_Pass_Desc1.RasterizerDesc = RasterizerLightDesc;
  Light_Pass_Desc1.InputLayoutDesc = Light_InputlayoutDesc;

 m_Light_Pass.Init(Light_Pass_Desc1, m_Swapchain);
  /**
 * @brief : Initialization of Depth Stencil View
 */
  mfDepthStencilViewDesc depthStencilViewDesc;
  depthStencilViewDesc.texFormat = depthStencilDesc.TexFormat/*descDepth.Format*/;
  depthStencilViewDesc.ViewDimension = mf_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDesc.MipSlice = 0;
#ifdef mfDIRECTX
  depthStencilViewDesc.Resource = m_DepthStencil.getInterface().ID;
#endif // mfDIRECTX

  m_DepthStencilView.Init(depthStencilViewDesc);

#ifdef mfDIRECTX
  m_Light_Pass.m_backbuffer->Release();
#endif // mfDIRECTX
  if (FAILED(hr))
    return hr;
  return S_OK;
}

void mfRenderManager::Update()
{
  // Update our time
  if (m_driverType == mf_DRIVER_TYPE_REFERENCE)
  {
    m_Time += (float)mf_PI * 0.0125f;
  }
  else
  {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    m_Time = (dwTimeCur - dwTimeStart) / 1000.0f;
  }
  /**
   * @brief : Update gBuffer Pass
   */
  m_gBuffer_Pass.Update(m_DepthStencilView, m_Time);
  m_Light_Pass.Update(m_DepthStencilView, m_gBuffer_Pass.getCamera().getDescriptor().Eye);
//   mfGraphic_API::getSingleton().GetDeviceContext().getInterface().ID->OMSetRenderTargets(1, &m_RenderTarget.getInterfaceRT().ID, m_DepthStencilView.getInterface().ID);
}

void mfRenderManager::Render()
{
  /**
   * @brief : Render gBuffer Pass
   */
  m_gBuffer_Pass.Render();
  m_Light_Pass.Render();
  m_Swapchain.Present(0, 0);
}

void mfRenderManager::Destroy()
{
  mfGraphic_API::getSingleton().Destroy();
  m_Swapchain.Destroy();
  m_DepthStencil.Destroy();
  m_DepthStencilView.Destroy();
  /**
   * @brief : Release gBuffer Pass 
   */
  m_gBuffer_Pass.Destroy();
  m_Light_Pass.Destroy();
}

void mfRenderManager::InitWindow(WNDPROC _wndProc, HINSTANCE _HInstance, int _CmdShow)
{
  mfRegisterClassWindowDesc wndRegisterClass;
  wndRegisterClass.cbSize = sizeof(WNDCLASSEX);
  wndRegisterClass.style = CS_HREDRAW | CS_VREDRAW;
  wndRegisterClass.lpfnWndProc = _wndProc;
  wndRegisterClass.cbClsExtra = 0;
  wndRegisterClass.cbWndExtra = 0;
  wndRegisterClass.hInstance = _HInstance;
  wndRegisterClass.hIcon = LoadIcon(_HInstance, (LPCTSTR)IDI_TUTORIAL1);
  wndRegisterClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndRegisterClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wndRegisterClass.lpszMenuName = NULL;
  wndRegisterClass.lpszClassName = L"TutorialWindowClass";
  wndRegisterClass.hIconSm = LoadIcon(_HInstance, (LPCTSTR)IDI_TUTORIAL1);
  mfWindowDesc wndDesc;
  wndDesc.width = SCREEN_WIDTH;
  wndDesc.height = SCREEN_HEIGHT;
  wndDesc.nCmdShow = _CmdShow;

  m_Window.Init(wndRegisterClass, wndDesc);
}
