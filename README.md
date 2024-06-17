# Convert Image to Audio

Converte imagens em áudio.

O programa só aceita como saída áudios no formato FLAC, OGG e WAV.

Feito em C++.

Tem a versão em Rust no arquivo `rust-version`.

## Instalação e Execução

### Requisitos

-   CMake
-   GCC/G++
-   Make
-   SndFile
-   stb

### Instruções:

1. Clone o repositório:

```
git clone https://github.com/lucasfturos/ConvertImageToAudio.git && cd ConvertImageToAudio
```

2. Crie a pasta build:

```
cmake -S . -B build && cd build
```

3. Compile o programa:

```
make
```

4. Execute o programa:

```
./src/ConvertImageToAudio caminha/da/imagem.{jpg, png, etc} caminho/de/saída/do/audio.{flac, ogg, wav} <Mono = 1 | Estero = 2> <Ganho em dB> <duração em segundos>
```

-   Exemplo de uso:

```
./src/ConvertImageToAudio assets/img/image.png assets/audio/out.wav 1 500.0 100
```

Nota: O tempo máximo de duração é de 100s.

## Screenshot

**Parâmetros:**

-   Ganho: 200 dB.

-   Escala: Linear.

### Tela do Audacity.

![](/assets/screenshot/resultHouse.png)

## Créditos

[Audacity](https://www.audacityteam.org/)

[House.bmp - www.hlevkin.com](https://www.hlevkin.com/hlevkin/TestImages/houses.bmp)

[libsndfile](https://github.com/libsndfile/libsndfile)

[stb](https://github.com/nothings/stb)
