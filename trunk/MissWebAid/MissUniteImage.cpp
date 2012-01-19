#include <wx/bitmap.h>
#include <wx/colour.h>
#include "MissUniteImage.h"
#include <wx/dcmemory.h>
#include <functional>
#include <algorithm>

template <typename FirstT, typename SecondT>
struct second : public std::unary_function<std::pair<FirstT, SecondT>, SecondT>
{
    const SecondT& operator()(const std::pair<FirstT, SecondT>& value) const throw()
    {
        return (value.second);
    }
};

MissUniteImage::MissUniteImage()
{
    //ctor
}

MissUniteImage::~MissUniteImage()
{
    //dtor
}

void MissUniteImage::SetWH(int nWidth, int nHeight)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
}

void MissUniteImage::AddImage(const ImageInfo& info)
{
    m_vecImages.push_back(info);
}

void MissUniteImage::AddPoint(const PointInfo& info)
{
    m_vecPoints.push_back(info);
}

void MissUniteImage::SetImages(const std::vector<ImageInfo>& vecImages)
{
    m_vecImages = vecImages;
}

void MissUniteImage::SetPoints(const std::vector<PointInfo>& vecPoints)
{
    m_vecPoints = vecPoints;
}

void MissUniteImage::UniteImage(const wxString& strFilePath)
{
    wxBitmap bSrc(m_nWidth, m_nHeight, 32);
    wxMemoryDC memdc(bSrc);
    for(std::vector<ImageInfo>::iterator itor = m_vecImages.begin();
        itor != m_vecImages.end(); ++itor)
    {
        memdc.DrawBitmap(itor->bImage, itor->nX,itor->nY);
    }

    for(std::vector<PointInfo>::iterator itor = m_vecPoints.begin();
        itor != m_vecPoints.end(); ++itor)
    {
        memdc.SetPen(wxPen(itor->cPoint));
        memdc.DrawPoint(itor->nX, itor->nY);
        //memdc.DrawBitmap(itor->bImage,itor->nX,itor->nY);
    }
    bSrc.SaveFile(strFilePath,wxBITMAP_TYPE_PNG );
}

void MissUniteImage::SetImages(const std::map<std::string, ImageInfo>& mapImages)
{
    m_vecImages.clear();
    std::transform(mapImages.begin(), mapImages.end(), std::back_inserter(m_vecImages), second<std::string, ImageInfo>());
}
