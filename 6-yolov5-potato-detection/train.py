"""
YOLOv5 Potato Anomaly Detection - Training Script
Google Colab environment

Dataset: AI-hub Agricultural Product QC images
Classes: Normal, Sprout, Rotten
"""

# ── 1. Mount Google Drive ──
from google.colab import drive
drive.mount('/content/drive')

# ── 2. Clone YOLOv5 ──
import os
os.system('git clone https://github.com/ultralytics/yolov5')
os.chdir('yolov5')
os.system('pip install -r requirements.txt')

# ── 3. Prepare dataset split ──
from glob import glob
from sklearn.model_selection import train_test_split

img_list = glob("/content/drive/MyDrive/yolov5/dataset/image_potato/*.jpg")
img_list += glob("/content/drive/MyDrive/yolov5/dataset/image_potato/*.png")
print(f"Total images: {len(img_list)}")

train_img_list, val_img_list = train_test_split(
    img_list, test_size=0.2, random_state=1000
)
print(f"Train: {len(train_img_list)}, Val: {len(val_img_list)}")

with open('/content/drive/MyDrive/yolov5/dataset/train_jpg.txt', 'w') as f:
    f.write('\n'.join(train_img_list) + '\n')
with open('/content/drive/MyDrive/yolov5/dataset/val_jpg.txt', 'w') as f:
    f.write('\n'.join(val_img_list) + '\n')

# ── 4. Update data.yaml paths ──
import yaml

with open('/content/drive/MyDrive/yolov5/dataset/data.yaml', 'r') as f:
    data = yaml.load(f, Loader=yaml.FullLoader)

data['train'] = '/content/drive/MyDrive/yolov5/dataset/train_jpg.txt'
data['val'] = '/content/drive/MyDrive/yolov5/dataset/val_jpg.txt'

with open('/content/drive/MyDrive/yolov5/dataset/data.yaml', 'w') as f:
    yaml.dump(data, f)

# ── 5. Train ──
os.chdir('/content/drive/MyDrive/yolov5')
os.system(
    'python train.py '
    '--img 704 --batch 16 --epochs 50 '
    '--data /content/drive/MyDrive/yolov5/dataset/data.yaml '
    '--cfg /content/drive/MyDrive/yolov5/models/yolov5x.yaml '
    '--weights yolov5x.pt '
    '--name potato'
)

# ── 6. Detect (example) ──
# os.system(
#     'python detect.py '
#     '--img 704 '
#     '--weights "runs/train/potato/weights/best.pt" '
#     '--source "/path/to/test/image.png"'
# )
