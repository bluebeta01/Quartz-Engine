#include "c_renderwindow.h"

RenderWindow::RenderWindow()
{

}

void RenderWindow::render(ID3D11Device* device)
{
	if (m_enabled)
	{
        if (m_framebuffer)
        {
            if (m_framebufferView)
                m_framebufferView->Release();

            //Create shader resource view
            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
            memset(&SRVDesc, 0, sizeof(SRVDesc));
            SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRVDesc.Texture2D.MipLevels = 1;
            
            
            HRESULT r = device->CreateShaderResourceView(m_framebuffer->m_renderTexture, &SRVDesc, &m_framebufferView);
        }

        static float f = 0.0f;
        static int counter = 0;


        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        
        
        ImVec2 v;
        v.x = ImGui::GetWindowSize().x;
        v.y = ImGui::GetWindowSize().y - 40;

        ImVec2 size = v;
        if (m_width != size.x || m_height != size.y)
        {
            m_resized = true;
        }
        else
        {
            m_resized = false;
        }
        m_width = size.x;
        m_height = size.y;

        
        
        //LOGDEBUG(ImGui::GetCursorScreenPos().x);
        POINT p;
        GetCursorPos(&p);
        GameWindow::s_cursorPosInRenderArea.x = p.x - ImGui::GetCursorScreenPos().x;
        GameWindow::s_cursorPosInRenderArea.y = p.y - ImGui::GetCursorScreenPos().y;
        //LOGDEBUG(p.y - ImGui::GetCursorScreenPos().y);

        ImGui::Image(m_framebufferView, v);

        
        ImGui::End();
	}
}