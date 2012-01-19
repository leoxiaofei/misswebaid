#include "MissWxBitmapWriter.h"

#include <wx/bitmap.h>
#include <wx/mstream.h>
#include <iostream>

class MissWxBitmapWriter::MissWxBitmapWriterImpl
{
public:
    //wxBitmap m_bitmap;
    wxMemoryOutputStream m_buffer;
};

MissWxBitmapWriter::MissWxBitmapWriter():
    m_pImpl(new MissWxBitmapWriterImpl())
{
    //ctor
}

MissWxBitmapWriter::~MissWxBitmapWriter()
{
    //dtor
}

size_t MissWxBitmapWriter::WriteCallback(char* ptr, size_t size, size_t nmemb)
{
    m_pImpl->m_buffer.Write(ptr,size * nmemb);
    return m_pImpl->m_buffer.LastWrite();
}

void MissWxBitmapWriter::GetBitmap(wxBitmap& des)
{
    wxMemoryInputStream stream(m_pImpl->m_buffer);
    wxImage img(stream);
    des = wxBitmap(img);
}
