// Morphological Operations & Connected Component Labeling Algorithms
// Extracted from KFC framework — student-implemented functions only
// These functions are not standalone-compilable; they were originally
// member functions of KImageGray operating on internal pixel arrays.

#include <vector>

// -----------------------------------------------------------------------
// 5x5 Binary Dilation
// -----------------------------------------------------------------------
KImageGray KImageGray::BinaryDilate2(const int& nType)
{
    KImageGray  igOut = *this;

    if (nType == _FOREGROUND)
    {
        for (int i = 2; i < Row() - 2; ++i)
            for (int j = 2; j < Col() - 2; ++j)
            {
                int mask = 0;
                for (int x = -2; x <= 2; ++x)
                    for (int y = -2; y <= 2; ++y)
                        mask |= _ppA[i + x][j + y] ? 1 : 0;

                if (mask)
                    igOut._ppA[i][j] = 255;
            }
    }
    else if (nType == _BACKGROUND)
    {
        for (int i = 2; i < Row() - 2; ++i)
            for (int j = 2; j < Col() - 2; ++j)
            {
                int mask = 1;
                for (int x = -2; x <= 2; ++x)
                    for (int y = -2; y <= 2; ++y)
                        mask &= _ppA[i + x][j + y] ? 1 : 0;

                if (mask)
                    igOut._ppA[i][j] = 0;
            }
    }

    return igOut;
}

// -----------------------------------------------------------------------
// 5x5 Binary Erosion
// -----------------------------------------------------------------------
KImageGray KImageGray::BinaryErode2(const int& nType)
{
    KImageGray  igOut = *this;

    if (nType == _FOREGROUND)
    {
        for (int i = 2; i < Row() - 2; ++i)
            for (int j = 2; j < Col() - 2; ++j)
            {
                int mask = 1;
                for (int x = -2; x <= 2; ++x)
                    for (int y = -2; y <= 2; ++y)
                        mask &= _ppA[i + x][j + y] ? 1 : 0;

                if (mask != 1)
                    igOut._ppA[i][j] = 0;
            }
    }
    else if (nType == _BACKGROUND)
    {
        for (int i = 2; i < Row() - 2; ++i)
            for (int j = 2; j < Col() - 2; ++j)
            {
                int mask = 0;
                for (int x = -2; x <= 2; ++x)
                    for (int y = -2; y <= 2; ++y)
                        mask |= _ppA[i + x][j + y] ? 1 : 0;

                if (mask != 1)
                    igOut._ppA[i][j] = 255;
            }
    }

    return igOut;
}

// -----------------------------------------------------------------------
// Blob Merge Utility for Labeling
// -----------------------------------------------------------------------
using KBlob = std::vector<KPOINT>;
using KBlobList = std::vector<KBlob*>;

void MergeBlobs(KBlobList& lBlobList, int blobIdx1, int blobIdx2) {
    KBlob* blob1 = lBlobList[blobIdx1];
    KBlob* blob2 = lBlobList[blobIdx2];
    blob1->insert(blob1->end(), blob2->begin(), blob2->end());
}

// -----------------------------------------------------------------------
// Connected Component Labeling — N4 Connectivity
// -----------------------------------------------------------------------
KImageColor KImageGray::ImageLabelingN4()
{
    KImageColor icColor(this->Row(), this->Col());
    int num = 1;

    // First pass — assign initial labels
    for(int i=0; i<Row(); i++)
        if(_ppA[i][0] != 0) icColor._ppA[i][0].r = num;
        else num++;

    for(int j=1; j<Col(); j++)
        if(_ppA[0][j] != 0) icColor._ppA[0][j].r = num;
        else num++;

    for(int i=1; i<Row(); i++)
        for(int j=1; j<Col(); j++)
        {
            if(_ppA[i][j] != 0)
            {
                if(_ppA[i][j-1] != 0)
                    icColor._ppA[i][j].r = icColor._ppA[i][j-1].r;
                else if(_ppA[i-1][j] != 0)
                    icColor._ppA[i][j].r = icColor._ppA[i-1][j].r;
                else
                    icColor._ppA[i][j].r = ++num;
            }
        }

    // Second pass — resolve label conflicts
    for (int i = 0; i < Row(); i++)
        for (int j = 0; j < Col(); j++)
            if (icColor._ppA[i][j].r != 0)
            {
                if (i > 0 && icColor._ppA[i-1][j].r != 0 && icColor._ppA[i-1][j].r != icColor._ppA[i][j].r)
                {
                    int targetLabel = icColor._ppA[i-1][j].r;
                    for (int k = 0; k < Row(); k++)
                        for (int l = 0; l < Col(); l++)
                            if (icColor._ppA[k][l].r == icColor._ppA[i][j].r)
                                icColor._ppA[k][l].r = targetLabel;
                }
                else if (j > 0 && icColor._ppA[i][j-1].r != 0 && icColor._ppA[i][j-1].r != icColor._ppA[i][j].r)
                {
                    int targetLabel = icColor._ppA[i][j-1].r;
                    for (int k = 0; k < Row(); k++)
                        for (int l = 0; l < Col(); l++)
                            if (icColor._ppA[k][l].r == icColor._ppA[i][j].r)
                                icColor._ppA[k][l].r = targetLabel;
                }
            }

    // Colorize labels
    for (int i = 0; i < Row(); i++)
        for (int j = 0; j < Col(); j++)
            if (icColor._ppA[i][j].r != 0)
            {
                int label = icColor._ppA[i][j].r % 7;
                KColor color;
                switch (label) {
                case 0: color = {255,   0,   0}; break; // red
                case 1: color = {255, 165,   0}; break; // orange
                case 2: color = {255, 255,   0}; break; // yellow
                case 3: color = {  0, 255,   0}; break; // green
                case 4: color = {  0,   0, 255}; break; // blue
                case 5: color = {128,   0, 128}; break; // purple
                case 6: color = {152, 252, 152}; break; // light green
                }
                icColor._ppA[i][j].r = color.r;
                icColor._ppA[i][j].g = color.g;
                icColor._ppA[i][j].b = color.b;
            }

    return icColor;
}

// -----------------------------------------------------------------------
// Connected Component Labeling — N8 Connectivity
// -----------------------------------------------------------------------
KImageColor KImageGray::ImageLabelingN8()
{
    KImageColor icColor(this->Row(), this->Col());
    int num = 1;

    for (int i = 0; i < Row(); i++)
        for (int j = 0; j < Col(); j++)
        {
            if (_ppA[i][j] != 0)
            {
                int neighbors[] = { -1, 0, 1 };
                bool labeled = false;

                for (int ni : neighbors)
                {
                    for (int nj : neighbors)
                    {
                        int x = i + ni, y = j + nj;
                        if (x >= 0 && x < Row() && y >= 0 && y < Col() && icColor._ppA[x][y].r != 0)
                        {
                            icColor._ppA[i][j].r = icColor._ppA[x][y].r;
                            labeled = true;
                            break;
                        }
                    }
                    if (labeled) break;
                }

                if (!labeled)
                    icColor._ppA[i][j].r = num++;
            }
        }

    // Colorize labels (same as N4)
    for (int i = 0; i < Row(); i++)
        for (int j = 0; j < Col(); j++)
            if (icColor._ppA[i][j].r != 0)
            {
                int label = icColor._ppA[i][j].r % 7;
                KColor color;
                switch (label) {
                case 0: color = {255,   0,   0}; break;
                case 1: color = {255, 165,   0}; break;
                case 2: color = {255, 255,   0}; break;
                case 3: color = {  0, 255,   0}; break;
                case 4: color = {  0,   0, 255}; break;
                case 5: color = {128,   0, 128}; break;
                case 6: color = {152, 252, 152}; break;
                }
                icColor._ppA[i][j].r = color.r;
                icColor._ppA[i][j].g = color.g;
                icColor._ppA[i][j].b = color.b;
            }

    return icColor;
}
