class CDirectX
{
public:

    PDIRECT3D9              pD3d;
    PDIRECT3DDEVICE9        pDevice;
    D3DPRESENT_PARAMETERS   pParameters;

    CDirectX(CWindow* Window )
    {
        this->pD3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (!this->pD3d)
            return;

        ZeroMemory(&this->pParameters, sizeof(D3DPRESENT_PARAMETERS));
        this->pParameters.Windowed = TRUE;
        this->pParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        this->pParameters.BackBufferFormat = D3DFMT_UNKNOWN;
        this->pParameters.EnableAutoDepthStencil = TRUE;
        this->pParameters.AutoDepthStencilFormat = D3DFMT_D16;
        this->pParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        this->pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window->Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pParameters, &pDevice);
    }

    VOID cleanupDevice()
    {
        if (this->pDevice)
        {
            this->pDevice->Release();
            this->pDevice = NULL;
        }

        if (this->pD3d)
        {
            this->pD3d->Release();
            this->pD3d = NULL;
        }
    }

    HRESULT resetDevice()
    {
        return this->pDevice->Reset(&this->pParameters);
    }
};