# Caixa-Preta
Este é um repositório para armazenar todos os arquivos relativos ao projeto caixa preta, não só códigos.

O código do dispositivo se encontra em:
``` src > ino ```

O código relativo ao Painel de visualização em Matlab, se encontra em:
``` src > matlab >  dashboard ```

O código relativo a extração de dados do dispositivo e armazenamento em arquivo, se contra em:
``` src > Prog PC ```

Cada programa possue sua documentação na pasta:
``` docs ```
na raiz do projeto. Com pastas separadas para cada programa

## Organização das pastas:
```
├── docs (todas as documentações)
|   ├── ...
|   ├── commons (documentações comuns a todos os programas, e.x.: Formato padrão de dados)
|   ├── dashboard (documentação do dashboard em matlab)
|   └── dispositivo (documentação do código do dispositivo)
|
├── peripherals (datasheets dos periféricos do dispositivo)
├── schemas (esquemas de hardware do dispositivo)
└── src (códigos fonte dos programas, separados por linguagem)
    ├── ...
    ├── Prog PC (Programa para leitura de dados do dispositivo, e escrita em arquivo)
    ├── ino (códigos do dispositivo em arduino (C))
    └── matlab
        ├── ...
        ├── estudos (scripts com alguns estudos)
        └── dashboard (programa do dashboard em matlab)
```
