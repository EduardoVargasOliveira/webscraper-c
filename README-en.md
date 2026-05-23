# 🕷️ WebScraper in C

A command-line web scraper written in pure C that analyzes a given URL and extracts relevant technical information: SSL certificate details, server technologies, and external links found on the page.

---

## 📋 Features

- 🔐 **SSL Certificate Inspection** — displays Subject, start date, and expiration date
- 🌐 **Technology Detection** — identifies the web server and language/framework via HTTP headers (`Server`, `X-Powered-By`)
- 🔗 **External Link Extraction** — lists all links pointing outside the domain of the given URL
- 📡 **IP Resolution** — displays the server's IP address

---

## 🛠️ Technologies Used

| Technology | Description |
|---|---|
| **C (C99/C11)** | Main programming language |
| **libcurl** | HTTP/HTTPS requests, header and certificate collection |
| **libxml2** | HTML parsing and XPath queries |

---

## ⚙️ Prerequisites

Make sure the following libraries are installed:

```bash
# Debian/Ubuntu
sudo apt install libcurl4-openssl-dev libxml2-dev

# Fedora/RHEL
sudo dnf install libcurl-devel libxml2-devel

# macOS (Homebrew)
brew install curl libxml2
```

---

## 🚀 How to Build and Run

### Build

```bash
gcc WebScraper.c -o webscraper \
    $(xml2-config --cflags --libs) \
    -lcurl
```

### Run

```bash
./webscraper
```

The program will prompt you for a valid URL:

```
Digite uma URL valida: https://example.com
```

---

## 📤 Sample Output

```
Informacoes do certificado SSL
Subject: CN=example.com
Start date: Mar 10 00:00:00 2025 GMT
Expire date: Apr 10 23:59:59 2026 GMT

IP: 93.184.216.34

Tecnologias web utilizadas
Servidor: ECS
X-Powered-By: PHP/8.1

Links externos encontrados:
https://another-site.com/page
https://cdn.example.net/resource.js
```

---

## 🗂️ Project Structure

```
.
├── WebScraper.c   # Main source file
└── README.md      # Documentation
```

---

## ⚠️ Disclaimer

- Use this tool only on websites you are authorized to analyze.
- The program does not explicitly follow redirects — the provided URL should be the final destination.
- The `X-Powered-By` header may be absent depending on the server's configuration.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
