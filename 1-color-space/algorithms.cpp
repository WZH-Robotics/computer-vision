// Color Space Conversion Algorithms
// Extracted from KFC framework — student-implemented functions only
// These functions are not standalone-compilable; they were originally
// member functions of KImageColor operating on internal pixel arrays.

// -----------------------------------------------------------------------
// RGB to NRG (Normalized Red-Green) Color Space
// -----------------------------------------------------------------------
void KImageColor::RGBtoNRG(KArray<KNRG>& arNRG) const
{
    double dR, dG, dB;

    int     nRow = Row();
    int     nCol = Col();
    double  dSum;
    arNRG.Create(nRow,nCol);

    for(int i=0; i<nRow; i++){
            for(int j=0; j<nCol; j++)
            {
            dR   = _ppA[i][j].r/255.0;
            dG   = _ppA[i][j].g/255.0;
            dB   = _ppA[i][j].b/255.0;

            dSum          = dR + dG + dB;
            arNRG[i][j].n = dSum;
            arNRG[i][j].r = dR/ dSum;
            arNRG[i][j].g = dG/ dSum;
            }
    }
}

// -----------------------------------------------------------------------
// HSI to RGB Reconstruction
// -----------------------------------------------------------------------
KImageColor& KImageColor::FromHSI(KArray<KHSI>& arHSI)
{
    float Hue,Sat,Int, Heu, sd;
    int   idx;

    Create(arHSI.Row(),arHSI.Col());

    for(int i=0, ii=arHSI.Row(); ii; i++,ii--)
        for(int j=0, jj=arHSI.Col(); jj; j++,jj--)
        {
            Hue = arHSI[i][j].h*_2PI;   // 0 ~ 2PI
            Sat = arHSI[i][j].s;        // 0 ~ 1
            Int = 255*arHSI[i][j].i;    // 0 ~ 255

            Heu = Hue*3/_2PI;
            idx = (int)Heu;
            sd = _2PI/6; // 60 degrees

            switch(idx)
            {
            case 0:     //0<=Hue<120
                _ppA[i][j].r=Int*(1+Sat*(cos(Hue))/cos(sd-Hue));
                _ppA[i][j].g=Int*(1+Sat*(1-(cos(Hue))/cos(sd-Hue)));
                _ppA[i][j].b=Int*(1-Sat);
                break;

            case 1:     //120<=Hue<240
                _ppA[i][j].r=Int*(1-Sat);
                _ppA[i][j].g=Int*(1+Sat*(cos(Hue-2*sd))/cos(3*sd-Hue));
                _ppA[i][j].b=Int*(1+Sat*(1-(cos(Hue-2*sd))/cos(3*sd-Hue)));
                break;

            case 2:     //240<=Hue<360
                _ppA[i][j].r=Int*(1+Sat*(1-(cos(Hue-4*sd))/cos(5*sd-Hue)));
                _ppA[i][j].g=Int*(1-Sat);
                _ppA[i][j].b=Int*(1+Sat*(cos(Hue-4*sd))/cos(5*sd-Hue));
                break;
            }
        }

    return *this;
}

// -----------------------------------------------------------------------
// Extract Hue / Saturation / Intensity channels as grayscale images
// -----------------------------------------------------------------------
KImageColor& KImageColor::RGB2Hue(KArray<KHSI>& arHSI)
{
    Create(arHSI.Row(),arHSI.Col());
    for(int i=0, ii=arHSI.Row(); ii; i++,ii--)
        for(int j=0, jj=arHSI.Col(); jj; j++,jj--)
        {
            float Hue = 255*arHSI[i][j].h;
            _ppA[i][j].r = Hue;
            _ppA[i][j].g = Hue;
            _ppA[i][j].b = Hue;
        }
    return *this;
}

KImageColor& KImageColor::RGB2Sat(KArray<KHSI>& arHSI)
{
    Create(arHSI.Row(),arHSI.Col());
    for(int i=0, ii=arHSI.Row(); ii; i++,ii--)
        for(int j=0, jj=arHSI.Col(); jj; j++,jj--)
        {
            float Sat = 255*arHSI[i][j].s;
            _ppA[i][j].r = Sat;
            _ppA[i][j].g = Sat;
            _ppA[i][j].b = Sat;
        }
    return *this;
}

KImageColor& KImageColor::RGB2Int(KArray<KHSI>& arHSI)
{
    Create(arHSI.Row(),arHSI.Col());
    for(int i=0, ii=arHSI.Row(); ii; i++,ii--)
        for(int j=0, jj=arHSI.Col(); jj; j++,jj--)
        {
            float Int = 255*arHSI[i][j].i;
            _ppA[i][j].r = Int;
            _ppA[i][j].g = Int;
            _ppA[i][j].b = Int;
        }
    return *this;
}

// -----------------------------------------------------------------------
// NRG to RGB Reconstruction (full color / NR only / NG only)
// -----------------------------------------------------------------------
KImageColor& KImageColor::FromNRG(KArray<KNRG>& arNRG)
{
    double Nor,Red,Green;
    Create(arNRG.Row(),arNRG.Col());

    for(int i=0, ii=arNRG.Row(); ii; i++,ii--)
        for(int j=0, jj=arNRG.Col(); jj; j++,jj--)
        {
            Nor = 255 * arNRG[i][j].n;              // R+G+B
            Red = arNRG[i][j].r;                    // R/(R+G+B)
            Green = arNRG[i][j].g;                  // G/(R+G+B)

            _ppA[i][j].r = Red * Nor;
            _ppA[i][j].g = Green * Nor;
            _ppA[i][j].b = (1-(Red+Green)) * Nor;
        }
    return *this;
}

KImageColor& KImageColor::FromNR(KArray<KNRG>& arNRG)
{
    Create(arNRG.Row(),arNRG.Col());
    for(int i=0, ii=arNRG.Row(); ii; i++,ii--)
        for(int j=0, jj=arNRG.Col(); jj; j++,jj--)
        {
            double Nor = 255 * arNRG[i][j].n;
            double Red = arNRG[i][j].r;
            _ppA[i][j].r = Red * Nor;
            _ppA[i][j].g = 0;
            _ppA[i][j].b = 0;
        }
    return *this;
}

KImageColor& KImageColor::FromNG(KArray<KNRG>& arNRG)
{
    Create(arNRG.Row(),arNRG.Col());
    for(int i=0, ii=arNRG.Row(); ii; i++,ii--)
        for(int j=0, jj=arNRG.Col(); jj; j++,jj--)
        {
            double Nor = 255 * arNRG[i][j].n;
            double Green = arNRG[i][j].g;
            _ppA[i][j].r = 0;
            _ppA[i][j].g = Green * Nor;
            _ppA[i][j].b = 0;
        }
    return *this;
}
