# Projeto: Teclado Matricial com Controle de LEDs

## Descrição

Este projeto consiste na implementação de um teclado matricial 4x4 integrado a uma matriz de LEDs controlados por um microcontrolador Raspberry Pi Pico. Cada tecla pressionada no teclado aciona diferentes funções ou animações exibidas nos LEDs.

---

## Como Clonar

Para clonar este repositório em sua máquina local, siga os passos abaixo:

1. **Certifique-se de que você tem o Git instalado.** Se não tiver, instale-o a partir de [git-scm.com](https://git-scm.com/).

2. **Abra o terminal ou prompt de comando** e execute o seguinte comando:

    ```bash
    git clone https://github.com/TorRLD/matriz_rgb.git
    ```

3. **Navegue até o diretório do projeto:**

    ```bash
    cd matriz_rgb
    ```

---

## Como Contribuir

Contribuições são bem-vindas! Para contribuir com este projeto, por favor siga as etapas abaixo:

1. **Fork o Repositório:**
   - Clique no botão "Fork" no canto superior direito da página do repositório no GitHub para criar uma cópia pessoal.

2. **Clone o Repositório Forkado:**

    ```bash
    git clone https://github.com/seu-usuario/matriz_rgb.git
    cd matriz_rgb
    ```

3. **Crie uma Branch para sua Feature ou Correção:**

    ```bash
    git checkout -b nome-da-sua-branch
    ```

4. **Faça as Alterações Desejadas:**
   - Adicione novas funcionalidades, corrija bugs, melhore a documentação, etc.

5. **Adicione e Faça Commit das Suas Alterações:**

    ```bash
    git add .
    git commit -m "Descrição clara das alterações"
    ```

6. **Faça o Push para a Branch no GitHub:**

    ```bash
    git push origin nome-da-sua-branch
    ```

7. **Abra um Pull Request:**
   - Vá até o repositório original no GitHub e você verá uma notificação para abrir um Pull Request. Siga as instruções para enviar suas alterações para revisão.

**Notas Importantes:**
- **Mantenha seu Fork Atualizado:** Antes de iniciar novas alterações, certifique-se de que seu fork está atualizado com o repositório original.
- **Siga as Convenções de Código:** Respeite o estilo de codificação utilizado no projeto para manter a consistência.
- **Teste Suas Alterações:** Verifique se as novas funcionalidades ou correções não introduzem novos problemas.

---

## Organização do Repositório

### Líder do Projeto:
- **Heitor** foi responsável por organizar o repositório, gerenciar as tarefas e garantir a integração do código.

### Estrutura de Arquivos:
- **`matriz_rgb.c`**: Código principal com a lógica de controle do teclado e LEDs.
- **`ws2818b.pio.h`**: Biblioteca para controle dos LEDs WS2818.
- **`wokwi.toml`**: Configuração do wokwi no VSCode.
- **`diagram.json`**: Circuito do projeto no wokwi.

## Funcionalidades

Cada tecla do teclado é associada a uma funcionalidade específica. Abaixo está a tabela detalhada com as contribuições e funções implementadas por cada membro do grupo:

| **Tecla/Função** | **Descrição**                      | **Desenvolvedor** |
|-------------------|------------------------------------|-------------------|
| **Tecla 1**       | Animação bomberman                | Heitor            |
| **Tecla 0**       | Animação personangens Mario Bross | Heitor            |
| **Tecla 2**       | Animação ping ping                | Patrick           |
| **Tecla 3**       | Animação estilo Pokeball          | Alex              |
| **Tecla 4**       | Animação PacMan                   | Josimar           |
| **Tecla 5**       | Animação Jogo da Cobrinha         | João              |
| **Função A**      | Animação específica               | Heitor            |
| **Função B**      | Animação específica               | Patrick           |
| **Função C**      | Animação específica               | Heitor            |
| **Função D**      | Animação específica               | Josimar           |
| **Função #**      | Animação específica               | João              |
| **Vídeo**         | Vídeo funcionamento Wokwi         | André             |

---

## Link para o Vídeo do Funcionamento

[Assista ao vídeo](https://www.youtube.com/watch?v=ArrbpIgp6co)

---

## Estrutura do Código

- **Definições dos GPIOs:** Configuração dos pinos para o teclado matricial.
- **Matriz do Teclado:** Mapeamento das teclas para caracteres.
- **Leitura do Teclado:** Implementação do debounce e detecção de teclas pressionadas.
- **Funções Específicas:** Cada tecla aciona uma função ou animação correspondente.

---

## Requisitos de Hardware

1. **Microcontrolador:** Raspberry Pi Pico.
2. **Teclado Matricial:** 4x4.
3. **Matriz de LEDs:** LEDs WS2818B (25 LEDs).
4. **Componentes de Suporte:** Resistores pull-up, conexões físicas adequadas.

---

## Compilação e Execução

1. Instale as bibliotecas necessárias para o Raspberry Pi Pico.
2. Compile o código utilizando o SDK do Raspberry Pi Pico.
3. Faça o upload para o microcontrolador.
4. Conecte o hardware conforme especificado.

---

## Licença

Este projeto é de código aberto, sendo permitido o uso, modificação e redistribuição conforme as condições da licença MIT.

---

**Contato dos Desenvolvedores:**

Para dúvidas ou contribuições, entre em contato com os membros do grupo por meio deste repositório.

## Agradecimentos

Agradecemos a todos os membros do grupo por suas contribuições:

- **Heitor** (Líder)
- **Patrick**
- **João**
- **Alex**
- **Josimar**
- **André**

Além disso, agradecemos à comunidade por fornecer suporte e documentação sobre o Raspberry Pi Pico e os LEDs WS2818.

---
