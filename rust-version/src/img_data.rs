use image::{open, DynamicImage, GenericImageView, ImageBuffer, Luma};
use std::{fs::File, io::Write, path::Path};

pub struct Dimension {
    width: u32,
    height: u32,
}

pub struct ImageData {
    image: DynamicImage,
    size: Dimension,
}

impl ImageData {
    #[allow(dead_code)]
    pub fn new(image_path: &str, resize_width: u32) -> Self {
        let img = open(image_path).expect("Failed to open image");
        let aspect_ratio = img.width() as f32 / img.height() as f32;
        let resize_height = (resize_width as f32 / aspect_ratio).round() as u32;
        let resized_img = img.thumbnail_exact(resize_width, resize_height);
        ImageData {
            image: resized_img,
            size: Dimension {
                width: resize_width,
                height: resize_height,
            },
        }
    }

    #[allow(dead_code)]
    fn to_grayscale(&self) -> Vec<u8> {
        let mut data = Vec::new();
        for y in 0..self.size.height {
            for x in 0..self.size.width {
                let pixel = self.image.get_pixel(x, y);
                let (r, g, b) = (pixel[0] as f32, pixel[1] as f32, pixel[2] as f32);
                let grayscale = (0.299 * r + 0.587 * g + 0.114 * b) as u8;
                data.push(grayscale);
            }
        }
        data
    }

    #[allow(dead_code)]
    pub fn get_image_data(&self, grayscale: bool) -> Vec<u8> {
        if grayscale {
            self.to_grayscale()
        } else {
            let mut data = Vec::new();
            for y in 0..self.size.height {
                for x in 0..self.size.width {
                    let pixel = self.image.get_pixel(x, y);
                    data.push(pixel[0]);
                    data.push(pixel[1]);
                    data.push(pixel[2]);
                }
            }
            data
        }
    }

    #[allow(dead_code)]
    pub fn get_image_size(&self) -> [u32; 2] {
        [self.size.width, self.size.height]
    }

    fn to_grayscale_image(&self) -> DynamicImage {
        let gray_image: ImageBuffer<Luma<u16>, Vec<u16>> =
            ImageBuffer::from_fn(self.size.width, self.size.height, |x, y| {
                let pixel = self.image.get_pixel(x, y);
                let (r, g, b) = (pixel[0] as f32, pixel[1] as f32, pixel[2] as f32);
                let grayscale = (0.299 * r + 0.587 * g + 0.114 * b) as u16;
                Luma([grayscale])
            });
        DynamicImage::ImageLuma16(gray_image)
    }

    #[allow(dead_code)]
    pub fn save_as_ppm(&self, output_path: &Path, grayscale: bool) {
        let img = if grayscale {
            self.to_grayscale_image()
        } else {
            self.image.clone()
        };

        let mut file = File::create(output_path).expect("Failed to create file");
        write!(file, "P3\n{} {}\n255\n", self.size.width, self.size.height)
            .expect("Failed to write to file");

        let mut buffer = String::new();
        for y in 0..self.size.height {
            for x in 0..self.size.width {
                let pixel = img.get_pixel(x, y);
                let (r, g, b) = (pixel[0], pixel[1], pixel[2]);
                buffer.push_str(&format!("{} {} {} ", r, g, b));
            }
            buffer.push('\n');
        }
        file.write_all(buffer.as_bytes())
            .expect("Failed to write buffer to file");

        println!("PPM file generated successfully: {:?}", output_path);
    }
}
