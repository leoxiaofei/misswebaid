#include "MissFileWriter.h"

#include <fstream>

class MissFileWriter::MissFileWriterImpl
{
public:
    std::ofstream m_ofOut;
};

MissFileWriter::MissFileWriter(const std::string & strName):
m_pImpl(new MissFileWriterImpl())
{
    //ctor
    m_pImpl->m_ofOut.open(strName.c_str(),std::ios::out|std::ios::binary );
}

MissFileWriter::~MissFileWriter()
{
    //dtor
}

size_t MissFileWriter::WriteFileCallback(char* ptr, size_t size, size_t nmemb)
{
    // Calculate the real size of the incoming buffer
    size_t realsize = size * nmemb;

    m_pImpl->m_ofOut.write(ptr, realsize);

    // return the real size of the buffer...
    return realsize;
};
