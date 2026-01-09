# RickMortyAPI

Aplicação Qt Quick para consulta à API do Rick and Morty.

## Requisitos

- Qt 6.8+
- CMake 3.16+
- Compilador compatível com C++17 (MSVC 2022 recomendado)
- Ninja (opcional, mas recomendado)

## Instalação do Qt 6.8 (Windows)

### 1. Download do Instalador

Baixe o instalador oficial do Qt:

**https://www.qt.io/download-qt-installer-oss**

Arquivo: `qt-online-installer-windows-x64-4.10.0.exe` (versão pode variar)

### 2. Instalação Silenciosa (Recomendado)

Abra o **Prompt de Comando como Administrador** e execute:

```batch
qt-online-installer-windows-x64-4.10.0.exe --root C:\Qt --accept-licenses --default-answer --confirm-command install qt.qt6.680.win64_msvc2022_64 qt.qt6.680.addons.qtsvg qt.tools.cmake qt.tools.ninja
```

#### Componentes instalados:

| Componente | Descrição |
|------------|-----------|
| `qt.qt6.680.win64_msvc2022_64` | Qt 6.8.0 completo (Quick, Network, Qml, Core, Test) |
| `qt.qt6.680.addons.qtsvg` | Módulo SVG |
| `qt.tools.cmake` | CMake |
| `qt.tools.ninja` | Ninja build system |

### 3. Configurar PATH

Após a instalação, adicione ao PATH do sistema:

```batch
setx PATH "%PATH%;C:\Qt\6.8.0\msvc2022_64\bin;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja"
```

Ou configure manualmente em: **Configurações > Sistema > Sobre > Configurações avançadas do sistema > Variáveis de Ambiente**

## Build

```bash
# Configurar
cmake -B build -S .

# Compilar (Debug)
cmake --build build --config Debug

# Compilar (Release)
cmake --build build --config Release
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
cmake --build build --config Release --target deploy
.\install\bin\appRickMortyAPI.exe
```

## Testes

```bash
# Executar testes unitários
ctest --test-dir build --output-on-failure
```

## Estrutura do Projeto

```
RickMortyAPI/
├── CMakeLists.txt           # Configuração principal do build
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
