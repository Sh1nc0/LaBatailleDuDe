
WORKDIR = $(shell pwd)
DOCKER = dicewars

ifdef debug
	DIR = linux/debug
	SUFFIX = _d
else
	DIR = linux/release
	SUFFIX =
endif

export debug

build:
	docker run -v $(WORKDIR):/app $(DOCKER) make -B -f Makefile_etu debug=1

run:
	docker run -v $(WORKDIR):/app -p 5678:5678 $(DOCKER) /app/bin/$(DIR)/dicewars$(SUFFIX) -r /app/bin/$(DIR)/referee$(SUFFIX).so -m /app/bin/$(DIR)/genmap$(SUFFIX).so -g /app/bin/$(DIR)/gui$(SUFFIX).so -s /app/bin/$(DIR)/strategy$(SUFFIX).so -s /app/bin/$(DIR)/strategy$(SUFFIX).so

runtester:
	docker run -v $(WORKDIR):/app -p 5678:5678 $(DOCKER) /app/bin/$(DIR)/maptester$(SUFFIX) -m /app/bin/$(DIR)/genmap$(SUFFIX).so

