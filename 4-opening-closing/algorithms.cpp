// Opening, Closing & Boundary Detection Algorithms
// Extracted from KFC framework — student-implemented functions only
// These functions are not standalone-compilable; they were originally
// member functions of KImageGray operating on internal pixel arrays.

// -----------------------------------------------------------------------
// Boundary Detection
// Supports N4/N8 masks on foreground/background
// -----------------------------------------------------------------------
KImageGray KImageGray::Boundary(int Mask, int Ground)
{
    KImageGray  igOut = *this;

    int Bdr = 2*Mask + Ground;

    switch (Bdr) {
    case 0:  // N4, foreground
        for(int i=1,ii=Row()-2; ii; i++,ii--)
            for(int j=1,jj=Col()-2; jj; j++,jj--)
            {
                if(_ppA[i][j])
                    if(_ppA[i-1][j] && _ppA[i][j-1] && _ppA[i][j+1] && _ppA[i+1][j])
                        igOut._ppA[i][j] = 0;
            }
        break;

    case 1:  // N4, background
        for(int i=1,ii=Row()-2; ii; i++,ii--)
            for(int j=1,jj=Col()-2; jj; j++,jj--)
            {
                if(_ppA[i][j] == 0)
                    if(_ppA[i-1][j]==0 && _ppA[i][j-1]==0 && _ppA[i][j+1]==0 && _ppA[i+1][j]==0)
                        igOut._ppA[i][j] = 255;
            }
        break;

    case 2:  // N8, foreground
        for(int i=1,ii=Row()-2; ii; i++,ii--)
            for(int j=1,jj=Col()-2; jj; j++,jj--)
            {
                if(_ppA[i][j])
                    if(_ppA[i-1][j-1] && _ppA[i-1][j] && _ppA[i-1][j+1] && _ppA[i][j-1]
                        && _ppA[i][j+1] && _ppA[i+1][j-1] && _ppA[i+1][j] && _ppA[i+1][j+1])
                        igOut._ppA[i][j] = 0;
            }
        break;

    case 3:  // N8, background
        for(int i=1,ii=Row()-2; ii; i++,ii--)
            for(int j=1,jj=Col()-2; jj; j++,jj--)
            {
                if(_ppA[i][j] == 0)
                    if(_ppA[i-1][j-1]==0 && _ppA[i-1][j]==0 && _ppA[i-1][j+1]==0 && _ppA[i][j-1]==0
                        && _ppA[i][j+1]==0 && _ppA[i+1][j-1]==0 && _ppA[i+1][j]==0 && _ppA[i+1][j+1]==0)
                        igOut._ppA[i][j] = 255;
            }
        break;
    }

    return igOut;
}
