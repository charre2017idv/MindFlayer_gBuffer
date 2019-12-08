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

  m_ImGui.Init(mfGraphic_API::getSingleton().GetDevice(), mfGraphic_API::getSingleton().GetDeviceContext(), m_Window);

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

  /**
   * @brief : Load Model from file
   */
  //m_LoadModel.Init("Meshes/drakefire-pistol/drakefire_pistol_low.obj");
  //m_LoadModel.Init("Meshes/ice/source/LP.obj");
  //m_LoadModel.Init("Meshes/M-FF_iOS_HERO_Peter_Parker_Spider-Man_All_New/AllNew.obj");
  
  /**
   * @brief : Load SAQ from file
   */
  

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

  m_gBuffer_Effect.CreateTechniques(3);

  Initialize_gBuffer();
  m_gBuffer_Effect.getTechnique(GBUFFER_PASS);
  
  Initialize_Light_Pass();
  m_gBuffer_Effect.getRenderTargets(LIGHT_PASS);

  Initialize_CopyBB_Pass(m_gBuffer_Effect.m_LightRT);


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
  //m_Light_Pass.m_backbuffer->Release();
#endif // mfDIRECTX
  if (FAILED(hr))
    return hr;

  mfBufferDesc CameraBufferDesc;
  CameraBufferDesc.Usage = mf_USAGE_DEFAULT;
  CameraBufferDesc.ByteWidth = sizeof(CB_CameraBuffer);
  CameraBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  CameraBufferDesc.CPUAccessFlags = 0;

  mfCameraDesc gBufferCameraDesc;

  gBufferCameraDesc.CameraBufferDesc = CameraBufferDesc;

  gBufferCameraDesc.Eye = Vector4{ 200.0f, 0.0f, 0.0f, 0.0f };
  gBufferCameraDesc.Up = Vector4{ 0.0f, 1.0f, 0.0f, 0.0f };
  gBufferCameraDesc.LookAt = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };

  gBufferCameraDesc.Front = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
  gBufferCameraDesc.Right = Vector4{ 1.0f, 0.0f, 0.0f, 0.0f };
 
  gBufferCameraDesc.FOV = mf_PIDIV4;
  gBufferCameraDesc.Width = SCREEN_WIDTH;
  gBufferCameraDesc.Height = SCREEN_HEIGHT; 
  gBufferCameraDesc.Near = 0.1f;
  gBufferCameraDesc.Far = 10000.0f;

  m_Camera.Init(gBufferCameraDesc);

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


  m_Camera.Update();

  // Update gBuffer Effect
  m_Shader_Light_Attributes.Position = { Position[0], Position[1], Position[2], Position[3] };
  m_Shader_Light_Attributes.lightDir = { lightDir[0], lightDir[1], lightDir[2], lightDir[3] };
  m_Shader_Light_Attributes.lightDirColor = { lightDirColor[0], lightDirColor[1], lightDirColor[2], lightDirColor[3] };
  m_Shader_Light_Attributes.ambientColor = { ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3] };
  m_Shader_Light_Attributes.specularColor = { specularColor[0], specularColor[1], specularColor[2], specularColor[3] };
  m_Shader_Light_Attributes.specPower = specPower;
  m_Shader_Light_Attributes.kDiffuse  = kDiffuse;
  m_Shader_Light_Attributes.kAmbient  = kAmbient;
  m_Shader_Light_Attributes.kSpecular = kSpecular;

  if (isRotating)
  {
    Rotation[1] = m_Time;
  }
  m_Transform.setModelMatrix(m_Translate, m_Rotation, m_Scale,0);
  m_Translate = { Translate[0],Translate[1],Translate[2] };
  m_Rotation = { Rotation[0], Rotation[1],Rotation[2] };
  m_Scale = { Scale[0], Scale[1], Scale[2] };

  m_gBuffer_Effect.Update(m_DepthStencilView, m_Camera, m_Transform, &m_Shader_Light_Attributes, m_Time);
}

void mfRenderManager::Render()
{
  /**
   * @brief : Render gBuffer Pass
   */
  m_gBuffer_Effect.Render(m_DepthStencilView); 
  m_ImGui.Update();
  //m_ImGui.AestheticWindow(m_gBuffer_Effect.m_gBufferRT);
  
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= /*ImGuiWindowFlags_NoCollapse |*/ ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::SetNextWindowSize(ImVec2(450, 280), ImGuiCond_Always);
  ImGui::SetWindowPos(ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Set window background to red
  ImGui::Begin("Light Settings", NULL, window_flags);
  ImGui::PopStyleVar(2);

  ImGui::SliderFloat4("Position", Position, -200, 200);
  ImGui::SliderFloat4("Light Direction", lightDir, -200, 200);
  ImGui::SliderFloat4("Light Dir Color", lightDirColor, -1, 1);
  ImGui::SliderFloat4("Ambient Color", ambientColor, -0, 1);
  ImGui::SliderFloat4("Specular Color", specularColor, -200, 200);
  ImGui::SliderFloat("Specular Power", &specPower, 0, 200);
  ImGui::SliderFloat("Diffuse", &kDiffuse, 0, 200);
  ImGui::SliderFloat("Ambient", &kAmbient, 0, 200);
  ImGui::SliderFloat("Specular", &kSpecular, 0, 1);

  if (ImGui::Button("Reset Settings"))
  {
    Position[0] = 200;
    Position[1] = 0;
    Position[2] = 0;
    Position[3] = 0;

    lightDir[0] = -115;
    lightDir[1] = 10;
    lightDir[2] = 1;
    lightDir[3] = 1;

    lightDirColor[0] = 1.0f;
    lightDirColor[1] = 1.0f;
    lightDirColor[2] = 1.0f;
    lightDirColor[3] = 1.0f;

    ambientColor[0] = 0.0f;
    ambientColor[1] = 0.0f;
    ambientColor[2] = 0.0f;
    ambientColor[3] = 0.0f;

    specularColor[0] = 1;
    specularColor[1] = 0;
    specularColor[2] = 0;
    specularColor[3] = 0;

    specPower = 15.0f;
    kDiffuse = 1.0f;
    kAmbient = 0.14f;
    kSpecular = 1.0f;
  }
  ImGui::PopStyleColor();
  ImGui::End();
  RTSelector_Window(m_gBuffer_Effect.m_gBufferRT);
  Transform_Window();
  m_ImGui.Render();
  m_Swapchain.Present(0, 0);
}

void mfRenderManager::Destroy()
{
  mfGraphic_API::getSingleton().Destroy();
  m_Swapchain.Destroy();
  m_DepthStencil.Destroy();
  m_DepthStencilView.Destroy();
  m_Camera.Destroy();
  /**
   * @brief : Release gBuffer Pass 
   */
  m_gBuffer_Effect.Destroy();
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

void mfRenderManager::Initialize_gBuffer()
{
  mfBaseTextureDesc RenderTargetsDesc;
  RenderTargetsDesc.Width = SCREEN_WIDTH;
  RenderTargetsDesc.Height = SCREEN_HEIGHT;
  RenderTargetsDesc.MipLevels = 1;
  RenderTargetsDesc.Arraysize = 1;
  RenderTargetsDesc.TexFormat = mf_FORMAT_R32G32B32A32_FLOAT;
  RenderTargetsDesc.SamplerDescCount = 1;
  RenderTargetsDesc.SamplerDescQuality = 0;
  RenderTargetsDesc.Usage = mf_USAGE_DEFAULT;
  RenderTargetsDesc.Bindflags = mf_BIND_RENDER_TARGET | mf_BIND_SHADER_RESOURCE;
  RenderTargetsDesc.CPUAccessFlags = 0;
  RenderTargetsDesc.MiscFlags = 0;
  RenderTargetsDesc.Filepath = NULL;

  mfBufferDesc ModelBufferDesc;
  ModelBufferDesc.Usage = mf_USAGE_DEFAULT;
  ModelBufferDesc.ByteWidth = sizeof(CBChangesEveryFrame);
  ModelBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ModelBufferDesc.CPUAccessFlags = 0;

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

  mfInputLayoutDesc InputlayoutDesc;
  InputlayoutDesc.Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };

  // Initialize Model
  //m_LoadModel.Init("Meshes/drakefire-pistol/drakefire_pistol_low.obj");
  m_LoadModel.Init("Meshes/gun.obj");
  //m_LoadModel.Init("Meshes/laser/Sword.fbx");

  // Initialize Vertex Shader
  mfBaseShaderDesc VertexShaderDesc;
  VertexShaderDesc.FileName = L"GBuffer.fx";
  VertexShaderDesc.EntryPoint = "VS";
  VertexShaderDesc.ShaderModel = "vs_4_0";

  // Initialize Pixel Shader
  mfBaseShaderDesc PixelShaderDesc;
  PixelShaderDesc.FileName = L"GBuffer.fx";
  PixelShaderDesc.EntryPoint = "PS";
  PixelShaderDesc.ShaderModel = "ps_4_0";

  mfBaseTextureDesc AlbedoTexDesc;
  //AlbedoTexDesc.Filepath = L"base_albedo.jpg";
  AlbedoTexDesc.Filepath = L"Gun_albedo.png";
  //AlbedoTexDesc.Filepath = L"Sword_albedo.png";
  //AlbedoTexDesc.Filepath = L"laser_albedo.png";

  mfBaseTextureDesc NormalTexDesc;
  //NormalTexDesc.Filepath = L"base_normal.jpg";
  NormalTexDesc.Filepath = L"Gun_normal.png";
  //NormalTexDesc.Filepath = L"Sword_normal.png";
  //NormalTexDesc.Filepath = L"laser_normal.png";

  mfBaseTextureDesc SpecularTexDesc;
  //SpecularTexDesc.Filepath = L"base_specular.jpg";
  SpecularTexDesc.Filepath = L"Gun_specular.png";
  //SpecularTexDesc.Filepath = L"Sword_specular.png";
  //SpecularTexDesc.Filepath = L"laser_specular.png";
  /**
   * @brief : Initialize gBuffer Pass
   */
  mfBasePassDesc gBuffer_Pass_Desc;
  gBuffer_Pass_Desc.PassID = GBUFFER_PASS;
  gBuffer_Pass_Desc.RenderTargetsDesc.push_back(RenderTargetsDesc);
  gBuffer_Pass_Desc.RenderTargetsDesc.push_back(RenderTargetsDesc);
  gBuffer_Pass_Desc.RenderTargetsDesc.push_back(RenderTargetsDesc);
  gBuffer_Pass_Desc.RenderTargetsDesc.push_back(RenderTargetsDesc);
  gBuffer_Pass_Desc.ModelTexturesDesc.push_back(AlbedoTexDesc);
  gBuffer_Pass_Desc.ModelTexturesDesc.push_back(NormalTexDesc);
  gBuffer_Pass_Desc.ModelTexturesDesc.push_back(SpecularTexDesc);
  gBuffer_Pass_Desc.RawData.push_back(m_LoadModel.getInterface());
  gBuffer_Pass_Desc.VertexShaderDesc = VertexShaderDesc;
  gBuffer_Pass_Desc.PixelShaderDesc = PixelShaderDesc;
  gBuffer_Pass_Desc.Topology = mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  gBuffer_Pass_Desc.ModelBufferDesc = ModelBufferDesc;
  gBuffer_Pass_Desc.SamplerDesc = SamplerStateDesc;
  gBuffer_Pass_Desc.RasterizerDesc = RasterizerDesc;
  gBuffer_Pass_Desc.InputLayoutDesc = InputlayoutDesc;

  m_gBuffer_Effect.Init(gBuffer_Pass_Desc, 0);
}

void mfRenderManager::Initialize_Light_Pass()
{
  mfBaseTextureDesc RenderTargetsDesc;
  RenderTargetsDesc.Width = SCREEN_WIDTH;
  RenderTargetsDesc.Height = SCREEN_HEIGHT;
  RenderTargetsDesc.MipLevels = 1;
  RenderTargetsDesc.Arraysize = 1;
  RenderTargetsDesc.TexFormat = mf_FORMAT_R32G32B32A32_FLOAT;
  RenderTargetsDesc.SamplerDescCount = 1;
  RenderTargetsDesc.SamplerDescQuality = 0;
  RenderTargetsDesc.Usage = mf_USAGE_DEFAULT;
  RenderTargetsDesc.Bindflags = mf_BIND_RENDER_TARGET | mf_BIND_SHADER_RESOURCE;
  RenderTargetsDesc.CPUAccessFlags = 0;
  RenderTargetsDesc.MiscFlags = 0;
  RenderTargetsDesc.Filepath = NULL;

  mfBufferDesc ModelBufferDesc;
  ModelBufferDesc.Usage = mf_USAGE_DEFAULT;
  ModelBufferDesc.ByteWidth = sizeof(CBChangesEveryFrame);
  ModelBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ModelBufferDesc.CPUAccessFlags = 0;

  mfBufferDesc LightPassDesc;
  LightPassDesc.Usage = mf_USAGE_DEFAULT;
  LightPassDesc.ByteWidth = sizeof(mfLightShaderAttributes);
  LightPassDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  LightPassDesc.CPUAccessFlags = 0;

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

  mfInputLayoutDesc InputlayoutDesc;
  InputlayoutDesc.Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };

  // Initialize Model
  m_loadSAQ.Init("SAQ.obj");

  // Initialize Vertex Shader
  mfBaseShaderDesc VertexShaderDesc;
  VertexShaderDesc.FileName = L"Light.fx";
  VertexShaderDesc.EntryPoint = "VS";
  VertexShaderDesc.ShaderModel = "vs_4_0";

  // Initialize Pixel Shader
  mfBaseShaderDesc PixelShaderDesc;
  PixelShaderDesc.FileName = L"Light.fx";
  PixelShaderDesc.EntryPoint = "PS";
  PixelShaderDesc.ShaderModel = "ps_4_0";
  mfBaseTextureDesc SpecularTexDesc;
  SpecularTexDesc.Filepath = L"base_albedo.jpg";

  mfBasePassDesc Light_Pass_Desc;
  Light_Pass_Desc.PassID = LIGHT_PASS;
  Light_Pass_Desc.RenderTargetsDesc.push_back(RenderTargetsDesc);
  Light_Pass_Desc.RawData.push_back(m_loadSAQ.getInterface());
  Light_Pass_Desc.VertexShaderDesc = VertexShaderDesc;
  Light_Pass_Desc.PixelShaderDesc = PixelShaderDesc;
  Light_Pass_Desc.Topology = mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  Light_Pass_Desc.ModelBufferDesc = ModelBufferDesc;
  Light_Pass_Desc.SamplerDesc = SamplerStateDesc;
  Light_Pass_Desc.RasterizerDesc = RasterizerDesc;
  Light_Pass_Desc.InputLayoutDesc = InputlayoutDesc;
  Light_Pass_Desc.LightBufferDesc = LightPassDesc;
  Light_Pass_Desc.Swapchain = m_Swapchain;
  Light_Pass_Desc.tmpRenderTargets = m_gBuffer_Effect.m_gBufferRT;
  // Pass texture references
  m_gBuffer_Effect.Init(Light_Pass_Desc, 1);
}

void mfRenderManager::Initialize_CopyBB_Pass(vector<mfRenderTarget> & _tmpRenderTarget)
{
  mfBaseTextureDesc RenderTargetsDesc;
  RenderTargetsDesc.Width = SCREEN_WIDTH;
  RenderTargetsDesc.Height = SCREEN_HEIGHT;
  RenderTargetsDesc.MipLevels = 1;
  RenderTargetsDesc.Arraysize = 1;
  RenderTargetsDesc.TexFormat = mf_FORMAT_R32G32B32A32_FLOAT;
  RenderTargetsDesc.SamplerDescCount = 1;
  RenderTargetsDesc.SamplerDescQuality = 0;
  RenderTargetsDesc.Usage = mf_USAGE_DEFAULT;
  RenderTargetsDesc.Bindflags = mf_BIND_RENDER_TARGET | mf_BIND_SHADER_RESOURCE;
  RenderTargetsDesc.CPUAccessFlags = 0;
  RenderTargetsDesc.MiscFlags = 0;
  RenderTargetsDesc.Filepath = NULL;

  mfBufferDesc ModelBufferDesc;
  ModelBufferDesc.Usage = mf_USAGE_DEFAULT;
  ModelBufferDesc.ByteWidth = sizeof(CBChangesEveryFrame);
  ModelBufferDesc.BindFlags = mf_BIND_CONSTANT_BUFFER;
  ModelBufferDesc.CPUAccessFlags = 0;

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

  mfInputLayoutDesc InputlayoutDesc;
  InputlayoutDesc.Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };
  InputlayoutDesc.Desc[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 };

  // Initialize Model
  //m_loadSAQ.Init("SAQ.obj");

  // Initialize Vertex Shader
  mfBaseShaderDesc VertexShaderDesc;
  VertexShaderDesc.FileName = L"copy.fx";
  VertexShaderDesc.EntryPoint = "VS";
  VertexShaderDesc.ShaderModel = "vs_4_0";

  // Initialize Pixel Shader
  mfBaseShaderDesc PixelShaderDesc;
  PixelShaderDesc.FileName = L"copy.fx";
  PixelShaderDesc.EntryPoint = "PS";
  PixelShaderDesc.ShaderModel = "ps_4_0";


  mfBasePassDesc Copy_Pass_Desc;
  Copy_Pass_Desc.PassID = BACKBUFFER_PASS;
  Copy_Pass_Desc.RenderTargetsDesc.push_back(RenderTargetsDesc);
  Copy_Pass_Desc.RawData.push_back(m_loadSAQ.getInterface());
  Copy_Pass_Desc.VertexShaderDesc = VertexShaderDesc;
  Copy_Pass_Desc.PixelShaderDesc = PixelShaderDesc;
  Copy_Pass_Desc.Topology = mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  Copy_Pass_Desc.ModelBufferDesc = ModelBufferDesc;
  Copy_Pass_Desc.SamplerDesc = SamplerStateDesc;
  Copy_Pass_Desc.RasterizerDesc = RasterizerDesc;
  Copy_Pass_Desc.InputLayoutDesc = InputlayoutDesc;
//  Copy_Pass_Desc.LightBufferDesc = LightPassDesc;
  Copy_Pass_Desc.Swapchain = m_Swapchain;
  Copy_Pass_Desc.tmpRenderTargets = _tmpRenderTarget;
  // Pass texture references
  m_gBuffer_Effect.Init(Copy_Pass_Desc, 2);
}

void mfRenderManager::RTSelector_Window(vector<mfRenderTarget>& _RenderTargets)
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::SetNextWindowSize(ImVec2(1110, 190), ImGuiCond_Always);
  ImGui::SetWindowPos(ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Set window background to red
  ImGui::Begin("Render Target Selector", NULL, window_flags);
  ImGui::PopStyleVar(2);
 
  ImGui::SameLine();
  if (ImGui::ImageButton(_RenderTargets[0].getInterface().ResourceViewID, ImVec2(200, 128)))
  {
    Initialize_CopyBB_Pass(m_gBuffer_Effect.m_posRT);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton(_RenderTargets[1].getInterface().ResourceViewID, ImVec2(200, 128)))
  {
    Initialize_CopyBB_Pass(m_gBuffer_Effect.m_norRT);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton(_RenderTargets[2].getInterface().ResourceViewID, ImVec2(200, 128)))
  {
    Initialize_CopyBB_Pass(m_gBuffer_Effect.m_albRT);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton(_RenderTargets[3].getInterface().ResourceViewID, ImVec2(200, 128)))
  {
    Initialize_CopyBB_Pass(m_gBuffer_Effect.m_speRT);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton(m_gBuffer_Effect.m_LightRT[0].getInterface().ResourceViewID, ImVec2(200, 128)))
  {
    Initialize_CopyBB_Pass(m_gBuffer_Effect.m_LightRT);
  }

  ImGui::PopStyleColor();
  ImGui::End();
}

void mfRenderManager::Transform_Window()
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::SetNextWindowSize(ImVec2(450, 150), ImGuiCond_Always);
  ImGui::SetWindowPos(ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Set window background to red
  ImGui::Begin("Transform", NULL, window_flags);
  ImGui::PopStyleVar(2);

  ImGui::SliderFloat3("Translate", Translate, -100, 100);
  ImGui::SliderFloat3("Rotate", Rotation, -mf_PIDIV4 * 2, mf_PIDIV4 * 2);
  ImGui::SliderFloat3("Scale", Scale, 2, 100);

  if (ImGui::Button("Auto Rotate ON"))
    isRotating = true;
  if (ImGui::Button("Auto Rotate OFF"))
    isRotating = false;
  if (ImGui::Button("Reset Mesh"))
  {
    Translate[0] = 0;
    Translate[1] = 0;
    Translate[2] = 0;
    Rotation[0] = 0;
    Rotation[1] = 0;
    Rotation[2] = 0;
    Scale[0] = 50;
    Scale[1] = 50;
    Scale[2] = 50;
    isRotating = false;
  }

  ImGui::PopStyleColor();
  ImGui::End();
}
