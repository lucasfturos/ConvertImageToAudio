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
-   Alsa

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
./src/ConvertImageToAudio caminha/da/imagem.{jpg, png, etc} caminho/de/saída/do/audio.{flac, ogg, wav} <Mono = 1 | Estero = 2> <Ganho em dB> <duração em segundos> <modo de execução (play/save)>
```

-   Exemplo de uso:

```
./src/ConvertImageToAudio assets/img/image.png assets/audio/out.wav 1 500.0 100 save
```

**Nota:** O tempo máximo de duração é de 100s.

5. Arquivo `build.sh`:

O arquivo build.sh tem todas instruções para usar. Mas, caso tenha dúvidas, execute o comando `./build.sh -h` para receber mais informações. Ele automatiza os comandos explicados acima.

**Aviso:** É necessário ter instalado o Audacity caso queira usar a função `./build.sh -a`.

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
