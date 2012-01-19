#ifndef MISSWXBITMAPWRITER_H
#define MISSWXBITMAPWRITER_H

#include <cstddef>
#include <tr1/memory>
class wxBitmap;

class MissWxBitmapWriter
{
    class MissWxBitmapWriterImpl;
    public:
        MissWxBitmapWriter();
        virtual ~MissWxBitmapWriter();
        size_t WriteCallback(char* ptr, size_t size, size_t nmemb);
        void GetBitmap(wxBitmap& des);

    protected:
    private:
        std::tr1::shared_ptr<MissWxBitmapWriterImpl> m_pImpl;
};

#endif // MISSWXBITMAPWRITER_H
