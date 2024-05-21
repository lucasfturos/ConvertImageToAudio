mod audio_gen;
mod img_data;

use crate::audio_gen::AudioGenerator;
use crate::img_data::ImageData;

use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 7 {
        println!(
            "Uso: {} <filepath image> <resize_width> <grayscale (true/false)>
             <filepath output sound wav> <duration in seconds> <gain> <convert mp3 (true/false)>",
            args[0]
        );
        return;
    }

    let image_path = &args[1];
    let resize_width: u32 = args[2].parse().expect("Width must be a number");
    let grayscale: bool = args[3].parse().expect("Grayscale must be true or false");

    let wav_path = &args[4];
    let duration: u32 = args[5].parse().expect("Duration must be a number");
    let gain: f32 = args[6].parse().expect("Gain must be a double number");
    let convert_mp3: bool = args[7].parse().expect("Convert mp3 must be true or false");

    let image_data = ImageData::new(image_path, resize_width);
    let image_size = image_data.get_image_size();
    let image_data = image_data.get_image_data(grayscale);

    let audio_generator = AudioGenerator::new(image_data, image_size[0], image_size[1], gain);
    audio_generator.generate_wav(wav_path, duration, 1);

    if convert_mp3 {
        audio_generator.convert_to_mp3(wav_path);
    }
}
