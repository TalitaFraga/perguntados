Bem-vindo ao Perguntados! Este é um jogo de perguntas e respostas desenvolvido em C utilizando a biblioteca GTK 3.0 para a interface gráfica. Siga as instruções abaixo para instalar as dependências necessárias, compilar e executar o jogo.

Passo 1: Instalação da Biblioteca GTK 3.0

Para executar o jogo, é necessário baixar a biblioteca GTK 3.0. Siga as instruções abaixo conforme o seu sistema operacional:


Linux

Para instalar a biblioteca GTK 3.0 no Linux, utilize o gerenciador de pacotes da sua distribuição:

    sudo apt update
    sudo apt install libgtk-3-dev


macOS

Para instalar a biblioteca GTK 3.0 no macOS, você pode utilizar o Homebrew:

Instale o Homebrew, caso ainda não tenha:


    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Instale a biblioteca GTK 3.0:

    brew install gtk+3

Windows

Para instalar a biblioteca GTK 3.0 no Windows, siga estes passos:

    Baixe e instale o MSYS2 de https://www.msys2.org/.
    Abra o terminal MSYS2 e execute os seguintes comandos:

    pacman -Syu
    pacman -S mingw-w64-x86_64-gtk3

    Certifique-se de adicionar os binários do MSYS2 ao PATH do Windows para que os comandos sejam reconhecidos no terminal.

Passo 2: Compilação e Execução do Jogo

Depois de instalar a biblioteca GTK 3.0, siga os passos abaixo para compilar e executar o jogo:

    Abra o terminal.
    Navegue até o diretório do jogo:


cd /caminho/para/o/diretorio/jogo

Compile o jogo utilizando o comando:


    gcc `pkg-config --cflags gtk+-3.0` -o perguntados perguntados.c funcoes.c `pkg-config --libs gtk+-3.0`

Execute o programa:


    ./perguntados

Divirta-se jogando Perguntados!
