# Projekt 2: Kryptografie (KRY) 2022/2023

## Použití
`kry.py [-h] -m {client,server} -p PORT [-v]`
* `-h`, `--help` : vypiš tuhle zprávu a ukonči se
* `-m {client,server}`, `--mode {client,server}` : mód pro spuštění
* `-p PORT`, `--port PORT` : pro server -> na jakém portu se spusti; pro klienta -> na jaký port se připoj
* `-v`, `--verbose` : vypisuj extra debug info

## Použití 2
`make build` : vytvoží venv a nainstaluje potřebné moduly

`make -s run TYPE=c PORT=54321` : spustí se jako klient a pokusí se připojit na port 54321

`make -s run TYPE=s PORT=54321` : spustí se jako server na portu 54321

## Popis
### Vytváření klíčů
* RSA klíč je vytvořen pomocí funkce z modulu `cryptography`
* AES klíč je vytvořen vytažením potřebného množství bytů z `/dev/random`

### Použití klíčů
* RSA klíče klienta
  * Soukromý klíč je použit pro podpis hashe posílané zprávy
  * Veřejný klíč je poté použit u příjemce k rozšifrování podpisu a jeho ověření
* RSA klíče serveru
  * Veřejný klíč je použit klientem pro zašifrování AES klíče
  * Soukromý klíč je použit serverem k dešifrování AES klíče
* AES klíč
  * je použit klientem k zašifrování zprávy a jejího podpisu
  * je použit serverem k zašifrování zprávy a jejího podpisu

### Výměna klíčů
* veřejné klíče RSA jsou vyměněny mezi klientem a serverem hned po jejich připojení ve formátu `pem` v open textu
* klíč AES je připojen k odesílané zprávě v zašifrován pomocí RSA

### Zajištení integrity
* Zajištení integrity je zaopatřeno vytvářením hashe ze zprávy a následným podpisem tohoto hashe soukromým klíčem odesilatele. Příjemce poté vypočítá vlastní hash ze zprávy a zkontroluje ho z hashem přijatým od odesilatele, který rozluští veřejným klíčem odesilatele, hashe porovná a tím zjistí, zda došlo k manipulaci zprávy

### Předávání zpráv
* Když se klient a server připojí, tak si navzájem vymění svoje RSA veřejné klíče
* Klient po zadání zprávy pro odeslání následně odešle zprávu v následujícím tvaru:

| Velikost | Popis |
|--------------|-----------|
| 2B | Velikost zašifrovaného AES klíče v bytech |
| 2B | Velikost podpisu v bytech |
| 2B | Velikost extra paddingu přidaného pro doplnění bloku pro AES v bytech |
| 16B | init vector pro AES |
| ?? | Zašifrovaný text a podpis |
| ?? | Zašifrovaný AES klíč |

* serven si následně rozseká zprávu, rozšifruje jednotlivé části a ověří integritu

### Bezpečnost řešení
* pro RSA je využito délky klíče 2048b - tato velikost prakticky vylučuje možnost útoklu brute force
* pro AES je využito délky klíče 256b - tato velikost prakticky vylučuje možnost útoklu brute force
* pro RSA je využito paddingu PKCS1 v1.5. Tento padding je jednodušší na implementaci, ale je méně bezpečné než OAEP nebo PSS padding. Je nachýlný na takzvaný Oracle attack

## Poznámky
* server přijímá maximálně zprávu velikosti 2048 B. Pokud zaslaná zpráva bude mít více B, tak dojde k nedefinovanému chování
