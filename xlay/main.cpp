#include "lay6.h"
#include <stdio.h>
#include <xmllite.h>

#pragma comment(lib, "xmllite.lib")

class StdOut : public ISequentialStream
{
private:
    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override
    {
        if (IsEqualIID(riid, IID_ISequentialStream))
        {
            *ppvObject = (ISequentialStream *) this;
            return S_OK;
        }
        else if (IsEqualIID(riid, IID_IUnknown))
        {
            *ppvObject = (IUnknown *) this;
            return S_OK;
        }
        else
        {
            return E_NOINTERFACE;
        }
    }

    ULONG STDMETHODCALLTYPE AddRef(void) override
    {
        return 1;
    }

    ULONG STDMETHODCALLTYPE Release(void) override
    {
        return 1;
    }

    // ISequentialStream
    HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead) override
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE Write(const void *pv, ULONG cb, ULONG *pcbWritten) override
    {
        const char *pc = (const char *) pv;
        *pcbWritten = cb;
        while (cb > 0)
        {
            putc(*pc++, stdout);
            cb--;
        }
        return S_OK;
    }
};

int main()
{
    IXmlWriter *pXmlWriter;
    StdOut stdOut;
    FILE *lay;
    HRESULT hr;

    errno = fopen_s(&lay, "C:\\Users\\Sergey.Raevskiy\\Downloads\\test4.lay6", "rb");
    if (errno)
    {
        printf("fopen_s() failed: %d\n", errno);
        return EXIT_FAILURE;
    }

    hr = CreateXmlWriter(IID_IXmlWriter, (void **) &pXmlWriter, NULL);
    if (FAILED(hr))
    {
        printf("CreateXmlWriter() failed: %x\n", hr);
        return EXIT_FAILURE;
    }

    pXmlWriter->SetOutput(&stdOut);
    pXmlWriter->SetProperty(XmlWriterProperty_Indent, TRUE);

    pXmlWriter->WriteStartElement(NULL, L"xlay", NULL);

    LAY_FileHeader fileHeader;
    fread_s(&fileHeader, sizeof(fileHeader), sizeof(fileHeader), 1, lay);

    for (int i = 0; i < fileHeader.num_boards; i++)
    {
        LAY_BoardHeader bhdr;
        fread_s(&bhdr, sizeof(bhdr), sizeof(bhdr), 1, lay);
        CStringW wname(bhdr.name.str());
        CStringW sizeX;
        CStringW sizeY;
        CStringW zoom;
        CStringW centerX;
        CStringW centerY;
        //CStringW unk(bhdr.unk22.str());

        sizeX.Format(L"%d", bhdr.size_x);
        sizeY.Format(L"%d", bhdr.size_y);
        zoom.Format(L"%d", UINT(bhdr.zoom));
        centerX.Format(L"%u", bhdr.center_x);
        centerY.Format(L"%u", bhdr.center_y);

        pXmlWriter->WriteStartElement(NULL, L"bhdr", NULL);
        pXmlWriter->WriteAttributeString(NULL, L"name", NULL, wname);
        pXmlWriter->WriteAttributeString(NULL, L"sizeX", NULL, sizeX);
        pXmlWriter->WriteAttributeString(NULL, L"sizeY", NULL, sizeY);
        pXmlWriter->WriteAttributeString(NULL, L"activeGridStep", NULL, CStringW(bhdr.active_grid_val.str()));
        pXmlWriter->WriteAttributeString(NULL, L"zoom", NULL, zoom);
        pXmlWriter->WriteAttributeString(NULL, L"centerX", NULL, centerX);
        pXmlWriter->WriteAttributeString(NULL, L"centerY", NULL, centerY);
        pXmlWriter->WriteEndElement();
    }

    pXmlWriter->WriteEndElement();

    pXmlWriter->Flush();

    printf("%d\n", sizeof(LAY_BoardHeader));
    printf("%x\n", FIELD_OFFSET(LAY_BoardHeader, size_x));
    printf("%x\n", FIELD_OFFSET(LAY_BoardHeader, layer_visible));
    system("pause");
}
