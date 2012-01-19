#ifndef MISSUNITEIMAGE_H
#define MISSUNITEIMAGE_H

#include <vector>
#include <map>

class MissUniteImage
{
    public:
        struct ImageInfo
        {
            int nX;
            int nY;
            wxBitmap bImage;
        };
        struct PointInfo
        {
            int nX;
            int nY;
            wxColor cPoint;
        };
        MissUniteImage();
        virtual ~MissUniteImage();
        void SetWH(int nWidth, int nHeight);
        void SetImages(const std::vector<ImageInfo>& vecImages);
        void SetImages(const std::map<std::string,ImageInfo>& mapImages);
        void SetPoints(const std::vector<PointInfo>& vecPoints);
        void AddImage(const ImageInfo& info);
        void AddPoint(const PointInfo& info);

        void UniteImage( const wxString& strFilePath );
    protected:

    private:
        int m_nWidth,m_nHeight;
        std::vector<ImageInfo> m_vecImages;
        std::vector<PointInfo> m_vecPoints;
};

#endif // MISSUNITEIMAGE_H
