# Projeto: Teclado Matricial com Controle de LEDs

## Descrição

Este projeto consiste na implementação de um teclado matricial 4x4 integrado a uma matriz de LEDs controlados por um microcontrolador Raspberry Pi Pico. Cada tecla pressionada no teclado aciona diferentes funções ou animações exibidas nos LEDs.

---
## Organização do Repositório

### Líder do Projeto:
- **Heitor** foi responsável por organizar o repositório, gerenciar as tarefas e garantir a integração do código.

### Estrutura de Arquivos:
- **`matriz_rgb.c.c`**: Código principal com a lógica de controle do teclado e LEDs.
- **`ws2818b.pio.h`**: Biblioteca para controle dos LEDs WS2818.
- **`wokwi.toml`**: Configuração do wokwi no VSCode.
- **`diagram.json`**: Circuito do projeto no wokwi.
  
## Funcionalidades

Cada tecla do teclado é associada a uma funcionalidade específica. Abaixo está a tabela detalhada com as contribuições e funções implementadas por cada membro do grupo:

| **Tecla/Função** | **Descrição**                      | **Desenvolvedor** |
|-------------------|------------------------------------|-------------------|
| **Tecla 1**       | Animação personalizada            | Heitor            |
| **Tecla 0**       | Animação personalizada            | Heitor            |
| **Tecla 2**       | Animação personalizada            | Patrick           |
| **Tecla 3**       | Animação estilo Pokeball          | Alex              |
| **Tecla 4**       | Animação personalizada            | Josimar           |
| **Tecla 5**       | Animação personalizada            | João              |
| **Função A**      | Animação específica               | Heitor            |
| **Função B**      | Animação específica               | Patrick           |
| **Função C**      | Animação específica               | Heitor            |
| **Função D**      | Animação específica               | Josimar           |
| **Função #**      | Animação específica               | João              |
| **Vídeo**         | Vídeo funcionamento Wokwi         | André             |

---


## Link para o vídeo do funcionamento


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
