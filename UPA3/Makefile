.PHONY: zip clean build run

TO_ZIP = build.sh data.py data.tsv Makefile README requirements.txt run.sh urls.py urls.txt

zip:
	zip xsladk07_xkolec08_xmorav41.zip $(TO_ZIP)

clean:
	rm -r venv/

build:
	bash build.sh

run:
	bash run.sh
