# Projekt: Kryptografie (KRY) 2022/2023
## Popis
Jednoduchý nástroj na afinitní šifru. Umožňuje šifrovat, dešifrovat a prolamovat tuto šifru. Nástroj NENÍ BLBOVZDORNÝ - předpokládá korektní vstupy. Při nekorektních vstupech je chování neošetřeno.

### Předpoklady
* celý vstup je na jednom řádku
* vstupní text odpovídá `[a-zA-Z ]+`
* mezery nešifruje

## Synopsis
`./kry {-e|-d|-c} [-a keyA] [-b keyB] [-f filename|string] [-o filename] [-v] [-m map1 map2] [-l lang] [-n line]`

* `-e` : mód šifrování s klíčem
* `-d` : mód dešifrování s klíčem
* `-c`: mód dešifrování bez klíče
* `-a` : klíč A pro práci s klíčem (výchozí hodnota: 3)
* `-b` : klíč B pro práci s klíčem (výchozí hodnota: 7)
* `-f` : jméno souboru se vstupem
* `string` : vstupní text (použito, pokud `-f` není specifikováno)
* `-o` : výstupní soubor (pokud není specifikováno, tiskne se na stdout)
* `-v` : tisk různých debugovacích poznámek použitých při debugování
* `-m` : na jaké otevřené písmena (seřazená sestupně podle frekvence) se mají dvě nejčastější zašifrovaná písmena namapovat při dešifrování bez klíče
    * `map1` : výchozí hodnota 0
    * `map2` : výchozí hodnota 1
* `-l` : pro jaký jazyk se má prolamovat při dešifrování bez klíče (výchozí hodnota: CZ)
* `-n` : z jakého řádku v souboru se má číst (výchozí hodnota: 1)

## Šifrování s klíčem
Pro šifrování se využívá vzorce pro afinitní šifru: `E(x) = (a * x + b) % 26`

## Dešifrování s klíčem
Pro dešifrování se využívá vzorce: `D(x) = a^(-1)(x - b) % 26` . Kde `a^(-1)` se hledá jednoduchý iterování přes možné kandidáty, dokud se nenajde ten správný.

## Dešifrování bez klíče
1. Provede se načtení četností písmen ze souboru `.freq`, kde část před příponou odpovídá volbě `-l`. Tento soubor je v plain text formátu:
```
A 1234
B 87964
U 8524
D 789645
.
.
.
```
2. Provede se spočtení četností písmen v zašifrovaném textu.
3. Provede se vypočtení všech možných kombinací klíčů `A` a `B` za pomocí vzorce pro šifrování s klíčem : `y = (a * x + b) % 26`
    * `y` - Nejčastější písmeno v zašifrovaném textu
    * `x` - Nejčastější písmeno v otevřeném jazyce. Dá se měnit na písmeno s jinou četností pomocí `map1` v přepínači `-m`
    * Protože `a` patří do množiny `{1,3,5,7,11,15,17,19,21,23,25}` a `b` patří do množiny `{0..25}`, tak tento krok lze provést projitím všech možností
4. Provede se obdobná operace, jako v 3, avšak:
    * `y` - Druhé nejčastější písmeno v zašifrovaném textu
    * `x` - Druhé nejčastější písmeno v otevřeném jazyce. Dá se měnit na písmeno s jinou četností pomocí `map2` v přepínači `-m`
5. Provede se průnik vypočtených možných klíčů z 3 a 4. Klíče `A` a `B` z tohoto průniku se potom použijí pro dešifrování textu běžným dešifrováním s klíčem.

## Poznámky
* Výchozí nastavení mapování šifrovaných písmen na otevřená písmena funguje ve většině případů, ale ne vždy. Pokud je po dešifrování bez klíče na výstupu hatmatilka, je třeba toto mapování upravit pomocí přepínače `-m`.
* Z pár experimentů jsem vyvodil, že průnik v kroku 5 vždy obsahuje:
    * Právě jednu možnou kombinaci klíčů. Tyto klíče se použijí pro dešifrování.
    * Žádnou kombinaci. Potom program zahlásí na stderr, že nic nenašel a o dešifrování se nepokosí.
    * Což dává smysl, protože se v podstatě jedná o řešení soustavy dvou lineárních rovnic o dvou neznámých.
    
    Proto jiné případy nejsou ošetřeny

* Mě přiděleném textu v `ciphertext.txt` se v originálním textu nachází číslice 1709, která je dešifrována na KQJS
* Dále se v textu nachází slovo CHLOUPKAB, ale při referenci s originálním textem tam má být pouze CHLOUPKA. Chyba je v `ciphertext.txt`, protože již zde toto slovo obsahuje písmeno navíc.