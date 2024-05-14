#pragma once

#include <cmath>
#include <complex>
#include <cstdint>
#include <vector>

// Constantes
const double pi = M_PI;

// Macros
using namespace std::complex_literals;
template <typename ComplexType> using Complex = std::complex<ComplexType>;

/*!
 * Classe template para realizar FFT em dados unidimensionais.
 *
 * Esta classe implementa a Transformada de Fourier Rápida (FFT) para calcular
 * a transformada discreta de Fourier de um sinal unidimensional. Esta
 * implementação de FFT aplica o algoritmo de Cooley-Tukey. Suporta diferentes
 * tipos de dados, como `int`, `short`, `uint8_t`, `uint16_t`, `double` e
 * `float`.
 */
template <typename ComplexType = double> class FFT {
  private:
    inline ComplexType amp(Complex<ComplexType> z);

    // std::size_t bitReverse(std::size_t k, std::size_t n);

    void applyHannWindow(std::vector<Complex<ComplexType>> &in, std::size_t n);
    void applyHammingWindow(std::vector<Complex<ComplexType>> &in,
                            std::size_t n);
    void applyBlackmanWindow(std::vector<Complex<ComplexType>> &in,
                             std::size_t n);
    void applyFlattopWindow(std::vector<Complex<ComplexType>> &in,
                            std::size_t n);

    void fft(std::vector<Complex<ComplexType>> &in,
             std::vector<Complex<ComplexType>> &out, std::size_t n);

  public:
    void ifft(std::vector<Complex<ComplexType>> &in,
              std::vector<Complex<ComplexType>> &out, std::size_t n);
    void fftAnalyze(std::vector<Complex<ComplexType>> &in,
                    std::vector<Complex<ComplexType>> &out, std::size_t n);
};

// Instanciações da classe para tipos de dados suportados
template class FFT<double>;
