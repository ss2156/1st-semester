from flask import Flask, render_template, request, send_file, jsonify, session
from flask_session import Session
import numpy as np
import imageio.v2 as imageio
from io import BytesIO
from sklearn.decomposition import PCA
from sklearn.metrics import mean_squared_error
from PIL import Image
import os
import io

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_secret_key_here'  # Change this to a random secret key
app.config['SESSION_TYPE'] = 'filesystem'
Session(app)

# Function to perform PCA compression
def pca_compression(image_2d, numpc=100):
    mean = np.mean(image_2d, axis=0)
    centered_data = image_2d - mean
    pca = PCA(n_components=numpc)
    compressed_data = pca.fit_transform(centered_data)
    reconstructed_data = pca.inverse_transform(compressed_data) + mean
    return np.uint8(np.clip(reconstructed_data, 0, 255)), compressed_data.size + pca.components_.size

# Function to calculate PSNR
def calculate_psnr(original, compressed):
    mse = mean_squared_error(original.flatten(), compressed.flatten())
    if mse == 0:
        return float('inf')  # PSNR is infinite if MSE is zero
    pixel_max = 255.0
    psnr = 20 * np.log10(pixel_max / np.sqrt(mse))
    return psnr

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compress', methods=['POST'])
def compress():
    if 'image' not in request.files:
        return jsonify({"error": "No file uploaded!"}), 400

    file = request.files['image']
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400

    try:
        # Load image
        img = Image.open(file.stream)
        img_np = np.array(img)

        # Convert grayscale to RGB if needed
        if img_np.ndim == 2:  # Grayscale image
            img_np = np.stack([img_np] * 3, axis=-1)
        elif img_np.ndim == 3 and img_np.shape[2] == 1:  # Grayscale with 1 channel
            img_np = np.concatenate([img_np] * 3, axis=-1)
        
        # Ensure image is RGB
        if img_np.ndim != 3 or img_np.shape[2] != 3:
            return jsonify({"error": "Unsupported image format. Please upload an RGB image."}), 400

        # Split into RGB channels
        a_r = img_np[:, :, 0]
        a_g = img_np[:, :, 1]
        a_b = img_np[:, :, 2]

        # Perform PCA compression on each channel
        a_r_compressed, r_size = pca_compression(a_r, numpc=100)
        a_g_compressed, g_size = pca_compression(a_g, numpc=100)
        a_b_compressed, b_size = pca_compression(a_b, numpc=100)

        # Combine compressed channels into a single image
        compressed_image = np.dstack((a_r_compressed, a_g_compressed, a_b_compressed))
        compressed_img = Image.fromarray(compressed_image)

        # Save compressed image in session
        compressed_image_io = io.BytesIO()
        compressed_img.save(compressed_image_io, 'JPEG')
        compressed_image_io.seek(0)
        session['compressed_image'] = compressed_image_io.getvalue()

        # Calculate PSNR values
        psnr_r = calculate_psnr(a_r, a_r_compressed)
        psnr_g = calculate_psnr(a_g, a_g_compressed)
        psnr_b = calculate_psnr(a_b, a_b_compressed)

        # Calculate original and compressed image sizes
        original_size = img_np.size
        total_compressed_size = r_size + g_size + b_size

        # Return PSNR values and image details along with the image URL
        return jsonify({
            "psnr_r": psnr_r,
            "psnr_g": psnr_g,
            "psnr_b": psnr_b,
            "original_size": original_size,
            "compressed_size": total_compressed_size,
            "image_url": "/download"
        }), 200

    except Exception as e:
        return jsonify({"error": str(e)}), 500



@app.route('/download')
def download():
    if 'compressed_image' not in session:
        return "No image to download", 400

    compressed_image_io = BytesIO(session['compressed_image'])
    compressed_image_io.seek(0)
    return send_file(compressed_image_io, mimetype='image/jpeg', as_attachment=True, download_name='compressed_image.jpg')

if __name__ == '__main__':
    app.run(debug=True)
