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

## Bootstrap (Recomendado)

Após instalar o Qt, use o script de bootstrap para configurar o ambiente, compilar e preparar a aplicação:

```batch
:: Clique direito > Executar como administrador
bootstrap.bat
```

O bootstrap irá:
1. Verificar/instalar MSVC Build Tools (se necessário)
2. Verificar Qt, CMake e Ninja no PATH
3. Configurar e compilar o projeto em Release
4. Executar deploy para `install\bin\`
5. Perguntar se deseja executar a aplicação

## Build Manual

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
.\build\Debug\appRickMortyAPI.exe

# Windows (Release)
.\build\Release\appRickMortyAPI.exe
```

## Deploy (Release)

```bash
cmake --build build --config Release --target deploy
.\install\bin\appRickMortyAPI.exe
```

## Testes

```bash
# Executar testes unitários (Release)
ctest --test-dir build -C Release --output-on-failure

# Executar testes unitários (Debug)
ctest --test-dir build -C Debug --output-on-failure
```

## Estrutura do Projeto

```
RickMortyAPI/
├── CMakeLists.txt           # Configuração principal do build
├── bootstrap.bat            # Script de bootstrap (wrapper)
├── bootstrap.ps1            # Script de bootstrap (PowerShell)
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
