# 🕷️ WebScraper em C

Um scraper de linha de comando escrito em C puro que analisa uma URL e extrai informações técnicas relevantes: certificado SSL, tecnologias do servidor e links externos presentes na página.

---

## 📋 Funcionalidades

- 🔐 **Inspeção de certificado SSL** — exibe Subject, data de início e data de expiração
- 🌐 **Detecção de tecnologias** — identifica o servidor web e a linguagem/framework via headers HTTP (`Server`, `X-Powered-By`)
- 🔗 **Extração de links externos** — lista todos os links que apontam para fora do domínio da URL informada
- 📡 **Resolução de IP** — exibe o endereço IP do servidor

---

## 🛠️ Tecnologias utilizadas

| Tecnologia | Descrição |
|---|---|
| **C (C99/C11)** | Linguagem principal |
| **libcurl** | Requisições HTTP/HTTPS, coleta de headers e certificados |
| **libxml2** | Parsing de HTML e consultas XPath |

---

## ⚙️ Pré-requisitos

Certifique-se de ter instalado:

```bash
# Debian/Ubuntu
sudo apt install libcurl4-openssl-dev libxml2-dev

# Fedora/RHEL
sudo dnf install libcurl-devel libxml2-devel

# macOS (Homebrew)
brew install curl libxml2
```

---

## 🚀 Como compilar e executar

### Compilar

```bash
gcc WebScraper.c -o webscraper \
    $(xml2-config --cflags --libs) \
    -lcurl
```

### Executar

```bash
./webscraper
```

Ao executar, o programa solicitará uma URL válida:

```
Digite uma URL valida: https://exemplo.com
```

---

## 📤 Exemplo de saída

```
Informacoes do certificado SSL
Subject: CN=exemplo.com
Start date: Mar 10 00:00:00 2025 GMT
Expire date: Apr 10 23:59:59 2026 GMT

IP: 93.184.216.34

Tecnologias web utilizadas
Servidor: ECS
X-Powered-By: PHP/8.1

Links externos encontrados:
https://outro-site.com/pagina
https://cdn.exemplo.net/recurso.js
```

---

## 🗂️ Estrutura do projeto

```
.
├── WebScraper.c   # Código-fonte principal
└── README.md      # Documentação
```

---

## ⚠️ Avisos

- Use esta ferramenta apenas em sites que você tem permissão para analisar.
- O programa não segue redirecionamentos de forma explícita — a URL informada deve ser a final.
- O campo `X-Powered-By` pode estar ausente dependendo da configuração do servidor.

---

## 📄 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
