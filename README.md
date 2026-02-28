# Computer Vision

> Course assignments for **Computer Vision** (2023-2, Kwangwoon University)

Image processing algorithms implemented in C++ with Qt framework, plus a YOLOv5-based potato anomaly detection term project.

## Repository Structure

```
computer-vision/
├── 1-contrast-transform/            # Image Contrast Enhancement
├── 2-morphology-labeling/           # Morphological Ops & Component Labeling
├── 3-opening-closing/               # Opening & Closing Operations
├── 4-histogram-equalization/        # Histogram Equalization & Matching
├── 5-hough-transform/               # Hough Transform for Circle Detection
└── 6-yolov5-potato-detection/       # YOLOv5 Potato Anomaly Detection (Term Project)
```

---

## 1. Contrast Transform

Basic image contrast enhancement using intensity transformation functions.

![Contrast Transform](1-contrast-transform/docs/result.png)

---

## 2. Morphology & Labeling

Morphological image processing with dilation, erosion, boundary detection, and connected component labeling.

- **Dilation / Erosion**: 3x3 and 5x5 structuring elements
- **Component Labeling**: N4 and N8 connectivity
- **Boundary Detection**: Contour extraction

![Morphology](2-morphology-labeling/docs/result.png)

---

## 3. Opening & Closing

Composite morphological operations for noise removal and shape refinement.

- **Opening** (erosion -> dilation): Removes small objects
- **Closing** (dilation -> erosion): Fills small holes

![Opening & Closing](3-opening-closing/docs/result.png)

---

## 4. Histogram Equalization

Histogram equalization for contrast improvement and histogram matching between image pairs.

- **Equalization**: Redistributes pixel intensity for better contrast
- **Matching**: Transforms source histogram to match target histogram
- Implemented from scratch (no library functions)

| Equalization | Matching |
|:---:|:---:|
| ![Equalization](4-histogram-equalization/docs/equalization.png) | ![Matching](4-histogram-equalization/docs/matching.png) |

---

## 5. Hough Transform

Circle detection using Hough Transform for Korean coin recognition.

- **Standard Hough**: Detects circular coins (100w, 50w, 10w)
- **Generalized Hough**: Shape matching with model template
- **Preprocessing**: Canny edge detection + gradient computation

![Hough Transform](5-hough-transform/docs/result.png)

---

## 6. YOLOv5 Potato Anomaly Detection (Term Project)

Automated food safety screening system that detects sprouted and rotten potatoes using YOLOv5.

- **Dataset**: AI-hub Agricultural Product QC images
- **Classes**: Normal, Sprout, Rotten
- **Model**: YOLOv5x, 50 epochs on Google Colab
- **Accuracy**: 97% (sprout), 95% (rotten)

| Normal | Sprout | Rotten |
|:---:|:---:|:---:|
| ![Normal](6-yolov5-potato-detection/docs/normal.png) | ![Sprout](6-yolov5-potato-detection/docs/sprout.jpg) | ![Rotten](6-yolov5-potato-detection/docs/rotten.png) |

```bash
# Training (Google Colab)
python train.py --img 704 --batch 16 --epochs 50 --data data.yaml --weights yolov5x.pt
```

> Based on [ultralytics/yolov5](https://github.com/ultralytics/yolov5)

---

## Tech Stack

- **HW 1-5**: C++ / Qt 6.5 / KFC Image Processing Library
- **Term Project**: Python / YOLOv5 / Google Colab
