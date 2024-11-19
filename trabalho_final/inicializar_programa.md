# Como rodar programa

1. Instalar MSYS
2. Rodar `pacman -Syu` no terminal do MSYS
3. Rodar `pacman -S mingw-w64-ucrt-x86_64-gtk3 mingw-w64-ucrt-x86_64-toolchain` no terminal do MSYS
4. Rodar `pacman -S mingw-w64-x86_64-hicolor-icon-theme`
5. Rodar `pacman -S mingw-w64-x86_64-gsettings-desktop-schemas`
6. Rodar `pacman -S mingw-w64-x86_64-jansson`
7. Abrir terminal do MSYS mingw e navegar para a pasta do projeto cd C:/Users/ramos/Documents/Github/LinguagensDeMarcacaoExtensiveis/trabalho_final
8. Rodar o seguinte comando para compilar a aplicação

```bash
export PATH=/ucrt64/bin:$PATH
gcc main.c -o main `pkg-config --cflags --libs gtk+-3.0`
```

Rodar a aplicação ainda no terminal minsys com ./main
