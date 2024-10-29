# UXIa
## Fast setup
### Snadné přemisťování souborů
```
mkdir merlin
sshfs xlogin00@merlin.fit.vutbr.cz: merlin
```

### Vygenerování klíčů pro přihlašování na merlina
```
ssh-keygen
```
* .pub nakorírujte do .ssh/authorized_keys na merlinovi

https://www.digitalocean.com/community/tutorials/how-to-set-up-ssh-keys-on-ubuntu-20-04

* na lokále do souboru `.ssh/config` vložte záznam:
```
Host merlin
        User xlogin00
        Hostname merlin.fit.vutbr.cz
```

S tímhle se půjde připojit na merlina:
```
ssh merlin

nebo

sshfs merlin: merlin
```

A taky jednoduše ve VS code připojit na merlina a developit přímo tam.

### Proč tohle děláme?
Protože když si na merlinovi do složky WWW nakopírujete nějakou webovku, tak je potom můžete přistupovat z https://www.stud.fit.vutbr.cz/~xlogin00/index.html

Takže když si do té složky naklonujete repo, tak rovnou uvidíte změny, co budete dělat a nemusíte rozbíhat web server na lokále.

Aby webovky fungovaly, tak je ještě potřeba udělat:
```
chmod -R a+rX WWW
chmod o+x $HOME
```

### Klonování z gitu na merlina
Pokud nechcete pořád zadávat heslo při každým push/pull, tak je třeba na merlinovi vygenerovat klíče na přihlašování na github.
https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent

Po vygenerování klíčů je ještě na merlinovi třeba udělat:
```
eval "$(ssh-agent -s)"
ssh-add .ssh/privatni_klic_na_github
```
Pak ve složce WWW:
```
git clone git@github.com:DavSladk/UXIa.git .
```

Což vám tenhle projekt naclonuje do WWW/ a budete moct pracovat, gitovat a rovnou uvidíte změny na

https://www.stud.fit.vutbr.cz/~xlogin00/index.html

### Nebo...

Nemusíte řešit klonování na merlina, jenom udělat sshfs na merlina a pak s tím pracovat jako by to byl lokál.