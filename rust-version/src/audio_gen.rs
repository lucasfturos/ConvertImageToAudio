use hound;
use std::{f32::consts::PI, fs, i16, process::Command};

pub const MIN_FREQ: u32 = 0;
pub const MAX_FREQ: u32 = 19000;
pub const SAMPLE_RATE: u32 = 44100;
pub const AMP_NORMALIZED: f32 = 32767.0;

#[allow(dead_code)]
pub struct AudioGenerator {
    image_data: Vec<u8>,
    width: u32,
    height: u32,
}

impl AudioGenerator {
    #[allow(dead_code)]
    pub fn new(image_data: Vec<u8>, width: u32, height: u32) -> Self {
        if image_data.is_empty() {
            panic!("Image data is empty.");
        }
        AudioGenerator {
            image_data,
            width,
            height,
        }
    }

    #[allow(dead_code)]
    fn calculate_frequency(&self) -> Vec<f32> {
        let step_size: f32 = (MAX_FREQ - MIN_FREQ) as f32 / (self.height - 1) as f32;
        (0..self.height)
            .map(|i| MIN_FREQ as f32 + step_size * i as f32)
            .collect()
    }

    fn generate_samples(&self, duration: u32) -> Vec<i16> {
        let num_samples = duration * SAMPLE_RATE;
        let frequencies = self.calculate_frequency();
        let mut samples = Vec::with_capacity(num_samples as usize);

        for frame in 0..num_samples {
            let t = frame as f32 / SAMPLE_RATE as f32;
            let mut signal_value = 0.0;
            let col = (frame * self.width as u32 / num_samples) as u32;

            for row in 0..self.height {
                let inverted_row = self.height - 1 - row;
                let intensity =
                    self.image_data[(inverted_row * self.width + col) as usize] as f32 / 255.0;
                let frequency = frequencies[row as usize];
                signal_value += intensity * (2.0 * PI * frequency * t).sin();
            }

            let sample_value = ((signal_value / self.height as f32) * AMP_NORMALIZED) as i16;
            samples.push(sample_value);
        }

        samples
    }

    #[allow(dead_code)]
    pub fn generate_wav(&self, wav_path: &str, duration: u32, channels: u16) {
        let spec = hound::WavSpec {
            channels: channels,
            sample_rate: SAMPLE_RATE,
            bits_per_sample: 16,
            sample_format: hound::SampleFormat::Int,
        };

        let mut writer =
            hound::WavWriter::create(wav_path, spec).expect("Failed to create WAV writer");

        let samples = self.generate_samples(duration);
        for sample in samples {
            writer
                .write_sample(sample)
                .expect("Failed to write sample to WAV file");
        }
        writer.finalize().expect("Failed to finalize WAV file");
    }

    #[allow(dead_code)]
    pub fn convert_to_mp3(&self, wav_path: &str) {
        let mp3_path = wav_path.replace(".wav", ".mp3");
        if fs::metadata(&mp3_path).is_ok() {
            fs::remove_file(&mp3_path).expect("Failed to delete existing MP3 file");
        }

        let output = Command::new("ffmpeg")
            .arg("-i")
            .arg(wav_path)
            .arg(&mp3_path)
            .output()
            .expect("Failed to execute ffmpeg");
        if output.status.success() {
            println!("MP3 file generated successfully: {}", mp3_path);
        } else {
            eprintln!("Error during MP3 conversion: {:?}", output);
        }
    }
}
