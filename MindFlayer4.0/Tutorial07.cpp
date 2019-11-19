/**
 * @LC	   : 15/10/2019
 * @file   : Tutorial07.cpp
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 04/10/2019
 * @brief  : Main document in charge of setting and initializing multiple classes.
 * @bug	   : No known bugs.
 */

/**
 * Headers
 */
#include "mfDefines.h"
#include "mfRenderManager.h"
/**
 * Global Variables
 */
mfRenderManager RenderManager;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    /**
     * @brief : Initialize the module of Render Manager
     */
    mfGraphic_API::Prepare();

    RenderManager.InitWindow(WndProc, hInstance, nCmdShow);
    if( FAILED( RenderManager.Init() ) )
    {
       RenderManager.Destroy();
        return 0;
    }

    /**
     * @brief : Initialize ImGui
     */
#ifdef mfDIRECTX
//     Imgui.Init
//     (
//       mfGraphic_API::getSingleton().GetDevice(), 
//       mfGraphic_API::getSingleton().GetDeviceContext(), 
//       Window
//     );
#endif // mfDIRECTX

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
      if (msg.message == WM_KEYDOWN)
      {
        XMVECTOR rot;
        Vector3 movement{ 0.0f, 0.0f, 0.0f };
        switch (msg.wParam)
        {
        case VK_LEFT:
        {
          movement.x -= 1;
          break;
        }

        case VK_RIGHT:
        {
          movement.x += 1;
          break;
        }
        case VK_DOWN :
          movement.y -= .001f;
          break;
        case VK_UP:
          movement.y += .001f;
          break;

        case VK_PRIOR :
        {
          movement.z -= 1;
          break;
        }

        case VK_NEXT:
        {
          movement.z += 1;
          break;
        }
        case VK_ESCAPE:
        {
          exit(1);
          break;
        }
        default:
          break;
        }
//         Camera.move(&movement);
// 
//         CBNeverChanges cbNeverChanges;
//         cbNeverChanges.mView = XMMatrixTranspose(Camera.getViewMatrix());
//         NeverChangesBuffer.Update(&cbNeverChanges);
//         Camera.setProjMatrix(mf_PIDIV4, SCREEN_WIDTH, SCREEN_HEIGHT, 0.01f, 100.0f);
      }

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
          RenderManager.Update();
          RenderManager.Render();
        }
    }

    RenderManager.Destroy();
    
    /**
     * @brief : Release the Render Manager Module
     */
    mfGraphic_API::ShutDown();

    return ( int )msg.wParam;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
      return true;
    }
    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}
