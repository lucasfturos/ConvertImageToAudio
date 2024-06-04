#!/bin/bash

AUDIO_PATH="assets/audio"
AUDIO_OUTPUT_PATH=$AUDIO_PATH"/out.wav"

# Função para mostrar a mensagem de uso
show_use() {
    echo -e "Uso: $0"
    echo -e "\t-a\t--audacity\tAbrir o áudio no Audacity"
    echo -e "\t-b\t--build\t\tConstruir o programa"
    echo -e "\t-i\t--init\t\tInicializar o diretório de build"
    echo -e "\t-l\t--list\t\tListar o conteúdo da pasta de áudio"
    echo -e "\t-ppm\t--run-ppm\tExecuta o programa e gera uma"\
            "imagem PPM (apenas para teste de cores)."
    echo -e "\t-rm\t--remove\tRemover o arquivo de áudio"
    echo -e "\t-r\t--run\t\tExecutar o programa, ex: $0 -r /caminho/para/imagem.bmp 500.0"
}

# Caso não informe nenhum argumento
if [ $# -eq 0 ]; then
    echo "Erro: Nenhum argumento fornecido."
    show_use
    exit 1
fi

case "$1" in
    -i | --init)
        # Configura o diretório de build
        cmake -S . -B build
        ;;
    -b | --build)
        # Constroi o programa
        cmake --build build/
        ;;
    -r | --run)
        # Verifica se foi passado o caminho da imagem como parâmetro
        if [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ]; then
            echo "Erro: é necessário especificar o caminho da imagem, o ganho da onda ou a duração do áudio."
            exit 1
        fi
        # Comando para compilar e executar o programa;
        cmake --build build/ && ./build/src/ConvertImageToAudio "$2" \
                "$AUDIO_OUTPUT_PATH" "1" "$3" "$4"
        ;;
    -ppm | --run-ppm)
        # Verifica se foi passado o caminho da imagem como parâmetro
        if [ -z "$2" ] || [ -z "$3" ]; then
            echo "Erro: é necessário especificar o caminho da imagem ou o ganho da onda."
            exit 1
        fi
        # Comando para compilar e executar o programa;
        cmake --build build/ && ./build/src/ConvertImageToAudio "$2" \
                "$AUDIO_OUTPUT_PATH" "1" "$3" > build/out.ppm
        ;;
    -a  | --audacity)
        audacity "$AUDIO_OUTPUT_PATH" 2>/dev/null
        ;;
    -rm | --remove)
        # Remove o arquivo de áudio
        rm -rf "$AUDIO_OUTPUT_PATH"
        ;;
    -l | --list)
        # Lista o conteúdo da pasta de áudio
        ls -A -lh "$AUDIO_PATH"
        ;;
    *)
        # Caso passe um argumento incorreto
        echo "Erro: Argumento inválido."
        show_use
        exit 1
        ;;
esac