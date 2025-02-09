import os
import sys
from PIL import Image

def create_image_row(input_dir, output_path):
    images = sorted([f for f in os.listdir(input_dir) if f.lower().endswith(('png', 'jpg', 'jpeg'))])
    if not images:
        print("No images found in the directory.")
        return
    
    img_list = [Image.open(os.path.join(input_dir, img)).convert("RGBA") for img in images]
    max_size = max(max(img.size) for img in img_list)  # Find the largest dimension
    
    resized_imgs = [img.resize((max_size, max_size), Image.LANCZOS) for img in img_list]
    total_width = max_size * len(resized_imgs)
    new_image = Image.new('RGBA', (total_width, max_size), (255, 255, 255, 0))
    
    x_offset = 0
    for img in resized_imgs:
        new_image.paste(img, (x_offset, 0), img)
        x_offset += max_size
    
    new_image.save(output_path, format='PNG')
    print(f"Image row saved to {output_path}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_directory> <output_image>")
        sys.exit(1)
    
    input_directory = sys.argv[1]
    output_file = sys.argv[2]
    create_image_row(input_directory, output_file)
