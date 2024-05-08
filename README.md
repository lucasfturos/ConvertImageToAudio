# Convert Image to Audio

Converte imagem em áudio.

Feito em C++.

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
./src/ConvertImageToAudio assets/img/image.png " "assets/audio/out.wav 1
```
