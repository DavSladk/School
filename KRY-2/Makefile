.PHONY: all build run zip clean req source runv
.ONESHELL:

TO_ZIP=\
Makefile\
kry.py\
client.py\
server.py\
dokumentace.md\
requirements.txt

TO_CLEAN=\
175082.zip\
venv/

all: clean build

build:
	python3 -m venv venv/
	source venv/bin/activate
	pip3 install -r requirements.txt
	deactivate

clean:
	rm -rf $(TO_CLEAN)

zip:
	cp README.md dokumentace.md
	zip 175082.zip $(TO_ZIP)
	rm dokumentace.md

req:
	pip3 freeze > requirements.txt

run:
	. venv/bin/activate
	if [ "$(TYPE)" = "c" ]; then
		python3 ./kry.py -m client -p $(PORT)
	elif [ "$(TYPE)" = "s" ]; then
		python3 ./kry.py -m server -p $(PORT)
	else
		python3 ./kry.py -m u -p $(PORT)
	fi
	deactivate

runv:
	. venv/bin/activate
	if [ "$(TYPE)" = "c" ]; then
		python3 ./kry.py -m client -p $(PORT) -v
	elif [ "$(TYPE)" = "s" ]; then
		python3 ./kry.py -m server -p $(PORT) -v
	else
		python3 ./kry.py -m u -p $(PORT) -v
	fi
	deactivate
