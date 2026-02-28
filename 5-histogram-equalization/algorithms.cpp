// Histogram Equalization & Matching Algorithms
// Extracted from KFC framework — student-implemented functions only
// These functions are not standalone-compilable; they were originally
// member functions of KImageColor operating on internal pixel arrays.

// -----------------------------------------------------------------------
// Color Histogram Equalization (on HSI intensity channel)
// -----------------------------------------------------------------------
void KImageColor::ColorHistoEqualization(KArray<KHSI>& arHSI) const
{
    int histogram[256] = {0};
    int totalPixels = arHSI.Row() * arHSI.Col();

    for (int i = 0; i < arHSI.Row(); ++i)
        for (int j = 0; j < arHSI.Col(); ++j)
        {
            int intensity = static_cast<int>(arHSI[i][j].i * 255.0);
            histogram[intensity]++;
        }

    int cumulativeHistogram[256] = {0};
    cumulativeHistogram[0] = histogram[0];
    for (int i = 1; i < 256; ++i)
        cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];

    for (int i = 0; i < arHSI.Row(); ++i)
        for (int j = 0; j < arHSI.Col(); ++j)
        {
            int intensity = static_cast<int>(arHSI[i][j].i * 255.0);
            double newIntensity = static_cast<double>(cumulativeHistogram[intensity]) / totalPixels;
            arHSI[i][j].i = static_cast<float>(newIntensity);
        }
}

// -----------------------------------------------------------------------
// Color Histogram Matching (per-channel CDF matching)
// -----------------------------------------------------------------------
void KImageColor::ColorHistoMatching(KImageColor& icMain, KImageColor& icTarget)
{
    for(int channel = 0; channel < 3; channel++)
    {
        int MainHisto[256] = {0};
        int TargetHisto[256] = {0};
        int matching[256] = {0};

        // Build histograms
        for(int i = 0; i < icMain.Row(); i++)
            for(int j = 0; j < icMain.Col(); j++)
                MainHisto[icMain[i][j][channel]]++;

        for(int i = 0; i < icTarget.Row(); i++)
            for(int j = 0; j < icTarget.Col(); j++)
                TargetHisto[icTarget[i][j][channel]]++;

        // Compute CDFs
        for(int i = 1; i < 256; i++)
        {
            MainHisto[i] += MainHisto[i - 1];
            TargetHisto[i] += TargetHisto[i - 1];
        }

        // Build matching LUT
        for(int i = 0; i < 256; i++)
        {
            int value = 0;
            while(value < 256 && MainHisto[i] > TargetHisto[value])
                value++;
            matching[i] = value;
        }

        // Apply matching
        for(int i = 0; i < icMain.Row(); i++)
            for(int j = 0; j < icMain.Col(); j++)
                icMain[i][j][channel] = matching[icMain[i][j][channel]];
    }
}
