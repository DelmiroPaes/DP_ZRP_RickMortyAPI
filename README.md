# RickMortyAPI

Aplicação Qt Quick para consulta à API do Rick and Morty.

## Teste Rápido (Windows x64)

Para testar a aplicação sem compilar, execute diretamente:

```
RickMortyAPI_Ready_to_Run\appRickMortyAPI.exe
```

## Requisitos

- Qt 6.8+ com MinGW 64-bit
- CMake 3.16+
- Ninja

## Instalação do Qt 6.8 (Windows)

1. Baixe o instalador oficial do Qt para **Windows x64**:

   **https://www.qt.io/download-qt-installer-oss**

2. Execute o instalador e selecione os seguintes componentes:
   - **Qt 6.8.x** > MinGW 64-bit
   - **Qt 6.8.x** > Qt SVG (Addon)
   - **Developer and Designer Tools** > CMake
   - **Developer and Designer Tools** > Ninja
   - **Developer and Designer Tools** > MinGW 13.x 64-bit

## Bootstrap (Recomendado)

Após instalar o Qt, use o script de bootstrap para compilar e preparar a aplicação:

```batch
bootstrap.bat
```

O bootstrap irá:
1. Verificar Qt, CMake, Ninja e MinGW no PATH
2. Configurar e compilar o projeto em Release
3. Executar deploy para `install\bin\`
4. Perguntar se deseja executar a aplicação

## Build Manual

```bash
# Configurar (Release)
cmake -B build/release -S . -G Ninja -DCMAKE_BUILD_TYPE=Release

# Compilar
cmake --build build/release

# Configurar (Debug)
cmake -B build/debug -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug

# Compilar
cmake --build build/debug
```

## Executar

```bash
# Windows (Debug)
.\build\debug\appRickMortyAPI.exe

# Windows (Release)
.\build\release\appRickMortyAPI.exe
```

## Deploy (Release)

```bash
cmake --build build/release --target deploy
.\install\bin\appRickMortyAPI.exe
```

## Testes

```bash
# Executar testes unitários (a partir do diretório de build)
ctest --test-dir build/release --output-on-failure
```

## Estrutura do Projeto

```
RickMortyAPI/
├── CMakeLists.txt           # Configuração principal do build
├── bootstrap.bat            # Script de bootstrap (wrapper)
├── bootstrap.ps1            # Script de bootstrap (PowerShell)
├── RickMortyAPI_Ready_to_Run/ # Binários pré-compilados para teste rápido
├── src/
│   ├── main.cpp             # Ponto de entrada da aplicação
│   ├── core/                # Camada de domínio (entidades)
│   ├── services/            # Camada de serviços (API, repositórios)
│   └── viewmodels/          # Camada de ViewModels (MVVM)
├── modules/
│   └── RickMortyUI/         # Módulo QML da interface
├── qml/
│   └── Main.qml             # Componente QML principal
├── resources/
│   └── icon.svg             # Ícone da aplicação
├── tests/                   # Testes unitários (Google Test)
└── docs/                    # Documentação do projeto
```

## Documentação

Consulte a documentação completa da arquitetura em:
- [RMEB_Analisys_v1.2.md](docs/RMEB_Analisys_v1.2.md) - Markdown
- [RMEB_Analisys_v1.2.html](docs/RMEB_Analisys_v1.2.html) - HTML (tema escuro)
