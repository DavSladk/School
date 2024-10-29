# Multimédia
Hodnocení: 24/29

## Multimediální prezentátor
Vypracovali (2.5.2022):
* David Sladký (xsladk07)
* Filip Dráber (xdrabe09)

### Zadání
Vytvořte multimediální aplikaci, která bude zobrazovat multimediální obsah na slajdech:
* Vytvořte formát souboru, který bude schopen ukládat předpis multimediálních slajdů, bude podporovat ukládání více slajdů.
* Vytvořte aplikaci, která bude umožňovat přehrávat dané multimediální objekty a která bude umožňovat přepínat mezi slajdy vhodným způsobem
* Presentační aplikace musí být též multimediální, např. přehrávání zvuků při přechodu slajdů, animace přechodů slajdů, animace zoomování určité části slajdu atp.

### Řešení
Pro řešení tohoto projektu jsme využili HTML, CSS, JavaScirpt a XML.

#### XML
XML jsme využili pro definici samotných slidů a elementů, které se na nich nachází a CSS tříd, které tyto elementy mají. Samotný formát XML je definován následujícím DTD:
```
<!ELEMENT slides (slide+)>
<!ELEMENT slide (slideClass|text|button|image|audio|video)*>
<!ELEMENT slideClass (#PCDATA)>

<!ELEMENT text (textClass*,value)>
<!ELEMENT textClass (#PCDATA)>
<!ELEMENT value (#PCDATA)>

<!ELEMENT button (buttonClass*,to,value)>
<!ELEMENT buttonClass (#PCDATA)>
<!ELEMENT to (#PCDATA)>
<!ELEMENT value (#PCDATA)>

<!ELEMENT image (imageClass*,src)>
<!ELEMENT imageClass (#PCDATA)>

<!ELEMENT audio (audioClass*,src,loop?)>
<!ELEMENT audioClass (#PCDATA)>

<!ELEMENT video (videoClass*,src,loop?)>
<!ELEMENT viodeClass (#PCDATA)>

<!ELEMENT src (#PCDATA)>
<!ELEMENT loop EMPTY>
```
* `slides` je root element xml, který obsahuje elementy `slide`, které reprezentují samotné slidy. Každý může mít libovolný počet elementů CSS tříd, textu, tlačítek, obrázků, zvuku a videa. Každý tento element může mít libovolný počet CSS tříd. 
* `text` má pouze element `value`, který udává obsah textu.
* `button` má element `to`, který udává slide, na který se má přejít po kliknutí na toto tlačítko. Dále má element `value`, který udává text tlačítka.
* `image` má element `src`, který udává obrázek, který se má zobrazit.
* `audio` má element `src`, který udává audio stopu, která se má přehrát. Podporuje pouze mp3. Dále je možné specifikovat element `loop`, aby se audio opakovalo.
* `video` má element `src`, který udává video, které se má přehrát. Podporuje pouze mp4. Dále je možné specifikovat element `loop`, aby se video opakovalo.

#### CSS
CSS jsme využili k tomu, k čemu je CSS určeno - k definování vzhledu slidů a jejich elementů.

#### HTML
Do HTML převádíme slidy definované v XML, aby je bylo možné zobrazit ve webovém prohlížeči. Než se načtou slidy, tak je zobrazen výběr souboru.

#### JavaScript
Používáme k načítání jednotlivých slidů a přepínání mezi nimi. Tento script se skládá z několika funkců:
* `loadXML()` - Volá se po výběru XML k načtení. Převede XML do vnitřní reprezentace a zobrazí první slide.
* `loadSlide(slideNumber)` - Zobrazí slide z XML určený `slideNumber`.
* `createTexts(texts, slideNode)` - Vytváří HTML elementy `div` pro text a připojí je k elementu v slideNode.
* `createButtons(buttons, slideNode)` - Vytváří HTML elementy `button` pro tlačítka a připojí je k elementu v slideNode.
* `createImages(images, slideNode)` - Vytváří HTML elementy `img` pro obrázky a připojí je k elementu v slideNode.
* `createAudios(audios, slideNode)` - Vytváří HTML elementy `audio` pro audio a připojí je k elementu v slideNode.
* `createVideos(videos, slideNode)` - Vytváří HTML elementy `video` pro video a připojí je k elementu v slideNode.
* `addClasses(element, classes)` - Přidává `element` jeho třídy definované v XML.

### Obsluha
Vyberte svůj preferovaný webový prohlížeč a otevře v něm soubor `index.html`. Měli byste vidět výzvu k výběru souboru, vyberte tedy soubor se slidy k otevření. Po načtení slidů se mezi jednotlivými slidy naviguje klikáním na tlačítka.
