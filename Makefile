


all:
	python main.py


validate:
	python validate.py 


clean:
	rm -r ./cache/*


.PHONY: clean, all