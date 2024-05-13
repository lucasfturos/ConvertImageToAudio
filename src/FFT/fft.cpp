#include "fft.hpp"

/*!
 * Função para calcular a amplitude de um número complexo
 *
 * Calcula a amplitude (módulo) de um número complexo `z`.
 *
 * `z`: Um número complexo do tipo `std::complex<ComplexType>`.
 *
 * Rertono: A amplitude (módulo) do número complexo `z`, calculada como a
 * raiz quadrada da soma dos quadrados das partes real e imaginária.
 */
template <typename ComplexType>
ComplexType FFT<ComplexType>::amp(Complex<ComplexType> z) {
    ComplexType a = z.real();
    ComplexType b = z.imag();
    return std::sqrt(a * a + b * b);
}

/*!
 * Função para reversão de bits
 *
 * Realiza a reversão de bits de um índice `k` em um número
 * binário de `n` bits.
 *
 * `k`: O índice a ser revertido.
 *
 * `n`: O número de bits do índice `k`.
 *
 * Returno: O índice `k` com seus bits revertidos.
 */
template <typename ComplexType>
std::size_t FFT<ComplexType>::bitReverse(std::size_t k, std::size_t n) {
    std::size_t reversed = 0;
    for (std::size_t i = 0; i < n; i++) {
        reversed = (reversed << 1) | (k & 1);
        k >>= 1;
    }
    return reversed;
}

/*!
 * Janela de Hann
 *
 * Aplica a janela de Hann aos dados de entrada `in` com tamanho `n`.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `n`: O tamanho do vetor de dados de entrada.
 */
template <typename ComplexType>
void FFT<ComplexType>::applyHannWindow(std::vector<Complex<ComplexType>> &in,
                                       std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        ComplexType t = static_cast<ComplexType>(i) / (n - 1);
        ComplexType hann = 0.5 - 0.5 * std::cos(2 * pi * t);
        in[i] *= hann;
    }
}

/*!
 * Janela de Hamming
 *
 * Aplica a janela de Hamming aos dados de entrada `in` com tamanho `n`.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `n`: O tamanho do vetor de dados de entrada.
 */
template <typename ComplexType>
void FFT<ComplexType>::applyHammingWindow(std::vector<Complex<ComplexType>> &in,
                                          std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        ComplexType t = static_cast<ComplexType>(i) / (n - 1);
        ComplexType hamming = 0.54 - 0.46 * std::cos(2 * pi * t);
        in[i] *= hamming;
    }
}

/*!
 * Janela de Blackman
 *
 * Aplica a janela de Blackman aos dados de entrada `in` com tamanho `n`.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `n`: O tamanho do vetor de dados de entrada.
 */
template <typename ComplexType>
void FFT<ComplexType>::applyBlackmanWindow(
    std::vector<Complex<ComplexType>> &in, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        ComplexType t = static_cast<ComplexType>(i) / (n - 1);
        ComplexType blackman =
            0.42 + 0.5 * std::cos(2 * pi * t) + 0.8 * std::cos(4 * pi * t);
        in[i] *= blackman;
    }
}

/*!
 * Janela de Flattop
 *
 * Aplica a janela de Flattop aos dados de entrada `in` com tamanho `n`.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `n`: O tamanho do vetor de dados de entrada.
 */
template <typename ComplexType>
void FFT<ComplexType>::applyFlattopWindow(std::vector<Complex<ComplexType>> &in,
                                          std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        ComplexType t = static_cast<ComplexType>(i) / (n - 1);
        ComplexType flattop =
            1 - 1.93 * std::cos(2 * pi * t) + 1.29 * std::cos(4 * pi * t) -
            0.388 * std::cos(6 * pi * t) + 0.032 * std::cos(8 * pi * t);
        in[i] *= flattop;
    }
}

/*!
 * Função recursiva para realizar a FFT
 *
 * Realiza a Transformada Rápida de Fourier (FFT) nos dados de entrada.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `stride`: O passo entre os elementos do vetor de entrada.
 *
 * `out`: Uma referência a um vetor de número complexo do tipo
 *
 * `std::complex<ComplexType>` para armazenar a saída da FFT.
 *
 * `n`: O tamanho do vetor de dados de entrada e saída.
 */
template <typename ComplexType>
void FFT<ComplexType>::fft(std::vector<Complex<ComplexType>> &in,
                           std::vector<std::complex<ComplexType>> &out,
                           std::size_t n) {
    if (n == 1) {
        out[0] = in[0];
        return;
    }

    std::vector<Complex<ComplexType>> in_reversed(n, 0);
    for (std::size_t k = 0; k < n; k++) {
        std::size_t rev_k =
            bitReverse(k, static_cast<std::size_t>(std::log2(n)));
        in_reversed[k] = in[rev_k];
    }

    std::vector<Complex<ComplexType>> out_reversed(n / 2, 0);
    fft(in_reversed, out_reversed, n / 2);

    for (std::size_t k = 0; k < n / 2; k++) {
        ComplexType t = static_cast<ComplexType>(k) / n;
        Complex<ComplexType> v = exp(-2.0 * 1i * pi * t) * out_reversed[k];
        Complex<ComplexType> e = out_reversed[k];
        out[k] = e + v;
        out[k + n / 2] = e - v;
    }
}

/*!
 * Função principal para análise de espectro
 *
 * Realiza a análise do espectro dos dados de entrada usando a Transformada
 * Rápida de Fourier (FFT) e armazena o resultado em `out`.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `stride`: O passo entre os elementos do vetor de entrada.
 *
 * `out`: Uma referência a um vetor de números complexos do tipo
 * `std::complex<ComplexType>` para armazenar o espectro calculado.
 *
 * `n`: O tamanho do vetor de dados de entrada e saída.
 */
template <typename ComplexType>
void FFT<ComplexType>::fftAnalyze(std::vector<Complex<ComplexType>> &in,
                                  std::vector<Complex<ComplexType>> &out,
                                  std::size_t n) {
    applyHammingWindow(in, n);
    // applyHannWindow(in, n);
    // applyBlackmanWindow(in, n);
    // applyFlattopWindow(in, n);

    std::vector<Complex<ComplexType>> complex_output(n, 0.0);

    fft(in, complex_output, n);

    ComplexType step = 1.06;
    ComplexType lowf = 1.0;
    std::size_t m = 0;
    ComplexType max_amp = 1.0;

    for (ComplexType f = lowf; static_cast<std::size_t>(f) < n / 2;
         f = std::ceil(f * step)) {
        ComplexType f1 = std::ceil(f * step);
        ComplexType a = 0.0;
        for (std::size_t q = static_cast<std::size_t>(f);
             q < n / 2 && q < static_cast<std::size_t>(f1); ++q) {
            ComplexType b = amp(complex_output[q]);
            (b > a) ? a = b : 0;
        }
        (max_amp < a) ? max_amp = a : 0;
        out[m++] = a;
    }

    for (std::size_t i = 0; i < n / 2; ++i) {
        out[i] /= max_amp;
    }
}

/*!
 * Função para realizar a Transformada Rápida de Fourier Inversa (IFFT)
 *
 * Realiza a Transformada Rápida de Fourier inversa (FFT) sobre os dados de
 * entrada. Esta função opera diretamente sobre os dados de entrada,
 * conjugando-os e aplicando a FFT diretamente, e em seguida, conjugando
 * novamente e normalizando os resultados.
 *
 * `in`: Uma referência a um vetor de dados de entrada do tipo `T`.
 *
 * `stride`: O passo entre os elementos do vetor de entrada.
 *
 * `out`: Uma referência a um vetor de números complexos do tipo
 * `Complex<ComplexType>`.
 *
 * `n`: O tamanho do vetor de dados de entrada e saída.
 */
template <typename ComplexType>
void FFT<ComplexType>::ifft(std::vector<Complex<ComplexType>> &in,
                            std::vector<Complex<ComplexType>> &out,
                            std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        in[i] = std::conj(in[i]);
    }

    fft(in, out, n);

    for (std::size_t i = 0; i < n; ++i) {
        out[i] = std::conj(out[i]) / static_cast<ComplexType>(n);
    }
}