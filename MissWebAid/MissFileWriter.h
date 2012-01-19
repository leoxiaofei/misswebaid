#ifndef MISSFILEWRITER_H
#define MISSFILEWRITER_H

#include <cstddef>
#include <tr1/memory>

class MissFileWriter
{
    class MissFileWriterImpl;
    public:
        MissFileWriter(const std::string & strName);
        virtual ~MissFileWriter();
        size_t WriteFileCallback(char* ptr, size_t size, size_t nmemb);
    protected:
    private:
        std::tr1::shared_ptr<MissFileWriterImpl> m_pImpl;
};

#endif // MISSFILEWRITER_H
