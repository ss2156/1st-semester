from flask import Flask, render_template, request, send_file
import numpy as np
import imageio.v2 as imageio
from io import BytesIO
from sklearn.decomposition import PCA
from PIL import Image

app = Flask(__name__)

# Function to perform PCA compression
def pca_compression(image_2d, numpc=100):
    # Center the data (subtract mean)
    mean = np.mean(image_2d, axis=0)
    centered_data = image_2d - mean

    # Perform PCA
    pca = PCA(n_components=numpc)
    compressed_data = pca.fit_transform(centered_data)

    # Reconstruct the image
    reconstructed_data = pca.inverse_transform(compressed_data) + mean

    return np.uint8(np.clip(reconstructed_data, 0, 255))

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compress', methods=['POST'])
def compress():
    if 'image' not in request.files:
        return "No file uploaded!", 400
    
    # Get the uploaded image
    file = request.files['image']
    if file.filename == '':
        return "No selected file", 400

    # Read the image
    img = Image.open(file.stream)
    img_np = np.array(img)

    # Split into RGB channels
    a_r = img_np[:, :, 0]
    a_g = img_np[:, :, 1]
    a_b = img_np[:, :, 2]

    # Perform PCA compression on each channel
    a_r_compressed = pca_compression(a_r, numpc=100)
    a_g_compressed = pca_compression(a_g, numpc=100)
    a_b_compressed = pca_compression(a_b, numpc=100)

    # Combine compressed channels into a single image
    compressed_image = np.dstack((a_r_compressed, a_g_compressed, a_b_compressed))

    # Convert the numpy array back to an image
    compressed_img = Image.fromarray(compressed_image)

    # Save the image in memory to allow downloading
    img_io = BytesIO()
    compressed_img.save(img_io, 'JPEG')
    img_io.seek(0)

    return send_file(img_io, mimetype='image/jpeg', as_attachment=True, download_name='compressed_image.jpg')

if __name__ == '__main__':
    app.run(debug=True)
