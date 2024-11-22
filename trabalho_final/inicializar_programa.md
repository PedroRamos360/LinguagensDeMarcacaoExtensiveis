# Como rodar programa 1. Instalar MSYS

2. Rodar `pacman -Syu` no terminal do MSYS
3. Rodar `pacman -S mingw-w64-ucrt-x86_64-gtk3 mingw-w64-ucrt-x86_64-toolchain` no terminal do MSYS
4. Rodar `pacman -S mingw-w64-x86_64-hicolor-icon-theme`
5. Rodar `pacman -S mingw-w64-x86_64-gsettings-desktop-schemas`
6. Rodar `pacman -S mingw-w64-x86_64-jansson`
7. Rodar `pacman -S mingw-w64-ucrt-x86_64-json-c`
8. Abrir terminal do MSYS mingw e navegar para a pasta do projeto cd C:/Users/ramos/Documents/Github/LinguagensDeMarcacaoExtensiveis/trabalho_final
9. Rodar o seguinte comando para compilar a aplicação

```bash
export PATH=/ucrt64/bin:$PATH
make
```

Rodar a aplicação ainda no terminal minsys com ./main

OBS: se der problema verificar toolchain que está sendo instalado, quando eu fiz sempre era usado a ucrt64
