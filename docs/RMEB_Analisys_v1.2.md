# Rick and Morty Episode Browser

## 20260108 - Teste ZRP C++ QT Sênior - Delmiro Paes (delmiro.paes@gmail.com)

---

## Sumário

1. [Visão Geral](#1-visão-geral)
2. [Arquitetura do Sistema](#2-arquitetura-do-sistema)
3. [Estrutura de Diretórios](#3-estrutura-de-diretórios)
4. [Camada de Domínio (Core)](#4-camada-de-domínio-core)
5. [Camada de Serviços (Services)](#5-camada-de-serviços-services)
6. [Camada de Apresentação (ViewModels)](#6-camada-de-apresentação-viewmodels)
7. [Camada de Interface (View/UI)](#7-camada-de-interface-viewui)
8. [Fluxo de Dados](#8-fluxo-de-dados)
9. [Padrões de Projeto Aplicados](#9-padrões-de-projeto-aplicados)
10. [Princípios de Clean Code](#10-princípios-de-clean-code)
11. [Testes Automatizados](#11-testes-automatizados)
12. [Configuração de Build](#12-configuração-de-build)
13. [Decisões Arquiteturais](#13-decisões-arquiteturais)

---

## 1. Visão Geral

O **Rick and Morty Episode Browser** é uma aplicação desktop desenvolvida em **Qt 6 Quick (QML)** com **C++17**, projetada para consumir a [Rick and Morty API](https://rickandmortyapi.com/) pública. A aplicação permite aos usuários navegar por episódios da série, visualizar detalhes e explorar os personagens que aparecem em cada episódio.

### Tecnologias Utilizadas

| Tecnologia | Versão | Propósito |
|------------|--------|-----------|
| C++ | C++17 | Linguagem principal |
| Qt | 6.8+ | Framework de UI |
| Qt Quick/QML | 6.x | Interface declarativa |
| CMake | 3.16+ | Sistema de build |
| Google Test | Latest | Framework de testes |

### Funcionalidades Principais

- Listagem de todos os episódios com thumbnails de personagens
- Visualização detalhada de episódios selecionados
- Grade de personagens (cast) por episódio
- Painel de detalhes do personagem selecionado
- Interface responsiva com painel colapsável
- Sistema de design centralizado (Theme)

---

## 2. Arquitetura do Sistema

A aplicação segue os princípios da **Clean Architecture**, organizando o código em camadas com responsabilidades bem definidas e dependências direcionadas para o centro.

### Diagrama de Camadas

```
┌─────────────────────────────────────────────────────────────────┐
│                    CAMADA DE INTERFACE (View)                   │
│               QML Components (EpisodeBrowser, etc.)             │
└──────────────────────────────┬──────────────────────────────────┘
                               │ usa (property binding, signals)
┌──────────────────────────────▼──────────────────────────────────┐
│                  CAMADA DE APRESENTAÇÃO (ViewModels)            │
│        QAbstractListModel + QObject com Q_PROPERTY              │
└──────────────────────────────┬──────────────────────────────────┘
                               │ usa (injeção de dependência)
┌──────────────────────────────▼──────────────────────────────────┐
│                    CAMADA DE SERVIÇOS (Services)                │
│           Implementações de Repositórios + ApiService           │
└──────────────────────────────┬──────────────────────────────────┘
                               │ implementa
┌──────────────────────────────▼──────────────────────────────────┐
│                    CAMADA DE DOMÍNIO (Core)                     │
│              Entidades + Interfaces de Repositório              │
│                    (C++ puro, sem dependências Qt)              │
└─────────────────────────────────────────────────────────────────┘
```

### Princípios Arquiteturais

1. **Dependency Rule**: As dependências apontam sempre para dentro, da camada externa para a interna
2. **Separation of Concerns**: Cada camada possui responsabilidade única e bem definida
3. **Abstraction Principle**: Camadas superiores dependem de abstrações, não de implementações concretas
4. **Interface Segregation**: Interfaces de repositório são específicas para cada entidade

---

## 3. Estrutura de Diretórios

```
RickMortyAPI/
├── CMakeLists.txt                    # Configuração raiz do build
├── src/                              # Código-fonte C++
│   ├── main.cpp                      # Ponto de entrada da aplicação
│   ├── core/                         # Camada de Domínio
│   │   ├── CMakeLists.txt
│   │   ├── entities/                 # Entidades de domínio
│   │   │   ├── Character.h           # Modelo de Personagem
│   │   │   └── Episode.h             # Modelo de Episódio
│   │   └── repositories/             # Interfaces de repositório
│   │       ├── ICharacterRepository.h
│   │       └── IEpisodeRepository.h
│   ├── services/                     # Camada de Serviços
│   │   ├── CMakeLists.txt
│   │   ├── ApiService.h/cpp          # Cliente HTTP
│   │   ├── CharacterRepository.h/cpp # Impl. repositório de personagens
│   │   └── EpisodeRepository.h/cpp   # Impl. repositório de episódios
│   └── viewmodels/                   # Camada de Apresentação
│       ├── CMakeLists.txt
│       ├── ViewState.h               # Enumeração de estados
│       ├── EpisodeListViewModel.h/cpp
│       ├── EpisodeDetailViewModel.h/cpp
│       └── CharacterListViewModel.h/cpp
├── qml/                              # QML raiz
│   └── Main.qml                      # Janela principal
├── modules/                          # Módulos QML
│   └── RickMortyUI/                  # Componentes de UI
│       ├── CMakeLists.txt
│       └── qml/
│           ├── Theme.qml             # Design system (Singleton)
│           ├── EpisodeBrowser.qml    # Container principal
│           ├── EpisodeListPanel.qml  # Painel lateral de episódios
│           ├── EpisodeListItem.qml   # Item de lista de episódio
│           ├── EpisodeDetailPanel.qml# Painel de detalhes
│           ├── EpisodeHeader.qml     # Cabeçalho do episódio
│           ├── CastGrid.qml          # Grade de personagens
│           └── CharacterCard.qml     # Card de personagem
├── tests/                            # Testes unitários
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── core/                         # Testes de entidades
│   ├── services/                     # Testes de serviços
│   ├── viewmodels/                   # Testes de ViewModels
│   └── mocks/                        # Mocks para testes
└── resources/                        # Recursos da aplicação
    ├── icon.svg
    └── icon.ico
```

---

## 4. Camada de Domínio (Core)

A camada de domínio representa o coração da aplicação, contendo as regras de negócio e entidades principais. Esta camada é **completamente independente de frameworks**, escrita em C++17 puro.

### 4.1 Entidades

#### Episode

```cpp
struct Episode {
    int id;
    std::string name;           // Nome do episódio
    std::string airDate;        // Data de exibição
    std::string episodeCode;    // Código (ex: "S01E01")
    int season;                 // Número da temporada
    int episodeNumber;          // Número do episódio
    std::vector<int> characterIds;  // IDs dos personagens
    std::string imageUrl;       // URL da imagem

    std::string formattedSeason() const;  // "Season X, Episode Y"
    bool isValid() const;                 // Validação (id > 0)
};
```

#### Character

```cpp
struct Character {
    int id;
    std::string name;           // Nome do personagem
    std::string status;         // "Alive", "Dead", "unknown"
    std::string species;        // Espécie
    std::string type;           // Subtipo/variação
    std::string gender;         // Gênero
    std::string origin;         // Local de origem
    std::string location;       // Localização atual
    std::string imageUrl;       // URL do avatar
    int episodeCount;           // Quantidade de episódios

    std::string initial() const;  // Primeira letra do nome
    bool isValid() const;         // Validação (id > 0)
};
```

### 4.2 Interfaces de Repositório

O padrão **Repository** abstrai a origem dos dados, permitindo substituição de implementações sem afetar as camadas superiores.

#### IEpisodeRepository

```cpp
namespace core {
    using EpisodeResult = std::variant<std::vector<Episode>, std::string>;
    using EpisodeCallback = std::function<void(EpisodeResult)>;

    class IEpisodeRepository {
    public:
        virtual ~IEpisodeRepository() = default;
        virtual void fetchAllEpisodes(EpisodeCallback callback) = 0;
    };
}
```

#### ICharacterRepository

```cpp
namespace core {
    using CharacterResult = std::variant<std::vector<Character>, std::string>;
    using CharacterCallback = std::function<void(CharacterResult)>;

    class ICharacterRepository {
    public:
        virtual ~ICharacterRepository() = default;
        virtual void fetchCharacters(
            const std::vector<int>& ids,
            CharacterCallback callback
        ) = 0;
    };
}
```

**Tratamento de Erros**: O uso de `std::variant` permite retorno type-safe de sucesso ou erro, evitando exceções em contextos assíncronos.

---

## 5. Camada de Serviços (Services)

A camada de serviços implementa as interfaces definidas no domínio e gerencia a comunicação com APIs externas.

### 5.1 ApiService

Cliente HTTP centralizado que encapsula `QNetworkAccessManager`.

```cpp
class ApiService : public QObject {
    Q_OBJECT
public:
    using JsonResult = std::variant<QJsonArray, QString>;
    using JsonCallback = std::function<void(JsonResult)>;

    void get(const QString& endpoint, JsonCallback callback);

private:
    static constexpr auto BASE_URL = "https://rickandmortyapi.com/api";
    static constexpr int TIMEOUT_MS = 5000;

    QNetworkAccessManager* m_networkManager;
};
```

**Responsabilidades**:
- Gerenciamento de requisições HTTP GET
- Timeout de 5 segundos por requisição
- Parsing automático de JSON
- Extração do campo "results" em respostas paginadas
- Tratamento de erros de rede

### 5.2 EpisodeRepository

Implementação concreta de `IEpisodeRepository`.

```cpp
class EpisodeRepository : public QObject, public core::IEpisodeRepository {
    Q_OBJECT
public:
    explicit EpisodeRepository(ApiService* apiService, QObject* parent = nullptr);
    void fetchAllEpisodes(core::EpisodeCallback callback) override;

private:
    core::Episode parseEpisode(const QJsonObject& json);
    std::pair<int, int> parseEpisodeCode(const QString& code);
    std::vector<int> parseCharacterIds(const QJsonArray& urls);

    ApiService* m_apiService;
};
```

**Parsing de Dados**:
- Extrai código de episódio usando regex: `S(\d+)E(\d+)`
- Converte URLs de personagens em IDs numéricos
- Mapeia campos JSON para propriedades da entidade

### 5.3 CharacterRepository

Implementação concreta de `ICharacterRepository`.

```cpp
class CharacterRepository : public QObject, public core::ICharacterRepository {
    Q_OBJECT
public:
    explicit CharacterRepository(ApiService* apiService, QObject* parent = nullptr);
    void fetchCharacters(
        const std::vector<int>& ids,
        core::CharacterCallback callback
    ) override;

private:
    core::Character parseCharacter(const QJsonObject& json);
    ApiService* m_apiService;
};
```

**Características**:
- Busca múltiplos personagens em uma única requisição (IDs separados por vírgula)
- Ordenação alfabética por nome (case-insensitive)
- Extração de objetos aninhados (origin, location)
- Contagem de episódios a partir do array de URLs

---

## 6. Camada de Apresentação (ViewModels)

A camada de ViewModels implementa o padrão **MVVM**, servindo como ponte entre a lógica de negócios e a interface de usuário QML.

### 6.1 ViewState

Enumeração para gerenciamento de estados de carregamento.

```cpp
class ViewState {
    Q_GADGET
    Q_ENUMS(State)
public:
    enum State {
        Idle = 0,       // Estado inicial
        Loading = 1,    // Carregando dados
        Success = 2,    // Dados carregados com sucesso
        Error = 3       // Erro ao carregar
    };
};
```

### 6.2 EpisodeListViewModel

Modelo de lista para episódios, estendendo `QAbstractListModel`.

```cpp
class EpisodeListViewModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex
               WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        AirDateRole,
        EpisodeCodeRole,
        SeasonRole,
        EpisodeNumberRole,
        FormattedSeasonRole,
        ImageRole,
        ThumbnailUrl1Role,
        ThumbnailUrl2Role
    };

    Q_INVOKABLE void loadEpisodes();
    Q_INVOKABLE QVariantMap getEpisode(int index);
    Q_INVOKABLE QStringList getFirstCharacterImageUrls(int episodeId, int count);

signals:
    void episodeSelected(const QVariantList& characterIds);
    // ... outros sinais
};
```

**Funcionalidades**:
- Carregamento assíncrono de episódios
- Geração de thumbnails com seleção determinística de personagens
- Emissão de sinal `episodeSelected` com IDs de personagens ao selecionar episódio

### 6.3 EpisodeDetailViewModel

ViewModel para detalhes do episódio selecionado.

```cpp
class EpisodeDetailViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int episodeId READ episodeId NOTIFY episodeChanged)
    Q_PROPERTY(QString name READ name NOTIFY episodeChanged)
    Q_PROPERTY(QString airDate READ airDate NOTIFY episodeChanged)
    Q_PROPERTY(QString episodeCode READ episodeCode NOTIFY episodeChanged)
    Q_PROPERTY(QString formattedSeason READ formattedSeason NOTIFY episodeChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY episodeChanged)
    Q_PROPERTY(bool hasEpisode READ hasEpisode NOTIFY episodeChanged)

public:
    void setEpisode(const core::Episode& episode);
    void clear();

signals:
    void episodeChanged();
};
```

### 6.4 CharacterListViewModel

Modelo de lista para personagens com suporte a seleção.

```cpp
class CharacterListViewModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex
               WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY selectedIndexChanged)

    // Propriedades do personagem selecionado
    Q_PROPERTY(QString selectedName READ selectedName NOTIFY selectedIndexChanged)
    Q_PROPERTY(QString selectedStatus READ selectedStatus NOTIFY selectedIndexChanged)
    Q_PROPERTY(QString selectedSpecies READ selectedSpecies NOTIFY selectedIndexChanged)
    // ... outras propriedades de seleção

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        StatusRole,
        SpeciesRole,
        TypeRole,
        GenderRole,
        OriginRole,
        LocationRole,
        ImageRole,
        InitialRole,
        EpisodeCountRole
    };

    Q_INVOKABLE void loadCharacters(const QVariantList& ids);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void clearSelection();
};
```

**Thread Safety**: Utiliza `QMetaObject::invokeMethod` com `Qt::QueuedConnection` para garantir atualizações de UI na thread principal.

---

## 7. Camada de Interface (View/UI)

A camada de interface utiliza **Qt Quick/QML** para criar uma UI declarativa, responsiva e visualmente atraente.

### 7.1 Sistema de Design (Theme.qml)

Singleton que centraliza todas as configurações visuais da aplicação.

```qml
pragma Singleton
import QtQuick

QtObject {
    // Paleta de Cores
    readonly property color background: "#1a1a2e"      // Fundo principal
    readonly property color surface: "#16213e"         // Superfícies
    readonly property color surfaceLight: "#1f3460"    // Superfícies destacadas
    readonly property color accent: "#e94560"          // Cor de destaque
    readonly property color accentLight: "#ff6b6b"     // Destaque claro
    readonly property color textPrimary: "#eee"        // Texto principal
    readonly property color textSecondary: "#94a3b8"   // Texto secundário
    readonly property color divider: "#334155"         // Divisores

    // Tipografia
    readonly property int fontSizeSmall: 11
    readonly property int fontSizeMedium: 14
    readonly property int fontSizeLarge: 18
    readonly property int fontSizeTitle: 28
    readonly property int fontSizeHeader: 36

    // Espaçamento
    readonly property int spacingSmall: 4
    readonly property int spacingMedium: 8
    readonly property int spacingLarge: 16
    readonly property int spacingXLarge: 24

    // Dimensões de Componentes
    readonly property int episodeItemHeight: 80
    readonly property int characterCardWidth: 140
    readonly property int characterCardHeight: 170
    readonly property int avatarSize: 100
    readonly property int thumbnailWidth: 100
    readonly property int thumbnailHeight: 56
    readonly property int borderRadius: 8
    readonly property int episodePanelWidth: 320
}
```

### 7.2 Hierarquia de Componentes

```
Main.qml (Window)
│
└── EpisodeBrowser (Container Principal)
    │
    ├── EpisodeListPanel (Painel Lateral)
    │   │
    │   └── ListView
    │       └── EpisodeListItem (Delegado)
    │           ├── Thumbnail Grid (2 imagens)
    │           └── Informações do episódio
    │
    ├── EpisodeDetailPanel (Área de Detalhes)
    │   │
    │   ├── EpisodeHeader
    │   │   ├── Título do episódio
    │   │   └── Temporada formatada
    │   │
    │   ├── Painel de Detalhes do Personagem
    │   │   ├── Imagem do personagem
    │   │   └── Informações (nome, status, espécie, etc.)
    │   │
    │   └── CastGrid (Grade de Personagens)
    │       └── GridView
    │           └── CharacterCard (Delegado)
    │               ├── Avatar
    │               └── Nome
    │
    └── Toggle Button (Menu hambúrguer)
```

### 7.3 Componentes Principais

#### EpisodeBrowser.qml

Container principal que gerencia o layout da aplicação.

- Layout em duas colunas (sidebar + área principal)
- Painel colapsável com animação de 250ms
- Botão de toggle com ícone hambúrguer (☰)
- Gerenciamento de estado `panelVisible`

#### EpisodeListPanel.qml

Painel lateral com lista de episódios.

- Estados visuais: Idle, Loading, Success, Error
- ListView com delegate `EpisodeListItem`
- Indicador de seleção com borda colorida

#### EpisodeDetailPanel.qml

Área de detalhes do episódio e personagem.

- Animação de entrada para imagem do personagem (scale 0.9 → 1.02 → 1.0)
- Animação de "respiração" contínua
- Área scrollável para informações do personagem
- Indicador de scroll visual

#### CharacterCard.qml

Card individual de personagem na grade.

- Estados de imagem: Loading, Error, Loaded
- Spinner animado durante carregamento
- Placeholder com inicial do nome em caso de erro
- Feedback visual de hover e seleção

---

## 8. Fluxo de Dados

### Diagrama de Fluxo

```
┌──────────────────────────────────────────────────────────────────┐
│                       INTERFACE DE USUÁRIO                       │
│           (EpisodeBrowser, EpisodeListPanel, etc.)               │
└─────────────────────────────┬────────────────────────────────────┘
                              │
                 ┌────────────▼────────────┐
                 │  Usuário clica em um    │
                 │  episódio na lista      │
                 └────────────┬────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│                     EpisodeListViewModel                         │
│                                                                  │
│  1. setSelectedIndex(index) é chamado                            │
│  2. Emite sinal episodeSelected(characterIds)                    │
└─────────────────────────────┬────────────────────────────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│                         main.cpp                                 │
│                                                                  │
│  Slot conectado ao sinal:                                        │
│  1. Obtém episódio: episodeListVM.getEpisode(index)              │
│  2. Atualiza detalhes: episodeDetailVM.setEpisode()              │
│  3. Carrega cast: characterListVM.loadCharacters(ids)            │
└─────────────────────────────┬────────────────────────────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│                   CharacterListViewModel                         │
│                                                                  │
│  1. Define state = Loading                                       │
│  2. Chama repository.fetchCharacters(ids, callback)              │
└─────────────────────────────┬────────────────────────────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│                    CharacterRepository                           │
│                                                                  │
│  1. Formata IDs: "1,2,3,4,5"                                     │
│  2. Chama apiService.get("/character/1,2,3,4,5", callback)       │
└─────────────────────────────┬────────────────────────────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│                        ApiService                                │
│                                                                  │
│  1. Envia requisição HTTP GET                                    │
│  2. Recebe resposta JSON                                         │
│  3. Retorna via callback                                         │
└─────────────────────────────┬────────────────────────────────────┘
                              │
                              │ (Callback chain retorna)
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│               CharacterRepository (callback)                     │
│                                                                  │
│  1. Parse JSON → std::vector<Character>                          │
│  2. Ordena alfabeticamente                                       │
│  3. Retorna via callback                                         │
└─────────────────────────────┬────────────────────────────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│            CharacterListViewModel (callback)                     │
│                                                                  │
│  1. QMetaObject::invokeMethod (thread-safe)                      │
│  2. Atualiza m_characters                                        │
│  3. Define state = Success                                       │
│  4. Emite dataChanged()                                          │
└─────────────────────────────┬────────────────────────────────────┘
                              │
┌─────────────────────────────▼────────────────────────────────────┐
│                        QML (CastGrid)                            │
│                                                                  │
│  1. GridView detecta mudança no model                            │
│  2. Re-renderiza CharacterCards                                  │
│  3. UI atualizada com novos personagens                          │
└──────────────────────────────────────────────────────────────────┘
```

---

## 9. Padrões de Projeto Aplicados

| Padrão | Local de Aplicação | Benefício |
|--------|-------------------|-----------|
| **Clean Architecture** | Estrutura geral em 4 camadas | Separação de responsabilidades, testabilidade |
| **Repository Pattern** | `IEpisodeRepository`, `ICharacterRepository` | Abstração da fonte de dados |
| **Dependency Injection** | `main.cpp` | Acoplamento fraco, facilita testes |
| **MVVM** | ViewModels + QML | Separação UI/lógica de apresentação |
| **Observer** | Qt Signals/Slots | Comunicação desacoplada entre componentes |
| **Singleton** | `Theme.qml` | Design system centralizado |
| **State Machine** | `ViewState` enum | Gerenciamento de estados de carregamento |
| **Callback Pattern** | Operações assíncronas | Non-blocking, integração com event loop |
| **Factory** | `main.cpp` | Criação centralizada de componentes |
| **Delegate** | ListView/GridView | Renderização eficiente de listas |

---

## 10. Princípios de Clean Code

### 10.1 Nomenclatura Clara e Consistente

```cpp
// Nomes descritivos que revelam intenção
void fetchAllEpisodes(EpisodeCallback callback);
void loadCharacters(const QVariantList& ids);
QString formattedSeason() const;

// Prefixos consistentes
m_networkManager    // Membros privados
parseEpisode()      // Métodos de parsing
IEpisodeRepository  // Interfaces (I prefix)
```

### 10.2 Funções Pequenas e Focadas

```cpp
// Cada função tem uma única responsabilidade
core::Episode parseEpisode(const QJsonObject& json);
std::pair<int, int> parseEpisodeCode(const QString& code);
std::vector<int> parseCharacterIds(const QJsonArray& urls);
```

### 10.3 Tratamento de Erros Explícito

```cpp
// std::variant para retorno type-safe
using EpisodeResult = std::variant<std::vector<Episode>, std::string>;

// Callback recebe resultado ou erro
void fetchAllEpisodes(EpisodeCallback callback) {
    // ...
    if (std::holds_alternative<QString>(result)) {
        // Tratamento de erro
    } else {
        // Processamento de sucesso
    }
}
```

### 10.4 Imutabilidade e Const Correctness

```cpp
// Métodos const quando não modificam estado
QString name() const { return m_name; }
bool hasEpisode() const { return m_hasEpisode; }
std::string formattedSeason() const;

// Referências const para parâmetros
void fetchCharacters(const std::vector<int>& ids, ...);
```

### 10.5 Organização Modular

- **Separação por feature**: Cada componente QML em arquivo próprio
- **Separação por camada**: Diretórios distintos para core, services, viewmodels
- **Módulos QML**: `RickMortyUI` e `RickMortyViewModels`

---

## 11. Testes Automatizados

### 11.1 Framework e Estrutura

- **Google Test (gtest)**: Framework de testes unitários
- **Google Mock (gmock)**: Framework para criação de mocks

### 11.2 Suítes de Teste

| Executável | Camada | Cobertura |
|------------|--------|-----------|
| CoreTests | Domain | Entidades (Character, Episode) |
| ServicesTests | Services | Repositórios, parsing de dados |
| ViewModelTests | Presentation | ViewModels, signals, estados |

### 11.3 Exemplo de Mock

```cpp
class MockEpisodeRepository : public core::IEpisodeRepository {
public:
    MOCK_METHOD(void, fetchAllEpisodes,
                (core::EpisodeCallback callback), (override));
};

class MockCharacterRepository : public core::ICharacterRepository {
public:
    MOCK_METHOD(void, fetchCharacters,
                (const std::vector<int>& ids, core::CharacterCallback callback),
                (override));
};
```

### 11.4 Exemplo de Teste

```cpp
TEST_F(EpisodeListViewModelTests, LoadEpisodesChangesStateToLoading) {
    EXPECT_CALL(*mockRepository, fetchAllEpisodes(testing::_))
        .Times(1);

    viewModel->loadEpisodes();

    EXPECT_EQ(viewModel->state(), ViewState::Loading);
}
```

---

## 12. Configuração de Build

### 12.1 Requisitos

#### Build System
| Ferramenta | Versão | Observação |
|------------|--------|------------|
| CMake | 3.16+ | Sistema de build multiplataforma |

#### Linguagem
| Requisito | Versão | Observação |
|-----------|--------|------------|
| C++ Standard | C++17 | Requerido para std::variant, std::optional, etc. |

#### Compilador (Windows)
| Ferramenta | Versão | Observação |
|------------|--------|------------|
| MSVC | VS 2019+ | Recomendado, suporte completo a C++17 |
| MinGW-w64 | 8.0+ | Alternativa com suporte C++17 |

#### Framework Qt
| Componente | Versão | Uso |
|------------|--------|-----|
| Qt Base | 6.8+ | Versão mínima requerida |
| Qt6::Quick | 6.8+ | Engine QML e interface declarativa |
| Qt6::Network | 6.8+ | Requisições HTTP (QNetworkAccessManager) |
| Qt6::Svg | 6.8+ | Renderização de ícones SVG |
| Qt6::Qml | 6.8+ | Sistema de módulos QML |
| Qt6::Core | 6.8+ | Tipos base (QString, QObject, etc.) |
| Qt6::Test | 6.8+ | Apenas para testes unitários |

#### Testes (Opcional)
| Ferramenta | Versão | Observação |
|------------|--------|------------|
| Google Test | v1.14.0 | Obtido automaticamente via FetchContent |
| Google Mock | v1.14.0 | Incluído no Google Test |

#### Ferramentas Auxiliares (Windows)
| Ferramenta | Versão | Observação |
|------------|--------|------------|
| windeployqt | Qt 6.8+ | Incluído na instalação Qt |
| Git | Qualquer | Para download automático do Google Test |

#### Resumo de Instalação Mínima (Windows)
1. **Visual Studio 2019/2022** com workload "Desktop development with C++"
2. **Qt 6.8+** com componentes: Qt Quick, Qt Network, Qt Svg
3. **CMake 3.16+** (incluso no Visual Studio ou Qt)
4. **Git** (para FetchContent do Google Test)

### 12.2 Comandos de Build

```bash
# Configurar (a partir da raiz do projeto)
cmake -B build -S .

# Compilar (Debug)
cmake --build build --config Debug

# Compilar (Release)
cmake --build build --config Release

# Executar aplicação (Debug)
.\build\debug\appRickMortyAPI.exe

# Executar aplicação (Release)
.\build\release\appRickMortyAPI.exe

# Deploy para distribuição (Release)
cmake --build build --config Release --target deploy
.\install\bin\appRickMortyAPI.exe
```

### 12.3 Estrutura CMake

```
CMakeLists.txt (raiz)
├── src/core/CMakeLists.txt          # Biblioteca header-only
├── src/services/CMakeLists.txt      # Biblioteca estática
├── src/viewmodels/CMakeLists.txt    # Módulo QML
├── modules/RickMortyUI/CMakeLists.txt  # Módulo QML UI
└── tests/CMakeLists.txt             # Executáveis de teste
```

---

## 13. Decisões Arquiteturais

| Decisão | Justificativa |
|---------|---------------|
| **Core layer sem Qt** | Domínio reutilizável, testável, portátil |
| **Repository pattern** | Abstrai detalhes de API, permite mocking |
| **QAbstractListModel** | Integração nativa com QML, renderização eficiente |
| **std::variant para erros** | Type-safe, evita exceções em contexto assíncrono |
| **Callbacks assíncronos** | Compatível com event loop Qt, non-blocking |
| **Injeção de dependência em main.cpp** | Wiring centralizado, fácil modificação |
| **Singleton Theme.qml** | Design system consistente em toda UI |
| **MVVM** | Separação clara entre UI e lógica de apresentação |
| **Randomização determinística de thumbnails** | Mesmo episódio sempre mostra mesmos personagens |
| **QMetaObject::invokeMethod** | Thread safety para atualizações de UI |

---

## Considerações Finais

O **Rick and Morty Episode Browser** demonstra a aplicação prática de princípios modernos de arquitetura de software em um contexto de aplicação desktop Qt/QML. A separação em camadas bem definidas, combinada com padrões de projeto estabelecidos, resulta em um código:

- **Manutenível**: Mudanças em uma camada não afetam outras
- **Testável**: Cada componente pode ser testado isoladamente
- **Extensível**: Novos repositórios e ViewModels seguem padrões estabelecidos
- **Legível**: Nomenclatura clara e funções focadas

A arquitetura permite evolução independente de cada camada, facilitando tanto a manutenção quanto a adição de novas funcionalidades.

---

*Documento gerado em Janeiro de 2026*
