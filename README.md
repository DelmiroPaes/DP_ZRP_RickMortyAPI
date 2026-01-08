# RickMortyAPI

Aplicação Qt Quick para consulta à API do Rick and Morty.

## Requisitos

- Qt 6.8+
- CMake 3.16+
- Compilador compatível com C++17

## Build

```bash
cmake -B build -S .
cmake --build build
```

## Executar

```bash
# Windows
.\build\appRickMortyAPI.exe

# Linux/macOS
./build/appRickMortyAPI
```

## Estrutura do Projeto

```
RickMortyAPI/
├── CMakeLists.txt        # Configuração do build
├── src/
│   └── main.cpp          # Ponto de entrada da aplicação
├── qml/
│   ├── Main.qml          # Componente QML principal
│   └── components/       # Componentes QML reutilizáveis
├── resources/
│   └── images/           # Imagens e ícones
└── README.md
```
