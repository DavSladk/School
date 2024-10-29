Hodnocení: 6/12

Dle opory TIN je pro minimalizaci třeba provést následující kroky:
* odstranit nedosažitelné stavy - naimplementováno
* převést automat do plně definovaného stavu - naimplementováno
* samotná minimalizace - implementováno
* odstranění všech SINK stavů - implementováno

## Bugy
* Převádění do kakonické formy je implementováno. Avšak na seřazování polí bylo použito prostě funkce sort, což nevím jaký přesný efekt má na seřazování trojic, kterýma reprezentuji přechodovou funkci.

## Testy
* test0 - automat z opory TIN
* test1 - odstraňování nedosažitelných stavů
* test2 - odstraňování sink stavů
